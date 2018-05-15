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
  TFile* file_out_CR2 = new TFile("HighMassLFVMuTau/QCD_CR2.root", "RECREATE");
  TFile* file_out_CR0 = new TFile("HighMassLFVMuTau/QCD_CR0.root", "RECREATE");
  TFile* file_out_W = new TFile("HighMassLFVMuTau/WjetsSF.root", "RECREATE");
  vector<TFile*> file_in;
  int i_CR1 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR1.root", "R") );
  int i_CR3 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR3.root", "R") );
  int i_CR4 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR4.root", "R") );
  int i_CR5 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR5.root", "R") );
  int i_CR7 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR7.root", "R") );
  int i_CR9 = file_in.size();
  file_in.push_back( new TFile("Figures/histos_highmassmutau_CR9.root", "R") );

  vector<TString> CR_list;
  CR_list.push_back("CR1");
  CR_list.push_back("CR3");
  CR_list.push_back("CR4");
  CR_list.push_back("CR5");
  CR_list.push_back("CR7");
  CR_list.push_back("CR9");

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

  vector<TString> taun;
  taun.push_back("_realtau");
  taun.push_back("_faketau");

  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[file_in.size()][names.size()];
  for (unsigned int i=0; i<file_in.size(); ++i) {
    for (unsigned int j=0; j<names.size(); ++j) {
      for (unsigned int k=0; k<vars.size(); ++k) {
	if (k>0 && i != i_CR4) break;
	h[i][j].push_back( (TH1F*) file_in[i]->Get(names[j]+vars[k]+taun[0]) );
	h[i][j][k]->SetName(CR_list[i]+"_"+names[j]+vars[k]+taun[0]);
	TH1F* hhh = (TH1F*) file_in[i]->Get(names[j]+vars[k]+taun[1]);
	h[i][j][k]->Add(hhh);
      }
    }
  }


  //Get WJets SF in SS iso mu, anti-iso tau CR
  TH1F* h_data_CR7 = (TH1F*) h[i_CR7][0][0]->Clone("data_wjetsCR7");
  for (unsigned int j=2; j<names.size(); ++j) h_data_CR7->Add(h[i_CR7][j][0], -1);
  float data_error_sq = 0;
  float MC_error_sq = 0;
  for (unsigned int iBin=1; iBin<h_data_CR7->GetNbinsX()+1; ++iBin) {
    data_error_sq += pow(h_data_CR7->GetBinError(iBin), 2);
    MC_error_sq += pow(h[i_CR7][1][0]->GetBinError(iBin), 2);
  }
  float aa = data_error_sq, bb = MC_error_sq;
  float WJets_SF_CR7 = h_data_CR7->Integral()/h[i_CR7][1][0]->Integral();
  float WJets_SF_CR7_err = sqrt( data_error_sq/pow(h_data_CR7->Integral(), 2) + MC_error_sq/pow(h[i_CR7][1][0]->Integral(), 2) ) * WJets_SF_CR7;
  cout << "WJets SF CR7 " << WJets_SF_CR7 << "  stat err " << WJets_SF_CR7_err << "  QCD err " << 0.5*h[i_CR7][2][0]->Integral()/h[i_CR7][1][0]->Integral() << endl;


  //Get WJets SF in SS iso mu, iso tau CR
  TH1F* h_data_CR9 = (TH1F*) h[i_CR9][0][0]->Clone("data_wjetsCR9");
  for (unsigned int j=2; j<names.size(); ++j) h_data_CR9->Add(h[i_CR9][j][0], -1);
  data_error_sq = 0;
  MC_error_sq = 0;
  for (unsigned int iBin=1; iBin<h_data_CR9->GetNbinsX()+1; ++iBin) {
    data_error_sq += pow(h_data_CR9->GetBinError(iBin), 2);
    MC_error_sq += pow(h[i_CR9][1][0]->GetBinError(iBin), 2);
  }
  aa = data_error_sq, bb = MC_error_sq;
  float WJets_SF_CR9 = h_data_CR9->Integral()/h[i_CR9][1][0]->Integral();
  float WJets_SF_CR9_err = sqrt( data_error_sq/pow(h_data_CR9->Integral(), 2) + MC_error_sq/pow(h[i_CR9][1][0]->Integral(), 2) ) * WJets_SF_CR9;
  cout << "WJets SF CR9 " << WJets_SF_CR9 << "  stat err " << WJets_SF_CR9_err << "  QCD err " << 0.5*h[i_CR9][2][0]->Integral()/h[i_CR9][1][0]->Integral() << endl;


  //rescale WJets histos (using the combined value for the SF)
  h[i_CR3][1][0]->Scale(WJets_SF_CR7), h[i_CR1][1][0]->Scale(WJets_SF_CR9);

  //Get 3to1 WJets tau-pt-dependent SF
  float x[] = {30, 40, 50, 70, 100, 500};
  int len_x = 6;
  TH1F* ptratio = new TH1F("WJetsSF_byTauPt", "WJetsSF_byTauPt", len_x-1, x);
  TH1F* denominator = new TH1F("den", "den", len_x-1, x);
  int jBin = 1;
  float bin_content_num = 0, bin_error_num = 0;
  float bin_content_den = 0, bin_error_den = 0;
  for (unsigned int iBin=1; iBin < h[i_CR1][1][0]->GetNbinsX()+1; ++iBin) {
    if (h[i_CR1][1][0]->GetBinCenter(iBin) < x[jBin]) {
      bin_content_num += h[i_CR1][1][0]->GetBinContent(iBin);
      bin_error_num += pow(h[i_CR1][1][0]->GetBinError(iBin), 2);
      bin_content_den += h[i_CR3][1][0]->GetBinContent(iBin);
      bin_error_den += pow(h[i_CR3][1][0]->GetBinError(iBin), 2);
    }
    else {
      ptratio->SetBinContent(jBin, bin_content_num);
      ptratio->SetBinError(jBin, sqrt(bin_error_num));
      bin_content_num = h[i_CR1][1][0]->GetBinContent(iBin);
      bin_error_num = pow(h[i_CR1][1][0]->GetBinError(iBin), 2);

      denominator->SetBinContent(jBin, bin_content_den);
      denominator->SetBinError(jBin, sqrt(bin_error_den));
      bin_content_den = h[i_CR3][1][0]->GetBinContent(iBin);
      bin_error_den = pow(h[i_CR3][1][0]->GetBinError(iBin), 2);
      ++jBin;
    }
  }
  cout << "jBin      " << jBin << endl << endl;
  ptratio->SetBinContent(jBin, bin_content_num);
  ptratio->SetBinError(jBin, sqrt(bin_error_num));
  denominator->SetBinContent(jBin, bin_content_den);
  denominator->SetBinError(jBin, sqrt(bin_error_den));

  //flat SF
  cout << "WJets flat 3to1 SF             " << ptratio->Integral()/denominator->Integral() << endl;
  cout << "WJets data-driven flat 7to9 SF " << h_data_CR9->Integral()/h_data_CR7->Integral() << endl;

  //pt-dependent SF
  ptratio->Divide(denominator);
  file_out_W->cd();
  ptratio->Write();
  file_out_W->Close();


  //Get mu iso SF (5to3 SF) (approx. of 4to2) and mutau iso SF (5to1) (approx. of 4to0)
  TH1F* h_data5 = (TH1F*) h[i_CR5][0][0]->Clone("data_CR5");
  TH1F* h_data3 = (TH1F*) h[i_CR3][0][0]->Clone("data_CR3");
  TH1F* h_data1 = (TH1F*) h[i_CR1][0][0]->Clone("data_CR1");
  for (unsigned int j=1; j<names.size(); ++j) {
    if (j==2) continue;
    h_data5->Add(h[i_CR5][j][0], -1);
    h_data3->Add(h[i_CR3][j][0], -1);
    h_data1->Add(h[i_CR1][j][0], -1);
  }
  float data5_error_sq = 0;
  float data3_error_sq = 0;
  float data1_error_sq = 0;
  float MC5_error_sq = 0;
  float MC3_error_sq = 0;
  float MC1_error_sq = 0;
  for (unsigned int iBin=1; iBin<h_data5->GetNbinsX()+1; ++iBin) {
    data5_error_sq += pow(h_data5->GetBinError(iBin), 2);
    data3_error_sq += pow(h_data3->GetBinError(iBin), 2);
    data1_error_sq += pow(h_data1->GetBinError(iBin), 2);
    MC5_error_sq += pow(h[i_CR5][2][0]->GetBinError(iBin), 2);
    MC3_error_sq += pow(h[i_CR3][2][0]->GetBinError(iBin), 2);
    MC1_error_sq += pow(h[i_CR1][2][0]->GetBinError(iBin), 2);
  }
  float mu_iso_SF = h_data3->Integral()/h_data5->Integral();
  float mu_iso_SF_err = sqrt( data5_error_sq/pow(h_data5->Integral(), 2) + data3_error_sq/pow(h_data3->Integral(), 2) ) * mu_iso_SF;
  float mu_iso_SF_MC = h[i_CR3][2][0]->Integral()/h[i_CR5][2][0]->Integral();
  float mu_iso_SF_MC_err = sqrt( MC5_error_sq/pow(h[i_CR5][2][0]->Integral(), 2) + MC3_error_sq/pow(h[i_CR3][2][0]->Integral(), 2) ) * mu_iso_SF_MC;
  cout << "mu iso SF               " << mu_iso_SF << "  stat err " << mu_iso_SF_err << endl;
  cout << "mu iso SF (from QCD MC) " << mu_iso_SF_MC << "  stat err " << mu_iso_SF_MC_err << endl;
  
  float mutau_iso_SF = h_data1->Integral()/h_data5->Integral();
  float mutau_iso_SF_err = sqrt( data5_error_sq/pow(h_data5->Integral(), 2) + data1_error_sq/pow(h_data1->Integral(), 2) ) * mutau_iso_SF;
  float mutau_iso_SF_MC = h[i_CR1][2][0]->Integral()/h[i_CR5][2][0]->Integral();
  float mutau_iso_SF_MC_err = sqrt( MC5_error_sq/pow(h[i_CR5][2][0]->Integral(), 2) + MC1_error_sq/pow(h[i_CR1][2][0]->Integral(), 2) ) * mutau_iso_SF_MC;
  cout << "mutau iso SF               " << mutau_iso_SF << "  stat err " << mutau_iso_SF_err << endl;
  cout << "mutau iso SF (from QCD MC) " << mutau_iso_SF_MC << "  stat err " << mutau_iso_SF_MC_err << endl;
  
  
  file_out_CR2->cd();
  for (unsigned int k=0; k<vars.size(); ++k) {
    TH1F* h_QCD_CR2 = (TH1F*) h[i_CR4][0][k]->Clone("QCD_"+vars[k]+taun[0]);
    h_QCD_CR2->Add(h[i_CR4][1][k], -1);
    for (unsigned int j=3; j<names.size(); ++j) h_QCD_CR2->Add(h[i_CR4][j][k], -1);
  
    for (unsigned int iBin = 0; iBin<h_QCD_CR2->GetNbinsX(); ++iBin) {
      if (h_QCD_CR2->GetBinContent(iBin) < 0) h_QCD_CR2->SetBinContent(iBin,0);
    }
    h_QCD_CR2->Scale(mu_iso_SF);
    TH1F* h2_QCD_CR2 = (TH1F*) h_QCD_CR2->Clone("QCD_"+vars[k]+taun[1]);
    for (unsigned int iQCD=1; iQCD< h2_QCD_CR2->GetNbinsX()+1; ++iQCD) h2_QCD_CR2->SetBinContent(iQCD, 0);
    h_QCD_CR2->Write();
    h2_QCD_CR2->Write();
  }
  
  
  file_out_CR0->cd();
  for (unsigned int k=0; k<vars.size(); ++k) {
    TH1F* h_QCD_CR0 = (TH1F*) h[i_CR4][0][k]->Clone("QCD_"+vars[k]+taun[0]);
    h_QCD_CR0->Add(h[i_CR4][1][k], -1);
    for (unsigned int j=3; j<names.size(); ++j) h_QCD_CR0->Add(h[i_CR4][j][k], -1);
  
    for (unsigned int iBin = 0; iBin<h_QCD_CR0->GetNbinsX(); ++iBin) {
      if (h_QCD_CR0->GetBinContent(iBin) < 0) h_QCD_CR0->SetBinContent(iBin,0);
    }
    h_QCD_CR0->Scale(mutau_iso_SF);
    TH1F* h2_QCD_CR0 = (TH1F*) h_QCD_CR0->Clone("QCD_"+vars[k]+taun[1]);
    for (unsigned int iQCD=1; iQCD< h2_QCD_CR0->GetNbinsX()+1; ++iQCD) h2_QCD_CR0->SetBinContent(iQCD, 0);
    h_QCD_CR0->Write();
    h2_QCD_CR0->Write();
  }
  file_out_CR0->Close();
  file_out_CR2->Close();


  return 0;
}
