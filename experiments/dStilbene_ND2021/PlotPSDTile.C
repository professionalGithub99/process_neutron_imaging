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
TH2F *h2;

void PlotPSDTile()
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
  c1->Divide(1,6,0,0); 
  //c1->SetFillAttributes(0);
  h2 = new TH2F("h2",";Light Output (MeVee); Counts",500,0,0.15,500,-0.014,0.3965);
  TString channel = "T11";
  TTree *T[10];// = (TTree*)f->Get(channel);
  TFile *f[10]; 

  f[0]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_32.root");
  f[0]->GetObject(channel, T[0]);
  T[0]->SetBranchAddress("d",&det);
  c1->cd(1);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);
  
  T[0]->Draw("d.psd:d.l >> h32(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h32 = (TH2F*)gDirectory->Get("h32");
  h32->SetTitle("Below Threshold");

  f[1]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_30.root");
  f[1]->GetObject(channel, T[1]);
  T[1]->SetBranchAddress("d",&det);
  c1->cd(2);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[1]->Draw("d.psd:d.l >> h30(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h30 = (TH2F*)gDirectory->Get("h30");
  h30->SetTitle("E_{n} = 0.17 MeV");

  //h2->Draw(); 
  f[2]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_28.root");
  f[2]->GetObject(channel, T[2]);
  T[2]->SetBranchAddress("d",&det);
  c1->cd(3);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[2]->Draw("d.psd:d.l >> h28(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h28 = (TH2F*)gDirectory->Get("h28");
  h28->SetTitle("E_{n} = 0.39 MeV");

  f[3]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_26.root");
  f[3]->GetObject(channel, T[3]);
  T[3]->SetBranchAddress("d",&det);
  c1->cd(4);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[3]->Draw("d.psd:d.l >> h26(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h26 = (TH2F*)gDirectory->Get("h26");
  h26->SetTitle("E_{n} = 0.58 MeV");

  f[4]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_24.root");
  f[4]->GetObject(channel, T[4]);
  T[4]->SetBranchAddress("d",&det);
  c1->cd(5);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[4]->Draw("d.psd:d.l >> h24(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h24 = (TH2F*)gDirectory->Get("h24");
  h24->SetTitle("E_{n} = 0.79 MeV");

  f[5]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_22.root");
  f[5]->GetObject(channel, T[5]);
  T[5]->SetBranchAddress("d",&det);
  c1->cd(6);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[5]->Draw("d.psd:d.l >> h22(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h22 = (TH2F*)gDirectory->Get("h22");
  h22->SetTitle("E_{n} = 0.98 MeV");

  /*f[6]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_20.root");
  f[6]->GetObject(channel, T[6]);
  T[6]->SetBranchAddress("d",&det);
  c1->cd(7);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[6]->Draw("d.psd:d.l >> h20(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h20 = (TH2F*)gDirectory->Get("h20");
  h20->SetTitle("E_{n} = 1.17 MeV");

  //h2->Draw(); 
  f[7]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_18.root");
  f[7]->GetObject(channel, T[7]);
  T[7]->SetBranchAddress("d",&det);
  c1->cd(6);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[7]->Draw("d.psd:d.l >> h18(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h18 = (TH2F*)gDirectory->Get("h18");
  h18->SetTitle("E_{n} = 1.36 MeV");

  f[8]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_16.root");
  f[8]->GetObject(channel, T[8]);
  T[8]->SetBranchAddress("d",&det);
  c1->cd(8);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[8]->Draw("d.psd:d.l >> h16(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h16 = (TH2F*)gDirectory->Get("h16");
  h16->SetTitle("E_{n} = 1.56 MeV");

  f[9]= new TFile("/media/7jn/T7/ND2021/code/data_Li7/run_14.root");
  f[9]->GetObject(channel, T[9]);
  T[9]->SetBranchAddress("d",&det);
  c1->cd(10);
  gPad->SetGridx(); 
  gPad->SetFillColor(0);
  c1->SetFillColor(0);

  T[9]->Draw("d.psd:d.l >> h14(500,0,0.15,500,-0.014,0.3965)","","col");
  auto h14 = (TH2F*)gDirectory->Get("h14");
  h14->SetTitle("E_{n} = 1.75 MeV");
  gPad->SetFillColor(0);
  c1->SetFillColor(0);
  */
}
