// objects for file IO
TTree *T;// = (TTree*)f->Get(channel);
TFile *f; 
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

Detector det;

void SingleAlphaSmear(double res = 0.2)
{
  double alpha =  .3;//0.13;//.069;//.061;//.069;
  // sim file
  TFile *f1 = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/stilbeneResponse/gamma/_en_0.05954.root");//Cs137_t8.root");
  TString channel = "t1";
  TTree *t;
  f1->GetObject(channel, t);
  double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  
  int nBins = 500;
  double sR = 0;
  double eR = 1;
  auto hSim  = new TH1F("hSim","",nBins,sR,eR);
  
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) 
      hSim->Fill(edep);
  }

  auto hSmear = new TH1F("hSmear",";;",hSim->GetNbinsX(),0,hSim->GetXaxis()->GetXmax());

  // exp file
  auto f2 = new TFile( "_run_1.root", "read" );
  channel = "T";
  TTree *T;
  double energy;
  f2->GetObject(channel, T);
  T->SetBranchAddress("d",&det);
  T->SetBranchAddress("E",&energy);
  // setup histos with the right ranges and bin numbers
  auto hExp = new TH1F("hExp","",nBins,sR,eR);
  T->Draw("d.l*0.000100624806702 >> hExp");
  hExp = (TH1F*)gDirectory->Get("hExp");
  
  hSim->SetLineColor(3);

  double lightSmear;
  double depositedEnergy;
  double resolution;

  TRandom1 rand; // 1D random number
  for( Int_t i=1; i<=hSmear->GetNbinsX(); i++ )
  {
		for( Int_t j=0; j<hSim->GetBinContent(i); j++ )
    {
      depositedEnergy = hSim->GetXaxis()->GetBinCenter(i);

      // smear the light output
      resolution = res;//.13;

      lightSmear = rand.Gaus(depositedEnergy, resolution * depositedEnergy / 2.35);

      while (lightSmear<=-0)
      {
        lightSmear = rand.Gaus(depositedEnergy, resolution * depositedEnergy / 2.35);
      }
			hSmear->Fill(lightSmear);//.35 for 15.1 MeV*/
		}
  }
  // scale for viewing
  sR = .03;
  eR = .1;
  hSim->GetXaxis()->SetRangeUser(sR,eR);
  //hSim->Scale(1/hSim->GetMaximum());
  hExp->GetXaxis()->SetRangeUser(sR,eR);
  hSmear->GetXaxis()->SetRangeUser(sR,eR);

  hExp->Scale(hSmear->Integral()/hExp->Integral());

  hSmear->Scale(1/hSmear->GetMaximum());
  hExp->Scale(1/hExp->GetMaximum());

  //hSim->Draw("hist");
  hSmear->SetLineColor(2);
  hExp->Draw("hist same");
  //new TCanvas;
  hSmear->Draw("same hist");
}
