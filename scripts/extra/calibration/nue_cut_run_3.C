TCutG * nue_cut_run_3(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.24/06
   TCutG *cutg = new TCutG("CUTG",12);
   cutg->SetVarX("");
   cutg->SetVarY("");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,113.43,0.41543);
   cutg->SetPoint(1,2734.59,0.213086);
   cutg->SetPoint(2,5309.76,0.169336);
   cutg->SetPoint(3,16330.9,0.163867);
   cutg->SetPoint(4,16346.2,0.137207);
   cutg->SetPoint(5,16116.3,0.0879883);
   cutg->SetPoint(6,7777.63,0.0845703);
   cutg->SetPoint(7,4052.83,0.0941406);
   cutg->SetPoint(8,787.88,0.115332);
   cutg->SetPoint(9,-131.824,0.135156);
   cutg->SetPoint(10,-70.5108,0.191895);
   cutg->SetPoint(11,113.43,0.41543);
   cutg->Draw("");
   return cutg;
}
