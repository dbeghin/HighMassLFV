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
  //tau iso SF (3to1 SF) (approx. of 2to0)
  float tau_iso_SF = 0.609718;//0.0129414; //WJets 3to1 SF
  cout << "tau iso SF               " << tau_iso_SF << endl;


  TFile* file_out = new TFile("HighMassLFVMuTau/Wjets_CR0.root", "RECREATE");
  TFile* file_in = new TFile("Figures/histos_highmassmutau_CR2.root", "R");
  //TFile* file_in_SF = new TFile("Figures/WjetsSF.root", "R");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("WJets_");//1
  names.push_back("QCD_");//2
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("tau_eta");
  vars.push_back("ev_Mvis");
  vars.push_back("ev_Mtot");
  vars.push_back("tau_pt");
  vars.push_back("tau_phi");
  vars.push_back("mu_pt");
  vars.push_back("mu_eta");
  vars.push_back("mu_phi");
  vars.push_back("ev_DRmutau");
  vars.push_back("ev_DeltaPhimutau");
  vars.push_back("ev_DeltaPhiMETtau");
  vars.push_back("ev_Mt_raw");
  vars.push_back("ev_MET");
  vars.push_back("ev_Mcol");

  //retrieve histograms from all control regions
  vector<TH1F*> h[names.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) {
      h[j].push_back( (TH1F*) file_in->Get(names[j]+vars[k]) );
      h[j][k]->SetName(names[j]+vars[k]);
    }
  }

  file_out->cd();
  for (unsigned int k=0; k<vars.size(); ++k) {
    TH1F* h_WJets = (TH1F*) h[0][k]->Clone("WJets_"+vars[k]);
    for (unsigned int j=2; j<names.size(); ++j) h_WJets->Add(h[j][k], -1);

    for (unsigned int iBin = 0; iBin<h_WJets->GetNbinsX(); ++iBin) {
      if (h_WJets->GetBinContent(iBin) < 0) h_WJets->SetBinContent(iBin,0);
    }
    h_WJets->Scale(tau_iso_SF);
    h_WJets->Write();
  }
  file_out->Close();


  return 0;
}
