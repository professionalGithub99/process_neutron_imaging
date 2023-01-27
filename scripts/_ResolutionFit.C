double fitf(double *v, double *par)
{
	double en = v[0];

  double fitval =
  sqrt ( pow(par[0],2) + pow(par[1],2) / en + pow(par[2]/en,2) );

	return fitval;
}

void _ResolutionFit()
{

	//TFile *RootFile;
	//RootFile = new TFile("resolution.root", "RECREATE");
	const int numOfPoints = 5;
	double x[numOfPoints] =
  {
0.0596,
0.341,
0.477,
1.06,
4.188

  };

  double y[numOfPoints] =
  { 
0.659254696,
0.251519175,
0.20529468,
0.137414411,
0.068306317
  };

  double ex[numOfPoints] =
  {
    0,0,0,0,0
  };

  double ey[numOfPoints] =
  {
    0.032962735,0.012575959,0.010264728,0.006870721,0.003415316
  };

  TGraphErrors *points = new TGraphErrors(numOfPoints, x, y, ex, ey);

	TF1 *func = new TF1("fit", fitf, 0.02, 4.5, 3);
	//func->SetParameters(.001, .001, .001);
	points->Fit(func,"r");
	points->Draw("AP*");

	//RootFile->Write();
	//RootFile->Close();

	return 0;
}
