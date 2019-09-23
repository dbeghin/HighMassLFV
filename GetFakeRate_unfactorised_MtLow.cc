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
#include "aux.h"

using namespace std;
int main(/*int argc, char** argv*/) {
  TFile* file_in  = new TFile("Figures/histos_fakerate_MtLow.root", "R");
  TFile* file_out = new TFile("HighMassLFVMuTau/fakerate_unfactorised_MtLow.root", "RECREATE");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("taupt_ratio_pass");
  vars.push_back("taupt_ratio_fail");

  vector<TString> systs;
  systs.push_back("nominal");
  
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    systs.push_back(systs_aux[iAux]+"_up");
    systs.push_back(systs_aux[iAux]+"_down");
  }

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> taun;
  taun.push_back("realtau");  int n_real = taun.size()-1;


  //vector<float> xpoints_all {0, 30, 40, 50, 60, 70, 80, 100, 120, 150, 300, 1000};
  vector<vector<float>> xpoints;                                                                        vector<TString> sector_name;
  vector<float> xpoints_left {0, 30, 40, 50, 70, 100, 150};  xpoints.push_back(xpoints_left);   sector_name.push_back("taupt_0_150");
  vector<float> xpoints_right {150, 1000};                   xpoints.push_back(xpoints_right);  sector_name.push_back("taupt_150_1000");

  vector<vector<float>> ypoints;
  vector<float> ypoints_left {0, 0.5, 0.6, 0.65, 0.7, 0.75, 1., 3.};  ypoints.push_back(ypoints_left);
  vector<float> ypoints_right {0, 0.7, 1., 3};                 ypoints.push_back(ypoints_right);

  vector<TH2F*> h[names.size()][vars.size()][systs.size()];
  vector<TH2F*> h_MC[vars.size()][systs.size()];
  vector<TH2F*> h_data[vars.size()][systs.size()];
  for (unsigned int k=0; k<vars.size(); ++k) { 
    for (unsigned int l=0; l<systs.size(); ++l) {
      for (unsigned int m=0; m<eta.size(); ++m) {
	for (unsigned int j=0; j<names.size(); ++j) {
	  TString name_in = names[j]+vars[k]+"_"+systs[l]+"_MtLow_OS_"+eta[m]+"_"+taun[n_real];
	  h[j][k][l].push_back( (TH2F*) file_in->Get(name_in) );
	  h[j][k][l][m]->SetName(names[j]+vars[k]+"_"+systs[l]+"_"+eta[m]+"_"+taun[n_real]);

	  name_in = names[j]+vars[k]+"_"+systs[l]+"_MtLow_SS_"+eta[m]+"_"+taun[n_real];
	  TH2F* h_temp = (TH2F*) file_in->Get(name_in);
	  h[j][k][l][m]->Add(h_temp);
	  delete h_temp;
	}
	TString name_in = vars[k]+"_"+systs[l]+"_"+eta[m];
	h_data[k][l].push_back( (TH2F*) h[0][k][l][m]->Clone("data_"+name_in) );
	h_MC[k][l].push_back( (TH2F*) h[1][k][l][m]->Clone("MC_"+name_in) );
	for (unsigned int j=2; j<names.size(); ++j) {
	  h_MC[k][l][m]->Add(h[j][k][l][m]);
	  delete h[j][k][l][m];
	}
	delete h[0][k][l][m];
	delete h[1][k][l][m];
      }
    }
  }


  //data
  int n_sectors = xpoints.size();
  cout << "n_sectors " << n_sectors << endl;
  int half_var = vars.size()/2;//even->pass, odd->fail
  vector<TH2F*> hpass_data[half_var][systs.size()][n_sectors];
  vector<TH2F*> hfail_data[half_var][systs.size()][n_sectors];

  TH2F* hpass_data_total[half_var][systs.size()][n_sectors];
  TH2F* hfail_data_total[half_var][systs.size()][n_sectors];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int l=0; l<systs.size(); ++l) {
      for (unsigned int m=0; m<h_data[k][l].size(); ++m) {

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
        
	  int half_k = k/2;
          
	  //even->pass, odd->fail
	  if (k%2 == 0) {
	    TString name_in = "FakeRate_"+vars[k]+"_data_"+systs[l]+"_"+eta[m]+"_"+sector_name[sector];
	    hpass_data[half_k][l][sector].push_back( new TH2F(name_in, name_in, array_size_x-1, rebin_array_x, array_size_y-1, rebin_array_y) );
	  }
	  else {
	    TString name_in = "den2_"+vars[k]+"_"+systs[l]+"_"+eta[m]+"_"+sector_name[sector];
	    hfail_data[half_k][l][sector].push_back( new TH2F(name_in, name_in, array_size_x-1, rebin_array_x, array_size_y-1, rebin_array_y) );
	  }
          
	  int jBinX = 1, jBinY = 1, binStartX = 1, binStartY = 1;
	  while (jBinX <= array_size_x) { 
	    unsigned int iBinX=binStartX;
	    unsigned int iBinY=binStartY;
	    float bin_content = 0, bin_error=0;
	    while (h_data[k][l][m]->GetXaxis()->GetBinCenter(iBinX) < rebin_array_x[jBinX]) {
	      iBinY=binStartY;
	      if (h_data[k][l][m]->GetXaxis()->GetBinCenter(iBinX) > rebin_array_x[jBinX-1]) {
		while (h_data[k][l][m]->GetYaxis()->GetBinCenter(iBinY) < rebin_array_y[jBinY]) {
		  if (h_data[k][l][m]->GetYaxis()->GetBinCenter(iBinY) > rebin_array_y[jBinY-1]) {
		    bin_content = bin_content + h_data[k][l][m]->GetBinContent(iBinX, iBinY) - h_MC[k][l][m]->GetBinContent(iBinX, iBinY);
		    bin_error = bin_error + pow(h_data[k][l][m]->GetBinError(iBinX, iBinY), 2) + pow(h_MC[k][l][m]->GetBinError(iBinX, iBinY), 2);
		  }
		  ++iBinY;
		}
	      }
	      ++iBinX;
	    }
	    
	    if (bin_content < 0) bin_content = 0;
	    if (k%2 == 0) {
	      hpass_data[half_k][l][sector][m]->SetBinContent(jBinX, jBinY, bin_content);
	      hpass_data[half_k][l][sector][m]->SetBinError(jBinX, jBinY, sqrt(bin_error));
	    }
	    else {
	      hfail_data[half_k][l][sector][m]->SetBinContent(jBinX, jBinY, bin_content);
	      hfail_data[half_k][l][sector][m]->SetBinError(jBinX, jBinY, sqrt(bin_error));
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
	  
          
	  //if (k%2 != 0) hfail_data[half_k][l][sector][m]->Add(hpass_data[half_k][l][sector][m]);
	  
	  if (m==0) {
	    if (k%2 != 0) hfail_data_total[half_k][l][sector] = (TH2F*) hfail_data[half_k][l][sector][m]->Clone("den2_total_"+vars[k]+"_"+sector_name[sector]);
	    if (k%2 == 0) hpass_data_total[half_k][l][sector] = (TH2F*) hpass_data[half_k][l][sector][m]->Clone("FakeRateByTauPtAndRatio_total_"+systs[l]+"_"+sector_name[sector]);
	  }
	  else {
	    if (k%2 != 0) hfail_data_total[half_k][l][sector]->Add(hfail_data[half_k][l][sector][m]);
	    if (k%2 == 0) hpass_data_total[half_k][l][sector]->Add(hpass_data[half_k][l][sector][m]);
	  }
	  
	  if (k%2 != 0) hpass_data[half_k][l][sector][m]->Divide(hfail_data[half_k][l][sector][m]);
	  hpass_data[half_k][l][sector][m]->SetName("FakeRateByTauPtAndRatio_"+systs[l]+"_"+eta[m]+"_"+sector_name[sector]);
	  hpass_data[half_k][l][sector][m]->SetTitle("FakeRateByTauPtAndRatio_"+systs[l]+"_"+eta[m]+"_"+sector_name[sector]);
        }
      }
    }
  }
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    for (unsigned int l=0; l<systs.size(); ++l) {
      for (unsigned int sector=0; sector<n_sectors; ++sector) {
	hpass_data_total[half_k][l][sector]->Divide(hfail_data_total[half_k][l][sector]);
      }
    }
  }

  file_out->cd();
  for (unsigned int sector=0; sector<n_sectors; ++sector) {
    for (unsigned int l=0; l<systs.size(); ++l) {
      for (unsigned int half_k=0; half_k<half_var; ++half_k) {
	hpass_data_total[half_k][l][sector]->Write();
	cout << half_k << " " << l << " " << sector << " " << hpass_data_total[half_k][l][sector]->GetName() << endl;
	for (unsigned int m=0; m<eta.size(); ++m) {
	  hpass_data[half_k][l][sector][m]->Write();
	}
      }
    }
  }
  file_out->Close();



  return 0;
  
}
