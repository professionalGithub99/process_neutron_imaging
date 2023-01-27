void _Timestamp(string fN, string tN) {
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
    ULong64_t time;            // timestamp
  } Detector;
  TFile *file = new TFile(fN.c_str());
  TTree *tree = (TTree*)file->Get(tN.c_str());
  //loop through timestamp printing out thj time
  ULong64_t current_timestamp,previous_timestamp, start_timestamp, end_timestamp;
  previous_timestamp = 0;
  Detector det;
  tree->SetBranchAddress("run_data", &det);
  for(int i = 0; i < tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if(i == 0) {
      start_timestamp = det.time;
    }
    if(i == tree->GetEntries()-1) {
      end_timestamp = det.time;
    }
    std::cout << "Timestamp: " << det.time << std::endl;
    std::cout<<"time difference = "<<det.time-previous_timestamp<<std::endl;
    previous_timestamp = det.time;
    std::cout<<"previous_timestamp = "<<previous_timestamp<<std::endl;
  }
  std::cout<<"start time = "<<start_timestamp<<std::endl;
  std::cout<<"end time = "<<end_timestamp<<std::endl;
  std::cout<<"difference = "<<end_timestamp-start_timestamp<<std::endl;
}
