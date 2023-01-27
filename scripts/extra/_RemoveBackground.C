//________________

// 
//void ProcessWfs(string fPath, string oFN = "_wf_out.root");
//________________
void _RemoveBackground(string bFN, string fN,string bTN, string tN, string subFN)
{
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
  //open a file for bFn and fN 
  //create a tree for both of them
  //create a histogram for both of them
  //set the histogram of the background to negative
  //add the two histograms together
  //draw the histogram 
  //set the directory to nullptr for all three to save state
  TFile *bFile = new TFile(bFN.c_str());
  TFile *file = new TFile(fN.c_str());
  TTree *bTree = (TTree*)bFile->Get(bTN.c_str());
  TTree *tree = (TTree*)file->Get(tN.c_str());
  Detector b;
  Detector d;
  bTree->SetBranchAddress("run_data",&b);
  tree->SetBranchAddress("run_data",&d);
  TH1F *bHist = new TH1F("bHist","background;ADC;Counts",16384,0,16384);
  TH1F *hist= new TH1F("hist","hist;ADC;Counts",16384,0,16384);
  TH1F *bSub = new TH1F("histSubB","without background;ADC;Counts;",16384,0,16384);
  cout<<"btree entries "<<bTree->GetEntries()<<endl;
  cout<<"tre entries "<<tree->GetEntries()<<endl;
  for(int i = 0; i < bTree->GetEntries(); i++)
  {
    bTree->GetEntry(i);
    cout<<b.amp<<endl;
    bHist->Fill(b.amp);
  }
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    hist->Fill(d.amp);
  }
  bSub->Add(hist,bHist,1,-1);
  /*new TCanvas();
  bHist->Draw();
  new TCanvas();
  hist->Draw();
  new TCanvas();
  bSub->Draw();
  bHist->SetDirectory(nullptr);
  hist->SetDirectory(nullptr);
  bSub->SetDirectory(nullptr);*/
  TFile *subFile = new TFile(subFN.c_str(),"RECREATE");
  bSub->Write();
  bFile->Close();
  file->Close();
  subFile->Close();
}
