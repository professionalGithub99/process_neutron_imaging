//Date: 5-1-2019
//author: J. Nattress
//version: 01 (4-17-2019)
#ifndef _LBDCalibration_H_
#define _LBDCalibration_H_
//Root classes
#include <TString.h>
#include <vector>
#include <WFProcess.h>
#include <CAEN.h>
//c++ classes
//user classes
using namespace std;

//namespace MYNGM{

class LBDCalibration
{
  private:
    WFProcess w[4];
    CAEN m_c[4];
    int numOfPmts;
    double m_offset[4];
    TH1F *hPSP;

  public:
    LBDCalibration(std::vector<std::string> iFN, TString dataType);

    void setPMTOffSet(vector<double> offset);
    void createFlood(TString oFN);
    // run through events and return a FOM based on short and long gate position
    // from the trigger. Requires an NGM file with waveforms saved
    double optimizeFOM(TString oFN);

    void plot_4_Wfs(int eventNum);

    double calcFOM();
    // some histograms
    TH1F   *hE[4];
};
//}

#endif
