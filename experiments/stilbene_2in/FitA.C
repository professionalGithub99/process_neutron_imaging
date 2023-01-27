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
vector<double> s_en; // sim values of deposited energy spectrum 
vector<double> s_y;  // sim yield values
vector<double> ev;   // measured ev value 
// histograms
TH1F *hSim;
TH1F *hSmear;
TH1F *hExp;
//double sFR = 0.9; // Na22  // 0.35; // Cs137
//double eFR = 1.1; // Na22  // 0.55; // Cs137
//double sFR = 0.1;  // Ba133  
//double eFR = 0.25; // Ba133 
double sFR = 0.7;  // Ba133  
double eFR = 1.3; // Ba133 

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

Detector det;

// smear function: smears the deposited energy spectrum based on the mean and 
// sigma provided and scale to equal the number of events
// **** fit function ****
double fcn(double par);


// ********************
// *** main routine ***
// ********************
void FitA(TString eF = "expFile.root", 
          TString bF = "background.root",
          TString sF = "sim.root")
{
  // ****************
  // *** sim file ***
  // ****************
  //TFile *f1 = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_0.662.root");//05954.root");//Cs137_t8.root");
  //TFile *f1 = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_1.27454.root");//05954.root");//Cs137_t8.root");
  //TFile *f1 = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/Ba133_t8.root");//05954.root");//Cs137_t8.root");
  TFile *f1 = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/Co60_t8.root");//05954.root");//Cs137_t8.root");

  TString channel = "t1";
  TTree *t;
  f1->GetObject(channel, t);
  double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  
  int nBins = 500;
  double sR = 0;
  double eR = 2;
  hSim  = new TH1F("hSim","",nBins,sR,eR);
  
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) 
      hSim->Fill(edep);
  }

  hSmear = new TH1F("hSmear",";;",hSim->GetNbinsX(),0,hSim->GetXaxis()->GetXmax());

  // exp file
  auto f2 = new TFile( "_run_5.root", "read" );
  channel = "T";
  TTree *T;
  double energy, timestamp;
  f2->GetObject(channel, T);
  T->SetBranchAddress("d",&det);
  T->SetBranchAddress("E",&energy);
  T->SetBranchAddress("timestamp",&timestamp);
  // setup histos with the right ranges and bin numbers
  hExp = new TH1F("hExp","",nBins,sR,eR);
  // use ratio to line up perfectly to edge
  //T->Draw("d.l*0.000100624806702 >> hExp"); // Am241
  //T->Draw("d.l*7.42360878682948E-05 >> hExp"); // Cs137
  //T->Draw("d.l*7.11950508546761E-05 >> hExp"); // Na22
  //T->Draw("d.l*7.88308959863376E-05 >> hExp"); // Ba133
  T->Draw("d.l*7.23908756566431E-05 >> hExp"); // Co60

  hExp = (TH1F*)gDirectory->Get("hExp");
  T->GetEntry(0);
  double firstTime = timestamp;
  T->GetEntry(T->GetEntriesFast()-1);
  double lastTime  = timestamp;
  double totTime = lastTime - firstTime;
  totTime *= 1E-9; // convert to seconds
  hExp->Scale(1./totTime);
  
  hSim->SetLineColor(3);

  double t_res;
  double t_chi2;
  auto ttt = new TTree("ttt","res and chi2");
  ttt->Branch("res",&t_res,"t_res/D");
  ttt->Branch("chi2",&t_chi2,"t_chi2/D");
  double res_in = .001;
  int n_i = 1/res_in; 
  int minPos = 0;
  double minRes = 0;
  double minVal;
  for(int i=0; i<n_i; i++)
  {
    if(i%100 == 0) cout << "iteration: " << i << " complete\n";
    t_res  = (i+1) * res_in;
    t_chi2 = fcn(t_res);
    if(i==0) minVal = t_chi2;
    else if(t_chi2 < minVal)
    {
      minPos = i;
      minVal = t_chi2;
      minRes = t_res;
    }
    ttt->Fill();
  }
  
  //gROOT->SetBatch(); // prevent actually showing the plots here
  // must draw something first to fill data structures
  cout << "pos: " << minPos << " val check: " << minVal << endl; 
  cout << "min chi2: " << ttt->GetMinimum("t_chi2") << std::endl;
  cout << "min res: " << minRes << endl;
  fcn(minRes); 
  // *** Draw some stuff ***
  // scale for viewing
  hSim->GetXaxis()->SetRangeUser(sFR,eFR);
  hExp->GetXaxis()->SetRangeUser(sFR,eFR);
  hSmear->GetXaxis()->SetRangeUser(sFR,eFR);

  //hExp->Scale(hSmear->Integral()/hExp->Integral());
  //hSmear->Scale(1/hSmear->GetMaximum());
  //hExp->Scale(1/hExp->GetMaximum());

  //hSim->Draw("hist");
  hSmear->SetLineColor(2);
  hExp->Draw("hist");
  //new TCanvas;
  hSmear->Draw("same hist");
}  

double fcn(double par)
{
  hSmear->Reset();
  double lightSmear;
  double depositedEnergy;
  double resolution;
  resolution = par;

  TRandom1 rand; // 1D random number
  for( Int_t i=1; i<=hSmear->GetNbinsX(); i++ )
  {
		for( Int_t j=0; j<hSim->GetBinContent(i); j++ )
    {
      depositedEnergy = hSim->GetXaxis()->GetBinCenter(i);

      // smear the light output
      //resolution = res;//.13;

      lightSmear = rand.Gaus(depositedEnergy, resolution * depositedEnergy / 2.35);

      while (lightSmear<=-0)
      {
        lightSmear = rand.Gaus(depositedEnergy, resolution * depositedEnergy / 2.35);
      }
			hSmear->Fill(lightSmear);//.35 for 15.1 MeV*/
		}
  }
  //hSim->GetXaxis()->SetRangeUser(sR,eR);
  hExp->GetXaxis()->SetRangeUser(sFR,eFR);
  hSmear->GetXaxis()->SetRangeUser(sFR,eFR);

  hSmear->Scale(hExp->Integral() / hSmear->Integral());


  // calc Chi2
  int i_s = hExp->FindBin(sFR);
  int i_e = hExp->FindBin(eFR);
  double chi2 = 0;
  for( Int_t i=i_s; i<i_e; i++ )
  {
    auto oVal = hSmear->GetBinContent(i);
    auto eVal = hExp->GetBinContent(i);
    chi2 += ( (oVal-eVal)*(oVal-eVal) ) / TMath::Sqrt(eVal); 
  }
  //cout << chi2 << " : " << resolution << endl;
  return chi2;
}
