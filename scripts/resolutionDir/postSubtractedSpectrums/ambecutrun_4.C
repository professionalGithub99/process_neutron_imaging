TCutG *ambecutrun_4(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.26/10
   
   TCutG *cutg = new TCutG("CUTG",11);
   cutg->SetVarX("run_data.amp");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,-532.934,0.0841071);
   cutg->SetPoint(1,440.12,0.07875);
   cutg->SetPoint(2,1143.71,0.0716071);
   cutg->SetPoint(3,2341.32,0.0653571);
   cutg->SetPoint(4,3074.85,0.0626786);
   cutg->SetPoint(5,13000,0.0626786);
   cutg->SetPoint(6,15440.1,0.0707143);
   cutg->SetPoint(7,15559.9,0.968036);
   cutg->SetPoint(8,-1550.9,0.958214);
   cutg->SetPoint(9,-1550.9,0.0939286);
   cutg->SetPoint(10,-532.934,0.0841071);
   cutg->Draw("");
   return cutg;
}
