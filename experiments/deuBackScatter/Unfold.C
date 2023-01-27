// Light-output spectrum unfolding
/*
 *\brief a test program for UNFOLDING 
 */

typedef struct
{
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

// **************
void Unfold(TString iFN, bool unfold = 1)
{ 
  // read in experiment
  Detector d;

  //TString iFN = "../data_sr/DataR_CH3@DT5730S_14096_AmBe-shielded-long.csv.root";  
  TFile *f = new TFile(iFN);
  
  TString channel = "run_data";
  TTree *T;// = (TTree*)f->Get(channel);
  f->GetObject(channel, T);
  T->SetBranchAddress("odessa",&d);
  // read in cut files
  // ej301d_sr_ch1_cut.root
  auto fNCut = new TFile("_odessa_dt_cut.root");//ej301d_cut.root");//ej301d_cut.root");
  auto CUTG = (TCutG*)fNCut->Get("CUTG");
  CUTG->SetName("CUTG");

  CUTG->SetVarX("odessa.ene");
  CUTG->SetVarY("odessa.psd");
  
  cout << "enteries: " << T->GetEntriesFast() << endl;
  //T->Draw("E*.9 >> hL(800,0,20)","E>.1 && d.psd>0.16 && d.psd<0.45"); // for CCO SR data
  T->Draw("odessa.ene >> hL(3000,0,30)","CUTG");
  auto hL = (TH1F*)gDirectory->Get("hL");
  T->Draw("odessa.psd:odessa.ene >> h2(800,0,20,250,0,0.3)","","colz");
  auto h2 = (TH2F*)gDirectory->Get("h2");

  // ***** move onto UNFOLDING *****
  // Creates an unfolding object 
  if(unfold)
  {
    UNFOLD::FILEIO *fio;
    fio = new UNFOLD::FILEIO();
    //Sets parameters for unfolding relation.
    //fio->setRebinVal(4);
    fio->setNbins(3000);
    fio->setEbins(600);
    fio->setLightRange(0,30);
    fio->setLOThreshold(0.1);
    fio->setNeuEnRange(0.05,30);
    fio->setEnergyIncrement(0.05); // in units of MeV
    fio->setResponse("odessa_simMatrix_30MeV.txt");

    fio->readInExpSpectrum(hL);
  
    UNFOLD::UNFOLD s(fio);
    s.printParameters();
    // add these to FILEIO class
    cout << "setting response matrix\n";
    s.setResponseMatrix();
    s.setData();
    // Ensure that the highest bin you include is not higher than energy_increment*fio->setNbins(<your number>)
    cout << "calculating normal matrix\n";
    s.calcNormMatrix();
    cout << "setting initial guess\n";
    s.setInitialGuess(1.);
    //prints the forward projection, light output spectrum, neutron guess, and ground truth neutron specrum does the mlem unfolding for i counts
    for(int i=0; i<200; i++)
    {
      s.updateMLEM(i);
    }
    //s.plotRMSE();
    //s.plotProjection();
    //s.plotAnswer(true);
    bool printResults = false;
    s.plotResults(printResults);
    //s.correctForEff("_detEff.root");
    s.writeResults();
  }
  cout << "input spectrum coutntsL: " << hL->Integral() << endl; 
  return;
}


