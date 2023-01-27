//Date: 5-1-2019
//author: J. Nattress
//purpose: create base classes to interact with NGM files
//version: 01 (4-17-2019)
#ifndef _LBD_H_
#define _LBD_H_
//Root classes
#include <TString.h>
#include <TFile.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TCutG.h>
//c++ classes
//NGM classes
//#include <NGMHit.h>
//#include <NGMSystemConfiguration.h>
#include <WFProcess.h>
#include <CAEN.h>
using namespace std;

class LBD{// : public CAENWf{
  private:
    TH2F *h_lut;
    Int_t pixelNumber;
    bool lutFlg;
    TCutG *gCut;
    TCutG *nCut;
    const int nPix = 64;
    const int nPMTs = 4;
    double m_offset[4];
  public:
    CAEN m_c[4];
    bool clippedFlg;
    int numOfSamples;
    double ePmt[4];
    double phPMT[4];
    double q_pkTime[4];
    double m_eTot;
    double lgTot;
    double q_lg[4];
    double sTot;
    double q_sg[4];
    double m_ph;
    double m_psp;
    double pspTot;
    double q_psp[4];
    double pixEn;
    double calFactor[64];
  private:  
    float x, y;
    Int_t binx, biny;
    double pixGain[64];
    double pmtGain[4];
    std::vector<TString> m_iFN;
    int pID;

  public:
    LBD();
    LBD(std::vector<TString> iFN, TString dataType);
    void setDefaults();
    void loadFile(std::vector<std::string> iFN, TString dataType);
    void loadFile(TString dataType);
    void setPath(std::vector<TString> iFN);
    void setPixelCalFactors();
    void addLUT(TString iFN);
    void addCut(TString iFN, TString particleType);

    void setPartID();
    int  getPartID(){return pID;}
    void setPMTOffSet(vector<double> offset);
    void setPixelGainMap(TString iFN);

    void   calcPMTRatio();
    int    setPixNumber();
    inline Int_t getPixelNumber(){return pixelNumber;}
    inline bool checkClipped(){return clippedFlg;}
    void   setPixEnergy(int pN);
    double getPixEnergy(){return pixEn;}
    double getLg(){return lgTot;}
    // returns pulse height for an individual PMT in the quad
    double getPMTPH(int nPMT) {return phPMT[nPMT];} 

    bool   checkTimeAlignment();

    double getX(int n);
    double getY(int n);

    void   calcPixPSP();
    inline double getPSP(){return m_psp;}

    bool processAnLBDEvent(int ientry);
    void processARawLBDEvent(int ientry);

    int  getNEntries(int pmtN);
    void processAllLBDEvents();
    void CreatePixelByPixelPH(TString oFN);

//ClassDef(LBD, 1)
};


#endif
