TCutG *TCutGrun_00(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.26/10
   
   TCutG *cutg = new TCutG("CUTG",6);
   cutg->SetVarX("run_data.amp");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,-1924.81,0.0572605);
   cutg->SetPoint(1,15706.8,0.0578026);
   cutg->SetPoint(2,14203,-0.102661);
   cutg->SetPoint(3,-872.181,-0.0988666);
   cutg->SetPoint(4,-872.181,0.0572605);
   cutg->SetPoint(5,-1924.81,0.0572605);
   cutg->Draw("");
   return cutg;
}
