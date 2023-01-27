//________________
typedef struct
{
  Float_t l;               // Long integral
  Float_t s;               // Short integral
  Float_t amp;             // Amplitude
  Float_t cfd;             // Trigger time
  Float_t psd;             // PSD parameter s/l
  Float_t trg;             // Detector trigger

} Detector;

// 
void ProcessWfs(string fPath, string oFN = "_wf_out.root");
//________________
void _ProcessWfs(string fPath = "in.root", int sF = 0, int eF = 0)
{
  for(int i=sF; i<=eF; i++)
  {
    string fNum = to_string(i);
    string fName = fPath + "run_" + fNum + "/RAW/DataR_CH2@DT5730S_13949_run_" + fNum + ".root";
    string oFName = "_dt_run_" + to_string(i) + ".root";
    cout << fName <<  " >> " << oFName << endl;
    ProcessWfs(fName,oFName);
    cout << endl;
  }
}
// ************************
// *** event processing ***
// ************************
void ProcessWfs(string fPath, string oFN = "_wf_out.root")
{
  Compass c(fPath);
  //c.LoadFile(fPath);
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(11);
  c.m_wp->setEndBin(100);
  c.m_wp->setClippedVal(14750);
  c.m_wp->printWfLimits();

  TString ts_oFN(oFN);
  TFile *ff = new TFile(ts_oFN, "RECREATE");
		
  // Create an array of trees
  Detector det;
  TTree *tt;
  double energy;
  double timestamp;
	tt = new TTree("T", "run_data");
	tt->Branch("d",&det,"l:s:amp:cfd:psd:trg");
	tt->Branch("E",&energy,"E/d");
	tt->Branch("timestamp",&timestamp,"timestamp/d");
  
  // calibration factor
  float p0 = 0;//0.02178;//0.00429523;
  //float p1 = 0.002260665; // for EJ-309 in SR measurements  (CH3)
  //float p1 = 0.000075;//0.00215265249; // for EJ-301D in OR measurements (CH3)
  float p1 = 7.25917921599042E-05;// 6.990047E-5;//7.09675054657768E-5; // 2-in stilbene D21 cal measurements
  bool checkEndOfFile = false;
  while(!checkEndOfFile)
  {
    if(c.eventCounter%5000 == 0)
    {
      cout << "events processed -----> \t" << c.eventCounter 
           << "\t\r" << std::flush;
    }

    // grab entry
    checkEndOfFile = c.GetROOTEntry();
    c.ProcessAWfEvent();

    //if(!c.m_wp->getClippedVal())
    //{  
      c.m_wp->m_full;

      double sVal = c.m_wp->m_sg;
      double lVal = c.m_wp->m_full;
      // fill your histograms  
      //if(c.m_wp->m_ph < 14750) // check clipped
      //{        
        det.l     = lVal;
        det.s     = sVal;
        if(det.l > 0) det.psd = det.s / det.l;
        else{ det.psd = -1; };
        // FLAGS: not used
 		    det.amp = -1;
		    det.cfd = -1;
		    energy  = p1*(det.l) + p0;	
        timestamp = c.Timestamp;
        //cout << timestamp << endl;
	      tt->Fill();
      //}
  }
  cout << endl; 
  tt->GetEntry(0);
  double firstTime = timestamp;
  tt->GetEntry(tt->GetEntriesFast()-1);
  double lastTime  = timestamp;
  double totTime = lastTime - firstTime;
  totTime *= 1E-9; // convert to seconds
  cout << "run time: " << totTime << " seconds\n";
  //auto oFile = new TFile(oFN,"recreate");
  tt->Write();
  ff->Close();
  //delete tt;
}
