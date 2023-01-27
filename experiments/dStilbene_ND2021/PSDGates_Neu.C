// g++ FitABG_wrapper.C  `root-config --libs --cflags` -o fit -O3
// Date: 2-10-2022
// Project: d-stilbene UND
// Purpose: The main function of this script is to determine the neutron PSD
// bounds for a given detector input. The gamma bounds must be determined firt,
// as they are needed in this script if you select to reject events not in the 
// gamma cut. 
// ROOT
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TString.h>
#include <TH1F.h>
#include <TF1.h>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom1.h"
#include "TError.h"
#include "TStopwatch.h"
// c++ 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
// namespaces
using namespace std;
TH2F *sumh2;
TH2F *h2;
// *** functions ***
void sumAll();
// *** main method ***
void PSDGates_Neu(/*TString iFN*/)
{
  // read in experiment
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

  // setup canvas
  //auto h2 = new TH2F("h2","h2;Light Output (MeVee); Counts",500,0,.4,500,0,.45);
  TString channel = "T11";
  TTree *T;// = (TTree*)f->Get(channel);

  TChain chain("T11");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_30.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_28.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_26.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_24.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_23.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_22.root");
  //chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_21.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_20.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_19.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_18.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_17.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_16.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_15.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_14.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_13.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_12.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_11.root");

  //chain.Draw("d.psd:d.l >> h2(60,0,.6,500,0,.45)","","colz");
  //auto h2 = (TH2F*)gDirectory->Get("h2");
  //f= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  h2 = new TH2F("h2","",40,0,.4,500,0,.45);
  auto h2Full = new TH2F("h2Full","",40,0,.4,500,0,.45);

  //sumh2 = new TH2F("sumh2","",500,0,0.15,500,-0.014,0.3965);
  //f= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  //chain.GetObject(channel, T);
  chain.SetBranchAddress("d",&det);
  int entries = chain.GetEntries();
  cout << "entries: " << entries << endl;
  // read in Gamma TSpline3
  TFile *f1 = new TFile("gamUpperOut_3sig.root","read" );                                       
  auto *tsGam =(TSpline3*)f1->Get("hUpper"); 

  for(int i=0; i<entries; i++)
  {
    chain.GetEntry(i);
    auto checkVal = tsGam->Eval(det.l);    
    //if(checkVal < det.psd) 
    h2Full->Fill(det.l,det.psd);
    if(det.psd > 0.2) h2->Fill(det.l,det.psd);
  }
  h2Full->Draw("colz"); 
  tsGam->Draw("same");
  h2->FitSlicesY(0,0,-1,0,"QN2S");
  
  TH1D *hMean  = (TH1D*)gDirectory->Get("h2_1");// the mean
  TH1D *hSigma = (TH1D*)gDirectory->Get("h2_2");// the sigma

  int nPoints = 40;
  int i_point = 0;
  auto hLower = new TH1D("hLower","",nPoints,0,.4);
  auto hUpperN = new TH1D("hUpperN","",nPoints,0,.4);

  //auto cutg = new TCutG("gamUpperCut",nPoints);
 	TGraph *points = new TGraph();
  for(int i=0; i<hLower->GetNbinsX(); i++)
  {
    auto binVal = hMean->GetBinContent(i+1) - 3*hSigma->GetBinContent(i+1);
    hLower->SetBinContent(i+1,binVal);
    binVal = hMean->GetBinContent(i+1) + 3*hSigma->GetBinContent(i+1);
    hUpperN->SetBinContent(i+1,binVal);

    /*points->SetPoint(i_point,hLower->GetBinCenter(i+0),binVal);
    cout << "points->SetPoint(" 
         << i_point << "," << hLower->GetBinCenter(i+0) << "," << binVal
         << ");" << endl;
    i_point++;*/
  }
  hLower->Smooth(2);
  TSpline3 *sL = new TSpline3(hLower);
  sL->SetTitle("sNULower");
  sL->Draw("same");
  
  hUpperN->Smooth(2);
  TSpline3 *sU = new TSpline3(hUpperN);
  sU->SetTitle("sNuUpper");
  sU->Draw("same");
  
  auto tfOut = new TFile("tfOut.root","recreate");
  sL->Write();
  sU->Write();
  tsGam->Write();
  //tsGam->Draw("same");*/
}
// *** end ***
 
void sumAll()
{
  // read in experiment
  /*typedef struct
  {
    Float_t l;               // Long integral
    Float_t s;               // Short integral
    Float_t amp;             // Amplitude
    Float_t cfd;             // Trigger time
    Float_t psd;             // PSD parameter s/l
    Float_t trg;             // Detector trigger

  } Detector;
  Detector det;
  // setup canvas
  TString channel = "T11";
  TTree *T];// = (TTree*)f->Get(channel);
  TFile *f; 
  TChain chain("h42");
  
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_30.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_28.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_26.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_24.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_22.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_20.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_18.root");
  chain.Add("/media/7jn/T7/ND2021/code/data_Li7/run_16.root");
  */
}  

