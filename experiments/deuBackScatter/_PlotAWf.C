//**********************
void _PlotAWf(std::string iFN = "in.root")
{
  Compass w;
  int dN = 0;
  
  w.LoadFile(iFN);
  w.m_wp->setPolarity(-1);

  bool keepGoing = true;
  auto c = new TCanvas;
  auto h = w.GetWF();
  h->Draw("same hist");
  c->Update();
  int count = 0;
  
  while(keepGoing)
  {
    cin.ignore();
    count++;
    h = w.GetWF();
    h->Draw("same hist");
    c->Update();
    cout << "Viewing event number: " << count << endl;
  }  
}

