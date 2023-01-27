#include <iostream>
#include "TGraph.h"
#include "TF1.h"
#include "TMath.h"
#include "TFile.h"
// g++ fit.cc `root-config --libs --cflags` -o fit
// resolution function
double fitf(double *v, double *par)
{
	double en = v[0];

  double fitval =
  sqrt ( pow(par[0],2) + pow(par[1],2) / en + pow(par[2]/en,2) );

	return fitval;
}

int ResolutionFit()
{

	//TFile *RootFile;
	//RootFile = new TFile("resolution.root", "RECREATE");

	const int numOfPoints = 6;

	double x[numOfPoints] =
  {
0.0595,
0.207,
0.4770,
1.062,
1.1180,
7.68
  };

  double y[numOfPoints] =
  { 
0.566,
0.191,
0.132,
0.098,
0.099,
0.07
  };

  double ex[numOfPoints];
  double ey[numOfPoints];

  for(int i=0; i<numOfPoints; i++)
  {
    ex[i] = x[i]*.05;
    ey[i] = y[i]*.05;
  }

  TGraphErrors *points = new TGraphErrors(numOfPoints, x, y, ex, ey);

  //TGraph *points = new TGraphErrors(numOfPoints, x, y);

	TF1 *func = new TF1("fit", fitf, 0.005, 10, 3);
	//func->SetParameters(.001, .001, .001);
  func->SetParLimits(0,0,1);
  func->SetParLimits(1,0,1);
  func->SetParLimits(2,0,1);
	points->Fit(func,"r");
	points->Draw("AP");

	//RootFile->Write();
	//RootFile->Close();

	return 0;
}
