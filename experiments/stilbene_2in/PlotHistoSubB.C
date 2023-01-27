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
// objects for file IO
// **************

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

// ********************
// *** main routine ***
// ********************
void PlotHistoSubB(TString eF = "expFile.root", TString bF = "background.root")
{
  // *** histos ***
  // **************
  int nBins = 500;
  double sR = 0;
  double eR = 1; 
  auto hB = new TH1F("hB","",nBins,sR,eR);
  auto hS = new TH1F("hS","",nBins,sR,eR);

  cout << hB->GetNbinsX() << endl;
  // ******************
  // *** background ***
  // ******************
  TString channel;
  channel = "T";
  auto fB = new TFile(bF); 
  TTree *tB;
  fB->GetObject(channel, tB);
  double energy, timestamp;
  tB->SetBranchAddress("d",&det);
  tB->SetBranchAddress("E",&energy);
  tB->SetBranchAddress("timestamp",&timestamp);
  // setup histos with the right ranges and bin numbers
  tB->Draw("E >> hB(500,0,1)");
  hB = (TH1F*)gDirectory->Get("hB");
  // scale to counts per second
  tB->GetEntry(0);
  double firstTime = timestamp;
  tB->GetEntry(tB->GetEntriesFast()-1);
  double lastTime  = timestamp;
  double totTime = lastTime - firstTime;
  totTime *= 1E-9; // convert to seconds
  cout << hB->GetNbinsX() << endl;
  hB->Scale(1./totTime);
  // **********************
  // *** read in experiment
  // **********************
  auto fS = new TFile(eF);
  TTree *tS;
  fS->GetObject(channel, tS);
  tS->SetBranchAddress("d",&det);
  tS->SetBranchAddress("E",&energy);
  tS->SetBranchAddress("timestamp",&timestamp);
  // setup histos with the right ranges and bin numbers
  tS->Draw("E >> hS(500,0,1)");
  hS = (TH1F*)gDirectory->Get("hS");
   // scale to counts per second
  tS->GetEntry(0);
  firstTime = timestamp;
  tS->GetEntry(tS->GetEntriesFast()-1);
  lastTime  = timestamp;
  totTime = lastTime - firstTime;
  totTime *= 1E-9; // convert to seconds
  hS->Scale(1./totTime);
  hS->Add(hB,-1);
  hS->Draw("hist"); 
  
}  


