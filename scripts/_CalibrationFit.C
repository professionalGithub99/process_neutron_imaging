double fitf(double *v, double *par)
{
	double en = v[0];
        //create a linear function
        double fitval = par[0] + par[1]*en;
        return fitval;
}

int _CalibrationFit()
{
    const int numOfPoints = 4;
    double y[numOfPoints] =
  {
    0.341,
    0.477,
    1.06,
    4.188
  };


  double x[numOfPoints] =

  { 

    643.20,929.15,2059.379,8198.312
  };

  double ex[numOfPoints] =
  {
    0,0,0,0
  };

  double ey[numOfPoints] =
  {
    0,0,0,0
  };

  TGraphErrors *points = new TGraphErrors(numOfPoints, x, y, ex, ey);

  //TGraph *points = new TGraph(numOfPoints, x, y);

	TF1 *func = new TF1("fit", fitf, 0,10000, 2);
	//func->SetParameters(.001, .001, .001);
  //func->SetParLimits(0,0,1);
  //func->SetParLimits(1,0,1);
  //func->SetParLimits(2,0,1);
	points->Fit(func,"r");
	points->Draw("AP*");

	//RootFile->Write();
	//RootFile->Close();

	return 0;
}

