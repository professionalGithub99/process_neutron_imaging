//my classes
#include "LBDCalibration.h"
#include "CAEN.h"
#include "WFProcess.h"
//c++ classes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
//Root classes
#include <TFile.h>
#include <TCanvas.h>
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

using namespace std;
//using namespace MYNGM;

LBDCalibration::LBDCalibration(std::vector<std::string> iFN, TString dataType)
{
  numOfPmts = 4;

  for(int ipmt = 0; ipmt<numOfPmts; ipmt++)
  {
    std::string s = "pmt_" + std::to_string(ipmt);
    m_c[ipmt].LoadROOTFile(iFN[ipmt]);
    m_offset[ipmt] = 1;
    // Block detectors have (+) pulses
    m_c[ipmt].m_wp->setPolarity(1); // set to 1 or -1
    m_c[ipmt].m_wp->setBlPnts(10);
    m_c[ipmt].m_wp->setSgBin(20); // 11,  # of bins past the peak to start your short gate
    m_c[ipmt].m_wp->setEndBin(200); // bin number to integrate to
    m_c[ipmt].m_wp->setClippedVal(14750);
    m_c[ipmt].m_wp->printWfLimits(); 
    
    cout << "f\n";
  }

  // check number of counts synced
  cout << "______checking # of counts______\n";
  //for(int ipmt = 0; ipmt<numOfPmts; ipmt++) cout << w[ipmt].GetNEntries() << endl;
  cout << "________________________________\n";

  /*if(  (w[0].GetNEntries() != w[1].GetNEntries())
    && (w[0].GetNEntries() != w[2].GetNEntries())
    && (w[0].GetNEntries() != w[3].GetNEntries())
  )
  {
    cout << "ERROR: total events not equal in the channel group. Check run parame"
         << "ters and/or file paths\n";
    return;
  }*/
  
  for(int i=0; i<4; i++)
  {
    TString hn = "hPMT_" + to_string(i);  
    hE[i] = new TH1F(hn,";Light Output (arb. units);Counts",1000,0,200E3);
  }
}
// Use this function for fine tuning the gain.
void LBDCalibration::setPMTOffSet(vector<double> offset)
{
  cout << "setting PMT offsets.\n";
  for(int i=0; i< numOfPmts; i++) m_offset[i] = offset.at(i);
}

void LBDCalibration::createFlood(TString oFN = "tmp_flood.root")
{
  TFile *nFile = new TFile(oFN,"RECREATE");

  auto hPSPEf = new TH2F("hPSPEf",";Light Output (arb. units);PSP",500,0,1E6,250,0,1);
  auto hFlood = new TH2F("hFlood",";;",500,0,1,500,0,1);
  auto hEE = new TH1F("hEE",";Light Output (arb. units);Counts",1000,0,1E6);
  auto hHSlice = new TH1F("hHSlice","L.O. (arb. units);Counts",1000,0,200E3);
  auto hVSlice = new TH1F("hVSlice","L.O. (arb. units);Counts",1000,0,200E3);

  typedef struct
  {
    Float_t l;              // Long integral
    Float_t s;              // Short integral
    Float_t amp;            // Amplitude
    Float_t psd;             // PSD parameter s/l
    Float_t trigPos;
  } PixBlockCh;

  typedef struct
  {
    Float_t l;              // Long integral
    Float_t s;              // Short integral
    Float_t amp;            // Amplitude
    Float_t x;
    Float_t y;
    Float_t psd;            // PSD parameter s/l
  } PixBlockTot;

  // Create an array of trees
  PixBlockCh ch[4];
  PixBlockTot pbt; 

  TTree *chTree[4];
  TTree *tt;
  
  double energy;
  double timestamp;
  char buffer[50];

  /*for(int i=0; i<4; i++)
  {
		sprintf(buffer, "TCh%d", i);
    chTree[i] = new TTree(buffer, "channel_data");
	  chTree[i]->Branch("ind",&ch[i],"l:s:amp:psd:trigPos");
  }*/  
  
  tt = new TTree("TSum", "tot_data");
	tt->Branch("sum",&pbt,"l:s:amp:x:y:psd");
  
  for(int i=0; i<4; i++)
  {
		sprintf(buffer, "ch%d", i);
    //chTree[i] = new TTree(buffer, "channel_data");
	  tt->Branch(buffer,&ch[i],"l:s:amp:psd:trigPos");
  } 
  bool checkEndOfFile = false;
  
  int eventCounter = 0;
  while( !checkEndOfFile ) 
  {
    if(eventCounter%20000==0) 
      cout << eventCounter << " events processed (" << 100.*eventCounter/m_c[0].t_nentries << "%)" << endl;
    eventCounter++;
    double phTot = 0;
    double sTot = 0.;
    double lgTot = 0;
    bool clipped[4] = {0};
    // grab entry
    bool pkIsBad = false;
    for(int ipmt = 0; ipmt<numOfPmts; ipmt++)
    {
      clipped[ipmt] = m_c[ipmt].GetROOTEntry();
      m_c[ipmt].ProcessAWfEvent();
      
      ch[ipmt].amp = m_c[ipmt].m_wp->m_ph;
      phTot += ch[ipmt].amp;
      
      ch[ipmt].s = m_c[ipmt].m_wp->m_sg;
      ch[ipmt].l = m_c[ipmt].m_wp->m_full;
      sTot  += ch[ipmt].s;//m_c[ipmt].m_wp->m_sg;//* m_offset[ipmt];//(w[ipmt].Energy - w[ipmt].EnergyShort);//sVal;
      lgTot += ch[ipmt].l;//m_c[ipmt].m_wp->m_full;//* m_offset[ipmt];
      // check trigger position
      ch[ipmt].trigPos = m_c[ipmt].m_wp->m_peakPos;
      if(ipmt == 3) checkEndOfFile = clipped[ipmt];
    }
    // pusle integral 
    double x = (ch[0].amp + ch[1].amp)  / phTot;  //(m_c[0].m_wp->m_full + m_c[1].m_wp->m_full) / lgTot;
    double y = (ch[0].amp + ch[2].amp)  / phTot; //(m_c[2].m_wp->m_full + m_c[0].m_wp->m_full) / lgTot;
    //m_c[0].m_wp->m_ph
    // Fill trees 
    pbt.l   = lgTot;
    pbt.s   = sTot;
    pbt.amp = phTot; 
    pbt.psd = sTot/lgTot * 1.;
    pbt.x   = x;
    pbt.y   = y;
    tt->Fill();

    //for(int i=0; i<4; i++) chTree[i]->Fill();
    // Fill histograms    
    hFlood->Fill(x,y);
    for(int i=0; i<4; i++) hE[i]->Fill(m_c[i].m_wp->m_full);
    //hEE->Fill(lgTot);
    //if(x>0.47 && x<0.53 && y>0.45 && y<0.54)
    double _thres = 200;
    //if(ch[0].amp>_thres && ch[1].amp>_thres && ch[2].amp>_thres && ch[3].amp>_thres)
    hPSPEf->Fill(lgTot, sTot/lgTot);
  }
  tt->Write(); 
  hFlood->SetDirectory(0);
  hFlood->Draw("colz");
  auto t0 = new TCanvas;
  hPSPEf->SetDirectory(0);
  hPSPEf->Draw("colz");
  auto t1 = new TCanvas;

  for(int ipmt = 0; ipmt<numOfPmts; ipmt++)
  {
    hE[ipmt]->SetLineColor(ipmt+1);
    hE[ipmt]->SetDirectory(0);
    hE[ipmt]->Draw("same");
    //chTree[ipmt]->Write();
  }
  auto t2 = new TCanvas;
  hEE->SetDirectory(0);
  hEE->Draw();
  hFlood->Write();
  for(int i=0; i<4; i++) hE[i]->Write();
  return;
}

double LBDCalibration::optimizeFOM(TString oFN = "tmp_fom.root")
{
  /*hPSP = new TH1F("hPSP",";2SD; Counts",250,0,1);
  auto hFOM  =  new TH2F("hFOM",";;",400,0,400,0,100,100);

  int lg = 200;
  //for(int lg = 100; lg<400; lg++)
  //{
  for(int sg=50; sg<52; sg++) // long gate / full
  {

    for(Long64_t ientry = 0; ientry<w[0].GetNEntries(); ientry++)
    {
      double eTot = 0.;
      double sTot = 0.;
      double lgTot = 0;
      bool clipped[4] = {0};

      if(ientry%25000 == 0)
      {
        cout << "events processed: \t" << 100*ientry/w[0].GetNEntries() << "%"
             << "\t\r" << std::flush;
      }

      // grab entry
      for(int ipmt = 0; ipmt<numOfPmts; ipmt++)
      {
        w[ipmt].GetEntry(ientry);
        eTot += w[ipmt].Energy * m_offset[ipmt];

        clipped[ipmt] = w[ipmt].processASingleWf(ientry, sg, lg);
        double sVal = w[ipmt].m_sg;
        double lVal = w[ipmt].m_full;
        //cout << w[ipmt].m_sg << endl;
        sTot  += w[ipmt].m_sg* m_offset[ipmt];//(w[ipmt].Energy - w[ipmt].EnergyShort);//sVal;
        lgTot += w[ipmt].m_full* m_offset[ipmt];

        //cout << w[ipmt].Energy << " ";

        hE[ipmt]->Fill(w[ipmt].Energy* m_offset[ipmt]);
      }

      if(!clipped[0] && !clipped[1] && !clipped[2] && !clipped[3] && lgTot > 20000)
      {
        hPSP->Fill(sTot/lgTot);
      }
    } // histo fill loop
    auto fom = calcFOM();
    hFOM->Fill(lg,sg,fom);
  } // SG loop
  //}
  hFOM->SetDirectory(0);
  hFOM->Draw("colz");
  auto *fpsp = new TFile(oFN,"recreate");
  hFOM->Write();
  Int_t MaxBin = hFOM->GetMaximumBin();
  Int_t x,y,z;
  hFOM->GetBinXYZ(MaxBin, x, y, z);
  printf("The bin having the maximum value is (%d,%d)\n",x,y);
  */
}

void LBDCalibration::plot_4_Wfs(int eventNum)
{
  /*auto tc = new TCanvas;
  Long64_t ientry  = eventNum;
  for(int ipmt = 0; ipmt<numOfPmts; ipmt++)
  {
    w[ipmt].plotAWf(eventNum);
    w[ipmt].processASingleWf(ientry, 13, 180);
  }*/

}

double LBDCalibration::calcFOM()
{
  // find peaks and set limits
  //hPSP->Smooth(2);
  TSpectrum *s = new TSpectrum();
  Int_t nfound = s->Search(hPSP,2,"",0.1);
  Double_t *xpeaks;
  xpeaks = s->GetPositionX();

  if(nfound != 2){
    cout << "cannot find 2 peaks... skipping this iteration\n";
    return 0.;
  }
  else{cout << "2 peaks found...\n";}

  for (int p=0;p<nfound;p++)
  {
    if(p==0) cout << "Peak positions found at: ";
    cout << xpeaks[p] << " ";
    if(p==1) cout << endl;
  }

  auto g1 = new TF1("m1","gaus",xpeaks[1]-.05*xpeaks[1], xpeaks[1]+.05*xpeaks[1]);
  auto g2 = new TF1("m2","gaus",xpeaks[0]-.05*xpeaks[0], xpeaks[0]+.05*xpeaks[0]);

  //auto both = new TF1("both","gaus(0)+gaus(3)",xpeaks[1]-.03,xpeaks[0]+.03);
  // Fit each function and add it to the list of functions
  Double_t par[9];
  hPSP->Fit(g1,"RQ");
  hPSP->Fit(g2,"R+Q");
  // Get the parameters from the fit
  g1->GetParameters(&par[0]);
  g2->GetParameters(&par[3]);
  auto mean1 = par[1];
  auto sig1  = par[2];
  auto mean2 = par[4];
  auto sig2  = par[5];

  auto fom = (TMath::Abs(mean1-mean2) / (2.35*(sig1+sig2)) );

  // Use the parameters on the sum
  //both->SetParameters(par);
  //hPSPT->Fit(both,"R+");
  delete s;
  cout << "\n\t____FOM: " << fom << "____\n\n";
  return 1.;//fom;
}
