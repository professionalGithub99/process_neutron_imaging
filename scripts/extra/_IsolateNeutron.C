typedef struct {
  Float_t amp;             // ADC value
  Float_t psd;             // PSD parameter s/l
} CondensedDetector;

//________________

// 
//void ProcessWfs(string fPath, string oFN = "_wf_out.root");
//________________
void _IsolateNeutron(string fN, string hN, string nFN,TCutG *gCut)
{
  //create a new file called file with fN.c_str() as the title and read priviledges
  //create new TDirectoryFile named psdEFile using file->Get<TDirectoryFile>("PSD_E;1")
  //create a new TH2I histogram named hist2D using psdEDirFile->Get<TH2I>(hSN);
  //create a new file named newFile with nFN.c_str()
  //Create a new Tree called newTree 
  //loop through the thist2 and check whether the point is in the GCut
  //if it is, add it to the tree 
  //potentially convert the treeEnergy units to volts as well.
  TFile *file = new TFile(fN.c_str(),"READ");
  TDirectoryFile *psdEFile = file->Get<TDirectoryFile>("PSD_E;1");
  cout<<"psdEFile: "<<psdEFile<<endl;
  psdEFile->ls();
  TH2I *hist2D = psdEFile->Get<TH2I>("_R_PSDvsECH0@DT5730S_13949;1");
  new TCanvas;
  hist2D->SetDirectory(0);
  hist2D->Draw();
  TFile *newFile = new TFile(nFN.c_str(),"RECREATE");
  TTree *tt;
  CondensedDetector cDet;
  tt = new TTree("T", "run_data");
  tt->Branch("run_data",&cDet,"psd:amp");

  cout<<hist2D<<endl;
  cout<<hist2D->GetEntries()<<" entries"<<endl;

  //checker to ensure after summing through all getbincontents it equals GetEntries
  int total = 0;
  int xBinMax = hist2D->GetXaxis()->GetXmax();
  int xBinMin = hist2D->GetXaxis()->GetXmin();
  int yBinMax = hist2D->GetYaxis()->GetXmax();
  int yBinMin = hist2D->GetYaxis()->GetXmin();
  int pointsInside = 0;
  cout<<xBinMin<<" xbinmin "<<xBinMax<<"xBinmax"<<endl;
  cout<<yBinMin<<" ybinmin "<<yBinMax<<"yBinMax"<<endl;
  cout<<hist2D->GetNbinsX()<<" xbins "<<hist2D->GetNbinsY()<<" ybins"<<endl;
  hist2D->SetDirectory(0);
  hist2D->Draw();
for(int j = 0; j<=hist2D->GetNbinsY()+1;j++){
  for(int i = 0; i <= hist2D->GetNbinsX()+1; i++){
    if(hist2D->GetBin(j,i) <= xBinMax){
      total += hist2D->GetBinContent(j,i);
      }
      if(gCut->IsInside(hist2D->GetXaxis()->GetBinCenter(i),hist2D->GetYaxis()->GetBinCenter(j))){
        for (int x = 0; x<hist2D->GetBinContent(j,i);x++){
          cDet.amp = hist2D->GetXaxis()->GetBinCenter(i);
          cDet.psd = hist2D->GetYaxis()->GetBinCenter(j);
          //cout<<hist2D->GetXaxis()->GetBinCenter(i)<<" "<<hist2D->GetYaxis()->GetBinCenter(j)<<endl;
          tt->Fill();
          pointsInside++;
        }
      }
  }
  }
  cout<<total<<" total"<<endl;
  cout<<pointsInside<<" points inside"<<endl;
  //close all files
  tt->Write();
  newFile->Close();
  file->Close();

}

