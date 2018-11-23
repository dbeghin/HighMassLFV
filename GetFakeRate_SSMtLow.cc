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
  TFile* file_out = new TFile("HighMassLFVMuTau/fakerate_SSMtLow.root", "RECREATE");
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
	    TString name_in = names[j]+vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+taun[n];
	    h[j][k][l][m].push_back( (TH2F*) file_in->Get(name_in) );
	    h[j][k][l][m][n]->SetName(names[j]+vars[k]);
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
	  if (n==1) h_data[k].push_back( (TH2F*) h[0][k][l][m][n_fake]->Clone("data_"+name_in) );
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
  vector<TH2F*> hpass_data;
  vector<TH2F*> hfail_data;

  TH2F* hpass_data_total;
  TH2F* hfail_data_total;
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_data[k].size(); ++i) {
      int l = i/eta.size(), m = i % eta.size();
      vector<float> xpoints;
      xpoints.clear();
      xpoints = xpoints_all;

      int array_size = xpoints.size();
      float rebin_array[array_size];
      for (unsigned int iVector=0; iVector<array_size; ++iVector) rebin_array[iVector] = xpoints[iVector];
      
      //even->pass, odd->fail
      if (k%2 == 0) {
	TString name_in = "FakeRate_"+vars[k]+"_data_"+dms[l]+"_"+eta[m];
	hpass_data.push_back( new TH2F(name_in, name_in, array_size-1, rebin_array, array_size-1, rebin_array) );
      }
      else {
	TString name_in = "den2_"+vars[k]+"_"+dms[l]+"_"+eta[m];
	hfail_data.push_back( new TH2F(name_in, name_in, array_size-1, rebin_array, array_size-1, rebin_array) );
      }


      int jBinX = 1, jBinY = 1, binStartX = 1, binStartY = 1;
      while (jBinX <= array_size) { 
	unsigned int iBinX=binStartX;
	unsigned int iBinY=binStartY;
	float bin_content = 0, bin_error=0;
	while (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) < rebin_array[jBinX]) {
	  iBinY=binStartY;
	  while (h_data[k][i]->GetYaxis()->GetBinCenter(iBinY) < rebin_array[jBinY]) {
	    bin_content = bin_content + h_data[k][i]->GetBinContent(iBinX, iBinY) - h_MC[k][n_real][i]->GetBinContent(iBinX, iBinY);
	    bin_error = bin_error + pow(h_data[k][i]->GetBinError(iBinX, iBinY), 2) + pow(h_MC[k][n_real][i]->GetBinError(iBinX, iBinY), 2);
	    ++iBinY;
	  }
	  ++iBinX;
	}
	
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_data[i]->SetBinContent(jBinX, jBinY, bin_content);
	  hpass_data[i]->SetBinError(jBinX, jBinY, sqrt(bin_error));
	}
	else {
	  hfail_data[i]->SetBinContent(jBinX, jBinY, bin_content);
	  hfail_data[i]->SetBinError(jBinX, jBinY, sqrt(bin_error));
	}	    

	binStartY = iBinY;
	++jBinY;
	if (jBinY == array_size) {
	  jBinY = 1;
	  binStartY = 1;
	  ++jBinX;
	  binStartX = iBinX;
	}
      }

      
      if (k%2 != 0) hfail_data[i]->Add(hpass_data[i]);
      if (i==0) {
        if (k%2 != 0) hfail_data_total = (TH2F*) hfail_data[i]->Clone("den2_total_"+vars[k]);
        if (k%2 == 0) hpass_data_total = (TH2F*) hpass_data[i]->Clone("hpass_"+vars[k]+"_data_total");
      }
      else {
        if (k%2 != 0) hfail_data_total->Add(hfail_data[i]);
        if (k%2 == 0) hpass_data_total->Add(hpass_data[i]);
      }
      if (k%2 != 0) hpass_data[i]->Divide(hfail_data[i]);
    }
  }
  hpass_data_total->Divide(hfail_data_total);




  file_out->cd();
  hpass_data_total->Write();
  for (unsigned int i=0; i<hpass_data.size(); ++i) {
    hpass_data[i]->Write();
  }
  file_out->Close();



  return 0;
}
