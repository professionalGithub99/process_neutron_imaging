//
#define TOF_cxx
#include "TOF.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
// c++ classes
#include <iostream>
// namespaces
using namespace std;

TOF::TOF()
{
  // set up histograms
  int xLim = 600;
  int timeBins = xLim*2;
  hTOF = new TH1F("hTOF","TOF;Time (ns);Counts",timeBins,-xLim,xLim);
  hTOF_Gam = new TH1F("hTOF_Gam","TOF;Time (ns);Counts",timeBins,-xLim,xLim);
  hTOF_Neu = new TH1F("hTOF_Neu","TOF;Time (ns);Counts",timeBins,-xLim,xLim);
  hNeuEn = new TH1F("hNeuEn","E_{n};Energy [MeV];Counts",500,0,20);
  hEnLO = new TH2F("hEnLO",";E_{n} (MeV);Light Output (MeVee)",500,0,20,500,0,5);
  hTOFLO= new TH2F("hTOFLO",";TOF (ns);Light Output (MeVee)",600,0,600,500,0,2);

  distance = 1; // default to 1 m
  offSet = 0;
  //hDeltaT = new TH1F("hTOF","TOF;Time (ns);Counts",1000,-500,500);
  TString hname[10] = {"h0_1","h0_2","h0_3","h0_4","h0_4"
                       "h0_5","h0_6","h_7","h0_8","h0_9"};
  for(int i=0; i<10; i++)
  {
    hEnBin[i] = new TH1F(hname[i],";;",500,0,2);
  }
}

TOF::~TOF() {;}

void TOF::SetOffSet(double val)
{
  offSet = val; // in ns
}

void TOF::SetDistance(double val)
{
  distance = val; // in m
}

// assumes time0 is the tagging signal and time1 is the detector
double TOF::CalcEn(double time0, double time1)
{
  double mn = 1.6749E-27;
  double jtomev = 6.242E12;
  double c = 299792458; // (m/s)
  deltaT = (time1 - time0) - offSet + distance / .3;
  if(deltaT<0) deltaT = deltaT + 600;
  //hTOF->Fill(deltaT);
  //time = time * 1E-9;
  // assume classical
  double velocity = distance / (deltaT * 1E-9);
  //energy = 0.5 * mn * pow(velocity,2) * jtomev;
  // relativistic
  energy = mn * pow(c,2) * ( 1 / sqrt(1 -  (pow(velocity,2)/pow(c,2))) -1);
  energy *= jtomev;
  return energy;
}

void TOF::FillHistos(int partID, double lo)
{
  // part ID: gamma = 0, neutron = 1
  if(partID == 0) hTOF_Gam->Fill(deltaT);
  if(partID == 1)
  {
    hTOF_Neu->Fill(deltaT);
    hEnLO->Fill(energy,lo);
    hTOFLO->Fill(deltaT,lo);
  }

  hTOF->Fill(deltaT);
  hNeuEn->Fill(energy);

  // calculate limits
  double startEn = 1; // (MeV)
  double enIncrement = .2;
  const int nIncrements = 10;
  double lEn[nIncrements];
  double uEn[nIncrements];

  for(int i=0; i<nIncrements; i++)
  {
    lEn[i] = startEn + i*enIncrement;
    uEn[i] = lEn[i] + enIncrement;
  }

  if(partID == 1)
  {
    for(int i=0; i<nIncrements; i++)
    {
      if(energy>lEn[i] && energy<=uEn[i])
        hEnBin[i]->Fill(lo);
    }
  }
}

void TOF::PlotHistos()
{
  auto cTOF = new TCanvas();
  cTOF->Divide(1,3);
  hTOF->SetDirectory(0);
  hTOF_Gam->SetDirectory(0);
  hTOF_Neu->SetDirectory(0);
  hNeuEn->SetDirectory(0);

  cTOF->cd(1); hTOF->Draw();
  cTOF->cd(2); hTOF_Gam->Draw();
  cTOF->cd(3); hTOF_Neu->Draw();

  auto cEn = new TCanvas();
  hNeuEn->Draw();

  auto cEnLO = new TCanvas();
  hEnLO->Draw("colz");

  auto cTOFLO = new TCanvas();
  hTOFLO->Draw("colz");

  auto c10 = new TCanvas();
  c10->Divide(2,5);
  for(int i=0; i<10; i++)
  {
    c10->cd(i+1);
    hEnBin[i]->Draw();
  }

  auto c11 = new TCanvas();
  for(int i=0; i<10; i++)
  {
    //hEnBin[i]->Scale(1/)
    hEnBin[i]->SetLineColor(i+40);
    hEnBin[i]->Draw("same");
  }
}

void TOF::PlotHistos(double lo)
{
  PlotHistos();
  auto cEnLO = new TCanvas();
  hEnLO->Draw("colz");
}

void TOF::WriteToFile(TString oFN)
{
  auto oTf = new TFile(oFN,"recreate");
  hTOF->Write();
  hTOF_Gam->Write();
  hTOF_Neu->Write();
  hNeuEn->Write();
  hEnLO->Write();
  hTOFLO->Write();
  for(int i=0; i<10; i++)
    hEnBin[i]->Write();
}
