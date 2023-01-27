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

//________________
void ProcessWfs(TString fPath, TString oFN = "_wf_out.root")
{
  Compass c(fPath);
  //c.LoadFile(fPath);
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(11);
  c.m_wp->setEndBin(100);
  c.m_wp->setClippedVal(14750);
  c.m_wp->printWfLimits();

  TFile *ff = new TFile(oFN, "RECREATE");
		
  // Create an array of trees
  Detector det;
  TTree *tt;
  double energy;
  double timestamp;
	tt = new TTree("T", "run_data");
	tt->Branch("d",&det,"l:s:amp:cfd:psd:trg");
	tt->Branch("E",&energy,"E/d");
	tt->Branch("time",&timestamp,"time/d");
  
  // calibration factor
  float p0 = 0;
  float p1 = 6.69651335933544E-05; // DT 3500
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
        timestamp = 
        det.l     = lVal;
        det.s     = sVal;
        if(det.l > 0) det.psd = det.s / det.l;
        else{ det.psd = -1; };
        // FLAGS: not used
 		    det.amp = -1;
		    det.cfd = -1;
		    energy  = p1*(det.l) + p0;	
	      tt->Fill();
      //}
  }
  cout << endl; 
  //auto oFile = new TFile(oFN,"recreate");
  tt->Write();
}
