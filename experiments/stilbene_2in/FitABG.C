// ROOT
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TString.h>
#include <TH1F.h>
#include <TF1.h>
// c++ 
#include <iostream>
#include <vector>
// namespaces
using namespace std;
// vectors
// use next two variables to calulate underlying Gaus distribution
vector<double> s_en[3]; // sim values of deposited energy spectrum 
vector<double> s_y[3];  // sim yield values
vector<double> ev[3];   // measured ev value 
vector<double> rNums;
// histograms
TH1F *hSim[3];
TH1F *hSmear[3];
TH1F *hExp[3];
TF1 *g2;
// canvas
TCanvas *c0;
// variables
double expMean;
double sR[3], eR[3];
int nBins;
double binWidth = 0.005;
// objects for file IO
TTree *T[3];// = (TTree*)f->Get(channel);
TFile *f[3]; 
TString channel;
//TF1 *fg;
// data structure
typedef struct
  {
    Float_t l;               // Long integral
    Float_t s;               // Short integral
    Float_t amp;             // Amplitude
    Float_t cfd;             // Trigger time
    Float_t psd;             // PSD parameter s/l
    Float_t trg;             // Detector trigger

  } Detector;

Detector det[3];

// read files in
void setData(TString iFN, int fNum);
// smear function: smears the deposited energy spectrum based on the mean and 
// sigma provided and scale to equal the number of events
TH1* makeGaus(double mean, double sigma, double theInt)
{
  TF1 *fg = new TF1("fg","gaus(0)",0,2); 
  nBins = (2-0) / binWidth; 
  fg->SetNpx(nBins);
  double height = 0.3989*theInt / sigma;
  fg->FixParameter(0,height);
  fg->FixParameter(1,mean);
  fg->FixParameter(2,sigma);
  auto h = fg->GetHistogram();
  //delete fg;
  return h;
}

// create underlying gaus distr.
void makeDistr(double a, double b, double g, int i_fn)
{
  for(size_t i=0; i<s_en[i_fn].size(); i++)
  {
    auto res = sqrt( pow(a,2) + pow(b,2) / s_en[0][i] + pow(g/s_en[i_fn][i],2) );
    auto sig = res / 2.35;

    auto iH = makeGaus(s_en[i_fn][i], sig, s_y[i_fn][i]);  
    hSmear[i_fn]->Add(iH);
    delete iH;
  }
}

// **** fit function ****
double fcn(const double *par)
{
  double rchi2 = 0;
  for(int i_fn=1; i_fn<3; i_fn++)
  {
    hSmear[i_fn]->Reset(); // clear out old data in histogram
    makeDistr(par[0],par[1],par[2], i_fn); 
    // truncate histogram
    //cout << "\n" << i_fn << " " << sR[i_fn] << endl;
    hSmear[i_fn]->GetXaxis()->SetRangeUser(sR[i_fn],eR[i_fn]);
    // fit Gaus to smeared sim for x shift 
    //hSmear[0]->Fit(g2,"QR");
    //auto simMean = g2->GetParameter(1); 
    //double xShift = expMean / simMean; // horizontal scale
    // scale 
    double scaleVal = hExp[i_fn]->GetMaximum();//Integral();
    hSmear[i_fn]->Scale(scaleVal/hSmear[i_fn]->GetMaximum());//Integral()); // vertical scale
    // calc rchi2 only in region of interest
    double rss = 0; // reduced sum of squares
    for(int i=0; i<hExp[i_fn]->GetNbinsX(); i++)
    {
      int k = hSmear[i_fn]->GetXaxis()->FindBin(hExp[i_fn]->GetBinCenter(i+1));
      auto val = hSmear[i_fn]->GetBinContent(k) - hExp[i_fn]->GetBinContent(i+1);
      val *= val; // square it
      val /= ev[i_fn][i];// hExp[0]->GetBinContent(i+1));
      rss += val; // sum it
    }
    double numOfPars = 3;
    double ndf = hExp[i_fn]->GetNbinsX() - numOfPars; // degrees of freedom
    //cout << "rss: " << rss << endl;
    rchi2 += rss;
  }
  cout << par[0] << " " << par[1] << " " 
       << par[2] << " " << rchi2 << endl; 
  return rchi2;
}

// ********************
// *** main routine ***
// ********************
void FitABG()
{
  double full_eR = 2;
  // ***************************
  // *** set up Am241 source ***
  // ***************************
  channel = "T";
  int fNum = 0;
  auto fbAm = new TFile("_am_background.root"); 
  TTree *tbAm;
  ///media/7jn/T7/ND2021/code/data_cal/run_0.root");
  fbAm->GetObject(channel, tbAm);
  double energy;
  Detector dBAm;
  tbAm->SetBranchAddress("d",&dBAm);
  tbAm->SetBranchAddress("E",&energy);
  // need to change these limits depending on the source that you use
  sR[fNum] = 0.03;  // [MeV]
  eR[fNum] = 0.08; // [MeV]
  nBins = (eR[fNum] - sR[fNum]) / binWidth;
  // setup histos with the right ranges and bin numbers
  auto hbAm  = new TH1F("hbAm","",nBins,sR[fNum],eR[fNum]);
  cout << "Am241 background bins " << hbAm->GetNbinsX() << endl;
  tbAm->Draw("E >> hbAm");
  hbAm = (TH1F*)gDirectory->Get("hbAm");
  // read in experiment
  f[fNum] = new TFile("_am.root");
  f[fNum]->GetObject(channel, T[fNum]);
  T[fNum]->SetBranchAddress("d",&det[fNum]);
  T[fNum]->SetBranchAddress("E",&energy);
  // setup histos with the right ranges and bin numbers
  hExp[fNum]   = new TH1F("hExp[0]","",nBins,sR[fNum],eR[fNum]);
  T[fNum]->Draw("E >> hExp[0]");
  hExp[fNum] = (TH1F*)gDirectory->Get("hExp[0]");
  
  cout << "Am241 exp bins: " << hExp[fNum]->GetNbinsX() << endl;
  hExp[fNum]->Add(hbAm,-1);
  
  for(Int_t i=0; i<hExp[fNum]->GetNbinsX(); i++)
    ev[fNum].push_back(hExp[fNum]->GetBinContent(i+1));
  // read in simulation deposited energy

  nBins = (full_eR-0) / binWidth;
  hSim[fNum]   = new TH1F("hSim[0]","",nBins,0,full_eR);
  hSmear[fNum] = new TH1F("hSmear[0]","",nBins,0,full_eR);
  // check bin width
  cout << hExp[fNum]->GetBinWidth(0) << " " << hSmear[fNum]->GetBinWidth(0) << endl; 
  TTree *t;// = (TTree*)f->Get(channel);
  TFile *fAm = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_0.05954.root");//Cs137_t8.root");
  channel = "t1";
  fAm->GetObject(channel, t);
  double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) 
      hSim[fNum]->Fill(edep);
  }
  // loop over sim histo and store mean and counts for faster processing
  for(Int_t i=0; i<hSim[fNum]->GetNbinsX(); i++)
  {
    s_en[fNum].push_back(hSim[fNum]->GetBinCenter(i+1));
    s_y[fNum].push_back(hSim[fNum]->GetBinContent(i+1));
  }
  // *************************************
  // read in background for Cs137 and Na22
  // *************************************
  fNum = 1;
  auto fb = new TFile("_background.root");
  TTree *tb;
  channel = "T";
  ///media/7jn/T7/ND2021/code/data_cal/run_0.root");
  fb->GetObject(channel, tb);
  //double energy;
  Detector dB;
  tb->SetBranchAddress("d",&dB);
  tb->SetBranchAddress("E",&energy);
  // need to change these limits depending on the source that you use
  sR[fNum] = 0.35;  // [MeV]
  eR[fNum] = 0.5; // [MeV]
  nBins = (eR[fNum] - sR[fNum]) / binWidth;
  // setup histos with the right ranges and bin numbers
  auto hb  = new TH1F("hb","",nBins,sR[fNum],eR[fNum]);
  cout << "Cs137 background bins " << hb->GetNbinsX() << endl;
  
  tb->Draw("E >> hb");
  hb = (TH1F*)gDirectory->Get("hb");
  // read in experiment
  f[fNum] = new TFile("_cs.root");
  f[fNum]->GetObject(channel, T[fNum]);
  T[fNum]->SetBranchAddress("d",&det[fNum]);
  T[fNum]->SetBranchAddress("E",&energy);
  // setup histos with the right ranges and bin numbers
  hExp[fNum]   = new TH1F("hExp[1]","",nBins,sR[fNum],eR[fNum]);
  T[fNum]->Draw("E >> hExp[1]");
  hExp[fNum] = (TH1F*)gDirectory->Get("hExp[1]");
  
  cout << "Cs137 exp bins: " << hExp[fNum]->GetNbinsX() << endl;
  cout << "161: " << hExp[fNum]->GetNbinsX() << " " << hb->GetNbinsX() << endl;
  hExp[fNum]->Add(hb,-1);
  
  for(Int_t i=0; i<hExp[fNum]->GetNbinsX(); i++)
    ev[fNum].push_back(hExp[fNum]->GetBinContent(i+1));
  // read in simulation deposited energy

  nBins = (full_eR-0) / binWidth;
  hSim[fNum]   = new TH1F("hSim[1]","",nBins,0,full_eR);
  hSmear[fNum] = new TH1F("hSmear[1]","",nBins,0,full_eR);
 
  //TTree *t;// = (TTree*)f->Get(channel);
  //TFile *sf = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_0.662.root");//Cs137_t8.root");
  TFile *sf = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_0.662.root");//Cs137_t8.root");

  channel = "t1";
  sf->GetObject(channel, t);
  //double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) 
      hSim[fNum]->Fill(edep);
  }
  // loop over sim histo and store mean and counts for faster processing
  for(Int_t i=0; i<hSim[fNum]->GetNbinsX(); i++)
  {
    s_en[fNum].push_back(hSim[fNum]->GetBinCenter(i+1));
    s_y[fNum].push_back(hSim[fNum]->GetBinContent(i+1));
  }
  // **************************
  // read in next source (Na22)
  // **************************
  fNum = 2;
  sR[fNum] = 0.9; // [MeV]
  eR[fNum] = 1.15; // [MeV]
  nBins = (eR[fNum] - sR[fNum]) / binWidth;

  auto hbNa  = new TH1F("hbNa","",nBins,sR[fNum],eR[fNum]);
  tb->Draw("E >> hbNa");
  hbNa = (TH1F*)gDirectory->Get("hbNa");
  cout << "Na22 background nbins: " << nBins << endl;
  f[fNum] = new TFile("_na.root");
  ///media/7jn/T7/ND2021/code/data_cal/run_0.root");
  channel = "T";
  f[fNum]->GetObject(channel, T[fNum]);
  T[fNum]->SetBranchAddress("d",&det[fNum]);
  cout << "exp entries: " << T[fNum]->GetEntriesFast() << endl;
  // need to change these limits depending on the source that you use
  nBins = (eR[fNum] - sR[fNum]) / binWidth;
  cout << "Na22 nBins: " << nBins << endl;
  // setup histos with the right ranges and bin numbers
  hExp[fNum]   = new TH1F("hExp[2]","",nBins,sR[fNum],eR[fNum]);
  T[fNum]->Draw("E >> hExp[2]");
  hExp[fNum] = (TH1F*)gDirectory->Get("hExp[2]");
  for(Int_t i=0; i<hExp[fNum]->GetNbinsX(); i++)
    ev[fNum].push_back(hExp[fNum]->GetBinContent(i+1));
  // subtract background
  hbNa->Scale(2);
  hExp[fNum]->Add(hbNa,-1);
  // set range wider for the simulation and smeared histos
  nBins = (full_eR-0) / binWidth;  ;
  hSim[fNum]   = new TH1F("hSim[2]","",nBins,0,full_eR);
  hSmear[fNum] = new TH1F("hSmear[2]","",nBins,0,full_eR);
  // read in simulation deposited energy
  //sf = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_1.27454.root");
  sf = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/Na22_t8.root");

  channel = "t1";
  sf->GetObject(channel, t);
  //double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) 
      hSim[fNum]->Fill(edep);
  }
  // loop over sim histo and store mean and counts for faster processing
  for(Int_t i=0; i<hSim[fNum]->GetNbinsX(); i++)
  {
    s_en[fNum].push_back(hSim[fNum]->GetBinCenter(i+1));
    s_y[fNum].push_back(hSim[fNum]->GetBinContent(i+1));
  }
  //makeDistr(0.035,0.053,0.009); 
  // **************
  // *** Minuit ***
  // **************
  const char * minName = "Minuit2";
  const char *algoName = "";//"Simplex"; 
  ROOT::Math::Minimizer* minimum =
     ROOT::Math::Factory::CreateMinimizer(minName, algoName); 
  if(!minimum) 
  {
     std::cerr << "Error: cannot create minimizer \"" << minName
               << "\". Maybe the required library was not built?" << std::endl;
     return 1;
  }
  // set tolerance , etc...
  minimum->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
  minimum->SetMaxIterations(1000000);  // for GSL
  //minimum->SetTolerance(1);
  minimum->SetPrintLevel(2);
  // create function for minimizer
  ROOT::Math::Functor theFunc(&fcn,3);
  double step[3] = {0.001,0.001,0.001};
  // starting point
  double variable[3] = {0.1, 0.1, 0.001};//{0.05,.005,.005};// {0.5,0.05,0.005};
  minimum->SetFunction(theFunc);
  // Set the free variables to be minimized !
  minimum->SetLimitedVariable(0,"a", variable[0], step[0], 0, 1);
  minimum->SetLimitedVariable(1,"b", variable[1], step[1], 0, 1);
  minimum->SetLimitedVariable(2,"g", variable[2], step[2], 0, 1);

  // do the minimization
  minimum->Minimize();
 
  const double *xs = minimum->X();
  std::cout << "Minimum: func(" << xs[0] << "," << xs[1] << "," 
                                << xs[2]   
                                << "): " << minimum->MinValue()  << std::endl;
  // *** Draw some stuff ***
  auto c  = new TCanvas;
  c->Divide(1,3);
  for(int i=0; i<3; i++)
  {
    c->cd(i+1);
    double scaleVal = hExp[i]->Integral();
    hExp[i]->Draw("same hist");
    hSmear[i]->GetXaxis()->SetRangeUser(sR[i],eR[i]);
    hSmear[i]->Scale(scaleVal/hSmear[i]->Integral()); // vertical scale
    hSmear[i]->SetLineColor(3);
    hSmear[i]->Draw("same hist");
    cout << "fn: " << i << " exp:" << hExp[i]->Integral()
                        << " sim:" << hSmear[i]->Integral() << endl;
  }  

}  


