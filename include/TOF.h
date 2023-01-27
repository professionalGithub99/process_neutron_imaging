//
#ifndef TOF_h
#define TOF_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TCutG.h>

// Header file for the classes stored in the TTree if any.

class TOF {
private:
  double deltaT;
  double offSet;
  double distance;
  double energy;

public :
  TOF();
  virtual ~TOF();
  double GetDeltaT(){return deltaT;}
  void   SetOffSet(double offSet); // in ns
  void   SetDistance(double distance); // in m
  double CalcEn(double time0, double time1); // time in ns; return En in MeV
  void   FillHistos(int partID, double lo);
  void   PlotHistos();
  void   PlotHistos(double lo);
  void   WriteToFile(TString oFN);

public :
  TH1F *hTOF;// = new TH1F("hTOF","TOF;Time (ns);Counts",1000,-500,500);
  TH1F *hTOF_Gam;// = new TH1F("hTOF","TOF;Time (ns);Counts",1000,-500,500);
  TH1F *hTOF_Neu;// = new TH1F("hTOF","TOF;Time (ns);Counts",1000,-500,500);
  TH1F *hNeuEn;
  TH2F *hEnLO;
  TH2F *hTOFLO;
  TH1F *hEnBin[10];

};

#endif
