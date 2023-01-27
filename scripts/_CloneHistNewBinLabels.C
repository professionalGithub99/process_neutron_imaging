// create a function that clones a histogram and changes the bin labels
// it takes a file name and a histogram name as arguments TString,and arguments a and b as floats
// it then creates a new file
// it then creates a new histogram with equal number of bins. However the bins are scaled to the new range newbin = oldbin*b + a
// it then sets the content of the old bin to the new bin
// it creates a clone of the new histogram, sets the directory to 0. Draws the clone
// Then it writes the new histogram to the new file

void _CloneHistNewBinLabels(TString fn, TString hFn, TString nFn, float a, float b)
{
    TFile *f = new TFile(fn);
    TH1F *h = (TH1F*)f->Get(hFn);
    cout << h->GetBinLowEdge(1)*b + a << " " << h->GetBinLowEdge(h->GetNbinsX())*b + a << endl;
    TFile *fnew = new TFile(nFn,"RECREATE");
    //th1f *hnew = new th1f(hfn,"hnew",h->getnbinsx(),h->getbinlowedge(1)*b + a,h->getbinlowedge(h->getnbinsx()+1)*b + a);
    //the reason for this is that in our simulation, we didnt use binning structure that we did here but instead 3000 bins from 0 to 30
    TH1F *hnew = new TH1F(hFn,"hnew",3000,0,30);
    for(int i = 1; i <= h->GetNbinsX(); i++)
    {
        hnew->Fill(h->GetBinLowEdge(i)*b + a,h->GetBinContent(i));
        cout<<i<<" " << h->GetBinLowEdge(i)*b + a << " " << h->GetBinContent(i) << endl;
    }

    cout<< h->GetNbinsX() << endl;
    cout<<h->GetBinLowEdge(1) << endl;
    cout<<h->GetBinLowEdge(0) << endl;
    cout<<h->GetBinLowEdge(h->GetNbinsX()) << endl;
    cout<<h->GetBinLowEdge(h->GetNbinsX()+1) << endl;
    //print bin content of the cloned histogram the only oen thats not 0 
    cout<<hnew->GetBinContent(1) << endl;
    TH1F *hclone = (TH1F*)hnew->Clone();
    hclone->SetDirectory(0);
    hclone->Draw();
    hnew->Write();
    fnew->Close();
    f->Close();
}
