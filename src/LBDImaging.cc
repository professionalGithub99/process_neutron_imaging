//my classes
#include "LBDImaging.h"
#include "LBD.h"
//c++ classes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
//Root classes
#include <TFile.h>
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TSpline.h"
#include "TCanvas.h"
#include "TString.h"

using namespace std;

LBDImaging::LBDImaging()
{
  setDefaults();
}

LBDImaging::LBDImaging(LBD *p_lbd0,LBD *p_lbd1,LBD *p_lbd2,LBD *p_lbd3)
{
  setDefaults();
  for(int i=0; i<4; i++) mp_lbd[i] = new LBD;
  mp_lbd[0] = p_lbd0;
  mp_lbd[1] = p_lbd1;
  mp_lbd[2] = p_lbd2;
  mp_lbd[3] = p_lbd3;
}

void LBDImaging::setDefaults()
{
  m_threshold = 0;
  m_r = 2;
  m_c = 2;
  int nXBins = 10 * m_r;
  int nYBins = 10 * m_c;
  hImage = new TH2F("hImage","",20,0,20,20,0,20);
  hGammaImage = new TH2F("hGammaImage","",20,0,20,20,0,20);
  hNeutronImage = new TH2F("hNeutronImage","",20,0,20,20,0,20);

  hRef = new TH2F("hRef","",20,0,20,20,0,20);

  int cn = 0;
  for(int c=0; c<20; c++)
  {
    for(int r=0; r<20; r++)
    {
      hRef->SetBinContent(r+1,c+1,cn);
      cn++;
    }
  }
}

// The below method assumes a 2x2 LBD array. The specific numbering of the
// detectors is required and is as follows:
//              |2|3|
//              |0|1|
int LBDImaging::getGlobalPix(int x, int y)
{
  auto g = hRef->GetBinContent(x,y);
  return g;
}

int LBDImaging::calcX(int dN, int pN)
{
  int g = 0;
  if(dN == 0 || dN == 2) g = pN%10;
  if(dN == 1 || dN == 3) g = pN%10 + 10;
  //cout << g << " ";
  return g;
}

int LBDImaging::calcY(int dN, int pN)
{
  int g = 0;
  if(dN == 0 || dN == 1) g = pN/10;
  if(dN == 2 || dN == 3) g = pN/10 + 10;
  //cout << g << " ";
  return g;
}

void LBDImaging::process()
{
  TString lut[4];
  lut[0] = "/home/7jn/code/CAEN/luts/sn1132_ch0_3_det2_lut.root";
  lut[1] = "/home/7jn/code/CAEN/luts/sn1132_ch4_7_det3_lut.root";
  lut[2] = "/home/7jn/code/CAEN/luts/sn1102_ch0_3_det0_lut.root";
  lut[3] = "/home/7jn/code/CAEN/luts/sn1102_ch4_7_det1_lut.root";

  for(int i=0; i<4; i++)
  {
    cout << "___processing___|" << i << "| LBD\n";
    //m_lbd[i].addLUT(lut[i]);
    for(int ientry=0; ientry< m_lbd[i].getNEntries(i); ientry++)
    {
      if(ientry%25000 == 0) cout << ientry << " events processed...\n";
      bool clipped = m_lbd[i].processAnLBDEvent(ientry);
      int x = calcX(i,m_lbd[i].setPixNumber());
      int y = calcY(i,m_lbd[i].setPixNumber());
      if(!clipped && m_lbd[i].getLg() > 15000) hImage->Fill(x,y);
    }
  }
  hImage->SetDirectory(0);
  hImage->Draw("colz");
  auto tf = new TFile("image.root","recreate");
  hImage->Write();
}

void LBDImaging::processP(TString oFN = "temp_image.root")
{
  for(int i=0; i<4; i++)
  {
    cout << "___processing___|" << i << "| LBD\n";
    for(int ientry=0; ientry< mp_lbd[i]->getNEntries(i); ientry++)
    {
      if(ientry%25000 == 0)
      {
        cout << "events processed: \t" << 100*ientry/mp_lbd[i]->getNEntries(i)
             << "%" << "\t\r" << std::flush;
      }

      bool clipped = mp_lbd[i]->processAnLBDEvent(ientry);
      int x = calcX(i,mp_lbd[i]->setPixNumber());
      int y = calcY(i,mp_lbd[i]->setPixNumber());
      if(!clipped && mp_lbd[i]->getLg() > m_threshold)
      {
        hImage->Fill(x,y);
        if(mp_lbd[i]->getPartID() == 0) hGammaImage->Fill(x,y);
        if(mp_lbd[i]->getPartID() == 1) hNeutronImage->Fill(x,y);
      }
    }
  }
  hImage->SetDirectory(0);
  auto tc = new TCanvas;
  tc->Divide(1,3);
  tc->cd(1);
  hImage->Draw("colz");
  tc->cd(2);
  hGammaImage->Draw("colz");
  tc->cd(3);
  hNeutronImage->Draw("colz");
  auto tf = new TFile(oFN,"recreate");
  hImage->Write();
  hGammaImage->Write();
  hNeutronImage->Write();
}
