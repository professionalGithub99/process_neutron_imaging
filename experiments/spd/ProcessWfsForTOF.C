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
void ProcessWfsForTOF(string fPath, string fcPath)
{
  // the detector you are characterizing
  Compass c;
  c.LoadFile(fPath);
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(11);
  c.m_wp->setEndBin(100);
  c.m_wp->setClippedVal(16000);
  c.m_wp->printWfLimits();
  // output file path
  TString oFN = fPath + "_tof.root";
  TFile *ff = new TFile(oFN, "RECREATE");
	// the fission chamber file
  // the detector you are characterizing
  Compass fc;
  fc.LoadFile(fcPath);

  // Create an array of trees
  Detector det;
  TTree *tt;
  double energy;
  double timestamp;
	tt = new TTree("T", "run_data");
	tt->Branch("d",&det,"l:s:amp:cfd:psd:trg");
	tt->Branch("E",&energy,"E/f");
	tt->Branch("time",&timestamp,"time/d");
  
  // calibration factor
  float p0 = 0;
  //float p1 = 0.002260665; // for EJ-309 in SR measurements  (CH3)
  //float p1 = 0.00215265249; // for EJ-301D in OR measurements (CH3)
  float p1 = 0.000075;//0.00215265249; // for EJ-301D in OR measurements (CH3)

  // just need to check the detector file, not the FC. The FC will either 
  // have more or the same number of counts
  while(!c.CheckEndOfFile())
  {
    if(c.eventCounter%25000 == 0)
    {
      cout << "events processed -----> \t" << c.eventCounter 
           << "\t\r" << std::flush;
    }

    // grab entry
    auto end = c.GetEntry();
    // get entry from fission chamber until within the TOF window
    while( TMath::Abs(c.timestamp - fc.timestamp) > 600)
    {
      fc.GetEntry();
      //cout << TMath::Abs(c.timestamp - fc.timestamp) << endl;
    }
    c.ProcessAWfEvent();
    auto tof = c.timestamp - fc.timestamp;
    //if(!c.m_wp->getClippedVal())
    //{  
      c.m_wp->m_full;

      double sVal = c.m_wp->m_sg;
      double lVal = c.m_wp->m_full;
      // fill your histograms  
      if(c.m_wp->m_ph < 2000000);//14750) // check clipped
      {        
        timestamp = tof;
        det.l     = lVal;
        det.s     = sVal;
        if(det.l > 0) det.psd = det.s / det.l;
        else{ det.psd = -1; };
        // FLAGS: not used
 		    det.amp = -1;
		    det.cfd = -1;
		    energy  = p1*(det.l) + p0;	
        //cout << energy << " " << det.l << endl;
	      tt->Fill();

        //hPH->Fill(c.m_wp->m_ph); 
      }
  }
  cout << "you're done!\n"; 
  //auto oFile = new TFile(oFN,"recreate");
  tt->Write();
}
