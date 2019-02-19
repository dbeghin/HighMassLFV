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
void Normalise(TH1D* h) {
  double weighted_sum = 0, norm_factor = 0;
  for (unsigned int iBin=1; iBin<h->GetNbinsX()+1; ++iBin) {
    double bin_content = h->GetBinContent(iBin);
    double bin_error = h->GetBinError(iBin);

    weighted_sum += 1/pow(bin_error, 2) * bin_content;
    norm_factor += 1/pow(bin_error, 2);
  }
  double av_fr = weighted_sum/norm_factor;
  h->Scale(1./av_fr);
  cout << av_fr << endl;
}



int main(int argc, char** argv) {

  string sstart = *(argv+1);
  string eend = *(argv+2);

  TString start = sstart;
  TString end = eend;

  TFile* file_out = new TFile("HighMassLFVMuTau/fakerate_MtLow_"+start+"_"+end+".root", "RECREATE");
  TFile* file_in  = new TFile("Figures/histos_fakerate_MtLow.root", "R");

  int startbinX = stoi(sstart);
  int endbinX = stoi(eend);
  cout << startbinX + endbinX << endl;

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");

  vector<TString> vars;
  vars.push_back("taupt_ratio_pass");
  vars.push_back("taupt_ratio_fail");

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> taun;
  taun.push_back("realtau");  int n_real = taun.size()-1;
  //taun.push_back("faketau");  int n_fake = taun.size()-1;

  vector<float> xpoints_taupt {0, 30, 40, 50, 60, 70, 80, 100, 120, 150, 300, 1000};
  //vector<float> ypoints_ratio {0, 0.7, 2.};
  //vector<float> ypoints_ratio {0, 2.};
  vector<float> ypoints_ratio {0, 0.3, 0.5, 0.6, 0.7, 0.75, 0.8, 0.85, 1., 2.};

  vector<TH2D*> h[names.size()][vars.size()][dms.size()][eta.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) { 
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  for (unsigned int n=0; n<taun.size(); ++n) {
	    TString name_in = names[j]+vars[k]+"_MtLow_SS_"+dms[l]+"_"+eta[m]+"_"+taun[n];
	    h[j][k][l][m].push_back( (TH2D*) file_in->Get(name_in) );
	    h[j][k][l][m][n]->SetName(names[j]+vars[k]+dms[l]+"_"+eta[m]+"_"+taun[n]);

	    TH2D* hh2 = (TH2D*) file_in->Get(names[j]+vars[k]+"_MtLow_OS_"+dms[l]+"_"+eta[m]+"_"+taun[n]);
	    h[j][k][l][m][n]->Add(hh2);
	  }
	}
      }
    }
  }


  vector<TH2D*> h_MC[vars.size()][taun.size()];
  vector<TH2D*> h_data[vars.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int n=0; n<taun.size(); ++n) {
      for (unsigned int l=0; l<dms.size(); ++l) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  TString name_in = vars[k]+"_"+dms[l]+"_"+eta[m];
	  if (n==n_real) h_data[k].push_back( (TH2D*) h[0][k][l][m][n_real]->Clone("data_"+name_in) );
	  h_MC[k][n].push_back( (TH2D*) h[1][k][l][m][n]->Clone("MC_"+name_in+"_"+taun[n]) );
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
  vector<TH1D*> hpass_taupt_data[half_var];
  vector<TH1D*> hfail_taupt_data[half_var];
  vector<TH1D*> hpass_ratio_data[half_var];
  vector<TH1D*> hfail_ratio_data[half_var];

  TH1D* hpass_taupt_data_eta[half_var][eta.size()];
  TH1D* hfail_taupt_data_eta[half_var][eta.size()];
  TH1D* hpass_ratio_data_eta[half_var][eta.size()];
  TH1D* hfail_ratio_data_eta[half_var][eta.size()];
  double integral_pass_eta[half_var][eta.size()];
  double integral_fail_eta[half_var][eta.size()];

  TH1D* hpass_taupt_data_total[half_var];
  TH1D* hfail_taupt_data_total[half_var];
  TH1D* hpass_ratio_data_total[half_var];
  TH1D* hfail_ratio_data_total[half_var];
  double integral_pass_total[half_var];
  double integral_fail_total[half_var];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_data[k].size(); ++i) {
      int l = i/eta.size(), m = i % eta.size();

      vector<float> xxpoints, yypoints;
      xxpoints.clear(), yypoints.clear();
      xxpoints = xpoints_taupt, yypoints = ypoints_ratio;

      int array_size_x = xxpoints.size();
      float rebin_array_x[array_size_x];
      for (unsigned int iVector=0; iVector<array_size_x; ++iVector) rebin_array_x[iVector] = xxpoints[iVector];
      int array_size_y = yypoints.size();
      float rebin_array_y[array_size_y];
      for (unsigned int iVector=0; iVector<array_size_y; ++iVector) rebin_array_y[iVector] = yypoints[iVector];

      int half_k = k/2;
        
      //even->pass, odd->fail
      if (k%2 == 0) {
	TString name_in = "FakeRateByTauPt_"+dms[l]+"_"+eta[m];
	hpass_taupt_data[half_k].push_back( new TH1D(name_in, name_in, array_size_x-1, rebin_array_x) );

	cout << name_in << endl;

	name_in = "RatioCorrectionFactor_"+dms[l]+"_"+eta[m];
	hpass_ratio_data[half_k].push_back( new TH1D(name_in, name_in, array_size_y-1, rebin_array_y) );
      }
      else {
	TString name_in = "den2_"+dms[l]+"_"+eta[m];
	hfail_taupt_data[half_k].push_back( new TH1D(name_in, name_in, array_size_x-1, rebin_array_x) );

	name_in = "den3_"+dms[l]+"_"+eta[m];
	hfail_ratio_data[half_k].push_back( new TH1D(name_in, name_in, array_size_y-1, rebin_array_y) );
      }
        

      //integrate out the Y part
      int jBinX = 1, binStartX = 1;
      int nBinsY = h_data[k][i]->GetNbinsY();
      while (jBinX <= array_size_x) { 
	unsigned int iBinX=binStartX;
	float bin_content = 0, bin_error=0;
	while (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) < rebin_array_x[jBinX]) {
	  if (h_data[k][i]->GetXaxis()->GetBinCenter(iBinX) > rebin_array_x[jBinX-1]) {
	    double error_temp_data, error_temp_MC;
	    float integral_data = h_data[k][i]->IntegralAndError(iBinX, iBinX, 1, nBinsY, error_temp_data);
	    float integral_MC = h_MC[k][n_real][i]->IntegralAndError(iBinX, iBinX, 1, nBinsY, error_temp_MC);
	    bin_content += integral_data - integral_MC;
	    bin_error += pow(error_temp_data, 2) + pow(error_temp_MC, 2);
	  }
	  ++iBinX;
	}
	  
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_taupt_data[half_k][i]->SetBinContent(jBinX, bin_content);
	  hpass_taupt_data[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}
	else {
	  hfail_taupt_data[half_k][i]->SetBinContent(jBinX, bin_content);
	  hfail_taupt_data[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}	    
	  
	++jBinX;
	binStartX = iBinX;
      }


      //same for the X axis
      int jBinY = 1, binStartY = 1;
      int nBinsX = h_data[k][i]->GetNbinsX();
      while (jBinY <= array_size_y) { 
	unsigned int iBinY=binStartY;
	float bin_content = 0, bin_error=0;
	while (h_data[k][i]->GetYaxis()->GetBinCenter(iBinY) < rebin_array_y[jBinY]) {
	  if (h_data[k][i]->GetYaxis()->GetBinCenter(iBinY) > rebin_array_y[jBinY-1]) {
	    double error_temp_data, error_temp_MC;
	    float integral_data = h_data[k][i]->IntegralAndError(startbinX+1, endbinX, iBinY, iBinY, error_temp_data);
	    float integral_MC = h_MC[k][n_real][i]->IntegralAndError(startbinX+1, endbinX, iBinY, iBinY, error_temp_MC);
	    bin_content += integral_data - integral_MC;
	    bin_error += pow(error_temp_data, 2) + pow(error_temp_MC, 2);
	  }
	  ++iBinY;
	}
	  
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_ratio_data[half_k][i]->SetBinContent(jBinY, bin_content);
	  hpass_ratio_data[half_k][i]->SetBinError(jBinY, sqrt(bin_error));
	}
	else {
	  hfail_ratio_data[half_k][i]->SetBinContent(jBinY, bin_content);
	  hfail_ratio_data[half_k][i]->SetBinError(jBinY, sqrt(bin_error));
	}	    
	  
	++jBinY;
	binStartY = iBinY;
      }
        
        
      //if (k%2 != 0) hfail_taupt_data[half_k][i]->Add(hpass_taupt_data[half_k][i]);
      //remove this, was causing issue with the 

      if (i==0) {
	if (k%2 != 0) hfail_taupt_data_total[half_k] = (TH1D*) hfail_taupt_data[half_k][i]->Clone("den2_total");
	if (k%2 == 0) hpass_taupt_data_total[half_k] = (TH1D*) hpass_taupt_data[half_k][i]->Clone("FakeRateByTauPt_total");
	if (k%2 != 0) hfail_ratio_data_total[half_k] = (TH1D*) hfail_ratio_data[half_k][i]->Clone("den3_total");
	if (k%2 == 0) hpass_ratio_data_total[half_k] = (TH1D*) hpass_ratio_data[half_k][i]->Clone("RatioCorrectionFactor_total");
	if (k%2 != 0) integral_fail_total[half_k] = h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
	if (k%2 == 0) integral_pass_total[half_k] = h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
      }
      else {
	if (k%2 != 0) hfail_taupt_data_total[half_k]->Add(hfail_taupt_data[half_k][i]);
	if (k%2 == 0) hpass_taupt_data_total[half_k]->Add(hpass_taupt_data[half_k][i]);
	if (k%2 != 0) hfail_ratio_data_total[half_k]->Add(hfail_ratio_data[half_k][i]);
	if (k%2 == 0) hpass_ratio_data_total[half_k]->Add(hpass_ratio_data[half_k][i]);
	if (k%2 != 0) integral_fail_total[half_k] += h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
	if (k%2 == 0) integral_pass_total[half_k] += h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
      }

      if (l==0) {
	if (k%2 != 0) hfail_taupt_data_eta[half_k][m] = (TH1D*) hfail_taupt_data[half_k][i]->Clone("den2_eta_"+eta[m]);
	if (k%2 == 0) hpass_taupt_data_eta[half_k][m] = (TH1D*) hpass_taupt_data[half_k][i]->Clone("FakeRateByTauPt_eta_"+eta[m]);
	if (k%2 != 0) hfail_ratio_data_eta[half_k][m] = (TH1D*) hfail_ratio_data[half_k][i]->Clone("den2_eta_"+eta[m]);
	if (k%2 == 0) hpass_ratio_data_eta[half_k][m] = (TH1D*) hpass_ratio_data[half_k][i]->Clone("RatioCorrectionFactor_eta_"+eta[m]);
	if (k%2 != 0) integral_fail_eta[half_k][m] = h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
	if (k%2 == 0) integral_pass_eta[half_k][m] = h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
      }
      else {
	if (k%2 != 0) hfail_taupt_data_eta[half_k][m]->Add(hfail_taupt_data[half_k][i]);
	if (k%2 == 0) hpass_taupt_data_eta[half_k][m]->Add(hpass_taupt_data[half_k][i]);
	if (k%2 != 0) hfail_ratio_data_eta[half_k][m]->Add(hfail_ratio_data[half_k][i]);
	if (k%2 == 0) hpass_ratio_data_eta[half_k][m]->Add(hpass_ratio_data[half_k][i]);
	if (k%2 != 0) integral_fail_eta[half_k][m] += h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
	if (k%2 == 0) integral_pass_eta[half_k][m] += h_data[k][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[k][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY);
      }
      
      if (k%2 != 0) hpass_taupt_data[half_k][i]->Divide(hfail_taupt_data[half_k][i]);
      if (k%2 != 0) {
	double av_fr = (h_data[0][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[0][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY))/(h_data[1][i]->Integral(startbinX+1, endbinX, 1, nBinsY) - h_MC[1][n_real][i]->Integral(startbinX+1, endbinX, 1, nBinsY));
	cout << hpass_ratio_data[half_k][i]->GetName() << " " << av_fr << endl;
	hpass_ratio_data[half_k][i]->Divide(hfail_ratio_data[half_k][i]);
	hpass_ratio_data[half_k][i]->Scale(1./av_fr);
      }
    }
  }
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    float iintegral = hpass_ratio_data_total[half_k]->Integral(1,2)/hfail_ratio_data_total[half_k]->Integral(1,2);

    hpass_taupt_data_total[half_k]->Divide(hfail_taupt_data_total[half_k]);
    hpass_ratio_data_total[half_k]->Divide(hfail_ratio_data_total[half_k]);
    double av_fr = integral_pass_total[half_k]/integral_fail_total[half_k];
    cout << hpass_ratio_data_total[half_k]->GetName() << " " << av_fr << endl;
    hpass_ratio_data_total[half_k]->Scale(1./av_fr);
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_taupt_data_eta[half_k][m]->Divide(hfail_taupt_data_eta[half_k][m]);
      hpass_ratio_data_eta[half_k][m]->Divide(hfail_ratio_data_eta[half_k][m]);
      av_fr = integral_pass_eta[half_k][m]/integral_fail_eta[half_k][m];
      cout << hpass_ratio_data_eta[half_k][m]->GetName() << " " << av_fr << endl;
      hpass_ratio_data_eta[half_k][m]->Scale(1./av_fr);
    }
  }



  file_out->cd();
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    hpass_taupt_data_total[half_k]->Write();
    hpass_ratio_data_total[half_k]->Write();
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_taupt_data_eta[half_k][m]->Write();
      hpass_ratio_data_eta[half_k][m]->Write();
    }
    for (unsigned int i=0; i<hpass_taupt_data[half_k].size(); ++i) {
      hpass_taupt_data[half_k][i]->Write();
      hpass_ratio_data[half_k][i]->Write();
    }
  }
  file_out->Close();



  return 0;
}
