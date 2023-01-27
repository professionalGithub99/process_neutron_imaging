//////////////////////////////////////////////////////////

#ifndef CAEN_h
#define CAEN_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TCutG.h>
#include <ROOT/TThreadedObject.hxx>
#include <fstream>
#include <vector>
#include <WFProcess.h>
#include <vector>
//#include <ROOT/TTreeProcessorM.hxx>
// Header file for the classes stored in the TTree if any.

class CAEN {
public:
   TCutG *gCut;
   TCutG *nCut;

public :
   // This specific format is taken from UM5960 ‐ CoMPASS User Manual rev. 14
   // pg. 63 and assumes that you are not using the calibrated energy output
   // stream. If you want to use that, you will have to change this as well as
   // the corresponding GetEntry method in the .cc file.
   // *******************
   // *** Binary file ***
   // *******************
   std::ifstream inFile;
   uint16_t header;
   int16_t  board;
   int16_t  channel;
   int64_t  timestamp; 
   int16_t  energy;
   int16_t  shortEnergy;
   int32_t  flags;
   int8_t   wfCode;
   int32_t  numberOfWaveSamplesToBeRead; //record length in samples
   bool     b_firstEvent;
   bool     b_waveforms;
   bool     b_waveformsPresent;
   // *****************
   // *** ROOT file ***
   // *****************
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   Long64_t        t_nentries; // number of tree entries
   // Fixed size dimensions of array or collections stored in the TTree if any.
   // Declaration of leaf types
   // notice that these variables are uppercase 
   UShort_t        Channel;
   ULong64_t       Timestamp;
   UShort_t        Board;
   UShort_t        Energy;
   UShort_t        EnergyShort;
   UInt_t          Flags;
   Int_t           Probe;
   TArrayS         *Samples;
   // List of branches
   TBranch        *b_Channel;   //!
   TBranch        *b_Timestamp;   //!
   TBranch        *b_Board;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_EnergyShort;   //!
   TBranch        *b_Flags;   //!
   TBranch        *b_Probe;   //!
   TBranch        *b_Samples;   //!
   // ******************
   // *** histograms ***
   // ******************
   TH1I           *h_theWave; // used for plotting purposes
   TString        label;  // detector label for orgainization i.e. histograms
   double         psp;
   double         p[3];   // cal parameters
   double         calE;   // [MeVee]
   int            partID; // gamma = 0, neutron = 1
   double         m_totalTime; // [s]
   double         m_firstTime; // [s]
   int            eventCounter;
   std::vector<int> currentWave;
   WFProcess *m_wp;
   // ******************** 
   // *** Constructors ***
   // ********************
   CAEN();
   CAEN(TString iFN,TTree *tree=0);
   virtual ~CAEN();
   // *****************
   // *** Functions ***
   // *****************
   void SetDefaults();
   // Arguements same as constructor above
   // Binary file load. This can be any binary file. For example, this will 
   // work for a Compass or Wavedump file
   void LoadBinFile(std::string iFN); 
   void LoadROOTFile(TString iFN,TTree *tree=0);
   // returns false if at the end of the file
   bool CheckEndOfFile();
   void SetInfoFile(std::string iFN);
   //
   bool GetBinEntry();      // for binary file
   bool GetROOTEntry();     // for ROOT file
   bool GetWaveDumpEntry(bool headerIncluded); // for wavedump binary file
   bool ProcessAWfEvent();
   // ***********
   // *** Vis ***
   // ***********
   TH1F* PlotRate();
   std::vector<int>* GetWaveformVector(); // returns pointer to vector waveform
   // *****************
   // *** some getter's
   // *****************
   double getLG(int eventNumber);
   double getPSD(int eventNumber);
   int    getNumberOfSamples();
   TH1I*  GetWF();
   TH1I* GetWaveformHisto(); // this returns current waveform
   std::vector<double> GetEntryAcc();
   // *********************
   // *** some setter's ***
   // *********************
   void setWFProcessing(bool b) { b_waveforms = b; } 
   void setNSamples(int nS);
   // ************************
   // *** helper functions ***
   // ************************
   std::vector<int> decToBinary(int n, int length);

};

#endif

//#ifdef CAEN_cxx
//CAEN::Compass()
//{
  //;
//}



//#endif // #ifdef CAEN_cxx
