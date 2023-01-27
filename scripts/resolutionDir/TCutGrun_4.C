TCutG *TCutGrun_4(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.26/10
   
   TCutG *cutg = new TCutG("CUTG",13);
   cutg->SetVarX("run_data.amp");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,-1476.05,0.0858095);
   cutg->SetPoint(1,-308.383,0.0734762);
   cutg->SetPoint(2,-83.8325,0.0699524);
   cutg->SetPoint(3,-128.743,0.0637857);
   cutg->SetPoint(4,10934.1,0.0611429);
   cutg->SetPoint(5,14706.6,0.0637857);
   cutg->SetPoint(6,15230.5,-0.0789286);
   cutg->SetPoint(7,10065.9,-0.168786);
   cutg->SetPoint(8,-188.623,-0.243667);
   cutg->SetPoint(9,-1146.71,-0.15381);
   cutg->SetPoint(10,-278.443,0.0629048);
   cutg->SetPoint(11,-128.743,0.0681905);
   cutg->SetPoint(12,-1476.05,0.0858095);
   cutg->Draw("");
   return cutg;
}
