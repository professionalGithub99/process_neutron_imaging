//________________
typedef struct
{
  Float_t l;               // Long integral
  Float_t s;               // Short integral
  Float_t amp;             // Amplitude
  Float_t ene;             // calibrated energy
  Float_t cfd;             // Trigger time
  Float_t psd;             // PSD parameter s/l
  Float_t trg;             // Detector trigger
  Float_t sBl;             // start baseline
  Float_t eBl;             // end baseline
} Detector;

// ************************
// *** event processing ***
// ************************
void _ProcessCoin(string oFN = "_coin_out.root")
{
  cout << "why don't you work\n";
  //ROOT::EnableThreadSafety()
  const int numOfChs = 3;
  CAEN c[numOfChs];
  string fPath[3];
  // Odessa
  fPath[0] = "/media/7jn/T7/deuBackScatter/wavedump/09-02-2022/dt_1/wave0.dat";
  // alpha
  fPath[1] = "/media/7jn/T7/deuBackScatter/wavedump/09-02-2022/dt_1/wave2.dat";
  // EJ-301D
  fPath[2] = "/media/7jn/T7/deuBackScatter/wavedump/09-02-2022/dt_1/wave3.dat";

  for(int i=0; i<numOfChs; i++)
  {
    c[i].LoadBinFile(fPath[i]);
    c[i].setNSamples(600);
    c[i].m_wp->setPolarity(-1); // set polarity to (-)
    c[i].m_wp->setBlPnts(20);
    c[i].m_wp->setSgBin(30);
    c[i].m_wp->setEndBin(200);
    c[i].m_wp->setClippedVal(16384);
    c[i].m_wp->printWfLimits();
  }

  TString ts_oFN(oFN);
  TFile *ff = new TFile(ts_oFN, "RECREATE");
		
  // Create an array of trees
  Detector det[numOfChs];
  TTree *tt;
  TString TName[numOfChs];
  TName[0] = "odessa";
  TName[1] = "alpha";
  TName[2] = "ej301";
	tt = new TTree("run_data", "run_data");
  // tree variables
  double energy[numOfChs];
  double timestamp[numOfChs];
  for(int i=0; i<numOfChs; i++)
  {
	  tt->Branch(TName[i],&det[i],"l:s:amp:ene:cfd:psd:trg:sBl:eBl");
  }
  
  // calibration factor
  float p0 = 0;//0.02178;//0.00429523;
  float p1 = 5.46730179969649E-05;// 
  bool checkEndOfFile[numOfChs] = {0};
  bool keepGoing = true;
  while(keepGoing)
  {
    for(int i_ch=0; i_ch<numOfChs; i_ch++)
    {
      if(!checkEndOfFile[i_ch])
      {
        if(c[i_ch].eventCounter%5000 == 0)
        {
          cout << "events processed -----> \t" << c[i_ch].eventCounter 
               << "\t\r" << std::flush;
        }
        // grab entry
        bool noHeader = false;
        checkEndOfFile[i_ch] = c[i_ch].GetWaveDumpEntry(noHeader = false);
        // 
        c[i_ch].ProcessAWfEvent();
        c[i_ch].m_wp->m_full;
        double sVal = c[i_ch].m_wp->m_sg;
        double lVal = c[i_ch].m_wp->m_full;
        //if(c.m_wp->m_ph < 14750) // check clipped
        det[i_ch].l     = lVal;
        det[i_ch].s     = sVal;
        if(det[i_ch].l > 0) det[i_ch].psd = det[i_ch].s / det[i_ch].l;
        else{ det[i_ch].psd = -1; };
        // FLAGS: not used
 		    det[i_ch].amp = c[i_ch].m_wp->m_ph;
		    det[i_ch].cfd = c[i_ch].m_wp->m_cfdPos;
		    det[i_ch].ene = p1*(det[i_ch].l) + p0;	
        det[i_ch].trg = c[i_ch].m_wp->m_peakPos;
        det[i_ch].sBl = c[i_ch].m_wp->m_startBl;
        det[i_ch].eBl = c[i_ch].m_wp->m_endBl;
        // do a pulse height check
        if(det[i_ch].amp < 50) 
        {
          det[i_ch].cfd = 0;
        }
        else{
          //if(i_ch == 2) cout << det[i_ch].cfd << " " << det[i_ch].trg <<  endl;
        }
      }
	    tt->Fill();
    }  
    if(checkEndOfFile[0] && checkEndOfFile[1] && checkEndOfFile[2])
      keepGoing = false;
  }
  cout << endl; 
  tt->Write();
  ff->Close();
  //delete tt;
}
