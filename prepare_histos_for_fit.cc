#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TDirectory.h>
#include "TString.h"
#include <iostream>
#include <vector>
#include "aux.h"

using namespace std;

int main(int argc, char** argv) {
  //float firstbin = 50;
  //float lastbin = 150;
  //int Nbins = 10;
  //int rebin = 10;//144-40=13*8

  TFile* file_in_mutau = new TFile("Figures/histos_highmassmutau_CR100.root", "R");
  TFile* file_in_signal = new TFile("Figures/histos_signal.root", "R");


  vector<TString> in_names,               out_names;
  in_names.push_back("faketau_");	  out_names.push_back("faketau");
  in_names.push_back("DY_");  		  out_names.push_back("DY");  
  in_names.push_back("VV_");   		  out_names.push_back("VV");   
  in_names.push_back("TT_");   		  out_names.push_back("TT");   
  in_names.push_back("ST_");   		  out_names.push_back("ST");   
  in_names.push_back("data_"); 		  out_names.push_back("data_obs");


  vector<TString> in_sys,                out_sys;
  in_sys.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    in_sys.push_back(systs_aux[iAux]+"_up");
    in_sys.push_back(systs_aux[iAux]+"_down");
  }
  
  out_sys = in_sys;

  vector<TString> mass;  
  mass.push_back("500"); 
  mass.push_back("600"); 
  mass.push_back("700"); 
  mass.push_back("800"); 
  mass.push_back("900"); 
  mass.push_back("1000");
  mass.push_back("1100");
  mass.push_back("1200");
  mass.push_back("1300");
  mass.push_back("1400");
  mass.push_back("1500");
  mass.push_back("1600");
  mass.push_back("1700");
  mass.push_back("1800");
  mass.push_back("1900");
  mass.push_back("2000");
  mass.push_back("2200");
  mass.push_back("2400");
  mass.push_back("2600");
  mass.push_back("2800");
  mass.push_back("3000");
  mass.push_back("3500");
  mass.push_back("4000");
  mass.push_back("4500");
  mass.push_back("5000");
  //mass.push_back("5500");
  //mass.push_back("6000");


  TFile* file_out = new TFile("mutauHistos_forLimitSetting.root", "RECREATE");
  
  file_out->cd();
  TDirectory* signal_dir = file_out->mkdir("signalregion");
  signal_dir->cd(); 
  for (unsigned int j=0; j<in_names.size(); ++j) {
    for (unsigned int k=0; k<in_sys.size(); ++k) {
      TString name_in = in_sys[k]+"/"+in_names[j]+in_sys[k]+"_ev_Mcol_MtHigh";
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
      h->SetTitle(name_out);
      h->Write();
    }
  }

  for (unsigned int j=0; j<mass.size(); ++j) {
    TString name_in = mass[j]+"_Mcol";
    TString name_out = "Zprime_"+mass[j];
    TH1F* h = (TH1F*) file_in_signal->Get(name_in);
    h->SetName(name_out);
    h->SetTitle(name_out);
    h->Write();
  }
  signal_dir->Close();
  file_out->Close();



  return 0;
}
