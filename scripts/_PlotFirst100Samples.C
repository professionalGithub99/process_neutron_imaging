void _PlotFirst100Samples(string oFN)
{
  TString ts_oFN(oFN);
  TFile *ff = new TFile(ts_oFN);
  TTree *tt = (TTree*)ff->Get("T");
  ff->ls();
  for(int i =1; i<=1000;i++)
  {
    char p[sizeof("hist")+sizeof(char)+sizeof(i)];
    sprintf(p, "hist%d", i);
    TH1F *hist = ff->Get<TH1F>(p);
    //set histogram x axis label to ns
    hist->GetXaxis()->SetTitle("ns");
    //set histogram y axis label to Volts
    hist->GetYaxis()->SetTitle("Volts");
    //set histogram title to oFN
    hist->SetTitle(ts_oFN);
    if(i==1)
    {
      new TCanvas;
      hist->Draw();
    }
    else
    {
      hist->Draw("same");
    }
    hist->GetYaxis()->SetRangeUser(0, 2);
  }
}
