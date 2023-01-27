//Date: 5-1-2019
//author: J. Nattress
//purpose: create base classes to interact with NGM files
//version: 01 (4-17-2019)
#ifndef _LBDImaging_H_
#define _LBDImaging_H_
// ROOT classes
#include <TString.h>
#include <TFile.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TCutG.h>
// my classes
#include <LBD.h>

using namespace std;

class LBDImaging{

  private:
    LBD m_lbd[4];
    LBD *mp_lbd[4];
    //LBD m_lbd1;
    //LBD m_lbd2;
    //LBD m_lbd3;

    int m_r; // number of rows
    int m_c; // number of columns
    double m_threshold; // default is 0 (hardware)
    const int nPix = 100;
    int globalPix;
    void setDefaults();
  public:
    TH2F *hImage;
    TH2F *hGammaImage;
    TH2F *hNeutronImage;
    TH2F *hRef;

  public:
    LBDImaging();
    LBDImaging(LBD *p_lbd0,LBD *p_lbd1,LBD *p_lbd2,LBD *p_lbd3);
    //LBDImaging(LBD&, LBD&, LBD&, LBD&);
    //void setLBDs(LBD& l0, LBD& l1, LBD& l2, LBD& l3);
    void setNumRowsCols(int r, int c);
    void setThreshold(double t) {m_threshold = t;}
    int calcX(int dN, int pN);
    int calcY(int dN, int pN);
    int getGlobalPix(int detNum, int pix);
    void process();
    void processP(TString oFN);

};


#endif
