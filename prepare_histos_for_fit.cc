#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TDirectory.h>
#include "TString.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  //float firstbin = 50;
  //float lastbin = 150;
  //int Nbins = 10;
  //int rebin = 10;//144-40=13*8

  TFile* file_in_mutau = new TFile("Figures/histos_highmassmutau_CR100.root", "R");
  //TFile* file_in_mumu = new TFile("Figures/histos_mumu.root", "R");


  vector<TString> in_names,                       out_names;
  in_names.push_back("faketau");	  out_names.push_back("faketau");
  in_names.push_back("DY");  		  out_names.push_back("DY");  
  in_names.push_back("VV");   		  out_names.push_back("VV");   
  in_names.push_back("TT");   		  out_names.push_back("TT");   
  in_names.push_back("ST");   		  out_names.push_back("ST");   
  in_names.push_back("data"); 		  out_names.push_back("data_obs");

  vector<TString> in_sys,             out_sys;
  in_sys.push_back("");               out_sys.push_back("");
  in_sys.push_back("fakerate_up_");   out_sys.push_back("fakerate_up_");
  in_sys.push_back("fakerate_down_"); out_sys.push_back("fakerate_down_");


  TFile* file_out = new TFile("mutauHistos_forLimitSetting.root", "RECREATE");
  
  file_out->cd();
  for (unsigned int j=0; j<in_names.size(); ++j) {
    for (unsigned int k=0; k<in_sys.size(); ++k) {
      TString name_in = in_names[j]+"_"+in_sys[k]+"ev_Mcol_MtHigh";
      TString name_out = out_names[j]+"_"+out_sys[k];
      
      TH1F* h = (TH1F*) file_in_mutau->Get(name_in);
      if (h == 0 && k>0) break; 

      for (unsigned int l=1; l<h->GetNbinsX()+1; ++l) {
	float bin_content = h->GetBinContent(l);
	if (bin_content <= 0) {
	  h->SetBinContent(l, 0);
	}
      }
      h->SetName(name_out);
      h->Write();
    }
  }
  file_out->Close();



  return 0;
}
