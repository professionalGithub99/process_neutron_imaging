// g++ FitABG_wrapper.C  `root-config --libs --cflags` -o fit -O3
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
// vectors
//TH2F *h2;
double fitf(double *v, double *par)
{
	double en = v[0]*1000;
	double fitval = par[0] / TMath::Sqrt(en) + par[1]*TMath::Exp(par[2]*en);
  //double fitval = par[0] / TMath::Sqrt(en) + par[1] * en + c;
	return fitval;
}

void PSDGates(TString iFN)
//int FitABG_brute()//(double i_x)
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
  TCanvas *c1 = new TCanvas("c1","test 2 histos",2000,5000); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  //auto h2 = new TH2F("h2","h2;Light Output (MeVee); Counts",500,0,.4,500,0,.45);
  TString channel = "T11";
  TTree *T;// = (TTree*)f->Get(channel);
  TFile *f; 

  //f= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  f = new TFile(iFN);
  f->GetObject(channel, T);
  T->SetBranchAddress("d",&det);
  //T->Draw("d.psd:d.l >> h2(60,0,.6,500,0,.45)","","");
  T->Draw("d.psd:d.l >> h2(40,0,.4,500,0,.45");
  auto h2 = (TH2F*)gDirectory->Get("h2");
  //h2->SetDirectory("h2");
  h2->Draw("colz");
  h2->FitSlicesY(0,0,-1,0,"QN2S");
  
  TH1D *hMean  = (TH1D*)gDirectory->Get("h2_1");// the mean
  TH1D *hSigma = (TH1D*)gDirectory->Get("h2_2");// the sigma

  int nPoints = 40;
  int i_point = 0;
  auto hUpper = new TH1D("hUpper","",nPoints,0,.4);
  //auto cutg = new TCutG("gamUpperCut",nPoints);
 	TGraph *points = new TGraph();
  for(int i=0; i<hUpper->GetNbinsX(); i++)
  {
    auto binVal = hMean->GetBinContent(i+1) + 2*hSigma->GetBinContent(i+1);
    hUpper->SetBinContent(i+1,binVal);
    points->SetPoint(i_point,hUpper->GetBinCenter(i+0),binVal);
    cout << "points->SetPoint(" 
         << i_point << "," << hUpper->GetBinCenter(i+0) << "," << binVal
         << ");" << endl;
    i_point++;
  }
  hUpper->Smooth(2);
  i_point++;
  //points->Draw();
  TSpline3 *s = new TSpline3(hUpper);
  s->SetTitle("sGUpper");
  s->Draw();
  /*h1->Smooth(10);                                                               
  TSpline3 *s = new TSpline3(h1);                                               
  s->SetLineColor(2);                                                           
  //cout << s->GetNp() << endl;                                                 
  TGraph *gDer = new TGraph;                                                  
  int pointN = 0;                                                             
  std::vector<Double_t> xp;                                                     
  std::vector<Double_t> y;                                                      
                                                                                
  for(int x=s->GetXmin(); x<=s->GetXmax(); x++)                                 
  {                                                                             
    xp.push_back(x);                                                            
    y.push_back(s->Derivative(x));                                              
  }*/                                                                             

	//TF1 *func = new TF1("fit", fitf, 0.1, 0.5, 3);
	//func->SetParameters(0.33,0.05,0);

	//points->Fit(func,"P");
  //points->Draw();
  auto fOut = new TFile("out.root","recreate");
  s->Write();

}
