void _TrimBackground() {
   TFile oldfile("../../EJ301D_Freedom/DAQ/run_00/RAW/DataR_CH0@DT5730S_13949_run_00.root");
   TTree *oldtree;
   oldfile.GetObject("Data_R", oldtree);
 
   const auto nentries = oldtree->GetEntries();
 
   ULong64_t current_timestamp;
   oldtree->SetBranchAddress("Timestamp", &current_timestamp);
 
   // Create a new file + a clone of old tree in new file
   TFile newfile("small.root", "recreate");
   auto newtree = oldtree->CloneTree(0);
 
   for (auto i : ROOT::TSeqI(nentries)) {
      oldtree->GetEntry(i);
      if (event->GetNtrack() > 605)
         newtree->Fill();
      event->Clear();
   }
 
   newtree->Print();
   newfile.Write();
}

