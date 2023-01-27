#define WFProcess_cxx
#include "WFProcess.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSpectrum.h>
#include <TDirectory.h>
#include <TMath.h>
// c++ classes
#include <iostream>
#include <string>
// namespace
using namespace std;

WFProcess::WFProcess()
{
  hwf = NULL;
  hwfV = NULL;
  defaults = false;
  blPnts = 10;
  endBin = 100;
  sgBin = 9;
  sFreq = 2; // 2 ns for 5730
  dRange = 2; // 2 vpp for 5730
  threshold = 0; // hardware threshold is implicit
  clipped = false;
  clippedVal = 16384; // assume 14-bit digitizer
  voltConversion = 0.00012207031; //it should be equivalent to dRange/clippedVal
  polarity = -1; // assumes negative pulse
  //printWfLimits();
}

WFProcess::~WFProcess()
{}

void WFProcess::printWfLimits()
{
  cout << "******\n"
       << "WF limits\n"
       << "******\n";
  cout << "blPnts: " << blPnts << "\nendBin: " << endBin
       << "\nsgBin: " << sgBin << "\nsFreq: " << sFreq << " ns"
       << "\nClipped val: " << clippedVal << endl;
}

// plots raw waveform with no baseline subtraction
TH1I* WFProcess::getAWfHisto(std::vector<int> aWf)
{
  int ss = aWf.size();
  if(hwf == NULL) hwf = new TH1I("aWf","",ss,0,ss);
  cout << "Number of samples: " << ss << endl;
  auto h1 = new TH1I("aWf","",ss,0,ss);
  
  for(int s=0; s<ss; s++)
    h1->Fill(s+1,aWf.at(s));

  return h1;
}

bool WFProcess::processASingleWf(std::vector<int> v_aWf,int waveCount)
{
  //cout << sgPnt << " " << lg << endl;
  //printWfLimits();
  int ss = v_aWf.size();
  v_currentWave = v_aWf; 
  // setup wave histo if this is the 1st event
  if(hwf == NULL)
  {
    hwf = new TH1I("aWf","",ss,0,ss);
    if(endBin > ss) 
      cout << "WARNING: full integration limit is > # of samples\n"; 
  }
  if (hwfV == NULL)
  {
    hwfV = new TH1F("aWfV","Handful of Samples;ns;Volts",ss,0,ss*sFreq);
    if(endBin > ss) 
      cout << "WARNING: full integration limit is > # of samples\n";
  }
  // calc bl
  int bl = 0;
  int sP = 1;
  for(int i=sP; i<(blPnts+sP); i++) bl += v_aWf[i];
  bl = bl / blPnts;
  m_startBl = bl;
  
  m_endBl = 0;
  sP = (v_aWf.size() - (blPnts+1));
  for(int i = sP; i<(sP+blPnts); i++) m_endBl += v_aWf[i];
  m_endBl /= blPnts;
  //cout << "bl: " << bl << " " << m_endBl << endl;
  
  clipped = false;
  //clipped max value is smaller than the actual dynamic range of the digitizer
  //This is because the voltage limit of the board is smaller than the voltage
  //limit of the digitizer. The board has a limit of 3.5 volts, whereas the
  //digitizer I am currently using has a range of 2 volts. Once you terminate
  //the board (50 ohms), this reduces your dymanic range to about 12000 ADCs.
  //int clipVal = 12000; // this is the clipped value for LBD
  //dynamic range – baseline
  //int clipVal = 16384 - bl;
  for(int s=0; s<ss; s++)
  {
    if(v_aWf[s] >= clippedVal ) 
      clipped = true;
      //cout << "clipped";
    // fill hwf with values from vector
    if(polarity < 0)
    {  
      hwf->SetBinContent(s+1,bl - v_aWf[s]);
      //if( (bl - v_aWf[s]) >= clippedVal )clipped = true; 
    }
    else{
      hwf->SetBinContent(s+1,v_aWf[s] - bl);
      //if( (v_aWf[s] - bl) >= clippedVal )clipped = true; 
    }
  }
  for(int i = 0; i < ss; i++)
  {
    if(polarity < 0)
    {  
      hwfV->SetBinContent(i+1,(v_aWf[i])*voltConversion);
    }
    else{
      hwfV->SetBinContent(i+1,(v_aWf[i]-bl)*voltConversion);
    }
  }

  m_ph = hwf->GetMaximum();
  if(m_ph>=clippedVal) clipped = true;
  //hPH->Fill(m_ph);
  int pkBin = hwf->GetMaximumBin();
  //int endBin = lg;
  m_sg = hwf->Integral(pkBin+sgBin, pkBin+endBin);
  //m_sg = hwf->Integral(pkBin + 3*sgBin, pkBin + endBin);
  /*if(m_sg < 0){
	  cout<<" negative sg "<<m_sg<<endl;
	  cout<<"wave count "<<waveCount<<endl;
  };*/
  if(waveCount == 54704){
	  TH1F *h = (TH1F*)hwf->Clone();
	  for(int i = 0; i < ss; i++)
	  {
		  cout<<h->GetBinContent(i+1)<<endl;
		  cout<<"i "<<i<<"bl "<<bl<<" v_aWf[i] "<<v_aWf[i]<<endl;
	  }
	  cout<<m_ph<<" peak bin "<<pkBin<<endl;
	  cout<<m_sg<<" m_sg"<<endl;
	  //set y axis of graph 
	  h->GetYaxis()->SetRangeUser(-10,250);
	  h->Draw();
	  h->SetDirectory(nullptr);
  };
  m_full  = hwf->Integral(pkBin - 30, pkBin+endBin);
  //m_full = hwf->Integral (pkBin - sgBin, pkBin + endBin);
  //m_full = hwf->Integral();
  m_cfdPos = CFD();
  m_peakPos = pkBin;
  m_psp = (float)m_sg/m_full;
  //if(clipped) cout <<"hi\n"; 
  hwf->Reset();
  return clipped;
}

// returns histogram of the current event. You have to process the waveform for
// this to work. 
TH1I* WFProcess::returnHisto()
{
  int ss = v_currentWave.size();
  auto aWf = v_currentWave;
  if(ss == 0) cout << "ERROR: process waveform!!!\n";
  if(hwf == NULL) hwf = new TH1I("aWf","",ss,0,ss);
  auto h1 = new TH1I("aWf","",ss,0,ss);
  
  for(int s=0; s<ss; s++)
    h1->Fill(s+1,aWf.at(s));

  return h1;
}

// check for multiple peaks. ProcessASingleWf must be called first or you will
// be processing an empty waveform. Typical pkThres is 0.05. Check ROOT class
// info for more on TSpectrum.
// This method will return a vector that list the peak positions. One can 
// simply grab the size fo the vector to veto waveforms that have a cetain 
// numnber of peaks.
std::vector<int> WFProcess::checkForMultiPeaks(double pkThres)
{
  TSpectrum *s = new TSpectrum();
  Int_t nfound = s->Search(hwf,5,"nodraw",0.3);
  Double_t *xpeaks;
  xpeaks = s->GetPositionX();
  if(nfound >= 2) clipped = true;
}

// calculates crossing at 75% of the peak but does not interpolate.
// Assumes (+) pulse
double WFProcess::CFD()
{
  // loop over waveform to find 75% crossing                                  
  int trigPos = 0;                                                            
  bool checkRise = false;
  double y = 0.5*hwf->GetMaximum();  
  for (int j=0;j < v_currentWave.size();j++)                                            
  {                                                                           
    if(hwf->GetBinContent(j)>y && !checkRise)                              
    {                                                                         
      trigPos = j;                                                            
      checkRise = true;                                                       
    }                                                                         
  } 
  // do a linear interpolation
  int p = 1; // number of points to go back
  auto y1 = hwf->GetBinContent(trigPos-p);
  auto y2 = hwf->GetBinContent(trigPos+p);
  auto x1 = hwf->GetBinCenter(trigPos-p);
  auto x2 = hwf->GetBinCenter(trigPos+p);

  double cfd_pos = (y-y1)*(x2-x1)/(y2-y1) + x1;
  //cout << trigPos << " " << cfd_pos << endl; 
  return cfd_pos;
}  

double WFProcess::GetTriggerPosition()
{
  int xPoint = CFD();
  double m, b, x1, x2, y1, y2;
  x1 = xPoint - 1;
  x2 = xPoint + 1;
  y1 = hwf->GetBinContent(x1);
  y2 = hwf->GetBinContent(x2);
  m = (y2 - y1) / (x2 - x1);
  b = y1 - m*x1;
  double yPos = 0.5*hwf->GetMaximum();
  return ( (yPos - b) / m); 
}
