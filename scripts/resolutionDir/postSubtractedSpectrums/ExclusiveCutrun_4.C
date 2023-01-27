TCutG *ExclusiveCutrun_4(){
//========= Macro generated from object: CUTG/Graph
//========= by ROOT version6.26/10
   TCutG *cutg = new TCutG("CUTG",9);
   cutg->SetVarX("run_data.amp");
   cutg->SetVarY("run_data.psd");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,-6.27955,0.0718571);
   cutg->SetPoint(1,6335.95,0.0631667);
   cutg->SetPoint(2,12709.6,0.0683809);
   cutg->SetPoint(3,15927.8,0.0944524);
   cutg->SetPoint(4,13007.8,1.06952);
   cutg->SetPoint(5,-367.347,1.06952);
   cutg->SetPoint(6,-1042.39,0.95481);
   cutg->SetPoint(7,-1042.39,0.95481);
   cutg->SetPoint(8,-6.27955,0.0718571);
   cutg->Draw("");
   return cutg;
}
