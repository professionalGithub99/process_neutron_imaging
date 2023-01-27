bool reject;
double fline(double *x, double *par)
{
    if (reject && x[0] > 2.5 && x[0] < 3.5) {
      TF1::RejectPoint();
      return 0;
   }
   return par[0] + par[1]*x[0];
}
void fitTest() {
  new TCanvas;
     TF1 *f1 = new TF1("f1","[0] +[1]*x +gaus(2)",0,5);
   f1->SetParameters(6,-1,5,3,0.2);
   // create and fill histogram according to the source function
   f1->Draw();
   new TCanvas;
   TF1 *fl = new TF1("fl",fline,0,5,2);
   fl->SetParameters(2,-1);
   fl->Draw();
   new TCanvas;
   TH1F *h = new TH1F("h","background + signal",100,0,5);
   h->FillRandom("f1",2000);
   h->Fit("[0] +[1]*x +gaus(2)");
   h->Draw();
};
