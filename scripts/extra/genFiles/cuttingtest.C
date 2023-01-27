TCutG *cuttingtest(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.24/06
   
   TCutG *cutg = new TCutG("CUTG",12);
   cutg->SetVarX("run_data.ene");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,0.101351,-0.107848);
   cutg->SetPoint(1,0.317192,-0.0346634);
   cutg->SetPoint(2,1.06794,-0.0159418);
   cutg->SetPoint(3,1.40578,-0.000624057);
   cutg->SetPoint(4,0.777027,0.0232035);
   cutg->SetPoint(5,0.467342,0.0163956);
   cutg->SetPoint(6,0.20458,0.0657526);
   cutg->SetPoint(7,0.0825825,0.120216);
   cutg->SetPoint(8,0.0544294,-0.143589);
   cutg->SetPoint(9,0.148273,-0.0993381);
   cutg->SetPoint(10,0.110736,-0.102742);
   cutg->SetPoint(11,0.101351,-0.107848);
   cutg->Draw("");
   return cutg;
}
