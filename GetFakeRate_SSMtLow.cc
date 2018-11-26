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

  //vector<float> xpoints_all {0, 30, 40, 50, 60, 70, 80, 100, 120, 150, 300, 1000};
  vector<vector<float>> xpoints;                                                                                        vector<TString> sector_name;
  vector<float> xpoints_lowleft {0, 30, 40, 50, 60, 70, 80, 100, 120, 150, 300};  xpoints.push_back(xpoints_lowleft);   sector_name.push_back("taupt_0_300");
  vector<float> xpoints_midleft {0, 100, 150, 300};                               xpoints.push_back(xpoints_midleft);   sector_name.push_back("taupt_0_300");
  vector<float> xpoints_upleft {0, 300};                                          xpoints.push_back(xpoints_upleft);    sector_name.push_back("taupt_0_300");
  vector<float> xpoints_upright {300, 1000};                                      xpoints.push_back(xpoints_upright);   sector_name.push_back("taupt_300_1000");
  vector<float> xpoints_lowright {300, 1000};                                     xpoints.push_back(xpoints_lowright);  sector_name.push_back("taupt_300_1000");

  vector<vector<float>> ypoints;										      
  vector<float> ypoints_lowleft {0, 30, 40, 50, 60, 70, 80, 100, 120, 150};       ypoints.push_back(ypoints_lowleft);   sector_name[0] += "_jetpt_0_150";
  vector<float> ypoints_midleft {150, 300};                                       ypoints.push_back(ypoints_midleft);   sector_name[1] += "_jetpt_150_300";
  vector<float> ypoints_upleft {300, 1000};					  ypoints.push_back(ypoints_upleft);    sector_name[2] += "_jetpt_300_1000";
  vector<float> ypoints_upright {300, 1000};					  ypoints.push_back(ypoints_upright);   sector_name[3] += "_jetpt_300_1000";
  vector<float> ypoints_lowright {0, 300};					  ypoints.push_back(ypoints_lowright);  sector_name[4] += "_jetpt_0_300";

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
  int n_sectors = xpoints.size();
  cout << "n_sectors " << n_sectors << endl;
  vector<TH2F*> hpass_data[n_sectors];
  vector<TH2F*> hfail_data[n_sectors];

  TH2F* hpass_data_eta[eta.size()][n_sectors];
  TH2F* hfail_data_eta[eta.size()][n_sectors];

  TH2F* hpass_data_total[n_sectors];
  TH2F* hfail_data_total[n_sectors];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_data[k].size(); ++i) {
      int l = i/eta.size(), m = i % eta.size();

      for (unsigned int sector = 0; sector<n_sectors; ++sector) {
	vector<float> xxpoints, yypoints;
	xxpoints.clear(), yypoints.clear();
	xxpoints = xpoints[sector], yypoints = ypoints[sector];

        int array_size_x = xxpoints.size();
        float rebin_array_x[array_size_x];
        for (unsigned int iVector=0; iVector<array_size_x; ++iVector) rebin_array_x[iVector] = xxpoints[iVector];
        int array_size_y = yypoints.size();
        float rebin_array_y[array_size_y];
        for (unsigned int iVector=0; iVector<array_size_y; ++iVector) rebin_array_y[iVector] = yypoints[iVector];
        
        //even->pass, odd->fail
        if (k%2 == 0) {
	  TString name_in = "FakeRate_"+vars[k]+"_data_"+dms[l]+"_"+eta[m]+"_"+sector_name[sector];
	  hpass_data[sector].push_back( new TH2F(name_in, name_in, array_size_x-1, rebin_array_x, array_size_y-1, rebin_array_y) );
        }
        else {
	  TString name_in = "den2_"+vars[k]+"_"+dms[l]+"_"+eta[m]+"_"+sector_name[sector];
	  hfail_data[sector].push_back( new TH2F(name_in, name_in, array_size_x-1, rebin_array_x, array_size_y-1, rebin_array_y) );
        }
        
        int jBinX = 1, jBinY = 1, binStartX = 1, binStartY = 1;
        while (jBinX <= array_size_x) { 
	  unsigned int iBinX=binStartX;
	  unsigned int iBinY=binStartY;
	  float bin_content = 0, bin_error=0;
	  while (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) < rebin_array_x[jBinX]) {
	    iBinY=binStartY;
	    if (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) > rebin_array_x[jBinX-1]) {
	      while (h_data[k][i]->GetYaxis()->GetBinCenter(iBinY) < rebin_array_y[jBinY]) {
		if (h_data[k][i]->GetYaxis()->GetBinCenter(iBinY) > rebin_array_y[jBinY-1]) {
		  bin_content = bin_content + h_data[k][i]->GetBinContent(iBinX, iBinY) - h_MC[k][n_real][i]->GetBinContent(iBinX, iBinY);
		  bin_error = bin_error + pow(h_data[k][i]->GetBinError(iBinX, iBinY), 2) + pow(h_MC[k][n_real][i]->GetBinError(iBinX, iBinY), 2);
		}
		++iBinY;
	      }
	    }
	    ++iBinX;
	  }
	  
	  if (bin_content < 0) bin_content = 0;
	  if (k%2 == 0) {
	    hpass_data[sector][i]->SetBinContent(jBinX, jBinY, bin_content);
	    hpass_data[sector][i]->SetBinError(jBinX, jBinY, sqrt(bin_error));
	  }
	  else {
	    hfail_data[sector][i]->SetBinContent(jBinX, jBinY, bin_content);
	    hfail_data[sector][i]->SetBinError(jBinX, jBinY, sqrt(bin_error));
	  }	    
	  
	  binStartY = iBinY;
	  ++jBinY;
	  if (jBinY == array_size_y) {
	    jBinY = 1;
	    binStartY = 1;
	    ++jBinX;
	    binStartX = iBinX;
	  }
        }
        
        
        if (k%2 != 0) hfail_data[sector][i]->Add(hpass_data[sector][i]);

        if (i==0) {
          if (k%2 != 0) hfail_data_total[sector] = (TH2F*) hfail_data[sector][i]->Clone("den2_total_"+vars[k]+"_"+sector_name[sector]);
          if (k%2 == 0) hpass_data_total[sector] = (TH2F*) hpass_data[sector][i]->Clone("hratio_data_"+vars[k]+"_"+sector_name[sector]+"_total");
        }
        else {
          if (k%2 != 0) hfail_data_total[sector]->Add(hfail_data[sector][i]);
          if (k%2 == 0) hpass_data_total[sector]->Add(hpass_data[sector][i]);
        }

        if (l==0) {
          if (k%2 != 0) hfail_data_eta[m][sector] = (TH2F*) hfail_data[sector][i]->Clone("den2_eta_"+eta[m]+"_"+vars[k]+"_"+sector_name[sector]);
          if (k%2 == 0) hpass_data_eta[m][sector] = (TH2F*) hpass_data[sector][i]->Clone("hratio_data_eta_"+eta[m]+"_"+vars[k]+"_"+sector_name[sector]);
        }
        else {
          if (k%2 != 0) hfail_data_eta[m][sector]->Add(hfail_data[sector][i]);
          if (k%2 == 0) hpass_data_eta[m][sector]->Add(hpass_data[sector][i]);
        }

        if (k%2 != 0) hpass_data[sector][i]->Divide(hfail_data[sector][i]);
      }
    }
  }
  for (unsigned int sector=0; sector<n_sectors; ++sector) {
    hpass_data_total[sector]->Divide(hfail_data_total[sector]);
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_data_eta[m][sector]->Divide(hfail_data_eta[m][sector]);
    }
  }



  file_out->cd();
  for (unsigned int sector=0; sector<n_sectors; ++sector) {
    hpass_data_total[sector]->Write();
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_data_eta[m][sector]->Write();
    }
    for (unsigned int i=0; i<hpass_data[sector].size(); ++i) {
      hpass_data[sector][i]->Write();
    }
  }
  file_out->Close();



  return 0;
}
