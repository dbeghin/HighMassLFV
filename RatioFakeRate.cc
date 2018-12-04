#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH2.h"
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
  TFile* file_out = new TFile("HighMassLFVMuTau/ratio_fakerate.root", "RECREATE");
  TFile* file_in_Zjets  = new TFile("HighMassLFVMuTau/fakerate.root", "R");
  TFile* file_in_SSMtLow  = new TFile("HighMassLFVMuTau/fakerate_SSMtLow.root", "R");


  TString base_var = "hratio_data_";
  TString base_var2 = "taupt_jetpt_pass_";

  vector<TString> eta;
  eta.push_back("total_");
  eta.push_back("eta_barrel_");
  eta.push_back("eta_endcap_");

  vector<TString> sectors;           
  sectors.push_back("taupt_0_300_jetpt_0_150"); 
  sectors.push_back("taupt_0_300_jetpt_150_300"); 
  sectors.push_back("taupt_0_300_jetpt_300_1000"); 
  sectors.push_back("taupt_300_1000_jetpt_300_1000"); 
  sectors.push_back("taupt_300_1000_jetpt_0_300"); 


  //retrieve histograms
  vector<TH2F*> h_Zjets[eta.size()];
  vector<TH2F*> h_SSMtLow[eta.size()];
  for (unsigned int i=0; i<eta.size(); ++i) {
    for (unsigned int j=0; j<sectors.size(); ++j) {
      TString name_in = base_var+eta[i]+base_var2+sectors[j];
      h_Zjets[i].push_back( (TH2F*) file_in_Zjets->Get(name_in) );
      h_Zjets[i][j]->SetName("Zjets_"+name_in);

      h_SSMtLow[i].push_back( (TH2F*) file_in_SSMtLow->Get(name_in) );
      h_SSMtLow[i][j]->SetName("SSMtLow_"+name_in);
      h_SSMtLow[i][j]->SetTitle("ratio_"+name_in);
      cout << name_in << endl;
      h_SSMtLow[i][j]->Divide(h_Zjets[0][j]);
      cout << "after division" << endl;
    }
  }



  file_out->cd();
  for (unsigned int i=0; i<eta.size(); ++i) {
    for (unsigned int j=0; j<sectors.size(); ++j) {
      h_SSMtLow[i][j]->Write();
    }
  }
  file_out->Close();



  return 0;
}
