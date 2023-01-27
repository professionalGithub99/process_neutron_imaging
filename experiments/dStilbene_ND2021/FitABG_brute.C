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
vector<double> sv; // sim values of deposited energy spectrum 
vector<double> ev; // measured ev value 
// histograms
TH1F *hSim;
TH1F *hExp;
TF1 *g2;
// variables
double expMean;
double sR, eR;
// **** fit function ****
static Double_t EvaluateRes(double a, double b, double g)
{
  hSim->Reset(); // clear out old data in histogram
  double light;
  double depEn;
  double res;
  vector<double> ls;
  TRandom1 rand; // 1D random number
  // *** edep loop ***
  for(auto i_dE : sv)
  {
    res = sqrt( pow(a,2) + pow(b,2) / i_dE + pow(g/i_dE,2) );
    double sig = res / 2.35;
    // smear the light output
    light = rand.Gaus(i_dE, sig * i_dE);
	  hSim->Fill(light);//*x_s);
    ls.push_back(light);
  }
  // fit Gaus to smeared sim for x shift 
  // fit Gaus to edge of experimental spectrum to align
  hSim->Fit(g2,"QR");
  auto simMean = g2->GetParameter(1); 
  double xShift = expMean / simMean; // horizontal scale
  hSim->Reset();
  for(auto i : ls) hSim->Fill(i*xShift);
  // scale 
  double scaleVal = hExp->Integral();
  hSim->Scale(scaleVal/hSim->Integral()); // vertical scale
  // calc rchi2
  double rss = 0; // reduced sum of squares
  for(int i=0; i<hSim->GetNbinsX(); i++)
  {
    auto val = hSim->GetBinContent(i+1) - hExp->GetBinContent(i+1);
    val *= val; // square it
    val /= TMath::Sqrt(ev[i]);// hExp->GetBinContent(i+1));
    rss += val; // sum it
  }
  double numOfPars = 4;
  double ndf = hSim->GetNbinsX() - numOfPars; // degrees of freedom
  auto rchi2 = rss/ndf;
  
  return rchi2;
}

int main()
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

  TTree *T;// = (TTree*)f->Get(channel);
  TFile *f = new TFile("/media/7jn/T7/ND2021/code/data_cal/run_0.root");
  TString channel = "T11";
  f->GetObject(channel, T);
  //T->Branch("d",&det,"l:s:amp:cfd:psd:trg");
  T->SetBranchAddress("d",&det);
  cout << "exp entries: " << T->GetEntriesFast() << endl;
  double cF = 0.477/24995; // initial calibration factor
  for(int i=0; i<T->GetEntries(); i++)
  {
    T->GetEntry(i);
    ev.push_back(det.l * cF);
  }
  // need to change these limits depending on the source that you use
  int nBins = 34;
  sR = 0.35;  // [MeV]
  eR = 0.52; // [MeV]
  hExp = new TH1F("hExp","",nBins,sR,eR);
  hSim = new TH1F("hSim","",nBins,sR,eR);
  T->Draw("d.l*0.477/24995 >> hExp");
  // fit Gaus to edge of experimental spectrum to align
  TF1 *g1    = new TF1("g1","gaus",0.42,eR);
  g2    = new TF1("g2","gaus",0.42,eR);

  hExp->Fit(g1,"RQ");
  expMean = g1->GetParameter(1); 

  // read in simulation deposited energy
  TTree *t;// = (TTree*)f->Get(channel);
  TFile *sf = new TFile("/home/7jn/ldropbox/G4Projects/singlePix/suzie_build/_cs137_0.662_0.477.root");
  channel = "t1";
  sf->GetObject(channel, t);
  double edep;
  t->SetBranchAddress("edep",&edep);
  cout << "sim entries: " << t->GetEntriesFast() << endl;
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    if(edep>0) sv.push_back(edep); // in my sim I set a value to -999 for
                                   // uniformity
  }
  // fill x_s vector
  //vector<double> v_x_s;
  //for(int i=0; i<20; i++)
    //v_x_s.push_back(0.9 + i*0.01);
  // fill a vector
  vector<double> v_a;
  for(int i=0; i<1000; i++)
    v_a.push_back(i*0.001);

  vector<double> v_b;
  for(int i=0; i<100; i++)
    v_b.push_back(0.01+i*0.001);

  vector<double> v_g;
  for(int i=0; i<10; i++)
    v_g.push_back(i*0.001);
  int totIts = v_b.size() * v_a.size() * v_g.size();
  //for(auto i : v_a) cout << i << endl; 
  double bestChi = 0;
  int counter = 0;
  TStopwatch ts;
  ts.Start();
  //for(auto i_x : v_x_s)
  //{
    //double i_x = 1.01;
    for(auto i_a : v_a)
    {
      for(auto i_b : v_b)
      {
        for(auto i_g : v_g)
        {
          auto theChi = EvaluateRes(i_a,i_b,i_g); 
          if(bestChi == 0)
          {
            bestChi = theChi;
            cout << "1st best: " << bestChi << endl;
          }
          if(theChi < bestChi)
          {
            cout << "( " << i_a << ", " << i_b << ", " << i_g << " )"  
                 <<  " ---> " << theChi << endl;
            bestChi = theChi;
          } 
          if(counter%1000 == 0) 
          {
            auto evtRate = counter / ts.RealTime();
            cout << evtRate << " evts/s " << " ";
            cout << "( " << 100.*counter/totIts << "% complete )\n";
            ts.Continue();
          }
          counter++; 
        }
      }
    }
  //}

  hExp->Draw("hist");
  hSim->SetLineColor(3);
  hSim->Draw("same hist");
  return 0;
}  
