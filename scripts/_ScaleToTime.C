//________________
void _ScaleToTime(string fN, string tN, ULong64_t duration, string bfN, string btN, ULong64_t bDuration, string sfN)
{
  //
  //send to smart contract
  //get the hash
  //send hash to canister and canister checks the hash and mints an amount relative to amount sent in hash
  //to release funds, you need to check whether the canister token has the 
  //funds to release. Then send a hash to the smart contract and the smart contract marks the hash and sends out the amount of tokens
  //
  //open the file of the calibration source
  //open the tree of the calibration source
  //open the file for the background source
  //open the tree for the background source
  //create a histogram for the calibration source
  //create a histogram for the background source
  //create a histogram for the scaled calibration source
  //create a histogram for the scaled background source
  //create a histogram to subtract the background from the calibration source
  //create a file to save the histogram for the subtracted background from the calibration source
  //write the file for the subtracted background from the calibration source
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
  //conver duration from picoseconds to seconds
  double dur = duration/1000000000000.000000000000;
  double bDur = bDuration/1000000000000.000000000000;
  //print duration with 12 decimal places
  cout << "duration = " << setprecision(15) << dur << endl;
  std::cout<<"Duration in ulong64_t: "<<duration<<std::endl;
  cout << "bDuration = " << setprecision(15) << bDur << endl;
  std::cout<<"bDuration in ulong64_t: "<<bDuration<<std::endl;

  TFile *file = new TFile(fN.c_str());
  TTree *tree = (TTree*)file->Get(tN.c_str());

  TFile *bFile = new TFile(bfN.c_str());
  TTree *bTree = (TTree*)bFile->Get(btN.c_str());


  Detector d;
  tree->SetBranchAddress("run_data",&d);
  TH1F *hist= new TH1F("hist","counts_per_second;ADC;Counts",16384,0,16384);
  cout<<"tree entries "<<tree->GetEntries()<<endl;
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    hist->Fill(d.amp);
  }

  Detector bD;
  bTree->SetBranchAddress("run_data",&bD);
  TH1F *bHist= new TH1F("bHist","counts_per_second;ADC;Counts",16384,0,16384);
  cout<<"bTree entries "<<bTree->GetEntries()<<endl;
  for(int i = 0; i < bTree->GetEntries(); i++)
  {
    bTree->GetEntry(i);
    bHist->Fill(bD.amp);
  }


  /*new TCanvas;
  TH1F *histcopy = (TH1F*)hist->Clone();
  histcopy->Draw();
  histcopy->SetDirectory(0);*/
  new TCanvas;
  hist->Scale(1./dur);
  hist->Draw();
  hist->SetDirectory(0);

  new TCanvas;
  bHist->Scale(1./bDur);
  bHist->Draw();
  bHist->SetDirectory(0);

  TFile *sFile = new TFile(sfN.c_str(),"RECREATE");
  new TCanvas;
  TH1F *sHist =  new TH1F("sHist","without background;ADC;Counts",16384,0,16384);
  sHist->Add(hist,bHist,1,-1);
  //clone sHist to sHistDraw so that it can be drawn since its directory is set to 0
  TH1F *sHistDraw = (TH1F*)sHist->Clone();
  sHistDraw->Draw();
  sHistDraw->SetDirectory(0);
  //write the hist to the file
  sHist->Write();
  sFile->Close();
  file->Close();
  bFile->Close();
}
