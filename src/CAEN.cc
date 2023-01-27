#define CAEN_cxx
#include "CAEN.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
// c++ classes
#include <iostream>
#include <fstream>
// namespaces
using namespace std;

CAEN::CAEN()
{
  SetDefaults();
}

CAEN::CAEN(TString iFN, TTree *tree) : fChain(0) 
{
  SetDefaults();
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) 
  {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(iFN);
    if (!f || !f->IsOpen()) 
    {
       f = new TFile(iFN);
    }
    // The title of the tree changes depending on whether or not it is RAW 
    // , FILTERED, or UNFILTERED data
    f->GetObject("Data_R",tree);
  }
  //Init(tree);
  Samples = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("Channel", &Channel, &b_Channel);
  fChain->SetBranchAddress("Timestamp", &Timestamp, &b_Timestamp);
  fChain->SetBranchAddress("Board", &Board, &b_Board);
  fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
  fChain->SetBranchAddress("EnergyShort", &EnergyShort, &b_EnergyShort);
  fChain->SetBranchAddress("Flags", &Flags, &b_Flags);
  fChain->SetBranchAddress("Probe", &Probe, &b_Probe);
  fChain->SetBranchAddress("Samples", &Samples, &b_Samples);
  
  t_nentries = fChain->GetEntriesFast();
  TFile *rawfile = new TFile("rawdata.root","RECREATE");
  fChain->InPlaceClone(rawfile);
}


void CAEN::SetDefaults()
{
  gCut = NULL;
  nCut = NULL;
  h_theWave = NULL;
  eventCounter = 0;
  m_wp = new WFProcess;
  b_firstEvent = true;
  
  return;
}

CAEN::~CAEN()
{
  ;
}

// load binary file
void CAEN::LoadBinFile(string iFN)
{
  inFile.open(iFN.c_str(), ios::binary|ios::in);
  if(!inFile.good()) cout << "ERROR: file is not open" << '\n'; //check file is open
}  

// load ROOT file
void CAEN::LoadROOTFile(TString iFN, TTree *tree) 
{
  fChain = 0;//new TChain;
  if (tree == 0) 
  {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(iFN);
    if (!f || !f->IsOpen()) 
    {
       f = new TFile(iFN);
    }
    // The title of the tree changes depending on whether or not it is RAW 
    // , FILTERED, or UNFILTERED data
    f->GetObject("Data_R",tree);
  }
  //Init(tree);
  Samples = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("Channel", &Channel, &b_Channel);
  fChain->SetBranchAddress("Timestamp", &Timestamp, &b_Timestamp);
  fChain->SetBranchAddress("Board", &Board, &b_Board);
  fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
  fChain->SetBranchAddress("EnergyShort", &EnergyShort, &b_EnergyShort);
  fChain->SetBranchAddress("Flags", &Flags, &b_Flags);
  fChain->SetBranchAddress("Probe", &Probe, &b_Probe);
  fChain->SetBranchAddress("Samples", &Samples, &b_Samples);
  
  t_nentries = fChain->GetEntriesFast();

}

// returns false if at the end of the file
bool CAEN::CheckEndOfFile()
{
  bool checkFile;
  checkFile = inFile.eof();
  return checkFile;
}  

void CAEN::SetInfoFile(std::string iFN)
{
  ifstream infoFile;
  infoFile.open(iFN.c_str());
  if(infoFile) cout << "info file open!\n";
  string line;
  while(std::getline(infoFile, line))
  //for(int i=0; i<4; i++)
  {
    std::getline(infoFile, line);
    std::cout << "line:" << line << std::endl;
  }
}

double CAEN::getLG(int eventNumber)
{
  return energy;
}  

double CAEN::getPSD(int eventNumber)
{
  return psp;
} 

int CAEN::getNumberOfSamples()
{
  return numberOfWaveSamplesToBeRead; 
}

bool CAEN::GetBinEntry()
{
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  if(b_firstEvent)
  {
    std::cout<<b_firstEvent<<std::endl;
    b_firstEvent = false;
    int headerSize = 16;
    inFile.read((char*)&header,sizeof(uint16_t));
    auto val = decToBinary(header,headerSize);
    // print header information and set 
    if(val[headerSize-1] == 1) cout << "Energy in Channels is present\n";
    if(val[headerSize-2] == 1) cout << "Energy in KeV/MeV (according to the calibration) "
                         << "is present\n";
    if(val[headerSize-3] == 1) cout << "Energy short is present\n";
    if(val[headerSize-4] == 1)
    {
      cout << "Waveform samples are present\n";
      b_waveformsPresent = true;
      // flip first event bool to print sample info
      b_firstEvent = true;
    }
    else{ 
      cout << "NO WAVEFORMS PRESENT\n";
      b_waveformsPresent = false;
    }
  }
  // continue reading file
  inFile.read((char*)&board,sizeof(int16_t));
  inFile.read((char*)&channel,sizeof(int16_t)); 
  inFile.read((char*)&timestamp,sizeof(int64_t)); 
  inFile.read((char*)&energy,sizeof(int16_t));
  inFile.read((char*)&shortEnergy,sizeof(int16_t));  
  inFile.read((char*)&flags,sizeof(int32_t));  
  // convert timestamp from ps to ns
  timestamp *= 1E-3;
  // print info for 1st event
  if(eventCounter==0)
  {
    cout << "board: "  << board       << endl;
    cout << "ch: "     << channel     << endl;
  }
  // read in waveform values if present
  if(b_waveformsPresent)
  {
    inFile.read((char*)&wfCode,sizeof(int8_t));  
    inFile.read((char*)&numberOfWaveSamplesToBeRead,sizeof(int32_t));  
    if(b_firstEvent)
    {
      b_firstEvent = false;
      int nS = numberOfWaveSamplesToBeRead;
      h_theWave = new TH1I("h_theWave","",nS,0,nS);
      cout << "#s: "     << numberOfWaveSamplesToBeRead << endl;
    }
    //m_firstTime = timestamp * 1E-12; // convert to [s]
    
    int16_t sample;
    // read in samples
    for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
    {
      inFile.read((char*)&sample,sizeof(int16_t));
      currentWave.push_back(sample);
    }
    //ProcessAWfEvent();
  }

  bool checkFile;
  checkFile = inFile.eof();
  eventCounter++;
  //if(eventCounter%25000==0) 
    //cout << eventCounter << "----> events processed" << "\t\r" << std::flush;
  return checkFile;
}  

void CAEN::setNSamples(int nS)
{
  numberOfWaveSamplesToBeRead = nS;
}

bool CAEN::GetWaveDumpEntry(bool headerIncluded)
{
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  if(headerIncluded)
  {
    ;
  }
  // continue reading file
  else
  {
    if(b_firstEvent)
    {
      b_firstEvent = false;
      int nS = numberOfWaveSamplesToBeRead;
      h_theWave = new TH1I("h_theWave","",nS,0,nS);
      cout << "#s: "     << numberOfWaveSamplesToBeRead << endl;
    }
    //m_firstTime = timestamp * 1E-12; // convert to [s]
    
    int16_t sample;
    // read in samples
    for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
    {
      inFile.read((char*)&sample,sizeof(int16_t));
      currentWave.push_back(sample);
    }
    //ProcessAWfEvent();
  }

  bool checkFile;
  checkFile = inFile.eof();
  eventCounter++;
  //if(eventCounter%25000==0) 
    //cout << eventCounter << "----> events processed" << "\t\r" << std::flush;
  return checkFile;
}

bool CAEN::GetROOTEntry()
{
  if(eventCounter == t_nentries)
    return true;

  std::cout<<fChain->GetEntries()<<"entries"<<std::endl;
  fChain->GetEntry(eventCounter);
  // clear vector that holds waveform 
  currentWave.clear();
  // read one event from binary file
  if(b_firstEvent)
  {
    b_firstEvent = false;
    numberOfWaveSamplesToBeRead = Samples->GetSize();
    int nS = numberOfWaveSamplesToBeRead;
    h_theWave = new TH1I("h_theWave","",nS,0,nS);
    cout << "#s: "     << numberOfWaveSamplesToBeRead << endl;
  
    cout << "board: "  << Board       << endl;
    cout << "ch: "     << Channel     << endl;
  }
  
  //Timestamp *= 1E-3; // convert ps to ns
  if(eventCounter < 500){
  cout<<"timestamp : "<<Timestamp<<"event number "<<eventCounter<<endl;
  };
  // read in samples
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
  {
    auto sample = Samples->At(i);
    currentWave.push_back(sample);
  }
  eventCounter++;
  //if(eventCounter%25000==0) 
    //cout << eventCounter << "----> events processed" << "\t\r" << std::flush;
  return false;
}

// analyze the accelerator signal from UND
vector<double> CAEN::GetEntryAcc()
{
  //cout << maxS << endl;
  vector<double> vals;
  return vals;//( (yPos - b) / m); 
}

TH1I* CAEN::GetWF()
{
  cout << "next event\n";
  
  GetROOTEntry();
  
  int nS = numberOfWaveSamplesToBeRead;
  h_theWave = new TH1I("h_theWave","",nS,0,nS);
  
  int16_t sample;
  // read in samples
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
  {
    h_theWave->SetBinContent(i+1, currentWave.at(i));
  }
  eventCounter++;
  
  return h_theWave;
}


// processes waveform using WFProcess method. Returns true if the waveform 
// is past the manually set high threshold which is your dynamic range of your
// electronics (digitizer + components on custom boards if you are using them)
bool CAEN::ProcessAWfEvent()
{
 bool clipped = m_wp->processASingleWf(currentWave,eventCounter);;
 return clipped;
}  

// returns histogram of event rate on a per minute basis
// *** need to fix for binary file ***
TH1F* CAEN::PlotRate()
{
  double time0 = timestamp * 1E-12; // **convert to s for rate**
  int eCounter = 0;
  int minPos = 0;
  vector<float> rate;
  auto hRate = new TH1F("hRate",";Time (min); cpm",rate.size(),0,rate.size());
  //for(int i=0; i<rate.size(); i++)
  //{
    //hRate->SetBinContent(i,rate[i]);
  //}
  return hRate;
}

TH1I* CAEN::GetWaveformHisto()
{
  for(int i=0;i<numberOfWaveSamplesToBeRead;i++)
    h_theWave->SetBinContent(i,currentWave.at(i));

 return h_theWave; 
}  

vector<int> CAEN::decToBinary(int n, int length)
{
  vector<int> v;
  // Size of an integer is assumed to be lenght bits
  for (int i = (length-1); i >= 0; i--) 
  {
    int k = n >> i;
    if (k & 1)
    {
      //cout << "1";
      v.push_back(1);
    }
    else{
      //cout << "0";
      v.push_back(0);
    }
  }
  cout << endl;
  return v;
}
