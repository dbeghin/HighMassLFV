//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb 26 18:54:28 2019 by ROOT version 6.12/07
// from TTree meta/Information about globalTag etc
// found on file: /pnfs/iihe/cms/store/user/amkalsi/Moriond2018_New_2016_June13_2018/WWTo2L2Nu_13TeV-powheg/crab_WWTo2L2Nu_13TeV-powheg/180614_114141/0000/out_file_12.root
//////////////////////////////////////////////////////////

#ifndef meta_h
#define meta_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "string"
#include "vector"
#include "vector"

class meta {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   string          *globalTag;
   Float_t         MCTruth_ptThreshold;
   Float_t         MCTruth_mThreshold;
   Float_t         MCTruth_DeltaROverlapThreshold;
   vector<float>   *mc_sumofWeights;
   vector<string>  *mc_weightsId;
   Float_t         mc_nEventsWeighted;
   Float_t         nEventsRaw;
   Float_t         nEventsStored;
   vector<float>   *nRuns;

   // List of branches
   TBranch        *b_globalTag;   //!
   TBranch        *b_MCTruth_ptThreshold;   //!
   TBranch        *b_MCTruth_mThreshold;   //!
   TBranch        *b_MCTruth_DeltaROverlapThreshold;   //!
   TBranch        *b_mc_sumofWeights;   //!
   TBranch        *b_mc_weightsId;   //!
   TBranch        *b_mc_nEventsWeighted;   //!
   TBranch        *b_nEventsRaw;   //!
   TBranch        *b_nEventsStored;   //!
   TBranch        *b_nRuns;   //!

   meta(TTree *tree=0);
   virtual ~meta();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Float_t  Loop(string type);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef IIHEAnalysis_cxx
meta::meta(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_New_2016_June13_2018/WWTo2L2Nu_13TeV-powheg/crab_WWTo2L2Nu_13TeV-powheg/180614_114141/0000/out_file_12.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_New_2016_June13_2018/WWTo2L2Nu_13TeV-powheg/crab_WWTo2L2Nu_13TeV-powheg/180614_114141/0000/out_file_12.root");
      }
      f->GetObject("meta",tree);

   }
   Init(tree);
}

meta::~meta()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t meta::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t meta::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void meta::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   globalTag = 0;
   mc_sumofWeights = 0;
   mc_weightsId = 0;
   nRuns = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("globalTag", &globalTag, &b_globalTag);
   fChain->SetBranchAddress("MCTruth_ptThreshold", &MCTruth_ptThreshold, &b_MCTruth_ptThreshold);
   fChain->SetBranchAddress("MCTruth_mThreshold", &MCTruth_mThreshold, &b_MCTruth_mThreshold);
   fChain->SetBranchAddress("MCTruth_DeltaROverlapThreshold", &MCTruth_DeltaROverlapThreshold, &b_MCTruth_DeltaROverlapThreshold);
   fChain->SetBranchAddress("mc_sumofWeights", &mc_sumofWeights, &b_mc_sumofWeights);
   fChain->SetBranchAddress("mc_weightsId", &mc_weightsId, &b_mc_weightsId);
   fChain->SetBranchAddress("mc_nEventsWeighted", &mc_nEventsWeighted, &b_mc_nEventsWeighted);
   fChain->SetBranchAddress("nEventsRaw", &nEventsRaw, &b_nEventsRaw);
   fChain->SetBranchAddress("nEventsStored", &nEventsStored, &b_nEventsStored);
   fChain->SetBranchAddress("nRuns", &nRuns, &b_nRuns);
   Notify();
}

Bool_t meta::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void meta::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t meta::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef IIHEAnalysis_cxx
