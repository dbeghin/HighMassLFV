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

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");


  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[names.size()][vars.size()][dms.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) { 
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  h[j][k][l].push_back( (TH1F*) file_in->Get(names[j]+vars[k]+"_"+dms[l]+"_"+eta[m]) );
	  h[j][k][l][m]->SetName(names[j]+vars[k]);
	}
      }
    }
  }


  vector<TH1F*> h_MC_pass;
  vector<TH1F*> h_MC_fail;
  vector<TH1F*> h_data_pass;
  vector<TH1F*> h_data_fail;
  for (unsigned int l=0; l<dms.size(); ++l) {
    for (unsigned int m=0; m<eta.size(); ++m) {
      //int i = l*eta.size()+m;
      h_MC_pass.push_back( (TH1F*) h[1][0][l][m]->Clone("MC_pass_"+dms[l]+"_"+eta[m]) );
      h_MC_fail.push_back( (TH1F*) h[1][1][l][m]->Clone("MC_fail_"+dms[l]+"_"+eta[m]) );
      h_data_pass.push_back( (TH1F*) h[0][0][l][m]->Clone("data_pass_"+dms[l]+"_"+eta[m]) );
      h_data_fail.push_back( (TH1F*) h[0][1][l][m]->Clone("data_fail_"+dms[l]+"_"+eta[m]) );
    }
  }


  //rebin
  float x[] = {30, 40, 50, 60, 70, 80, 90, 100, 150, 250, 1000};
  int len_x = 11;

  //MC
  vector<TH1F*> ptratio_MC;
  vector<TH1F*> denominator_MC;
  TH1F* ptratio_MC_total;
  TH1F* denominator_MC_total;
  for (unsigned int i=0; i<h_MC_pass.size(); ++i) {
    int l = i/eta.size(), m = i % eta.size();
    ptratio_MC.push_back( new TH1F("FakeRate_byTauPt_MC_"+dms[l]+"_"+eta[m], "FakeRate_byTauPt_MC_"+dms[l]+"_"+eta[m], len_x-1, x) );
    denominator_MC.push_back( new TH1F("den_"+dms[l]+"_"+eta[m], "den_"+dms[l]+"_"+eta[m], len_x-1, x) );
    for (unsigned int j=2; j<names.size(); ++j) {
      h_MC_pass[i]->Add(h[j][0][l][m]);
      h_MC_fail[i]->Add(h[j][1][l][m]);
    }
    int jBin = 1;
    float bin_content_num = 0, bin_error_num=0;
    float bin_content_den = 0, bin_error_den=0;
    for (unsigned int iBin=1; iBin < h_MC_pass[i]->GetNbinsX()+1; ++iBin) {
      if (h_MC_pass[i]->GetBinCenter(iBin) < x[jBin]) {
	bin_content_num += h_MC_pass[i]->GetBinContent(iBin);
	bin_error_num += pow(h_MC_pass[i]->GetBinError(iBin), 2);
	bin_content_den += h_MC_fail[i]->GetBinContent(iBin);
	bin_error_den += pow(h_MC_fail[i]->GetBinError(iBin), 2);
      }
      else {
	ptratio_MC[i]->SetBinContent(jBin, bin_content_num);
	ptratio_MC[i]->SetBinError(jBin, sqrt(bin_error_num));
	bin_content_num = h_MC_pass[i]->GetBinContent(iBin);
	bin_error_num = pow(h_MC_pass[i]->GetBinError(iBin), 2);
	
	denominator_MC[i]->SetBinContent(jBin, bin_content_den);
	denominator_MC[i]->SetBinError(jBin, sqrt(bin_error_den));
	bin_content_den = h_MC_fail[i]->GetBinContent(iBin);
	bin_error_den = pow(h_MC_fail[i]->GetBinError(iBin), 2);
	++jBin;
      }
    }
    denominator_MC[i]->Add(ptratio_MC[i]);
    if (i==0) {
      denominator_MC_total = (TH1F*) denominator_MC[i]->Clone("den_total");
      ptratio_MC_total = (TH1F*) ptratio_MC[i]->Clone("ptratio_MC_total");
    }
    else {
      denominator_MC_total->Add(denominator_MC[i]);
      ptratio_MC_total->Add(ptratio_MC[i]);
    }
    ptratio_MC[i]->Divide(denominator_MC[i]);
  }
  ptratio_MC_total->Divide(denominator_MC_total);


  //data
  vector<TH1F*> ptratio_data;
  vector<TH1F*> denominator_data;
  TH1F* ptratio_data_total;
  TH1F* denominator_data_total;
  for (unsigned int i=0; i<h_data_pass.size(); ++i) {
    int l = i/eta.size(), m = i % eta.size();
    ptratio_data.push_back( new TH1F("FakeRate_byTauPt_data_"+dms[l]+"_"+eta[m], "FakeRate_byTauPt_data_"+dms[l]+"_"+eta[m], len_x-1, x) );
    denominator_data.push_back( new TH1F("den2_"+dms[l]+"_"+eta[m], "den2_"+dms[l]+"_"+eta[m], len_x-1, x) );
    int jBin = 1;
    float bin_content_num = 0, bin_error_num=0;
    float bin_content_den = 0, bin_error_den=0;
    for (unsigned int iBin=1; iBin < h_data_pass[i]->GetNbinsX()+1; ++iBin) {
      if (h_data_pass[i]->GetBinCenter(iBin) < x[jBin]) {
	bin_content_num += h_data_pass[i]->GetBinContent(iBin);
	bin_error_num += pow(h_data_pass[i]->GetBinError(iBin), 2);
	bin_content_den += h_data_fail[i]->GetBinContent(iBin);
	bin_error_den += pow(h_data_fail[i]->GetBinError(iBin), 2);
      }
      else {
	ptratio_data[i]->SetBinContent(jBin, bin_content_num);
	ptratio_data[i]->SetBinError(jBin, sqrt(bin_error_num));
	bin_content_num = h_data_pass[i]->GetBinContent(iBin);
	bin_error_num = pow(h_data_pass[i]->GetBinError(iBin), 2);
	
	denominator_data[i]->SetBinContent(jBin, bin_content_den);
	denominator_data[i]->SetBinError(jBin, sqrt(bin_error_den));
	bin_content_den = h_data_fail[i]->GetBinContent(iBin);
	bin_error_den = pow(h_data_fail[i]->GetBinError(iBin), 2);
	++jBin;
      }
    }
    denominator_data[i]->Add(ptratio_data[i]);
    if (i==0) {
      denominator_data_total = (TH1F*) denominator_data[i]->Clone("den2_total");
      ptratio_data_total = (TH1F*) ptratio_data[i]->Clone("ptratio_data_total");
    }
    else {
      denominator_data_total->Add(denominator_data[i]);
      ptratio_data_total->Add(ptratio_data[i]);
    }
    ptratio_data[i]->Divide(denominator_data[i]);
  }
  ptratio_data_total->Divide(denominator_data_total);


  file_out->cd();
  ptratio_MC_total->Write();
  ptratio_data_total->Write();
  for (unsigned int i=0; i<h_data_pass.size(); ++i) {
    ptratio_MC[i]->Write();
    ptratio_data[i]->Write();
  }
  file_out->Close();


  return 0;
}
