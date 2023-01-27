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
void _ReverseEngLabBin(string fPath, string oFN = "_wf_out.root")
{
  CAEN c;
  c.LoadBinFile(fPath);
  c.SetDefaults();
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(30);
  c.m_wp->setEndBin(200);
  c.m_wp->setClippedVal(16384);
  c.m_wp->printWfLimits();

  int eventCounter = 0;
  TString ts_oFN(oFN);
  TFile *ff = new TFile(ts_oFN, "RECREATE");
		
  // Create an array of trees
  Detector det;
  TTree *tt;
	tt = new TTree("T", "run_data");
  tt->Branch("run_data",&det,"l:s:amp:ene:cfd:psd:trg:sBl:eBl");

  // calibration factor
  float p0 = 0;//0.02178;//0.00429523;
  float p1 = 5.46730179969649E-05;//0.00087;//5.46730179969649E-05;// 
  bool checkEndOfFile = false;
  while(!checkEndOfFile)
  {
    checkEndOfFile = c.GetBinEntry();
    det.l = c.energy;
    det.s = c.shortEnergy;
    if (det.l > 0) {det.psd = (det.l-det.s)/det.l;}
    else{ det.psd = -1;}
    det.ene = p1*(det.l) + p0;
    eventCounter++;
    std::cout<<"detl "<<det.l<<"dets "<<det.s<<std::endl;
    tt->Fill();
  }
  std::cout<<eventCounter<<std::endl;

    cout<<"tt dir"<<tt->GetDirectory()<<endl;
    cout<<"ff"<<ff<<endl;
  cout << endl; 
  tt->SetDirectory(ff);
  tt->Write();
  ff->Close();
}
