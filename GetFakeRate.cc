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
  TFile* file_out = new TFile("HighMassLFVMuTau/fakerate.root", "RECREATE");
  TFile* file_in  = new TFile("Figures/histos_fakerate.root", "R");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("WJets_");//1
  names.push_back("QCD_");//2
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("taupt_pass");
  vars.push_back("taupt_fail");

  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[names.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) {
      h[j].push_back( (TH1F*) file_in->Get(names[j]+vars[k]) );
      h[j][k]->SetName(names[j]+vars[k]);
    }
  }


  TH1F* h_MC_pass = (TH1F*) h[1][0]->Clone("MC_pass");
  TH1F* h_MC_fail = (TH1F*) h[1][1]->Clone("MC_fail");
  for (unsigned int j=2; j<names.size(); ++j) {
    h_MC_pass->Add(h[j][0]);
    h_MC_fail->Add(h[j][1]);
  }

  //rebin
  float x[] = {30, 40, 50, 60, 70, 80, 90, 100, 150, 250, 1000};
  int len_x = 11;

  //MC
  TH1F* ptratio_MC = new TH1F("FakeRate_byTauPt_MC", "FakeRate_byTauPt_MC", len_x-1, x);
  TH1F* denominator_MC = new TH1F("den", "den", len_x-1, x);
  int jBin = 1;
  float bin_content_num = 0, bin_error_num=0;
  float bin_content_den = 0, bin_error_den=0;
  for (unsigned int iBin=1; iBin < h_MC_pass->GetNbinsX()+1; ++iBin) {
    if (h_MC_pass->GetBinCenter(iBin) < x[jBin]) {
      bin_content_num += h_MC_pass->GetBinContent(iBin);
      bin_error_num += pow(h_MC_pass->GetBinError(iBin), 2);
      bin_content_den += h_MC_fail->GetBinContent(iBin);
      bin_error_den += pow(h_MC_fail->GetBinError(iBin), 2);
    }
    else {
      ptratio_MC->SetBinContent(jBin, bin_content_num);
      ptratio_MC->SetBinError(jBin, sqrt(bin_error_num));
      bin_content_num = h_MC_pass->GetBinContent(iBin);
      bin_error_num = pow(h_MC_pass->GetBinError(iBin), 2);

      denominator_MC->SetBinContent(jBin, bin_content_den);
      denominator_MC->SetBinError(jBin, sqrt(bin_error_den));
      bin_content_den = h_MC_fail->GetBinContent(iBin);
      bin_error_den = pow(h_MC_fail->GetBinError(iBin), 2);
      ++jBin;
    }
  }
  denominator_MC->Add(ptratio_MC);
  ptratio_MC->Divide(denominator_MC);

  //data
  TH1F* ptratio_data = new TH1F("FakeRate_byTauPt_data", "FakeRate_byTauPt_data", len_x-1, x);
  TH1F* denominator_data = new TH1F("den2", "den2", len_x-1, x);
  jBin = 1;
  bin_content_num = 0, bin_error_num=0;
  bin_content_den = 0, bin_error_den=0;
  for (unsigned int iBin=1; iBin < h[0][0]->GetNbinsX()+1; ++iBin) {
    if (h[0][0]->GetBinCenter(iBin) < x[jBin]) {
      bin_content_num += h[0][0]->GetBinContent(iBin);
      bin_error_num += pow(h[0][0]->GetBinError(iBin), 2);
      bin_content_den += h[0][1]->GetBinContent(iBin);
      bin_error_den += pow(h[0][1]->GetBinError(iBin), 2);
    }
    else {
      ptratio_data->SetBinContent(jBin, bin_content_num);
      ptratio_data->SetBinError(jBin, sqrt(bin_error_num));
      bin_content_num = h[0][0]->GetBinContent(iBin);
      bin_error_num = pow(h[0][0]->GetBinError(iBin), 2);

      denominator_data->SetBinContent(jBin, bin_content_den);
      denominator_data->SetBinError(jBin, sqrt(bin_error_den));
      bin_content_den = h[0][1]->GetBinContent(iBin);
      bin_error_den = pow(h[0][1]->GetBinError(iBin), 2);
      ++jBin;
    }
  }
  denominator_data->Add(ptratio_data);
  ptratio_data->Divide(denominator_data);


  file_out->cd();
  ptratio_MC->Write();
  ptratio_data->Write();
  file_out->Close();


  return 0;
}
