/////////////////////////////////////////////////////////////////////////
//   This class has been automatically generated 
//   (at Tue Oct 18 01:48:44 2022 by ROOT version 6.24/06)
//   from TTree T/run_data
//   found on file: _wf_out0.root
/////////////////////////////////////////////////////////////////////////


#ifndef generatedSel_h
#define generatedSel_h

#define R__BRANCHPROXY_GENERATOR_VERSION 2

// ROOT headers needed by the proxy
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TPad.h>
#include <TH1.h>
#include <TSelector.h>
#include <TBranchProxy.h>
#include <TBranchProxyDirector.h>
#include <TBranchProxyTemplate.h>
#include <TFriendProxy.h>
using namespace ROOT::Internal;
using ROOT::Detail::TBranchProxy;

// forward declarations needed by this particular proxy


// Header needed by this particular proxy


class _wf_out0_Interface {
   // This class defines the list of methods that are directly used by generatedSel,
   // and that can be overloaded in the user's script
public:
   void _wf_out0_Begin(TTree*) {}
   void _wf_out0_SlaveBegin(TTree*) {}
   Bool_t _wf_out0_Notify() { return kTRUE; }
   Bool_t _wf_out0_Process(Long64_t) { return kTRUE; }
   void _wf_out0_SlaveTerminate() {}
   void _wf_out0_Terminate() {}
};


class generatedSel : public TSelector, public _wf_out0_Interface {
public :
   TTree          *fChain;         //!pointer to the analyzed TTree or TChain
   TH1            *htemp;          //!pointer to the histogram
   TBranchProxyDirector fDirector; //!Manages the proxys

   // Optional User methods
   TClass         *fClass;    // Pointer to this class's description

   // Wrapper class for each unwounded class
   struct TPx_run_data
   {
      TPx_run_data(TBranchProxyDirector* director,const char *top,const char *mid=0) :
         ffPrefix     (top,mid),
         obj          (director, top, mid),
         l            (director, "run_data", "", "l"),
         s            (director, "run_data", "", "s"),
         amp          (director, "run_data", "", "amp"),
         ene          (director, "run_data", "", "ene"),
         cfd          (director, "run_data", "", "cfd"),
         psd          (director, "run_data", "", "psd"),
         trg          (director, "run_data", "", "trg"),
         sBl          (director, "run_data", "", "sBl"),
         eBl          (director, "run_data", "", "eBl")
      {};
      TPx_run_data(TBranchProxyDirector* director, TBranchProxy *parent, const char *membername, const char *top=0, const char *mid=0) :
         ffPrefix     (top,mid),
         obj          (director, parent, membername, top, mid),
         l            (director, "run_data", "", "l"),
         s            (director, "run_data", "", "s"),
         amp          (director, "run_data", "", "amp"),
         ene          (director, "run_data", "", "ene"),
         cfd          (director, "run_data", "", "cfd"),
         psd          (director, "run_data", "", "psd"),
         trg          (director, "run_data", "", "trg"),
         sBl          (director, "run_data", "", "sBl"),
         eBl          (director, "run_data", "", "eBl")
      {};
      ROOT::Internal::TBranchProxyHelper ffPrefix;
      InjecTBranchProxyInterface();
      TBranchProxy obj;

      TFloatProxy   l;
      TFloatProxy   s;
      TFloatProxy   amp;
      TFloatProxy   ene;
      TFloatProxy   cfd;
      TFloatProxy   psd;
      TFloatProxy   trg;
      TFloatProxy   sBl;
      TFloatProxy   eBl;
   };

   // Proxy for each of the branches, leaves and friends of the tree
   TPx_run_data   run_data;


   generatedSel(TTree *tree=0) : 
      fChain(0),
      htemp(0),
      fDirector(tree,-1),
      fClass                (TClass::GetClass("generatedSel")),
      run_data      (&fDirector,"run_data")
      { }
   ~generatedSel();
   Int_t   Version() const {return 1;}
   void    Begin(::TTree *tree);
   void    SlaveBegin(::TTree *tree);
   void    Init(::TTree *tree);
   Bool_t  Notify();
   Bool_t  Process(Long64_t entry);
   void    SlaveTerminate();
   void    Terminate();

   ClassDef(generatedSel,0);


//inject the user's code
#include "_wf_out0.root"
};

#endif


#ifdef __MAKECINT__
#pragma link C++ class generatedSel::TPx_run_data-;
#pragma link C++ class generatedSel;
#endif


inline generatedSel::~generatedSel() {
   // destructor. Clean up helpers.

}

inline void generatedSel::Init(TTree *tree)
{
//   Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fDirector.SetTree(fChain);
   if (htemp == 0) {
      htemp = fDirector.CreateHistogram(GetOption());
      htemp->SetTitle("_wf_out0.root");
      fObject = htemp;
   }
}

Bool_t generatedSel::Notify()
{
   // Called when loading a new file.
   // Get branch pointers.
   fDirector.SetTree(fChain);
   _wf_out0_Notify();
   
   return kTRUE;
}
   

inline void generatedSel::Begin(TTree *tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   _wf_out0_Begin(tree);

}

inline void generatedSel::SlaveBegin(TTree *tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   Init(tree);

   _wf_out0_SlaveBegin(tree);

}

inline Bool_t generatedSel::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TTree::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.

   // WARNING when a selector is used with a TChain, you must use
   //  the pointer to the current TTree to call GetEntry(entry).
   //  The entry is always the local entry number in the current tree.
   //  Assuming that fChain is the pointer to the TChain being processed,
   //  use fChain->GetTree()->GetEntry(entry).


   fDirector.SetReadEntry(entry);
   htemp->Fill(_wf_out0());
   _wf_out0_Process(entry);
   return kTRUE;

}

inline void generatedSel::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   _wf_out0_SlaveTerminate();
}

inline void generatedSel::Terminate()
{
   // Function called at the end of the event loop.
   htemp = (TH1*)fObject;
   Int_t drawflag = (htemp && htemp->GetEntries()>0);
   
   if (gPad && !drawflag && !fOption.Contains("goff") && !fOption.Contains("same")) {
      gPad->Clear();
   } else {
      if (fOption.Contains("goff")) drawflag = false;
      if (drawflag) htemp->Draw(fOption);
   }
   _wf_out0_Terminate();
}
