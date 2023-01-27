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
//#include <ROOT/TTreeProcessorM.hxx>
// Header file for the classes stored in the TTree if any.

class MIDAS {

public :
   MIDAS();
   virtual ~MIDAS();
   std::ifstream inFile;
   // histograms
   int            eventCounter;
   std::vector<int> currentWave;
   WFProcess *m_wp;
   //               Binary file load
   void             LoadFile(std::string iFN); 
   // returns false if at the end of the file
   bool             CheckEndOfFile();
   bool             GetEntry(); // for binary file
   std::vector<double> GetEntryAcc();
   // visualization
   TH1F* PlotRate();
   bool  ProcessAWfEvent();
   //_____________
   // some getter's
   // _____________
   TH1I*  GetWF();
};

#endif

//#ifdef CAEN_cxx
//CAEN::Compass()
//{
  //;
//}



//#endif // #ifdef CAEN_cxx
