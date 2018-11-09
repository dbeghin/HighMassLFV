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
  //names.push_back("WJets_");//1
  //names.push_back("QCD_");//2
  names.push_back("DY_");
  //names.push_back("TT_");
  //names.push_back("ST_");
  //names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("taupt_pass");
  vars.push_back("taupt_fail");
  int k_jet_start = vars.size();
  vars.push_back("jetpt_pass");
  vars.push_back("jetpt_fail");
  int k_ptratio_start = vars.size();
  vars.push_back("ptratio_pass");
  vars.push_back("ptratio_fail");

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> trigger;
  trigger.push_back("tautrfired");
  trigger.push_back("tautrindiff");

  vector<float> xpoints_all {0, 30, 40, 50, 70, 100, 200, 500, 1000};
  vector<float> xpoints_ratio {0, 0.2, 0.4, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.5, 2.0, 3.0, 10};

  vector<TString> pt_range;           
  pt_range.push_back("tau_pt_30_50"); 
  pt_range.push_back("tau_pt_50_100");
  pt_range.push_back("tau_pt_100");   

  //retrieve histograms from all control regions
  //only for CR4 (1) do we care to have all histos
  vector<TH1F*> h[names.size()][vars.size()][dms.size()][eta.size()][trigger.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) { 
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  for (unsigned int n=0; n<trigger.size(); ++n) {
	    for (unsigned int p=0; p<pt_range.size(); ++p) {
	      TString name_in = names[j]+vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+trigger[n]+"_"+pt_range[p];
	      h[j][k][l][m][n].push_back( (TH1F*) file_in->Get(name_in) );
	      h[j][k][l][m][n][p]->SetName(names[j]+vars[k]);
	    }
	  }
	}
      }
    }
  }

  int n_indiff = 1; //start with the tau trigger-indifference
  int n_tautr = 0;

  vector<TH1F*> h_MC[vars.size()];
  vector<TH1F*> h_data[vars.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int l=0; l<dms.size(); ++l) {
      for (unsigned int m=0; m<eta.size(); ++m) {
	for (unsigned int p=0; p<pt_range.size(); ++p) {
	  //int i = l*eta.size()*pt_range.size()+m*pt_range.size()+p;
	  TString name_in = vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+pt_range[p];
	  h_MC[k].push_back( (TH1F*) h[1][k][l][m][n_indiff][p]->Clone("MC_"+name_in) );
	  h_data[k].push_back( (TH1F*) h[0][k][l][m][n_indiff][p]->Clone("data_"+name_in) );
	}
      }
    }
  }



  //MC
  int half_var = vars.size()/2;//even->pass, odd->fail
  vector<TH1F*> hpass_MC[half_var];
  vector<TH1F*> hfail_MC[half_var];
  TH1F* hpass_MC_total[half_var][pt_range.size()];
  TH1F* hfail_MC_total[half_var][pt_range.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_MC[k].size(); ++i) {
      int l = i/(eta.size()*pt_range.size()), m = (i / pt_range.size()) % eta.size(), p = i % pt_range.size() ;
      vector<float> xpoints;
      if (k < k_ptratio_start) {
        xpoints = xpoints_all;
      }
      else {
        xpoints = xpoints_ratio;
      }
      int array_size = xpoints.size();
      float rebin_array[array_size];
      for (unsigned int iVector=0; iVector<array_size; ++iVector) rebin_array[iVector] = xpoints[iVector];
      
      int half_k = k/2;
      //even->pass, odd->fail
      if (k%2 == 0) {
	TString name_in = "FakeRate_"+vars[k]+"_MC_"+dms[l]+"_"+eta[m]+"_"+pt_range[p];
	hpass_MC[half_k].push_back( new TH1F(name_in, name_in, array_size-1, rebin_array) );
      }
      else {
	TString name_in = "den_"+vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+pt_range[p];
	hfail_MC[half_k].push_back( new TH1F(name_in, name_in, array_size-1, rebin_array) );
      }


      for (unsigned int j=2; j<names.size(); ++j) {
        h_MC[k][i]->Add(h[j][k][l][m][n_indiff][p]);
      }

      int jBin = 1;
      float bin_content = 0, bin_error=0;
      cout << k << " " << l << " " << m << " " << p << endl;
      for (unsigned int iBin=1; iBin < h_MC[k][i]->GetNbinsX()+1; ++iBin) {
        if (h_MC[k][i]->GetBinCenter(iBin) < rebin_array[jBin]) {
	  bin_content += h_MC[k][i]->GetBinContent(iBin);
	  bin_error += pow(h_MC[k][i]->GetBinError(iBin), 2);
        }
        else {
	  if (k%2 == 0) {
	    hpass_MC[half_k][i]->SetBinContent(jBin, bin_content);
	    hpass_MC[half_k][i]->SetBinError(jBin, sqrt(bin_error));
	  }
	  else {
	    hfail_MC[half_k][i]->SetBinContent(jBin, bin_content);
	    hfail_MC[half_k][i]->SetBinError(jBin, sqrt(bin_error));
	  }
	  bin_content = h_MC[k][i]->GetBinContent(iBin);
	  bin_error = pow(h_MC[k][i]->GetBinError(iBin), 2);
	  ++jBin;
        }
      }

      if (k%2 != 0) hfail_MC[half_k][i]->Add(hpass_MC[half_k][i]);
      if (i==0 || i==1 || i==2) {
        if (k%2 != 0) hfail_MC_total[half_k][p] = (TH1F*) hfail_MC[half_k][i]->Clone("den_total_"+vars[k]+"_"+pt_range[p]);
        if (k%2 == 0) hpass_MC_total[half_k][p] = (TH1F*) hpass_MC[half_k][i]->Clone("hpass_"+vars[k]+"_MC_total_tauindiff_"+pt_range[p]);
      }
      else {
        if (k%2 != 0) hfail_MC_total[half_k][p]->Add(hfail_MC[half_k][i]);
        if (k%2 == 0) hpass_MC_total[half_k][p]->Add(hpass_MC[half_k][i]);
      }
      if (k%2 != 0) hpass_MC[half_k][i]->Divide(hfail_MC[half_k][i]);
    }
  }
  for (unsigned int half_k=0; half_k<half_var; ++half_k) for (unsigned int p=0; p<pt_range.size(); ++p) hpass_MC_total[half_k][p]->Divide(hfail_MC_total[half_k][p]);


  //data
  vector<TH1F*> hpass_data[half_var];
  vector<TH1F*> hfail_data[half_var];
  TH1F* hpass_data_total[half_var][pt_range.size()];
  TH1F* hfail_data_total[half_var][pt_range.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_data[k].size(); ++i) {
      int l = i/(eta.size()*pt_range.size()), m = (i / pt_range.size()) % eta.size(), p = i % pt_range.size() ;
      vector<float> xpoints;
      if (k < k_ptratio_start) {
        xpoints = xpoints_all;
      }
      else {
        xpoints = xpoints_ratio;
      }

      int array_size = xpoints.size();
      float rebin_array[array_size];
      for (unsigned int iVector=0; iVector<array_size; ++iVector) rebin_array[iVector] = xpoints[iVector];
      
      int half_k = k/2;
      //even->pass, odd->fail
      if (k%2 == 0) {
	TString name_in = "FakeRate_"+vars[k]+"_data_"+dms[l]+"_"+eta[m]+"_"+pt_range[p];
	hpass_data[half_k].push_back( new TH1F(name_in, name_in, array_size-1, rebin_array) );
      }
      else {
	TString name_in = "den2_"+vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+pt_range[p];
	hfail_data[half_k].push_back( new TH1F(name_in, name_in, array_size-1, rebin_array) );
      }
      int jBin = 1;
      float bin_content = 0, bin_error=0;
      for (unsigned int iBin=1; iBin < h_data[k][i]->GetNbinsX()+1; ++iBin) {
        if (h_data[k][i]->GetBinCenter(iBin) < rebin_array[jBin]) {
	  bin_content += h_data[k][i]->GetBinContent(iBin);
	  bin_error += pow(h_data[k][i]->GetBinError(iBin), 2);
        }
        else {
	  if (k%2 == 0) {
	    hpass_data[half_k][i]->SetBinContent(jBin, bin_content);
	    hpass_data[half_k][i]->SetBinError(jBin, sqrt(bin_error));
	  }
	  else {
	    hfail_data[half_k][i]->SetBinContent(jBin, bin_content);
	    hfail_data[half_k][i]->SetBinError(jBin, sqrt(bin_error));
	  }	    
	  bin_content = h_data[k][i]->GetBinContent(iBin);
	  bin_error = pow(h_data[k][i]->GetBinError(iBin), 2);

	  ++jBin;
        }
      }
      if (k%2 != 0) hfail_data[half_k][i]->Add(hpass_data[half_k][i]);
      cout << k << " " << l << " " << m << " " << p << endl;
      if (i==0 || i==1 || i==2) {
        if (k%2 != 0) hfail_data_total[half_k][p] = (TH1F*) hfail_data[half_k][i]->Clone("den2_total_"+vars[k]+"_"+pt_range[p]);
        if (k%2 == 0) hpass_data_total[half_k][p] = (TH1F*) hpass_data[half_k][i]->Clone("hpass_"+vars[k]+"_data_total_tauindiff_"+pt_range[p]);
      }
      else {
        if (k%2 != 0) hfail_data_total[half_k][p]->Add(hfail_data[half_k][i]);
        if (k%2 == 0) hpass_data_total[half_k][p]->Add(hpass_data[half_k][i]);
      }
      if (k%2 != 0) hpass_data[half_k][i]->Divide(hfail_data[half_k][i]);
    }
  }
  for (unsigned int half_k = 0; half_k<half_var; ++half_k) for (unsigned int p=0; p<pt_range.size(); ++p) hpass_data_total[half_k][p]->Divide(hfail_data_total[half_k][p]);


  //With tau trigger applied
  /*TH1F* ptratio_MC_tautr = new TH1F("FakeRate_byTauPt_MC_"+trigger[n_tautr], "FakeRate_byTauPt_MC_"+trigger[n_tautr], len_x-1, x);
  TH1F* den_MC_tautr = new TH1F("den_MC_"+trigger[n_tautr], "den_MC_"+trigger[n_tautr], len_x-1, x);

  TH1F* aux_MC_pass = 0;
  TH1F* aux_MC_fail = 0;

  for (unsigned int j=1; j<names.size(); ++j) {
    for (unsigned int l=0; l<dms.size(); ++l) {
      for (unsigned int m=0; m<eta.size(); ++m) {
	for (unsigned int p=0; p<pt_range.size(); ++p) {
	  if (aux_MC_pass == 0) {
	    aux_MC_pass = (TH1F*) h[j][0][l][m][n_tautr][p]->Clone("MC_pass_"+trigger[n_tautr]);
	    aux_MC_fail = (TH1F*) h[j][1][l][m][n_tautr][p]->Clone("MC_fail_"+trigger[n_tautr]);
	  }
	  else {
	    aux_MC_pass->Add( h[j][0][l][m][n_tautr][p] );
	    aux_MC_fail->Add( h[j][1][l][m][n_tautr][p] );
	  }
	}
      }
    }
  }


  int jBin = 1;
  float bin_content_num = 0, bin_error_num=0;
  float bin_content_den = 0, bin_error_den=0;
  for (unsigned int iBin=1; iBin < aux_MC_pass->GetNbinsX()+1; ++iBin) {
    if (aux_MC_pass->GetBinCenter(iBin) < rebin_array[jBin]) {
      bin_content_num += aux_MC_pass->GetBinContent(iBin);
      bin_error_num += pow(aux_MC_pass->GetBinError(iBin), 2);
      bin_content_den += aux_MC_fail->GetBinContent(iBin);
      bin_error_den += pow(aux_MC_fail->GetBinError(iBin), 2);
    }
    else {
      ptratio_MC_tautr->SetBinContent(jBin, bin_content_num);
      ptratio_MC_tautr->SetBinError(jBin, sqrt(bin_error_num));
      bin_content_num = aux_MC_pass->GetBinContent(iBin);
      bin_error_num = pow(aux_MC_pass->GetBinError(iBin), 2);
      
      den_MC_tautr->SetBinContent(jBin, bin_content_den);
      den_MC_tautr->SetBinError(jBin, sqrt(bin_error_den));
      bin_content_den = aux_MC_fail->GetBinContent(iBin);
      bin_error_den = pow(aux_MC_fail->GetBinError(iBin), 2);
      ++jBin;
    }
  }
  den_MC_tautr->Add(ptratio_MC_tautr);
  ptratio_MC_tautr->Divide(den_MC_tautr);



  TH1F* ptratio_Data_tautr = new TH1F("FakeRate_byTauPt_Data_"+trigger[n_tautr], "FakeRate_byTauPt_Data_"+trigger[n_tautr], len_x-1, x);
  TH1F* den_Data_tautr = new TH1F("den_Data_"+trigger[n_tautr], "den_Data_"+trigger[n_tautr], len_x-1, x);

  TH1F* aux_Data_pass = 0;
  TH1F* aux_Data_fail = 0;

  for (unsigned int l=0; l<dms.size(); ++l) {
    for (unsigned int m=0; m<eta.size(); ++m) {
      for (unsigned int p=0; p<pt_range.size(); ++p) {
	if (aux_Data_pass == 0) {
	  aux_Data_pass = (TH1F*) h[0][0][l][m][n_tautr][p]->Clone("Data_pass_"+trigger[n_tautr]);
	  aux_Data_fail = (TH1F*) h[0][1][l][m][n_tautr][p]->Clone("Data_fail_"+trigger[n_tautr]);
	}
	else {
	  aux_Data_pass->Add( h[0][0][l][m][n_tautr][p] );
	  aux_Data_fail->Add( h[0][1][l][m][n_tautr][p] );
	}
      }
    }
  }


  jBin = 1;
  bin_content_num = 0, bin_error_num=0;
  bin_content_den = 0, bin_error_den=0;
  for (unsigned int iBin=1; iBin < aux_Data_pass->GetNbinsX()+1; ++iBin) {
    if (aux_Data_pass->GetBinCenter(iBin) < rebin_array[jBin]) {
      bin_content_num += aux_Data_pass->GetBinContent(iBin);
      bin_error_num += pow(aux_Data_pass->GetBinError(iBin), 2);
      bin_content_den += aux_Data_fail->GetBinContent(iBin);
      bin_error_den += pow(aux_Data_fail->GetBinError(iBin), 2);
    }
    else {
      ptratio_Data_tautr->SetBinContent(jBin, bin_content_num);
      ptratio_Data_tautr->SetBinError(jBin, sqrt(bin_error_num));
      bin_content_num = aux_Data_pass->GetBinContent(iBin);
      bin_error_num = pow(aux_Data_pass->GetBinError(iBin), 2);
      
      den_Data_tautr->SetBinContent(jBin, bin_content_den);
      den_Data_tautr->SetBinError(jBin, sqrt(bin_error_den));
      bin_content_den = aux_Data_fail->GetBinContent(iBin);
      bin_error_den = pow(aux_Data_fail->GetBinError(iBin), 2);
      ++jBin;
    }
  }
  den_Data_tautr->Add(ptratio_Data_tautr);
  ptratio_Data_tautr->Divide(den_Data_tautr);*/


  file_out->cd();
  for (unsigned int half_k = 0; half_k<half_var; ++half_k) {
    for (unsigned int p=0; p<pt_range.size(); ++p) {
      hpass_MC_total[half_k][p]->Write();
      hpass_data_total[half_k][p]->Write();
    }
    for (unsigned int i=0; i<hpass_data[half_k].size(); ++i) {
      hpass_MC[half_k][i]->Write();
      hpass_data[half_k][i]->Write();
    }
  }
  //ptratio_MC_tautr->Write();
  //ptratio_Data_tautr->Write();
  file_out->Close();



  return 0;
}
