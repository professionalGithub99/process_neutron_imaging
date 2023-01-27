void getFitParams(TString fN)
{
  new TCanvas;
  TFile *f = new TFile(fN);
  //open up a TF1 named PrevFitTMP
  TF1 *PrevFitTMP = (TF1*)f->Get("PrevFitTMP");
  PrevFitTMP->Draw();
  //get the bounds of the PrevFitTMP three standard deviations
  double xmin = PrevFitTMP->GetXmin();
  double xmax = PrevFitTMP->GetXmax();
  double xmean = PrevFitTMP->GetParameter(1);
  double xstd = PrevFitTMP->GetParameter(2);
 TF1 *fgaus = new TF1("fgaus","gaus(0)",xmean - 3*xstd,xmean + 3*xstd);
   cout << "Fit parameters: " << endl;
   cout<< "constant: " << PrevFitTMP->GetParameter(0) << endl;
    cout << "Mean: " << PrevFitTMP->GetParameter(1) << endl;
    cout << "Sigma: " << PrevFitTMP->GetParameter(2) << endl;
    //get 87.5% of constant
    //get 50% of constant
    //get 12.5% of constant
    //get 58% of constant
    float c = PrevFitTMP->GetParameter(0);
    float m = PrevFitTMP->GetParameter(1);
    float mEval = PrevFitTMP->Eval(PrevFitTMP->GetParameter(1));
    float s = PrevFitTMP->GetParameter(2);
    float c875 = c*0.875;
    float c50 = c*0.5;
    float c125 = c*0.125;
    float c58 = c*0.58;

    cout << "c875: " << c875 << endl;
    cout << "c50: " << c50 << endl;
    cout << "c125: " << c125 << endl;
    cout << "c58: " << c58 << endl;
    cout << "mEval: " << mEval << endl;
    cout << "m: " << m << endl;
      
   fgaus->SetParameters(PrevFitTMP->GetParameter(0),PrevFitTMP->GetParameter(1),PrevFitTMP->GetParameter(2));
    fgaus->SetLineColor(kRed);
    cout<<"fgaus getx 875: " << fgaus->GetX(c875,m,m+5*s) << endl;
    cout<<"fgaus getx 50: " << fgaus->GetX(c50,m,m+5*s) << endl;
    cout<<"fgaus getx 125: " << fgaus->GetX(c125,m,m+5*s) << endl;
    cout<<"fgaus getx 58: " << fgaus->GetX(c58,m,m+5*s) << endl;
    fgaus->Draw();
  f->Close();


  /*new TCanvas;
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
    */

}
