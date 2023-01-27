{
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.24/02
   
   TCutG *cutg = new TCutG("CUTG",15);
   cutg->SetVarX("odessa.ene");
   cutg->SetVarY("odessa.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetLineWidth(3);
   cutg->SetMarkerStyle(20);
   cutg->SetPoint(0,0.987066,0.119355);
   cutg->SetPoint(1,3.11096,0.0990789);
   cutg->SetPoint(2,5.54799,0.0847999);
   cutg->SetPoint(3,6.86862,0.0839432);
   cutg->SetPoint(4,8.59769,0.0870846);
   cutg->SetPoint(5,8.59769,0.104505);
   cutg->SetPoint(6,6.74609,0.103934);
   cutg->SetPoint(7,4.92172,0.109645);
   cutg->SetPoint(8,2.4983,0.127637);
   cutg->SetPoint(9,1.34105,0.145343);
   cutg->SetPoint(10,0.701157,0.165905);
   cutg->SetPoint(11,0.197413,0.177042);
   cutg->SetPoint(12,0.251872,0.120497);
   cutg->SetPoint(13,1.04152,0.117927);
   cutg->SetPoint(14,0.987066,0.119355);
   cutg->Draw("");
   auto tf = new TFile("_odessa_dt_cut.root","recreate");
   cutg->Write();
}
