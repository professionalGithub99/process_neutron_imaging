#define MIDAS_cxx
#include "MIDAS.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
// c++ classes
#include <iostream>
#include <fstream>
// namespaces
using namespace std;

MIDAS::MIDAS()
{
  h_theWave = NULL;
  eventCounter = 0;
  m_wp = new WFProcess;
}

MIDAS::~MIDAS()
{
  ;
}

// load binary file
void MIDAS::LoadFile(string iFN)
{
  inFile.open(iFN.c_str(), ios::binary|ios::in);
  if(!inFile.good()) cout << "ERROR: file is not open" << '\n'; //check file is open
}  

// returns false if at the end of the file
bool MIDAS::CheckEndOfFile()
{
  bool checkFile;
  checkFile = inFile.eof();
  return checkFile;
}  

bool MIDAS::GetEntry()
{
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  inFile.read((char*)&board,sizeof(int16_t));
  inFile.read((char*)&channel,sizeof(int16_t)); 
  inFile.read((char*)&timestamp,sizeof(int64_t)); 
  inFile.read((char*)&energy,sizeof(int16_t));
  inFile.read((char*)&shortEnergy,sizeof(int16_t));  
  inFile.read((char*)&flags,sizeof(int32_t));  
  inFile.read((char*)&numberOfWaveSamplesToBeRead,sizeof(int32_t));  
  // print info for 1st event
  if(eventCounter==0)
  {
    //cout << endl << "event__" << eventCounter << endl;
    cout << "board: " << board << endl;
    cout << "ch: " << channel << endl;
    //cout << "ts: " << timestamp << endl; // [ps]
    //cout << "E: " << energy << endl;
    //cout << "sE: " << shortEnergy << endl;
    cout << "#samples: " << numberOfWaveSamplesToBeRead << endl;
    int nS = numberOfWaveSamplesToBeRead;
    //m_firstTime = timestamp * 1E-12; // convert to [s]
    h_theWave = new TH1I("h_theWave","",nS,0,nS);
  }
  
  int16_t sample;
  // read in samples
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
  {
    inFile.read((char*)&sample,sizeof(int16_t));
    currentWave.push_back(sample);
  }
  ProcessAWfEvent();
  bool checkFile;
  checkFile = inFile.eof();
  eventCounter++;
  if(eventCounter%25000==0) 
    cout << eventCounter << "----> events processed" << "\t\r" << std::flush;
  return checkFile;
}  

// analyze the accelerator signal from UND
vector<double> MIDAS::GetEntryAcc()
{
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  inFile.read((char*)&board,sizeof(int16_t));
  inFile.read((char*)&channel,sizeof(int16_t)); 
  inFile.read((char*)&timestamp,sizeof(int64_t)); 
  inFile.read((char*)&energy,sizeof(int16_t));
  inFile.read((char*)&shortEnergy,sizeof(int16_t));  
  inFile.read((char*)&flags,sizeof(int32_t));  
  inFile.read((char*)&numberOfWaveSamplesToBeRead,sizeof(int32_t));  
  // print info for 1st event
  if(eventCounter==0)
  {
    //cout << endl << "event__" << eventCounter << endl;
    cout << "board: " << board << endl;
    cout << "ch: " << channel << endl;
    //cout << "ts: " << timestamp << endl; // [ps]
    //cout << "E: " << energy << endl;
    //cout << "sE: " << shortEnergy << endl;
    cout << "#samples: " << numberOfWaveSamplesToBeRead << endl;
    int nS = numberOfWaveSamplesToBeRead;
    //m_firstTime = timestamp * 1E-12; // convert to [s]
    h_theWave = new TH1I("h_theWave","",nS,0,nS);
  }
  
  int16_t sample;
  // read in samples
  //cout << "new event____\n";
  
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
  {
    inFile.read((char*)&sample,sizeof(int16_t));
    currentWave.push_back(sample);
    h_theWave->SetBinContent(i+1,sample);
  }
    int maxS = 0;
    int maxSPos = 0;
    cout << "new event____\n";
    for(int i=0;i<51/*numberOfWaveSamplesToBeRead*/;i++)
    {
      auto val = h_theWave->GetBinContent(i+1);
      if(val > maxS)
      {
        maxS = val;
        maxSPos = i+1;
      }
    }
   int xPoint = maxSPos;
   double m, b, x1, x2, y1, y2;
   x1 = xPoint + 2;
   x2 = xPoint + 4;
   y1 = h_theWave->GetBinContent(x1);
   y2 = h_theWave->GetBinContent(x2);
   m = (y2 - y1) / (x2 - x1);
   b = y1 - m*x1;
   double yPos = 0.8*maxS;
   double nX = ( (yPos - b) / m);

  //cout << maxS << endl;
  vector<double> vals;
  vals.push_back(nX);
  vals.push_back(maxS);
  //

  bool checkFile;
  checkFile = inFile.eof();
  vals.push_back(checkFile);
  eventCounter++;
  if(eventCounter%25000==0) 
    cout << eventCounter << "----> events processed" << "\t\r" << std::flush;
  //return checkFile;
  //
  cout << "pos: " << maxSPos << " " << maxS  << " " << xPoint << endl;
  return vals;//( (yPos - b) / m); 
}

TH1I* MIDAS::GetWF()
{
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  // print info for 1st event
  if(eventCounter==0)
  {
    //int nS = numberOfWaveSamplesToBeRead;
    h_theWave = new TH1I("h_theWave","",nS,0,nS);
  }
  
  int16_t sample;
  // read in samples
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
  {
    //inFile.read((char*)&sample,sizeof(int16_t));
    //currentWave.push_back(sample);
    //h_theWave->SetBinContent(i+1,sample);
  }
  eventCounter++;
  
  return h_theWave;
}


// processes waveform using WFProcess method. Returns true if the waveform 
// is past the manually set high threshold which is your dynamic range of your
// electronics (digitizer + components on custom boards if you are using them)
bool MIDAS::ProcessAWfEvent()
{
 bool clipped = m_wp->processASingleWf(currentWave);;
 return clipped;
}  

// returns histogram of event rate on a per minute basis
// *** need to fix for binary file ***
TH1F* MIDAS::PlotRate()
{
  double time0 = timestamp * 1E-12; // **convert to s for rate**
  int eCounter = 0;
  int minPos = 0;
  vector<float> rate;
  auto hRate = new TH1F("hRate",";Time (min); cpm",rate.size(),0,rate.size());
  return hRate;
}

TH1I* MIDAS::GetWaveformHisto()
{
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
    h_theWave->SetBinContent(i,currentWave.at(i));

 return h_theWave; 
}  


