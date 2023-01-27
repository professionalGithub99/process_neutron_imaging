typedef struct {
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
void _RemoveBackgroundAsTree(string bFN, string fN,string bTN, string tN, string subFN)
{
//__________
  //open a file for bFn and fN 
  //create a tree bFn called bTree and fN called tree a new tree for the subtraction called subTree
  //create a three detectors called bDet, det and subDet
  //set branch address of the trees to the detectors
  //create a map to hold branch numbers that have been found to be the same in det as in bDet
  //loop through the tree and check if it's amp value is equal to any of the bTree amp values
  //if one of the values is found in the btree then check whether the btree entry number has been stored in the map
  //if not,store it in the map and exit the bTree loop without adding to the subTree
  //If no value is found that is equal to any values in the bTree then add to the subTree
  //write that subTree to the subFile root file
  TFile *bFile = new TFile(bFN.c_str(),"READ");
  TFile *file = new TFile(fN.c_str(),"READ");
  TFile *subFile = new TFile(subFN.c_str(),"RECREATE");
  TTree *bTree = (TTree*)bFile->Get(bTN.c_str());
  TTree *tree = (TTree*)file->Get(tN.c_str());
  TTree *subTree = new TTree("T","run_data");
  std::unordered_map<int, bool> map(10000);
  Detector bDet;
  Detector det;
  Detector subDet;
  bTree->SetBranchAddress("run_data",&bDet);
  tree->SetBranchAddress("run_data",&det);
  subTree->Branch("run_data",&subDet,"l:s:amp:ene:cfd:psd:trg:sBl:eBl");
  int sameCount = 0;
  int sameCountUncut = 0;
  cout<<"bTree entries: "<<bTree->GetEntries()<<endl;
  cout<<"tree entries: "<<tree->GetEntries()<<endl;
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    bool found = false;
    for(int j = 0; j < bTree->GetEntries(); j++)
    {
      bTree->GetEntry(j);
      if(bDet.amp == det.amp)
      {
        if(map[j] == true)
        {
          sameCountUncut++;
        }
        else{
          found = true;
          sameCount +=1;
          map[j] = true;
          break;
        }
      }
    }
    if(found == false){
      subDet.l = det.l;
      subDet.s = det.s;
      subDet.amp = det.amp;
      subDet.ene = det.ene;
      subDet.cfd = det.cfd;
      subDet.psd = det.psd;
      subDet.trg = det.trg;
      subDet.sBl = det.sBl;
      subDet.eBl = det.eBl;
      cout<<"i = "<<i<<endl;
      cout<<det.psd<<" det.psd"<<endl;
      cout<<subDet.psd<<" subDet.psd"<<endl;
      cout<<det.amp<<" det.ene"<<endl;
      cout<<subDet.amp<<" subDet.ene"<<endl;
      subTree->Fill();
    }
  }
  cout<<sameCount<<" same values"<<endl;
  cout<<sameCountUncut<<" same count unct"<<endl;
  cout<<"subtree entry count: "<<subTree->GetEntries()<<endl;
  cout<<subTree->GetDirectory()<<endl;
  cout<<subFile<<endl;
  subTree->SetDirectory(subFile);
  subTree->Write();
  subFile->Close();
  file->Close();
  bFile->Close();
}
