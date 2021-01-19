#define IIHEAnalysis_cxx
#include "SF_and_systematics.cc"
#include "meta.h"
//#include <TH1.h>
//#include <TLorentzVector.h>
//#include <TCanvas.h>
//#include "TString.h"
//#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  string out = *(argv + 1);
  string out_name= out;
  string in = *(argv + 2);
  string inname= in;
  TFile *fIn = TFile::Open(inname.c_str());
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  TTree* mmeta = (TTree*) fIn->Get("meta");
  meta* m = new meta(mmeta);
  Float_t nEvents = m->Loop("");
  
  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop("", "", out_name, nEvents);
  return 0;
}


//Get weighted events                    
Float_t meta::Loop(string type_of_data) {
  if (fChain == 0) return -1;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  Float_t nEvents = -1;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    nEvents = mc_nEventsWeighted;
    break;
  }
  return nEvents;
}



void IIHEAnalysis::Loop(string phase, string type_of_data, string out_name, float nEvents) {
   if (fChain == 0) return;


   vector<TString> newNames;
   newNames.push_back("A");
   newNames.push_back("B");
   newNames.push_back("C");
   newNames.push_back("D");
   newNames.push_back("E");

   int nDivide = newNames.size();


   
   Long64_t nEntries = fChain->GetEntriesFast();
   Long64_t short_nEntries = nEntries/nDivide;
   Long64_t nbytes = 0, nb = 0;
   int print_count = 0;
   //start loop over all events

   Long64_t jStart = 0, jStop = 0;
   vector<TFile*> file_out;
   vector<TTree*> newTree;
   vector<TH1F*> h_total_events;
   for (unsigned int iSplit=0; iSplit<nDivide; ++iSplit) {
     file_out.push_back(new TFile(out_name.c_str()+newNames[iSplit]+".root","RECREATE"));
     newTree.push_back(fChain->CloneTree(0));
     

     jStart = iSplit*short_nEntries;
     if (iSplit == nDivide-1) {
       jStop = nEntries;
     }
     else {
       jStop = (iSplit+1)*short_nEntries;
     }
   
     for (Long64_t jEntry = jStart; jEntry < jStop; ++jEntry) {
        Long64_t iEntry = LoadTree(jEntry);
        if (iEntry < 0) break;
        if (jEntry % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", jEntry, nEntries);
        
        nb = fChain->GetEntry(jEntry);
        nbytes += nb;
     
        newTree[iSplit] -> Fill();
     
     }//loop over events
    
     float eventShare = nEvents/nDivide;
     h_total_events.push_back( new TH1F("weighted_events", "weighted_events", 1, 0, 1) );
     h_total_events[iSplit]->Fill(0.5, eventShare);

     file_out[iSplit]->cd();
     h_total_events[iSplit]->Write();
     newTree[iSplit]->AutoSave();
     file_out[iSplit]->Close();

   }

}
