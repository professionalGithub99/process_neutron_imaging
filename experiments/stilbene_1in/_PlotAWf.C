//**********************
void _PlotAWf(std::string iFN = "in.root")
{
  Compass c(iFN);
  //c.LoadFile(fPath);
  c.m_wp->setPolarity(-1); // set polarity to (-)
  c.m_wp->setBlPnts(20);
  c.m_wp->setSgBin(11);
  c.m_wp->setEndBin(100);
  c.m_wp->setClippedVal(16384);
  c.m_wp->printWfLimits();

  bool keepGoing = true;
  auto tc = new TCanvas;
  auto h = c.GetWF();
  h->Draw("same hist");
  tc->Update();
  int count = 0;
  
  while(keepGoing)
  {
    //h->Reset();
    cin.ignore();
    count++;
    h = c.GetWF();
    h->Draw("hist");
    tc->Update();
    cout << "Viewing event number: " << count << endl;
  }  
}

