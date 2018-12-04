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
int main(int argc, char** argv) {
  string nature = *(argv + 1);

  TFile* file_out;
  TFile* file_in;

  if (nature == "signal") {
    file_out = new TFile("HighMassLFVMuTau/Faketaus_CR0.root", "RECREATE");
    file_in = new TFile("Figures/histos_highmassmutau_CR101.root", "R");
  }
  else if (nature == "Wjets") {
    file_out = new TFile("HighMassLFVMuTau/Faketaus_CR102.root", "RECREATE");
    file_in = new TFile("Figures/histos_highmassmutau_CR103.root", "R");
  }
  

  vector<TString> names;
  names.push_back("data_");//0
  //names.push_back("WJets_");//1
  //names.push_back("QCD_");//2
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("tau_pt");
  vars.push_back("ev_Mvis");
  vars.push_back("ev_Mtot");
  vars.push_back("tau_eta");
  vars.push_back("tau_phi");
  vars.push_back("mu_pt");
  vars.push_back("mu_eta");
  vars.push_back("mu_phi");
  vars.push_back("mu_isolation");
  vars.push_back("ev_DRmutau");
  vars.push_back("ev_DeltaPhimutau");
  vars.push_back("ev_DeltaPhiMETtau");
  vars.push_back("ev_Mt");
  vars.push_back("ev_MET");
  vars.push_back("ev_Mcol");

  
  vector<TString> Mth;
  Mth.push_back("MtLow_OS");
  Mth.push_back("MtLow_SS");
  Mth.push_back("MtHigh");

  vector<TString> systs;            
  systs.push_back("");              
  systs.push_back("fakerate_up_");
  systs.push_back("fakerate_down_");
  

  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[names.size()][vars.size()][systs.size()];
  for (unsigned int i=0; i<names.size(); ++i) {
    for (unsigned int j=0; j<vars.size(); ++j) {
      for (unsigned int k=0; k<systs.size(); ++k) {
	for (unsigned int l=0; l<Mth.size(); ++l) {
	  h[i][j][k].push_back( (TH1F*) file_in->Get(names[i]+vars[j]+"_"+"realtau_"+systs[k]+Mth[l]) );
	  h[i][j][k][l]->SetName(names[i]+vars[j]+"_"+systs[k]+Mth[l]);
	}
      }
    }
  }


  file_out->cd();
  for (unsigned int j=0; j<vars.size(); ++j) {
    for (unsigned int k=0; k<systs.size(); ++k) {
      for (unsigned int l=0; l<Mth.size(); ++l) {
	TH1F* h_faketau = (TH1F*) h[0][j][k][l]->Clone("faketau_"+systs[k]+vars[j]+"_"+Mth[l]);
	for (unsigned int i=1; i<names.size(); ++i) h_faketau->Add(h[i][j][k][l], -1);//subtract all real tau bg
	
	for (unsigned int iBin = 0; iBin<h_faketau->GetNbinsX(); ++iBin) {
	  if (h_faketau->GetBinContent(iBin) < 0) h_faketau->SetBinContent(iBin,0);
	}
	h_faketau->Write();
      }
    }
  }
  file_out->Close();


  return 0;
}
