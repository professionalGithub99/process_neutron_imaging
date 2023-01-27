//my classes
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
//ngm classes
//#include "NGMMultiFormatReader.h"
//#include "NGMHit.h"
//#include "NGMHitIO.h"
//#include "NGMSystemConfiguration.h"

using namespace std;

LBD::LBD()
{
  setDefaults();
}

LBD::LBD(std::vector<TString> iFN, TString dataType)
{
  setDefaults();
}

void LBD::setDefaults()
{
  cout << "setting LBD defaults\n";
  lutFlg = false;
  h_lut = new TH2F();
  //nPix = nPix; // for LBDs. This variable could change in the future
  //nPMTs = 4;
  // set gain values to one in case you don't have them yet
  for(int i=0; i<nPix; i++) pixGain[i] = 1;
  for(int i=0; i<nPMTs; i++) pmtGain[i] = 1;

  for(int ipmt = 0; ipmt<nPMTs; ipmt++)
  { 
    m_offset[ipmt] = 1;
    m_c[ipmt].m_wp->setPolarity(-1);
    m_c[ipmt].m_wp->setClippedVal(16348);
    m_c[ipmt].m_wp->setBlPnts(10);
    m_c[ipmt].m_wp->setSgBin(11);
    m_c[ipmt].m_wp->setEndBin(200);
    //m_c.m_wp->setClippedVal(14750);
  }
  setPixelCalFactors();
  return;
}

void LBD::loadFile(std::vector<string> iFN, TString dataType)
{
  cout << "Loading file paths.\n";
  //w[0].LoadFile(iFN[0],"p",dataType,1);
  for(int ipmt = 0; ipmt<nPMTs; ipmt++)
  {
    std::string s = iFN[ipmt] + "_pmt_" + std::to_string(ipmt);
    m_c[ipmt].LoadBinFile(iFN[ipmt]);
    m_offset[ipmt] = 1;
  }
}

void LBD::loadFile(TString dataType)
{
  /*for(int ipmt = 0; ipmt<nPMTs; ipmt++)
  {
    TString s = m_iFN[ipmt] + "_pmt_" + std::to_string(ipmt);
    w[ipmt].LoadFile(m_iFN[ipmt],s,dataType,1);
    m_offset[ipmt] = 1;
  }*/
}

void LBD::setPath(std::vector<TString> iFN)
{
  for(int ipmt = 0; ipmt<nPMTs; ipmt++)
  {
    m_iFN.push_back(iFN[ipmt]);
  }
  setDefaults();
}

void LBD::addLUT(TString iFN = "iFN")
{
  cout << "using " << iFN << " for LUT\n";
  auto fA = new TFile(iFN, "READ");
  TString hN = "lut";
  h_lut = (TH2F*)fA->Get(hN);
  lutFlg = true;
}

//histo name is hard coded in
/*void LBD::addNeuCut(TString iFN = "iFN")
{
  cout << "using cuts from " << iFN << endl;
  auto fNCut = new TFile(iFN);
  nCut = (TCutG*)fNCut->Get("CUTG");
  nCut->SetVarX("");
  nCut->SetVarY("");
}

//histo name is hard coded in
void LBD::addGamCut(TString iFN = "iFN")
{
  cout << "using cuts from " << iFN << endl;
  auto fNCut = new TFile(iFN);
  gCut = (TCutG*)fNCut->Get("CUTG");
  gCut->SetVarX("");
  gCut->SetVarY("");
}*/
void LBD::addCut(TString iFN, TString particleType)
{
  cout << "using cuts from " << iFN << " for " << particleType << " cuts.\n";
  //__________________________
  auto fNCut = new TFile(iFN);
  if(particleType == "neutron")
    nCut = (TCutG*)fNCut->Get("CUTG");
  else if(particleType == "gamma")
    gCut = (TCutG*)fNCut->Get("CUTG");
  else{ cout << "ERROR: wrong PID\n";}

  return;
}

// 0 for gamma and 1 for neutron
// checks to see if cut files are loaded. If they are not, then pID is set to -1
void LBD::setPartID()
{
  /*pID = -1;
  if(gCut == NULL) return;
  if(gCut == NULL) return;
  if(gCut->IsInside(m_eTot,m_psp)) pID = 0;
  if(nCut->IsInside(m_eTot,m_psp)) pID = 1;
  */
  return;
}

void LBD::setPMTOffSet(vector<double> offset)
{
  for(int i=0; i<nPMTs; i++) pmtGain[i] = offset.at(i);
  return;
}

void LBD::setPixelGainMap(TString iFN)
{
  cout << "using gains for pixels...\n";
  TFile *fGain = new TFile(iFN);
  auto hG = (TH1F*)fGain->Get("hGain");

  double gain[nPix];
  for(int i=0; i<hG->GetNbinsX(); i++) gain[i] = hG->GetBinContent(i);

  // get max val in gains
  int mV = 0;
  for(int i=0; i<nPix; i++) if(gain[i] > mV)  mV = gain[i];

  // normalize gains across pixels for the 4 PMTs
  for(int i=0; i<nPix; i++) pixGain[i] = mV / gain[i];

  return;
}

// These calibration parameters are based on a Na22 source 1.275 MeV point. 
// This calibrates to MeVee.
void LBD::setPixelCalFactors()
{
  cout << "Setting calibration factors\n";
  double temp[nPix]; 
  for(int i=0; i<nPix; i++) calFactor[i] = temp[i];
}

// Calculates PMT ratio. This includes the gains for the PMTs. It will re-
// write ePmt[i] to the value before. Not sure if this is the way to go or not
// but as of right now it is independent of setPMTEnergy(). The Flood is created
// with gain-matched PMTs, so you need to have the gain matched values to get
// the right pixel.
void LBD::calcPMTRatio()
{
  sTot = 0.;
  lgTot = 0;
  m_ph = 0.;

  for(int ipmt = 0; ipmt<nPMTs; ipmt++)
  {
    m_eTot += m_c[ipmt].m_wp->m_full;

    double sVal = m_c[ipmt].m_wp->m_sg;
    double lVal = m_c[ipmt].m_wp->m_full;
    //cout << w[ipmt].m_sg << endl;
    sTot  += m_c[ipmt].m_wp->m_sg;//(w[ipmt].Energy - w[ipmt].EnergyShort);//sVal;
    lgTot += m_c[ipmt].m_wp->m_full;
    m_ph  += m_c[ipmt].m_wp->m_ph;
  }
  //cout << sTot << " " << lgTot << endl;
  x = (m_c[0].m_wp->m_full + m_c[1].m_wp->m_full) / lgTot;
  y = (m_c[2].m_wp->m_full + m_c[0].m_wp->m_full) / lgTot;
  //cout << x <<" "<<y<<endl;
  setPixNumber();

  //if(!clipped[0] && !clipped[1] && !clipped[2] && !clipped[3]) return true;
  //else{ return false; }
}

// need to calcPMTRatio before this method
int LBD::setPixNumber()
{
  //first check bounds of pix number
  binx = h_lut->GetXaxis()->FindBin(x);
  biny = h_lut->GetYaxis()->FindBin(y);
  pixelNumber = h_lut->GetBinContent(binx,biny);
  if(x<0 || y<0 || x>1 || y>1)
  {
    pixelNumber = -1;
  }
  //cout << pixelNumber << endl;
  //if(pixelNumber == 0) cout << x << " " << y << " " << pixelNumber << endl;
  return  pixelNumber;
}

void LBD::setPixEnergy(int pN)
{
  //for(int i=0; i<4; i++) m_eTot += w[ipmt].m_full;
  pixEn = lgTot * pixGain[pN];
  return;
}

void LBD::calcPixPSP()
{
  double c_psp[4];

  for(int ipmt=0; ipmt<nPMTs; ipmt++)
  {
    double sVal = m_c[ipmt].m_wp->m_sg;
    double lVal = m_c[ipmt].m_wp->m_full;
    c_psp[ipmt] = sVal/lVal;
  }

  m_psp = (c_psp[0] + c_psp[1] + c_psp[2] + c_psp[3]) / 4;
}

bool LBD::processAnLBDEvent(int ientry) //  change this input arguement
{
  bool clipped[nPMTs] = {0};
  clippedFlg = false;
  lgTot = 0;
  sTot  = 0;
  //******* PMT loop ****************
  for(int ipmt=0; ipmt<nPMTs; ipmt++)
  {
    clipped[ipmt] = m_c[ipmt].GetBinEntry();
    if(clipped[ipmt]) clippedFlg = true; 
    phPMT[ipmt] = m_c[ipmt].m_wp->m_ph;
    q_pkTime[ipmt] = m_c[ipmt].m_wp->m_peakPos;
    q_sg[ipmt]  = m_c[ipmt].m_wp->m_sg;
    q_lg[ipmt]  = m_c[ipmt].m_wp->m_full;
    q_psp[ipmt] = m_c[ipmt].m_wp->m_psp;  
    if(q_lg[ipmt]>0 && q_sg[ipmt]>0) lgTot += q_lg[ipmt];
    if(q_lg[ipmt]>0 && q_sg[ipmt]>0) sTot += q_sg[ipmt];
  }
  // try something different
  /*TH1I *sumHisto;
  TH1I *indHisto[4];
  for(int ipmt=0; ipmt<nPMTs; ipmt++)
  {
    clipped[ipmt] = m_c[ipmt].GetEntry();
    auto theHisto = m_c[ipmt].m_wp->returnHisto();
    //returnHisto // WFProcess 
    if(ipmt==0) sumHisto = (TH1I*)theHisto->Clone();
    else{ sumHisto->Add(theHisto);}
    indHisto[ipmt] = (TH1I*)theHisto->Clone();
  }
  // get the peak position of the summed WF
  int pkPos = sumHisto->GetMaximumBin();
  sTot = 0;
  lgTot = 0;
  for(int i=0; i<4; i++)
  {
    double val = indHisto[i]->Integral(pkPos+8,pkPos+150);
    sTot += val;
    val = indHisto[i]->Integral(pkPos-10,pkPos+150);  
  }  
  // set x,y
  x = (m_c[0].m_wp->m_full + m_c[1].m_wp->m_full) / lgTot;
  y = (m_c[2].m_wp->m_full + m_c[0].m_wp->m_full) / lgTot;
  */

  pspTot = sTot / lgTot;
  calcPMTRatio();
  setPixNumber();
  setPixEnergy(pixelNumber); // includes pixel gains
  calcPixPSP();
  setPartID();
  return clippedFlg;
}

bool LBD::checkTimeAlignment()
{
  double timeCriteria = 6; // in samples (2 ns = 1 sample)
  bool goodPulse = false;
  if(TMath::Abs(q_pkTime[0] - q_pkTime[1]) < timeCriteria&&
  TMath::Abs(q_pkTime[0] - q_pkTime[2]) < timeCriteria &&
  TMath::Abs(q_pkTime[0] - q_pkTime[3]) < timeCriteria &&
  TMath::Abs(q_pkTime[1] - q_pkTime[2]) < timeCriteria &&
  TMath::Abs(q_pkTime[1] - q_pkTime[3]) < timeCriteria &&
  TMath::Abs(q_pkTime[2] - q_pkTime[3]) < timeCriteria )
  {
    goodPulse = true;
  }
  
return goodPulse;

}
// process pulse info for PMT channels
void LBD::processARawLBDEvent(int ientry)
{
  bool clipped[nPMTs] = {0};
  clippedFlg = false;
  lgTot = 0;
  sTot  = 0;
  for(int ipmt=0; ipmt<nPMTs; ipmt++)
  {
    clipped[ipmt] = m_c[ipmt].GetBinEntry();
    if(clipped[ipmt]) clippedFlg = true; 
    phPMT[ipmt] = m_c[ipmt].m_wp->m_ph;
    q_pkTime[ipmt] = m_c[ipmt].m_wp->m_peakPos;
    q_sg[ipmt]  = m_c[ipmt].m_wp->m_sg;
    q_lg[ipmt]  = m_c[ipmt].m_wp->m_full;
    q_psp[ipmt] = m_c[ipmt].m_wp->m_psp;  
    if(lgTot>0) lgTot += q_lg[ipmt];
    if(lgTot>0) sTot += q_sg[ipmt];
    //if(ipmt == 0) cout << m_c[ipmt].m_wp->m_ph << endl;
  }
  pspTot = sTot / lgTot;
  return;
}  

int LBD::getNEntries(int pmtN)
{
  //return w[0].GetNEntries();
}

void LBD::processAllLBDEvents()
{
  //for(Long64_t ientry = 0; ientry<w[0].GetNEntries(); ientry++)
  //{
    //processAnLBDEvent(ientry);
    //if(ientry%25000 == 0) cout << ientry << " events processed...\n";
  //}
}

// This mapping considers the lut calculate from the numbering from the back of
// the PMT. See PMT numbering below
//         |3|1|
//         |4|2|
double LBD::getX(int n)
{
  //return x;
  if(n%10 == 0) return 9;
  else if(n%10 == 1) return 8;
  else if(n%10 == 2) return 7;
  else if(n%10 == 3) return 6;
  else if(n%10 == 4) return 5;
  else if(n%10 == 5) return 4;
  else if(n%10 == 6) return 3;
  else if(n%10 == 7) return 2;
  else if(n%10 == 8) return 1;
  else if(n%10 == 9) return 0;
}

double LBD::getY(int n)
{
  //return y;
  if(n<10) return 0;
  else if(n>9  && n<20) return 1;
  else if(n>19 && n<30) return 2;
  else if(n>29 && n<40) return 3;
  else if(n>39 && n<50) return 4;
  else if(n>49 && n<60) return 5;
  else if(n>59 && n<70) return 6;
  else if(n>69 && n<80) return 7;
  else if(n>79 && n<90) return 8;
  else {return 9;}
}

void LBD::CreatePixelByPixelPH(TString oFN = "pixbypix.root")
{
  // set up histos
  TH1F *hInt[nPix];
  TH1F *hPh[nPix];
  TH1F *hC[nPix];
  TH2F *hPSDE[nPix];
  for (int i=0; i<nPix; i++)
  {
    string hname = "hInt_" + to_string(i);
    string htitle = "pixel_" + to_string(i);
    hInt[i] = new TH1F(hname.c_str(), htitle.c_str(),1000,0,200000);
    
    hname = "hPh_" + to_string(i);
    htitle = "pixel_" + to_string(i);
    hPh[i] = new TH1F(hname.c_str(), htitle.c_str(),500,0,5000);

    hname = "hc_" + to_string(i);
    htitle = "pixel_" + to_string(i);
    hC[i] = new TH1F(hname.c_str(), htitle.c_str(),400,0,20);
    
    hname = "hpsde_" + to_string(i);
    htitle = "pixel_" + to_string(i);
    hPSDE[i] = new TH2F(hname.c_str(), htitle.c_str(),500,0,200000,250,0,1);
  }
  TH2F *h2 = new TH2F("h2",";Pixel Number;Raw Pulse Integral (ADC)",nPix,0,nPix,500,0,100000);
  cout << "_____processing events_____\n";
  //for(int i=0; i<w[0].GetNEntries(); i++)
  //for(int i=0; i<500000; i++)
  while( !m_c[0].CheckEndOfFile() && !m_c[1].CheckEndOfFile() &&
         !m_c[2].CheckEndOfFile() && !m_c[3].CheckEndOfFile()   ) 
  {
    processAnLBDEvent(0); // change this to a different input arguement
    int pN = getPixelNumber();
    double e = lgTot;
    //cout << pN << " " << e << endl;
    // Your pixel number can be negative if the pixel cannot be found from the 
    // look-up-table. The 'if' loop must at least check the the pix number is
    // greater than 0.
    bool b = checkTimeAlignment();
    if(pN>=0 && b)// && pID == 0)
    {
      bool c = false; 
      for(int i=0; i<4; i++) if(m_c[i].m_wp->getClippedVal() == true) c = true; 
      hInt[pN]->Fill(e);//m_ph);//e);
      hPh[pN]->Fill(m_ph);
      hC[pN]->Fill(e * calFactor[pN]);//e);
      h2->Fill(pN,e);
      hPSDE[pN]->Fill(e,sTot/lgTot);
    }
  }
  // create and write to file
  TFile *file = new TFile(oFN, "RECREATE");
  for (int i=0; i<nPix; i++){
    hInt[i]->Write();
    hPh[i]->Write();
    hPSDE[i]->Write();
  }
  h2->Draw("colz");
  h2->Write();
}


