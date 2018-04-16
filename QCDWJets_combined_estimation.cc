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
  TFile* file_out_CR0 = new TFile("HighMassLFVMuTau/QCDWJets_CR0.root", "RECREATE");
  vector<TFile*> file_in;
  int i_CR1 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR1.root", "R") );
  int i_CR2 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR2.root", "R") );
  int i_CR3 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR3.root", "R") );

  vector<TString> CR_list;
  CR_list.push_back("CR1");
  CR_list.push_back("CR2");
  CR_list.push_back("CR3");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("WJets_");//1
  names.push_back("QCD_");//2
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
  vars.push_back("ev_DRmutau");
  vars.push_back("ev_DeltaPhimutau");
  vars.push_back("ev_DeltaPhiMETtau");
  vars.push_back("ev_Mt_raw");
  vars.push_back("ev_MET");
  vars.push_back("ev_Mcol");

  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[file_in.size()][names.size()];
  for (unsigned int i=0; i<file_in.size(); ++i) {
    for (unsigned int j=0; j<names.size(); ++j) {
      for (unsigned int k=0; k<vars.size(); ++k) {
        if (k>0 && i != i_CR2) break;
        h[i][j].push_back( (TH1F*) file_in[i]->Get(names[j]+vars[k]) );
        h[i][j][k]->SetName(CR_list[i]+"_"+names[j]+vars[k]);
      }
    }
  }


  //Get mu iso SF (5to3 SF) (approx. of 4to2) and mutau iso SF (5to1) (approx. of 4to0)
  TH1F* h_data3 = (TH1F*) h[i_CR3][0][0]->Clone("data_CR3");
  TH1F* h_data1 = (TH1F*) h[i_CR1][0][0]->Clone("data_CR1");
  for (unsigned int j=1; j<names.size(); ++j) {
    if (j<=2) continue;
    h_data3->Add(h[i_CR3][j][0], -1);
    h_data1->Add(h[i_CR1][j][0], -1);
  }
  float data3_error_sq = 0;
  float data1_error_sq = 0;
  float MC3_error_sq = 0;
  float MC1_error_sq = 0;
  for (unsigned int iBin=1; iBin<h_data3->GetNbinsX()+1; ++iBin) {
    data3_error_sq += pow(h_data3->GetBinError(iBin), 2);
    data1_error_sq += pow(h_data1->GetBinError(iBin), 2);
    MC3_error_sq += pow(h[i_CR3][2][0]->GetBinError(iBin), 2);
    MC1_error_sq += pow(h[i_CR1][2][0]->GetBinError(iBin), 2);
  }

  float tau_iso_SF = h_data1->Integral()/h_data3->Integral();
  float tau_iso_SF_err = sqrt( data3_error_sq/pow(h_data3->Integral(), 2) + data1_error_sq/pow(h_data1->Integral(), 2) ) * tau_iso_SF;
  float tau_iso_SF_MC = h[i_CR1][2][0]->Integral()/h[i_CR3][2][0]->Integral();
  float tau_iso_SF_MC_err = sqrt( MC3_error_sq/pow(h[i_CR3][2][0]->Integral(), 2) + MC1_error_sq/pow(h[i_CR1][2][0]->Integral(), 2) ) * tau_iso_SF_MC;
  cout << "mutau iso SF               " << tau_iso_SF << "  stat err " << tau_iso_SF_err << endl;
  cout << "mutau iso SF (from QCD MC) " << tau_iso_SF_MC << "  stat err " << tau_iso_SF_MC_err << endl;


  file_out_CR0->cd();
  for (unsigned int k=0; k<vars.size(); ++k) {
    TH1F* h_faketau_CR0 = (TH1F*) h[i_CR2][0][k]->Clone("faketau_"+vars[k]);
    for (unsigned int j=3; j<names.size(); ++j) h_faketau_CR0->Add(h[i_CR2][j][k], -1);

    for (unsigned int iBin = 0; iBin<h_faketau_CR0->GetNbinsX(); ++iBin) {
      if (h_faketau_CR0->GetBinContent(iBin) < 0) h_faketau_CR0->SetBinContent(iBin,0);
    }
    h_faketau_CR0->Scale(tau_iso_SF);
    h_faketau_CR0->Write();
  }
  file_out_CR0->Close();


  return 0;
}
