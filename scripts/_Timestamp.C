void _Timestamp(string fN, string tN) {
  ULong64_t timestamp;
  TFile *file = new TFile(fN.c_str());
  TTree *tree = (TTree*)file->Get(tN.c_str());
  tree->SetBranchAddress("Timestamp", &timestamp);
  ULong64_t start_timestamp = 0;
  ULong64_t end_timestamp = 0;
  for(int i = 0; i < tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if(i == 0) {
      start_timestamp = timestamp;
    }
    if(i == tree->GetEntries()-1) {
      end_timestamp = timestamp;
    }
    std::cout<<"Entry: "<<i << "Timestamp: " << timestamp << std::endl;

  }
  std::cout << "Start Timestamp: " << start_timestamp << std::endl;
  std::cout << "End Timestamp: " << end_timestamp << std::endl;
  std::cout<<"difference = "<<end_timestamp-start_timestamp<<std::endl;
  std::cout<<"entries = "<<tree->GetEntries()<<std::endl;
}
