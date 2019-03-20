#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1.h"
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
  TFile* file_out = new TFile("taupthistos.root", "RECREATE");
  TFile* file_in  = new TFile("Figures/histos_fakerate_SSMtLow.root", "R");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("taupt_jetpt_pass");
  vars.push_back("taupt_jetpt_fail");
  //vars.push_back("taupt_mupt_pass");
  //vars.push_back("taupt_mupt_fail");

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> taun;
  taun.push_back("realtau");  int n_real = taun.size()-1;
  taun.push_back("faketau");  int n_fake = taun.size()-1;

  vector<float> xpoints_all {0, 30, 40, 50, 60, 70, 80, 100, 120, 150, 300, 1000};

  vector<TH2F*> h[names.size()][vars.size()][dms.size()][eta.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) { 
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  for (unsigned int n=0; n<taun.size(); ++n) {
	    TString name_in = names[j]+vars[k]+"_MtLow_SS_"+dms[l]+"_"+eta[m]+"_"+taun[n];
	    //cout << name_in << endl;
	    h[j][k][l][m].push_back( (TH2F*) file_in->Get(name_in) );
	    //h[j][k][l][m][n]->SetName(names[j]+vars[k]);
	  }
	}
      }
    }
  }


  vector<TH2F*> h_MC[vars.size()][taun.size()];
  vector<TH2F*> h_data[vars.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int n=0; n<taun.size(); ++n) {
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  TString name_in = vars[k]+"_"+dms[l]+"_"+eta[m];
	  if (n==n_real) h_data[k].push_back( (TH2F*) h[0][k][l][m][n_real]->Clone("data_"+name_in) );
	  h_MC[k][n].push_back( (TH2F*) h[1][k][l][m][n]->Clone("MC_"+name_in+"_"+taun[n]) );
	  int i = h_MC[k][n].size()-1;
	  for (unsigned int j=2; j<names.size(); ++j) {
	    h_MC[k][n][i]->Add(h[j][k][l][m][n]);
	  }
	}
      }
    }
  }



  //data
  int half_var = vars.size()/2;//even->pass, odd->fail
  vector<TH1F*> hpass_data[half_var];
  vector<TH1F*> hfail_data[half_var];

  TH1F* hpass_data_eta[half_var][eta.size()];
  TH1F* hfail_data_eta[half_var][eta.size()];

  TH1F* hpass_data_total[half_var];
  TH1F* hfail_data_total[half_var];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_data[k].size(); ++i) {
      int l = i/eta.size(), m = i % eta.size();

      vector<float> xxpoints;
      xxpoints.clear();
      xxpoints = xpoints_all;

      int array_size_x = xxpoints.size();
      float rebin_array_x[array_size_x];
      for (unsigned int iVector=0; iVector<array_size_x; ++iVector) rebin_array_x[iVector] = xxpoints[iVector];

      int half_k = k/2;
        
      //even->pass, odd->fail
      if (k%2 == 0) {
	TString name_in = "Pass_"+vars[k]+"_data_"+dms[l]+"_"+eta[m];
	hpass_data[half_k].push_back( new TH1F(name_in, name_in, array_size_x-1, rebin_array_x) );
      }
      else {
	TString name_in = "Fail_"+vars[k]+"_"+dms[l]+"_"+eta[m];
	hfail_data[half_k].push_back( new TH1F(name_in, name_in, array_size_x-1, rebin_array_x) );
      }
      
      int jBinX = 1, binStartX = 1, nBinsY = h_data[k][i]->GetNbinsY();
      while (jBinX <= array_size_x) { 
	unsigned int iBinX=binStartX;
	float bin_content = 0, bin_error=0;
	while (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) < rebin_array_x[jBinX]) {
	  if (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) > rebin_array_x[jBinX-1]) {
	    bin_content += h_data[k][i]->Integral(iBinX, iBinX, 1, nBinsY); //- h_MC[k][n_real][i]->Integral(iBinX, iBinX, 1, nBinsY);
	    bin_error += h_data[k][i]->Integral(iBinX, iBinX, 1, nBinsY);
	  }
	  ++iBinX;
	}
	  
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_data[half_k][i]->SetBinContent(jBinX, bin_content);
	  hpass_data[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}
	else {
	  hfail_data[half_k][i]->SetBinContent(jBinX, bin_content);
	  hfail_data[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}	    
	  
	++jBinX;
	binStartX = iBinX;
      }        
        
      if (k%2 != 0) hfail_data[half_k][i]->Add(hpass_data[half_k][i]);

      if (i==0) {
	if (k%2 != 0) hfail_data_total[half_k] = (TH1F*) hfail_data[half_k][i]->Clone("Fail_total_"+vars[k]);
	if (k%2 == 0) hpass_data_total[half_k] = (TH1F*) hpass_data[half_k][i]->Clone("Pass_data_total_"+vars[k]);
      }
      else {
	if (k%2 != 0) hfail_data_total[half_k]->Add(hfail_data[half_k][i]);
	if (k%2 == 0) hpass_data_total[half_k]->Add(hpass_data[half_k][i]);
      }

      if (l==0) {
	if (k%2 != 0) hfail_data_eta[half_k][m] = (TH1F*) hfail_data[half_k][i]->Clone("Fail_eta_"+eta[m]+"_"+vars[k]);
	if (k%2 == 0) hpass_data_eta[half_k][m] = (TH1F*) hpass_data[half_k][i]->Clone("Pass_data_eta_"+eta[m]+"_"+vars[k]);
      }
      else {
	if (k%2 != 0) hfail_data_eta[half_k][m]->Add(hfail_data[half_k][i]);
	if (k%2 == 0) hpass_data_eta[half_k][m]->Add(hpass_data[half_k][i]);
      }

      if (k%2 != 0) hpass_data[half_k][i]->Divide(hfail_data[half_k][i]);
    }
  }



  file_out->cd();
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    hpass_data_total[half_k]->Write();
    hfail_data_total[half_k]->Write();
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_data_eta[half_k][m]->Write();
      hfail_data_eta[half_k][m]->Write();
    }
    for (unsigned int i=0; i<hpass_data[half_k].size(); ++i) {
      hpass_data[half_k][i]->Write();
      hfail_data[half_k][i]->Write();
    }
  }
  file_out->Close();



  return 0;
}
