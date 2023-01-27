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
void _PlotPSP(string fN,string tN)
{
  //open a file for  fN 
  //create a tree for file
  //create a reference of detector called d
  //set the trees branch address to d
  //create a TH2F for file ranging from 0 -> 0.8 in the y and  0->10 in the x
  //the y label is psp and the x label is light output in MeVee
  //set Pallete for histogram as kBird
  //for all entries in the tree 
  //fill the histogram with d.psd on the y and d.ene on the x
  TFile *file = new TFile(fN.c_str(),"READ");
  TTree *tree = (TTree*)file->Get(tN.c_str());
  Detector d;
  tree->SetBranchAddress("run_data",&d);
  TH2F *h = new TH2F("h","PSP vs Light Output",1000,0,5,1000,-0.6,0.6);
  h->GetXaxis()->SetTitle("Light Output (MeVee)");
  h->GetYaxis()->SetTitle("PSP");
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    h->Fill(d.ene,d.psd);
    cout<<"i = "<<i<<endl;
    cout<<"d.ene = "<<d.ene<<endl;
    cout<<"d.psd = "<<d.psd<<endl;
  }
  //create a canvas for the histogram
  new TCanvas;
  //draw the histogram
  h->Draw("colz");
  h->SetDirectory(nullptr);
  file->Close();
}

