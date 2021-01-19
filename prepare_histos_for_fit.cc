#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TDirectory.h>
#include "TString.h"
#include <iostream>
#include <vector>
#include "aux.h"
#include "merge_aux.h"

using namespace std;

int main(int argc, char** argv) {
  //float firstbin = 50;
  //float lastbin = 150;
  //int Nbins = 10;
  int rebin = 1;

  bool bCustom = false;
  string binning = *(argv + 1);
  if (binning == "10pc") bCustom = true;
  else if (binning == "10GeV") bCustom = false;
  else cout << "binning: " << binning << endl;

  TFile* file_in_mutau = new TFile("Figures/histos_highmassmutau_CR100.root", "R");
  TFile* file_in_signal = new TFile("Figures/histos_signal.root", "R");


  vector<float> xpoints {50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 725.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1550.0, 1700.0, 1850.0, 2000.0, 2200.0, 2400.0, 2700.0, 3000.0, 3300.0, 3600.0, 4000.0, 4400.0, 4800.0, 5300.0, 5800.0, 6400.0, 7000.0, 8000.0, 9000., 10000., 11000., 12000., 13000.};

  vector<TString> in_names,               out_names;
  in_names.push_back("faketau_");	  out_names.push_back("faketau");
  in_names.push_back("DY_");  		  out_names.push_back("DY");  
  in_names.push_back("VV_");   		  out_names.push_back("VV");   
  in_names.push_back("TT_");   		  out_names.push_back("TT");   
  in_names.push_back("ST_");   		  out_names.push_back("ST");   
  in_names.push_back("data_"); 		  out_names.push_back("data_obs");


  vector<TString> in_sys,                out_sys;
  in_sys.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    if (systs_aux[iAux] == "topPt") continue;
    in_sys.push_back(systs_aux[iAux]+"_up");
    in_sys.push_back(systs_aux[iAux]+"_down");
  }

  vector<TString> systs_signalveto = GetSignalSysVeto();
  
  out_sys = in_sys;

  vector<TString> signal;  
  signal.push_back("ZPrime_500"); 
  signal.push_back("ZPrime_600"); 
  signal.push_back("ZPrime_700"); 
  signal.push_back("ZPrime_800"); 
  signal.push_back("ZPrime_900"); 
  signal.push_back("ZPrime_1000");
  signal.push_back("ZPrime_1100");
  signal.push_back("ZPrime_1200");
  signal.push_back("ZPrime_1300");
  signal.push_back("ZPrime_1400");
  signal.push_back("ZPrime_1500");
  signal.push_back("ZPrime_1600");
  signal.push_back("ZPrime_1700");
  signal.push_back("ZPrime_1800");
  signal.push_back("ZPrime_1900");
  signal.push_back("ZPrime_2000");
  signal.push_back("ZPrime_2200");
  signal.push_back("ZPrime_2400");
  signal.push_back("ZPrime_2600");
  signal.push_back("ZPrime_2800");
  signal.push_back("ZPrime_3000");
  signal.push_back("ZPrime_3500");
  signal.push_back("ZPrime_4000");
  signal.push_back("ZPrime_4500");
  signal.push_back("ZPrime_5000");
  signal.push_back("ZPrime_5500");
  signal.push_back("ZPrime_6000");
  
  signal.push_back("QBH_200");         
  signal.push_back("QBH_400");         
  signal.push_back("QBH_600");         
  signal.push_back("QBH_800");         
  signal.push_back("QBH_1000");        
  signal.push_back("QBH_1200");        
  signal.push_back("QBH_1400");        
  signal.push_back("QBH_1600");        
  signal.push_back("QBH_1800");        
  signal.push_back("QBH_2000");        
  signal.push_back("QBH_2500");        
  signal.push_back("QBH_3000");        
  signal.push_back("QBH_3500");        
  signal.push_back("QBH_4000");        
  signal.push_back("QBH_4500");        
  signal.push_back("QBH_5000");        
  signal.push_back("QBH_5500");        
  signal.push_back("QBH_6000");        
  signal.push_back("QBH_7000");        
  signal.push_back("QBH_9000");        
  signal.push_back("QBH_10000");       

  signal.push_back("RPV_l001_200");    
  signal.push_back("RPV_l001_300");    
  signal.push_back("RPV_l001_400");    
  signal.push_back("RPV_l001_500");    
  signal.push_back("RPV_l001_600");    
  signal.push_back("RPV_l001_700");    
  signal.push_back("RPV_l001_800");    
  signal.push_back("RPV_l001_900");
  signal.push_back("RPV_l001_1000");   
  signal.push_back("RPV_l001_1200");   
  signal.push_back("RPV_l001_1400");   
  signal.push_back("RPV_l001_1600");   
  signal.push_back("RPV_l001_1800");   
  signal.push_back("RPV_l001_2000");   
  signal.push_back("RPV_l001_2500");   
  signal.push_back("RPV_l001_3000");   
  signal.push_back("RPV_l001_3500");   
  signal.push_back("RPV_l01_3500");    
  signal.push_back("RPV_l001_4000");   
  signal.push_back("RPV_l01_4000");    
  signal.push_back("RPV_l02_4000");    
  signal.push_back("RPV_l05_4000");  
  signal.push_back("RPV_l001_4500");   
  signal.push_back("RPV_l02_4500");  
  signal.push_back("RPV_l001_5000");   
  signal.push_back("RPV_l02_5000");  
  signal.push_back("RPV_l001_5500"); 
  signal.push_back("RPV_l02_5500");  
  signal.push_back("RPV_l02_6000");  
  signal.push_back("RPV_l05_6000");  
  signal.push_back("RPV_l001_6500"); 
  signal.push_back("RPV_l02_6500");  
  signal.push_back("RPV_l05_6500");  


  TString nnname_out;
  if (bCustom) nnname_out = "mutauHistos_forLimitSetting_2016.root";
  else nnname_out = "mutauHistos_forLimitSetting_2016_MI.root";

  TFile* file_out = new TFile(nnname_out, "RECREATE");
  
  file_out->cd();
  //TDirectory* signal_dir = file_out->mkdir("signalregion");
  //signal_dir->cd(); 
  for (unsigned int j=0; j<in_names.size(); ++j) {
    for (unsigned int k=0; k<in_sys.size(); ++k) {
      if (in_names[j].Contains("data") && k>0) continue;
      TString name_in = in_sys[k]+"/"+in_names[j]+in_sys[k]+"_ev_Mcol_MtHigh";
      TString name_out = out_names[j]+"_"+out_sys[k];

      
      TH1F* h = (TH1F*) file_in_mutau->Get(name_in);
      if (h == 0 && k>0) break; 

      TH1F* h_r;
      if (bCustom) h_r = adjust_binning(h, false, xpoints, 10, false);
      else h_r = adjust_binning(h, true, xpoints, 10, false);
      h_r->SetName(name_out);
      h_r->SetTitle(name_out);

      for (unsigned int l=1; l<h_r->GetNbinsX()+1; ++l) {
	float bin_content = h_r->GetBinContent(l);
	if (bin_content <= 0 && bCustom) {
	  h_r->SetBinContent(l, 0);
	}
      }
      h_r->Write();
      delete h;
      delete h_r;
    }
  }

  //include PDF syst
  in_sys.push_back("PDF_up");    out_sys.push_back("PDF_up");  
  in_sys.push_back("PDF_down");	 out_sys.push_back("PDF_down");
  for (unsigned int j=0; j<signal.size(); ++j) {
    for (unsigned int k=0; k<in_sys.size(); ++k) {
      bool bVeto = false;
      for (unsigned int iVeto=0; iVeto<systs_signalveto.size(); ++iVeto) {
	if (in_sys[k] == systs_signalveto[iVeto]+"_up" || in_sys[k] == systs_signalveto[iVeto]+"_down") {
	  bVeto = true;
	  break;
	}
      }
      if (bVeto) continue;

      TString name_in = signal[j]+"_ev_Mcol_MtHigh";
      TString name_out = signal[j]+"_"+out_sys[k];
      TH1F* h = (TH1F*) file_in_signal->Get(in_sys[k]+"/"+name_in);

      TH1F* h_r;
      if (bCustom) h_r = adjust_binning(h, false, xpoints, 10, false);
      else h_r = adjust_binning(h, true, xpoints, 10, false);
      h_r->SetName(name_out);
      h_r->SetTitle(name_out);

      for (unsigned int l=1; l<h_r->GetNbinsX()+1; ++l) {
	float bin_content = h_r->GetBinContent(l);
	if (bin_content <= 0 && bCustom) {
	  h_r->SetBinContent(l, 0);
	}
      }
      h_r->Write();
      delete h;
      delete h_r;
    }
  }
  //signal_dir->Close();
  file_out->Close();



  return 0;
}
