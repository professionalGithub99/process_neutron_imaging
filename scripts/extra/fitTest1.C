Double_t myfunction(Double_t *x, Double_t *par)
{
   Float_t xx =x[0];
   Double_t f = TMath::Abs(par[0]*sin(par[1]*xx)/xx);
   return f;
}
void myfunc()
{
   auto f1 = new TF1("myfunc",myfunction,0,10,2);
   f1->SetParameters(11,11);
   f1->SetParNames("constant","coefficient");
   f1->Draw();
}
bool reject;
double fline(double *x, double *par)
{
    if (reject && x[0] > 2.5 && x[0] < 3.5) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0] + par[1]*x[0];
}
void fitTest1()
{
  new TCanvas;
   TF1 *fdata = new TF1("fdata","[0] +[1]*x +gaus(2)",0,5);
   fdata->SetParameters(6,-1,5,3,0.2);
   auto h1 = new TH1F("h1","test",100,0,5);
   h1->FillRandom("fdata",20000);
   TF1 *fl = new TF1("fl","gaus(0)",0,5);
   fl->SetRange(2.8,3.3);
   h1->Fit("fl","R");
   h1->Draw();
   //now use the fit parameters of the guassian to crate a new TF1 gaussian and see the output
   //of the fit
   new TCanvas;
   TF1 *fgaus = new TF1("fgaus","gaus(0)",0,5);
    fgaus->SetParameters(fl->GetParameter(0),fl->GetParameter(1),fl->GetParameter(2));
    fgaus->Draw();
    //get the y at the mean
    double mean = fgaus->GetParameter(1);
    double meany = fgaus->Eval(mean);

    //print mean and meanx
    cout << "mean = " << mean << endl;
    cout << "meanx = " << meany << endl;
    double x85 = fgaus->GetX(0.85*meany,2.9,5);
    cout<< "x85 = " << x85 << endl;

}
