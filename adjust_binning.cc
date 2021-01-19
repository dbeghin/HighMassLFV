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
#include "aux.h"
#include "merge_aux.h"
#include "TDirectory.h"

using namespace std;
int main(/*int argc, char** argv*/) {
  TFile* file_out = new TFile("Figures/histos_highmassmutau.root", "RECREATE");
  TFile* file_in  = new TFile("Figures/histos_highmassmutau_CR100.root", "R");
  TFile* file_in_sig  = new TFile("Figures/histos_signal.root", "R");

  vector<TString> names;
  names.push_back("data_");//0
  names.push_back("faketau_");
  names.push_back("DY_");
  names.push_back("TT_");
  names.push_back("ST_");
  names.push_back("VV_");


  vector<TString> sig_names;
  //sig_names.push_back("RPV_l001_1000_");
  //sig_names.push_back("RPV_l001_4000_");
  //sig_names.push_back("ZPrime_1000_");
  //sig_names.push_back("ZPrime_2000_");
  //sig_names.push_back("ZPrime_3000_");
  //sig_names.push_back("ZPrime_4000_");


  vector<TString> systs;
  systs.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    if (systs_aux[iAux] == "topPt") continue;
    systs.push_back(systs_aux[iAux]+"_up");
    systs.push_back(systs_aux[iAux]+"_down");
  }


  //rebin vectors
  //vector<float> xpoints {/*0, 10, 20, 30, 40, */50, 60, 70, 80, 90, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 450, 500, 600, 800, 1000, 1250, 1500, 2000, 2500, 3000, 4000, 6000, 8000};
  vector<float> xpoints {50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 725.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1550.0, 1700.0, 1850.0, 2000.0, 2200.0, 2400.0, 2700.0, 3000.0, 3300.0, 3600.0, 4000.0, 4400.0, 4800.0, 5300.0, 5800.0, 6400.0, 7000.0, 8000.0};
  cout << xpoints.size() << endl;

  vector<float> xpoints_MET {/*0, */10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 150, 200, 300, 500, 1000, 2000};

  vector<float> xpoints_pt {/*0, 10, 20, */30, 50, 60, 70, 80, 100, 150, 200, 300, 500, 1000, 2000};
  //vector<float> xpoints_pt {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 150, 300, 1000};

  //vector<float> xpoints_Mt {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120}; //for Mt low FIXME
  vector<float> xpoints_Mt = xpoints; //for Mt high

  //float x[] = {0, 50, 60, 70, 80, 90, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 450, 500, 550, 600, 650, 700, 800, 900, 1000, 1500, 2000, 4000};


  vector<TString> vars;                     vector<bool> simpleRebin;         vector<int> rebin;      vector<vector<float>> rebin_vector;
  vars.push_back("ev_Mvis");                simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints);
  vars.push_back("ev_Mtot");		    simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints);
  vars.push_back("tau_pt");		    simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints_pt);
  vars.push_back("tau_eta");		    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  vars.push_back("tau_phi");		    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  vars.push_back("mu_pt");		    simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints_pt);
  vars.push_back("mu_eta");		    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  vars.push_back("mu_phi");		    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  vars.push_back("mu_isolation");	    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  vars.push_back("ev_DRmutau");		    simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);
  //vars.push_back("ev_deltaMET");       	    simpleRebin.push_back(true);      rebin.push_back(10);    rebin_vector.push_back(xpoints);
  vars.push_back("ev_MET");		    simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints_MET);
  vars.push_back("ev_Mcol");                simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints);                
  vars.push_back("ev_Mt");                  simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints_Mt);                
  vars.push_back("sign");                   simpleRebin.push_back(true);      rebin.push_back(1);     rebin_vector.push_back(xpoints);                
  vars.push_back("ev_Nvertex");             simpleRebin.push_back(true);      rebin.push_back(5);     rebin_vector.push_back(xpoints);                
  int n_signal = vars.size(); //signal plots don't go beyond here                                                                                                                        
  vars.push_back("njet");                   simpleRebin.push_back(true);      rebin.push_back(1);     rebin_vector.push_back(xpoints);
  vars.push_back("nbjet");                  simpleRebin.push_back(true);      rebin.push_back(1);     rebin_vector.push_back(xpoints);
  //exclusively for TT region now                                                                                                                                                        
  int n_TT_plots = vars.size();
  vars.push_back("bjet_pt");                simpleRebin.push_back(false);     rebin.push_back(1);     rebin_vector.push_back(xpoints_pt);
  vars.push_back("bjet_eta");               simpleRebin.push_back(true);      rebin.push_back(1);     rebin_vector.push_back(xpoints);
  vars.push_back("bjet_phi");               simpleRebin.push_back(true);      rebin.push_back(2);     rebin_vector.push_back(xpoints);

  vector<TString> Mth;
  Mth.push_back("_MtHigh");
  Mth.push_back("_MtLow_OS");
  Mth.push_back("_MtLow_SS");
  Mth.push_back("_MtLow_TT");  int k_low_TT = Mth.size()-1;
  Mth.push_back("_MtHigh_TT");


  //retrieve all histos and rebin them
  vector<TH1F*> h[systs.size()][names.size()][Mth.size()];
  vector<TH1F*> h_rebinned[systs.size()][names.size()][Mth.size()];
  float PoissonUpperBoundAtZero = 1.8;
  for (unsigned int i=0; i<systs.size(); ++i) {
    for (unsigned int j=0; j<names.size(); ++j) {
      for (unsigned int l=0; l<Mth.size(); ++l) {
	for (unsigned int k=0; k<vars.size(); ++k) { 

          if (l < k_low_TT && k >= n_TT_plots) continue;

    	  h[i][j][l].push_back( (TH1F*) file_in->Get(systs[i]+"/"+names[j]+systs[i]+"_"+vars[k]+Mth[l]) );
    	  h[i][j][l][k]->SetName(names[j]+systs[i]+"_"+vars[k]+Mth[l]+"_old");
	  
	  h_rebinned[i][j][l].push_back( adjust_binning(h[i][j][l][k], simpleRebin[k], rebin_vector[k], rebin[k], true));
	  h_rebinned[i][j][l][k]->SetTitle(names[j]+systs[i]+"_"+vars[k]+Mth[l]);
	  h_rebinned[i][j][l][k]->SetName(names[j]+systs[i]+"_"+vars[k]+Mth[l]);
        }
      }
    }
  }

  cout << "a" << endl;

  vector<TH1F*> h_sig[sig_names.size()][n_signal];
  vector<TH1F*> h_sig_rebinned[sig_names.size()][n_signal];
  for (unsigned int k=0; k<n_signal; ++k) {
    for (unsigned int l=0; l<Mth.size(); ++l) {
      for (unsigned int j=0; j<sig_names.size(); ++j) {
	if (l!=0) continue;
	cout << "nominal/"+sig_names[j]+vars[k]+Mth[l] << endl;
        h_sig[j][k].push_back( (TH1F*) file_in_sig->Get("nominal/"+sig_names[j]+vars[k]+Mth[l]) );
        //cout << sig_names[j]+"_"+vars[k]+Mth[l] << endl;                             
	cout << h_sig[j][k][l] << endl;
        h_sig[j][k][l]->SetName(sig_names[j]+vars[k]+Mth[l]+"_old");
	cout << "a3" << endl;

	h_sig_rebinned[j][k].push_back( adjust_binning(h_sig[j][k][l], simpleRebin[k], rebin_vector[k], rebin[k], true));
	cout << "a4" << endl;
	h_sig_rebinned[j][k][l]->SetTitle(sig_names[j]+vars[k]+Mth[l]);
	h_sig_rebinned[j][k][l]->SetName(sig_names[j]+vars[k]+Mth[l]);
      }
    }
  }

  cout << "b" << endl;

  //Write the rebinned histos in the output file
  file_out->cd();
  for (unsigned int i=0; i<systs.size(); ++i) {
    TDirectory* dir = file_out->mkdir( systs[i] );
    dir->cd();
    for (unsigned int j=0; j<names.size(); ++j) {
      for (unsigned int k=0; k<vars.size(); ++k) {
	for (unsigned int l=0; l<Mth.size(); ++l) {
          if (l < k_low_TT && k >= n_TT_plots) continue;
	  h_rebinned[i][j][l][k]->Write();
	}
      }
    }
    dir->Close();
  }
  cout << "c" << endl;
  TDirectory* dir = file_out->mkdir( "signal" );
  dir->cd();
  for (unsigned int j=0; j<sig_names.size(); ++j) {
    for (unsigned int k=0; k<n_signal; ++k) {
      for (unsigned int l=0; l<Mth.size(); ++l) {
	if (l!=0) continue;
	h_sig_rebinned[j][k][l]->Write();
      }
    }
  }
  dir->Close();
  file_out->Close();
  cout << "d" << endl;



  return 0;
}
