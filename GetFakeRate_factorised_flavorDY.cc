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



int main(/*int argc, char** argv*/) {
  TFile* file_out = new TFile("HighMassLFVMuTau/fakerate_factorised_flavorDY.root", "RECREATE");
  TFile* file_in  = new TFile("Figures/histos_fakerate_DY.root", "R");

  vector<TString> names;
  names.push_back("data_");

  vector<TString> vars;
  vars.push_back("taupt_ratio_pass");
  vars.push_back("taupt_ratio_fail");

  vector<TString> flavor;
  flavor.push_back("quark");
  flavor.push_back("gluon");

  vector<TString> DM;
  DM.push_back("DM0");
  DM.push_back("DM1");
  DM.push_back("DM10");
  DM.push_back("DM11");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<float> xpoints_taupt {0, 30, 50, 70, 100, 150, 500};
  vector<float> ypoints_ratio {0, 0.3, 0.5, 0.6, 0.7, 0.75, 0.8, 0.85, 1., 2.};
  //vector<float> ypoints_ratio {0, 0.7, 2.};

  vector<TH2D*> h[names.size()][vars.size()][flavor.size()][DM.size()];
  for (unsigned int j=0; j<names.size(); ++j) {
    for (unsigned int k=0; k<vars.size(); ++k) { 
      for (unsigned int l=0; l<flavor.size(); ++l) {
	for (unsigned int dd=0; dd<DM.size(); ++dd) {
	  for (unsigned int m=0; m<eta.size(); ++m) {
	    cout << names[j]+vars[k]+"_"+DM[dd]+"_"+flavor[l]+"_"+eta[m] << endl;
	    TString name_in = names[j]+vars[k]+"_"+DM[dd]+"_"+flavor[l]+"_"+eta[m];
	    h[j][k][l][dd].push_back( (TH2D*) file_in->Get(name_in) );
	    h[j][k][l][dd][m]->SetName(names[j]+vars[k]+"_"+DM[dd]+"_"+eta[m]+"_"+flavor[l]);
	  }
	}
      }
    }
  }


  vector<TH2D*> h_MC[vars.size()];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int l=0; l<flavor.size(); ++l) {
      for (unsigned int dd=0; dd<DM.size(); ++dd) {
	for (unsigned int m=0; m<eta.size(); ++m) {
	  TString name_in = vars[k]+"_"+DM[dd]+"_"+eta[m]+"_"+flavor[l];
	  h_MC[k].push_back( (TH2D*) h[0][k][l][dd][m]->Clone("MC_"+name_in) );
	}
      }
    }
  }



  //data
  int half_var = vars.size()/2;//even->pass, odd->fail
  vector<TH1D*> hpass_taupt_MC[half_var];
  vector<TH1D*> hfail_taupt_MC[half_var];
  vector<TH1D*> hpass_ratio_MC[half_var];
  vector<TH1D*> hfail_ratio_MC[half_var];

  TH1D* hpass_taupt_MC_eta[half_var][eta.size()];
  TH1D* hfail_taupt_MC_eta[half_var][eta.size()];
  TH1D* hpass_ratio_MC_eta[half_var][eta.size()];
  TH1D* hfail_ratio_MC_eta[half_var][eta.size()];
  double integral_pass_eta[half_var][eta.size()];
  double integral_fail_eta[half_var][eta.size()];

  TH1D* hpass_taupt_MC_DMeta[half_var][DM.size()][eta.size()];
  TH1D* hfail_taupt_MC_DMeta[half_var][DM.size()][eta.size()];
  TH1D* hpass_ratio_MC_DMeta[half_var][DM.size()][eta.size()];
  TH1D* hfail_ratio_MC_DMeta[half_var][DM.size()][eta.size()];
  double integral_pass_DMeta[half_var][DM.size()][eta.size()];
  double integral_fail_DMeta[half_var][DM.size()][eta.size()];

  TH1D* hpass_taupt_MC_total[half_var];
  TH1D* hfail_taupt_MC_total[half_var];
  TH1D* hpass_ratio_MC_total[half_var];
  TH1D* hfail_ratio_MC_total[half_var];
  double integral_pass_total[half_var];
  double integral_fail_total[half_var];
  for (unsigned int k=0; k<vars.size(); ++k) {
    for (unsigned int i=0; i<h_MC[k].size(); ++i) {
      int l = i/(DM.size()*eta.size()), dd = (i % (DM.size()*eta.size()))/eta.size(), m = i % eta.size();
      cout << l << dd << m << endl;

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
	TString name_in = "FakeRateByTauPt_"+DM[dd]+"_"+eta[m]+"_"+flavor[l];
	hpass_taupt_MC[half_k].push_back( new TH1D(name_in, name_in, array_size_x-1, rebin_array_x) );

	cout << name_in << endl;

	name_in = "RatioCorrectionFactor_"+DM[dd]+"_"+eta[m]+"_"+flavor[l];
	hpass_ratio_MC[half_k].push_back( new TH1D(name_in, name_in, array_size_y-1, rebin_array_y) );
      }
      else {
	TString name_in = "den2_"+DM[dd]+"_"+eta[m]+"_"+flavor[l];
	hfail_taupt_MC[half_k].push_back( new TH1D(name_in, name_in, array_size_x-1, rebin_array_x) );

	name_in = "den3_"+DM[dd]+"_"+eta[m]+"_"+flavor[l];
	hfail_ratio_MC[half_k].push_back( new TH1D(name_in, name_in, array_size_y-1, rebin_array_y) );
      }
        

      //integrate out the Y part
      int jBinX = 1, binStartX = 1;
      int nBinsY = h_MC[k][i]->GetNbinsY();
      while (jBinX <= array_size_x) { 
	unsigned int iBinX=binStartX;
	float bin_content = 0, bin_error=0;
	while (h_MC[k][i]->GetXaxis()->GetBinCenter(iBinX) < rebin_array_x[jBinX]) {
	  if (h_MC[k][i]->GetXaxis()->GetBinCenter(iBinX) > rebin_array_x[jBinX-1]) {
	    double error_temp_MC;
	    float integral_MC = h_MC[k][i]->IntegralAndError(iBinX, iBinX, 1, nBinsY, error_temp_MC, "");
	    bin_content += integral_MC;
	    bin_error += pow(error_temp_MC, 2);
	  }
	  ++iBinX;
	}
	  
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_taupt_MC[half_k][i]->SetBinContent(jBinX, bin_content);
	  hpass_taupt_MC[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}
	else {
	  hfail_taupt_MC[half_k][i]->SetBinContent(jBinX, bin_content);
	  hfail_taupt_MC[half_k][i]->SetBinError(jBinX, sqrt(bin_error));
	}	    
	  
	++jBinX;
	binStartX = iBinX;
      }


      //same for the X axis
      int jBinY = 1, binStartY = 1;
      int nBinsX = h_MC[k][i]->GetNbinsX();
      while (jBinY <= array_size_y) { 
	unsigned int iBinY=binStartY;
	float bin_content = 0, bin_error=0;
	while (h_MC[k][i]->GetYaxis()->GetBinCenter(iBinY) < rebin_array_y[jBinY]) {
	  if (h_MC[k][i]->GetYaxis()->GetBinCenter(iBinY) > rebin_array_y[jBinY-1]) {
	    double error_temp_MC;
	    float integral_MC = h_MC[k][i]->IntegralAndError(1, nBinsX, iBinY, iBinY, error_temp_MC);
	    bin_content += integral_MC;
	    bin_error += pow(error_temp_MC, 2);
	  }
	  ++iBinY;
	}
	  
	if (bin_content < 0) bin_content = 0;
	if (k%2 == 0) {
	  hpass_ratio_MC[half_k][i]->SetBinContent(jBinY, bin_content);
	  hpass_ratio_MC[half_k][i]->SetBinError(jBinY, sqrt(bin_error));
	}
	else {
	  hfail_ratio_MC[half_k][i]->SetBinContent(jBinY, bin_content);
	  hfail_ratio_MC[half_k][i]->SetBinError(jBinY, sqrt(bin_error));
	}	    
	  
	++jBinY;
	binStartY = iBinY;
      }
        
        
      //if (k%2 != 0) hfail_taupt_MC[half_k][i]->Add(hpass_taupt_MC[half_k][i]);
      //remove this, was causing issue with the 

      if (i==0) {
	if (k%2 != 0) hfail_taupt_MC_total[half_k] = (TH1D*) hfail_taupt_MC[half_k][i]->Clone("den2_total");
	if (k%2 == 0) hpass_taupt_MC_total[half_k] = (TH1D*) hpass_taupt_MC[half_k][i]->Clone("FakeRateByTauPt_total_nominal");
	if (k%2 != 0) hfail_ratio_MC_total[half_k] = (TH1D*) hfail_ratio_MC[half_k][i]->Clone("den3_total");
	if (k%2 == 0) hpass_ratio_MC_total[half_k] = (TH1D*) hpass_ratio_MC[half_k][i]->Clone("RatioCorrectionFactor_total_nominal");
	if (k%2 != 0) integral_fail_total[half_k] = hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_total[half_k] = hpass_taupt_MC[half_k][i]->Integral();
      }
      else {
	if (k%2 != 0) hfail_taupt_MC_total[half_k]->Add(hfail_taupt_MC[half_k][i]);
	if (k%2 == 0) hpass_taupt_MC_total[half_k]->Add(hpass_taupt_MC[half_k][i]);
	if (k%2 != 0) hfail_ratio_MC_total[half_k]->Add(hfail_ratio_MC[half_k][i]);
	if (k%2 == 0) hpass_ratio_MC_total[half_k]->Add(hpass_ratio_MC[half_k][i]);
	if (k%2 != 0) integral_fail_total[half_k] += hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_total[half_k] += hpass_taupt_MC[half_k][i]->Integral();
      }

      if (l==0 && dd == 0) {
	if (k%2 != 0) hfail_taupt_MC_eta[half_k][m] = (TH1D*) hfail_taupt_MC[half_k][i]->Clone("den2_eta_"+eta[m]);
	if (k%2 == 0) hpass_taupt_MC_eta[half_k][m] = (TH1D*) hpass_taupt_MC[half_k][i]->Clone("FakeRateByTauPt_nominal_"+eta[m]);
	if (k%2 != 0) hfail_ratio_MC_eta[half_k][m] = (TH1D*) hfail_ratio_MC[half_k][i]->Clone("den3_eta_"+eta[m]);
	if (k%2 == 0) hpass_ratio_MC_eta[half_k][m] = (TH1D*) hpass_ratio_MC[half_k][i]->Clone("RatioCorrectionFactor_nominal_"+eta[m]);
	if (k%2 != 0) integral_fail_eta[half_k][m] = hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_eta[half_k][m] = hpass_taupt_MC[half_k][i]->Integral();
      }
      else {
	if (k%2 != 0) hfail_taupt_MC_eta[half_k][m]->Add(hfail_taupt_MC[half_k][i]);
	if (k%2 == 0) hpass_taupt_MC_eta[half_k][m]->Add(hpass_taupt_MC[half_k][i]);
	if (k%2 != 0) hfail_ratio_MC_eta[half_k][m]->Add(hfail_ratio_MC[half_k][i]);
	if (k%2 == 0) hpass_ratio_MC_eta[half_k][m]->Add(hpass_ratio_MC[half_k][i]);
	if (k%2 != 0) integral_fail_eta[half_k][m] += hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_eta[half_k][m] += hpass_taupt_MC[half_k][i]->Integral();
      }

      if (l==0) {
	if (k%2 != 0) hfail_taupt_MC_DMeta[half_k][dd][m] = (TH1D*) hfail_taupt_MC[half_k][i]->Clone("den2_"+DM[dd]+"_"+eta[m]);
	if (k%2 == 0) hpass_taupt_MC_DMeta[half_k][dd][m] = (TH1D*) hpass_taupt_MC[half_k][i]->Clone("FakeRateByTauPt_"+DM[dd]+"_"+eta[m]);
	if (k%2 != 0) hfail_ratio_MC_DMeta[half_k][dd][m] = (TH1D*) hfail_ratio_MC[half_k][i]->Clone("den3_"+DM[dd]+"_"+eta[m]);
	if (k%2 == 0) hpass_ratio_MC_DMeta[half_k][dd][m] = (TH1D*) hpass_ratio_MC[half_k][i]->Clone("RatioCorrectionFactor_"+DM[dd]+"_"+eta[m]);
	if (k%2 != 0) integral_fail_DMeta[half_k][dd][m] = hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_DMeta[half_k][dd][m] = hpass_taupt_MC[half_k][i]->Integral();
      }
      else {
	if (k%2 != 0) hfail_taupt_MC_DMeta[half_k][dd][m]->Add(hfail_taupt_MC[half_k][i]);
	if (k%2 == 0) hpass_taupt_MC_DMeta[half_k][dd][m]->Add(hpass_taupt_MC[half_k][i]);
	if (k%2 != 0) hfail_ratio_MC_DMeta[half_k][dd][m]->Add(hfail_ratio_MC[half_k][i]);
	if (k%2 == 0) hpass_ratio_MC_DMeta[half_k][dd][m]->Add(hpass_ratio_MC[half_k][i]);
	if (k%2 != 0) integral_fail_DMeta[half_k][dd][m] += hfail_taupt_MC[half_k][i]->Integral();
	if (k%2 == 0) integral_pass_DMeta[half_k][dd][m] += hpass_taupt_MC[half_k][i]->Integral();
      }


      if (k%2 != 0) hpass_taupt_MC[half_k][i]->Divide(hfail_taupt_MC[half_k][i]);
      if (k%2 != 0) {
	double av_fr = hpass_ratio_MC[half_k][i]->Integral()/hfail_ratio_MC[half_k][i]->Integral();
	hpass_ratio_MC[half_k][i]->Divide(hfail_ratio_MC[half_k][i]);
	hpass_ratio_MC[half_k][i]->Scale(1./av_fr);
      }
    }
  }
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    hpass_taupt_MC_total[half_k]->Divide(hfail_taupt_MC_total[half_k]);
    hpass_ratio_MC_total[half_k]->Divide(hfail_ratio_MC_total[half_k]);
    double av_fr = integral_pass_total[half_k]/integral_fail_total[half_k];
    cout << av_fr << endl;
    hpass_ratio_MC_total[half_k]->Scale(1./av_fr);
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_taupt_MC_eta[half_k][m]->Divide(hfail_taupt_MC_eta[half_k][m]);
      hpass_ratio_MC_eta[half_k][m]->Divide(hfail_ratio_MC_eta[half_k][m]);
      double av_fr = integral_pass_eta[half_k][m]/integral_fail_eta[half_k][m];
      cout << av_fr << endl;
      hpass_ratio_MC_eta[half_k][m]->Scale(1./av_fr);
    }
    for (unsigned int dd=0; dd<DM.size(); ++dd) {
      for (unsigned int m=0; m<eta.size(); ++m) {
	hpass_taupt_MC_DMeta[half_k][dd][m]->Divide(hfail_taupt_MC_DMeta[half_k][dd][m]);
	hpass_ratio_MC_DMeta[half_k][dd][m]->Divide(hfail_ratio_MC_DMeta[half_k][dd][m]);
	double av_fr = integral_pass_DMeta[half_k][dd][m]/integral_fail_DMeta[half_k][dd][m];
	cout << av_fr << endl;
	hpass_ratio_MC_DMeta[half_k][dd][m]->Scale(1./av_fr);
      }
    }
  }



  file_out->cd();
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    hpass_taupt_MC_total[half_k]->Write();
    hpass_ratio_MC_total[half_k]->Write();
    for (unsigned int m=0; m<eta.size(); ++m) {
      hpass_taupt_MC_eta[half_k][m]->Write();
      hpass_ratio_MC_eta[half_k][m]->Write();
    }
    for (unsigned int dd=0; dd<DM.size(); ++dd) {
      for (unsigned int m=0; m<eta.size(); ++m) {
	hpass_taupt_MC_DMeta[half_k][dd][m]->Write();
	hpass_ratio_MC_DMeta[half_k][dd][m]->Write();
      }
    }
  }
  for (unsigned int half_k=0; half_k<half_var; ++half_k) {
    for (unsigned int i=0; i<hpass_taupt_MC[half_k].size(); ++i) {
      hpass_taupt_MC[half_k][i]->Write();
      hpass_ratio_MC[half_k][i]->Write();
    }
  }
  file_out->Close();



  return 0;
}
