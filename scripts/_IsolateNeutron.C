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
//________________

// 
//void ProcessWfs(string fPath, string oFN = "_wf_out.root");
//________________
void _IsolateNeutron(string fN, string nFN, TCutG *gCut)
{
  TFile *file = new TFile(fN.c_str(),"READ");
  TTree *tree = (TTree*)file->Get("T");

  TFile *newFile = new TFile(nFN.c_str(),"RECREATE");
  TTree *newTree = new TTree("T","run_data");


  // Create an array of trees
  Detector det;
  tree->SetBranchAddress("run_data",&det);

  Detector newDet;
  TTree *tt;
  tt = new TTree("T", "run_data");
  tt->Branch("run_data",&newDet,"l:s:amp:ene:cfd:psd:trg:sBl:eBl");

  int insideCounter = 0;
  for (int i = 0; i < tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if(gCut->IsInside(det.ene,det.psd)){
      insideCounter+=1;
      newDet.l = det.l;
      newDet.s = det.s;
      newDet.amp = det.amp;
      newDet.ene = det.ene;
      newDet.cfd = det.cfd;
      newDet.psd = det.psd;
      newDet.trg = det.trg;
      newDet.sBl = det.sBl;
      newDet.eBl = det.eBl;
      tt->Fill();
    }
  }
  std::cout<<insideCounter<<std::endl;
  tt->Write();
  newFile->Close();
  file->Close();
}

