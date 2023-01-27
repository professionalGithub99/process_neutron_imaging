#ifndef WFProcess_h
#define WFProcess_h

#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TProfile.h>
#include <TH2.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <iostream>
#include <vector>

class WFProcess{ //: public CAEN {//}, public CompassList {
  public :
     WFProcess();
     virtual ~WFProcess();
     // histograms
     TH1I           *hwf; // used for single waveform plotting
     TH1F   	    *hwfV;// used for single waveform voltage vs time per sample plotting
     TProfile       *profileWf; // used for avg wf plotting
     // 
     std::vector<int> v_currentWave;
   private:
      int blPnts; // number of baseline points to average
      int endBin; // end bin position for full intergral
      int sgBin;  // start bin of short-gate (this will be from the peak)
      int sFreq;  // sampling frequency of digitizer in ns (1 sample = XX ns)
      int dRange; // dynamic range of digitizer in voltage
      bool clipped;
      bool peakChecking; // false by default
      double threshold;  // 0 by default
      int polarity;
      bool defaults;
      double clippedVal; // value above which the pulse is considered clipped
      double voltConversion; // conversion factor from ADC to voltage
   public:
      double m_sg;
      double m_full;
      double m_ph;
      double m_psp;
      int    m_peakPos;
      int    m_samples;
      double m_cfdPos;
      double m_startBl;
      double m_endBl;
      double m_volt;
      double m_time;

   public:
      void setBlPnts(int n) {blPnts = n;}
      void setEndBin(int e) {endBin = e;}
      void setSgBin (int s) {sgBin  = s;}
      void setSFreq (int s) {sFreq  = s;}
      void setVoltageConversion(double v) {voltConversion = v;}
      void setThreshold(double t) {threshold = t;}
      void setPeakChecking(bool c) {peakChecking = c;}
      void setClippedVal(double c) {clippedVal = c;}
      bool getClippedVal(){return clipped;}
      double getWfPSD() {return m_psp;}
      double getWfFull()  {return m_full;}
      // set the polarity of the waveform: -1 (-) pulse, 1 (+) pulse
      void setPolarity(int p) {polarity = p;}
      std::vector<int> checkForMultiPeaks(double pkThres);
      double  CFD();
      double GetTriggerPosition(); // returns position of trigger within a 
                                   // waveform at its 50% rise point
      void printWfLimits();
     // loop for histogram fills and FOM calc. 2nd arguement indicates if it is
     // for a FOM calc in which case it will alter the start point of the short
     // gate, otherwise it will use either default sgBin or the user set value
     //virtual void processEvents(int sgPnt = 0, int lgPnt=100, bool scrubbed = false, int startEvent = 0, int endEvent = -1);
     //virtual void processEventsMT(int sgPnt, int lgPnt, bool scrubbed, int startEvent, int endEvent);
     //void processFOMMT(int lg_i, int lg_f, int sg_i, int sg_f);
     bool processASingleWf(std::vector<int> v_aWf,int waveCount);
     TH1I* getAWfHisto(std::vector<int> wf);
     TH1I* returnHisto();
     TH1I* GetWF();

};

#endif // #ifdef WFProcess_cxx
