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

// 
//void ProcessWfs(string fPath, string oFN = "_wf_out.root");
//________________
/*void _ProcessWfs(string fPath = "in.root", int sF = 0, int eF = 0)
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
}*/
// ************************
// *** event processing ***
// ************************
void _ReverseEngLab(string fPath, string oFN = "_wf_out.root")
{
  CAEN c(fPath);
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(30);
  c.m_wp->setEndBin(200);
  c.m_wp->setClippedVal(16384);
  c.m_wp->printWfLimits();

  TString ts_oFN(oFN);
  TFile *ff = new TFile(ts_oFN, "RECREATE");
		
  // Create an array of trees
  Detector det;
  TTree *tt;
	tt = new TTree("T", "run_data");
  tt->Branch("run_data",&det,"l:s:amp:ene:cfd:psd:trg:sBl:eBl:time/g");

  // calibration factor
  float p0 = 0;//0.02178;//0.00429523;
  float p1 = 5.46730179969649E-05;//0.00087;//5.46730179969649E-05;// 
  bool checkEndOfFile = false;
  while(!checkEndOfFile)
  {
    if(c.eventCounter%5000 == 0)
    {
      cout << "events processed -----> \t" << c.eventCounter 
           << "\t\r" << std::flush;
    }
    // grab entry
    bool noHeader = false;
    checkEndOfFile = c.GetROOTEntry();
    if(checkEndOfFile){break;};
    c.ProcessAWfEvent();
    char p[sizeof("hist")+sizeof(char)+sizeof(c.eventCounter)];
    sprintf(p, "hist%d", c.eventCounter);
    //TH1F *hwfV1 = (TH1F*) c.m_wp->hwfV->Clone(p);
    //hwfV1->Write();
    //cout<<"the pointer to c.m_wp->hwfV is "<<c.m_wp->hwfV<<endl;
    //cout<<"the pointer to det.hwfV is "<<det.hwfV<<endl<<endl;
    c.m_wp->m_full;
    double sVal = c.m_wp->m_sg;
    double lVal = c.m_wp->m_full;
    det.l     = lVal;
    det.s     = sVal;
    if(det.l > 0) det.psd = det.s / det.l;
    else{ det.psd = -1; };
    // FLAGS: not used
 	  det.amp = c.m_wp->m_ph;
    det.cfd = c.m_wp->m_cfdPos;
    det.ene = p1*(det.l) + p0;	
    det.trg = c.m_wp->m_peakPos;;
    det.sBl = c.m_wp->m_startBl;
    det.eBl = c.m_wp->m_endBl;
    if(det.amp < 20) det.cfd = 0;

    //cout << det.trg << " " << det.cfd <<  " ph: " << det.amp << endl;
	  tt->Fill();
  }
  tt->SetDirectory(ff);
  tt->Write();
  ff->Close();
  //delete tt;
}
