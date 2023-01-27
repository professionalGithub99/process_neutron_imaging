TCutG * TCutGRun3(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.24/06
   TCutG *cutg = new TCutG("CUTG",7);
   cutg->SetVarX("run_data.ene");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,-0.000563919,0.880503);
   cutg->SetPoint(1,1.14135,0.883151);
   cutg->SetPoint(2,1.1188,1.07112);
   cutg->SetPoint(3,0.887594,1.31999);
   cutg->SetPoint(4,-0.031579,1.3041);
   cutg->SetPoint(5,-0.093609,1.21144);
   cutg->SetPoint(6,-0.000563919,0.880503);
   cutg->Draw("");
   return cutg;
}
