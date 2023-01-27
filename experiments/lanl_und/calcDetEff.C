// ROOT script to calculate intrinsic detection eff. from a Geant4 sim using
// histograms
// *******
#include <stdio.h>
#include <assert.h>

int calcDetEff(TString fExt = "in.root", double loThres = 0.05)
{
  cout << "LO threshold = " << loThres << " MeVee\n";
  
  int nFiles = 600;
  double enIn = 0.05;

  ofstream myFile;
  myFile.open ("detEff.txt");

  TFile *f1; //= new TFile(iFN,"read" ); 
  // hLightOutputRaw_En0.05MeV
  TH1F *h;
  TH1F *hSrc;
  double thres[3] = {.05,.1,.2};
  TH1F *hEff[3];
  for(int i=1; i<=nFiles; i++)
  { 
    // check number of bins in the input 
    double en = (i) * enIn;                          
    stringstream ss ;
    ss << fExt << "_en_" << en << ".root";                            
    TString fN = ss.str();                                                      
    f1 = TFile::Open(fN);
    h =    (TH1F*)f1->Get("Det0_lo_smear");             
    hSrc = (TH1F*)f1->Get("Det0_src");
    // loop over lo thresholds
    for(int t=0; t<3; t++)
    {
      if( i==1 )
      {
        TString hN = "thres_" + to_string(thres[t]);
        hEff[t] = new TH1F(hN,"",30/enIn,0,nFiles*enIn);
      }

      h->GetXaxis()->SetRangeUser(thres[t],h->GetXaxis()->GetXmax());
      double eff = h->Integral() / 2000000;//hSrc->Integral();
      //cout << en << " " << eff << endl;
      hEff[t]->Fill(en,eff);
      if(t == 0) myFile << en << " ";
      myFile << eff << " " ;
    } 
    myFile << endl;
    /*
    for(int j=0; j<nBins; j++)
    {
      myfile << h->GetBinContent(j+1) << " ";
    }
    myfile << endl;*/
  }
  auto of = new TFile("_detEff.root","recreate");
  auto c = new TCanvas;
  for(int t=0; t<3; t++)
  {
    hEff[t]->Write();
    hEff[t]->SetLineColor(t+1);
    hEff[t]->Draw("same hist");
  }
  c->BuildLegend();
  return 0;
}
