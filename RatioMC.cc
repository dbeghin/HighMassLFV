#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TLegend.h"
#include "THStack.h"
#include "TStyle.h"

using namespace std;
int main(/*int argc, char** argv*/) {
  TFile* file_out = new TFile("HighMassLFVMuTau/ratio_MC.root", "RECREATE");
  TFile* file_in_WJetsFromMC  = new TFile("Figures/histos_highmassmutau_test_rebinned.root", "R");
  TFile* file_in_fullFR  = new TFile("Figures/histos_highmassmutau.root", "R");


  TString base_var = "ev_Mcol_";

  vector<TString> process;
  process.push_back("faketau_");
  process.push_back("WJets_");
  process.push_back("DY_");
  process.push_back("TT_");
  process.push_back("ST_");
  process.push_back("VV_");

  vector<TString> Mth;           
  Mth.push_back("MtLow_OS"); 
  Mth.push_back("MtLow_SS"); 
  Mth.push_back("MtHigh"); 


  //retrieve histograms
  vector<TH1F*> h_WJetsFromMC;
  vector<TH1F*> h_fullFR;
  vector<TH1F*> h_ratio;
  
  for (unsigned int j=0; j<Mth.size(); ++j) {
    TString name_in = base_var+Mth[j];
    TH1F* h_temp_WJetsFromMC = (TH1F*) file_in_WJetsFromMC->Get( process[0]+base_var+Mth[j] );
    TH1F* h_temp_fullFR = (TH1F*) file_in_fullFR->Get( process[0]+base_var+Mth[j] );
    for (unsigned int i=1; i<process.size(); ++i) {
      TH1F* h0 = (TH1F*) file_in_WJetsFromMC->Get( process[i]+base_var+Mth[j] );
      h_temp_WJetsFromMC->Add( h0 );
      
      if (i==1) continue;
      TH1F* h1 = (TH1F*) file_in_fullFR->Get( process[i]+base_var+Mth[j] );
      h_temp_fullFR->Add( h1 );
    }
    h_WJetsFromMC.push_back( h_temp_WJetsFromMC );
    h_WJetsFromMC[j]->SetName("WJetsFromMC_"+name_in);
    
    h_fullFR.push_back( h_temp_fullFR );
    h_fullFR[j]->SetName("fullFR_"+name_in);

    h_ratio.push_back( (TH1F*) h_fullFR[j]->Clone("ratio_"+name_in) );
    h_ratio[j]->SetTitle("ratio_"+name_in);
    h_ratio[j]->Divide(h_WJetsFromMC[j]);
  }



  file_out->cd();
  for (unsigned int j=0; j<Mth.size(); ++j) {
    h_WJetsFromMC[j]->Write();
    h_fullFR[j]->Write();
    h_ratio[j]->Write();
  }
  file_out->Close();



  return 0;
}
