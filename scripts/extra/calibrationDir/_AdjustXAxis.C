void _AdjustXAxis(string fN, string hN, float a,float b)
{
  //read the file
  //read the hist
  //get x axis range
  //convert the x axis range to formula x = a + b * x
  //set a
  //set b
  //create new hist
  //ranging from a to b*(max of old hist)
  //with number of bins equal to old hist
  //fill the new hist
  TFile *oldFile = new TFile(fN.c_str());
  TH1F *oldHist = (TH1F*)oldFile->Get(hN.c_str());
  double xMin = oldHist->GetXaxis()->GetXmin();
  double xMax = oldHist->GetXaxis()->GetXmax();
  double xNewMin = a + b * xMin;
  double xNewMax = a + b * xMax;
  TH1F *newHist = new TH1F("newHist","newHist",oldHist->GetNbinsX(),xNewMin,xNewMax);
  for(int i = 1; i <= oldHist->GetNbinsX(); i++)
  {
    newHist->SetBinContent(i,oldHist->GetBinContent(i));
  }
  //create a clone of the old hist
  //set the new hist to the clone
  //setdirectory of the clone to 0      
  //draw the clone
  new TCanvas;
  newHist->Draw();
}
