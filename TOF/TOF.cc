#include <iostream>
#include <thread>
#include <vector>
#include <TString.h>
#include <cstdlib>
#include <Compass.h>
#include <CompassWf.h>
#include <TOF.h>
// namespaces
using namespace std;
//
//
int main(int argc, char** argv)
{

  CompassList det[2];

  det[0].LoadFile("/media/7jn/gogo/10.08.2020_CfChamber/DAQ/Cf_Time_4/RAW/DataR_CH0@DT5730_1132_Cf_Time_4.root", "Det0", true);
  det[1].LoadFile("/media/7jn/gogo/10.08.2020_CfChamber/DAQ/Cf_Time_4/RAW/DataR_CH1@DT5730_1132_Cf_Time_4.root", "Det1", true);

  det[0].SetCalFactor(1.);
  double c = 0.662/257;
  det[1].SetCalFactor(-218.5,10504);
  //det[1].AddNeuCut("/home/7jn/code/Compass/CutData/LLNL_EJ309_NeuCut.root");
  //det[1].AddGamCut("/home/7jn/code/Compass/CutData/LLNL_EJ309_GammaCut.root");

  //det1.FillLoop();
  //auto tf = new TFile("o.root","recreate");

  TOF tof;
  tof.SetOffSet(33.2); // time offset from gam-gam TOF distr
  int nentries = det[1].GetNEntries();
  for(int n=0; n<nentries; n++)
  {
    for(int dn=0; dn<2; dn++) det[dn].ProcessAnEvent(n);
    double t1 = det[1].Timestamp / 1000; // convert to ns
    double t0 = det[0].Timestamp / 1000; // convert to ns
    auto deltaT = (t1 - t0);
    //cout << deltaT << endl;
    auto en = tof.CalcEn(t0,t1); //cout << en << endl;
    tof.FillHistos(det[1].partID, det[1].calE);
  }

  for(int dn=0; dn<2; dn++)
  {
    det[dn].hPSPE_cal->SetDirectory(0);
    det[dn].hE_cal->SetDirectory(0);

    for(int i=0; i<2; i++)
    {
      det[dn].hPartE_cal[i]->SetDirectory(0);
      det[dn].hPartPSPE_cal[i]->SetDirectory(0);
    }
  }

  auto c0 = new TCanvas;
  det[1].hPartE_cal[1]->Draw("colz");

  auto c1 = new TCanvas;
  det[1].hPSPE_cal->Draw("colz");

  auto c2 = new TCanvas;
  det[1].hPartPSPE_cal[1]->Draw("colz");

  tof.PlotHistos();

  return 0;
}
