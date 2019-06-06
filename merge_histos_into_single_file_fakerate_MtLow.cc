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
#include "TDirectory.h"

using namespace std;


TH2D* MC_histo_TH2(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {
  cout << file_in->GetName() << endl;

  TH1F* h_events_data = (TH1F*) file_in_d->Get("weighted_events");
  double full_data = 7.86454e+08;
  double succ_data_ratio = h_events_data->Integral()/full_data;
  cout << "succesfull data ratio " << succ_data_ratio << endl;

  double lumi = 35.9 * pow(10,3) * succ_data_ratio; //luminosity in pb^-1

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();
  delete h_events;
  delete h_events_data;

  double e_Nevents = pow(Nevents,0.5);
  double e_xs = 0.01*xs;

  //Weight
  double w = 0;
  if (Nevents != 0) w = xs*lumi/Nevents;
  cout << "Events in data/events in MC " << w << endl;
  
  TH2D* h;
  h = (TH2D*) file_in -> Get(var);

  h -> Scale(w);
  h -> RebinX(rebin);
  h -> RebinY(rebin);
  
  return h;
}



int main(int argc, char** argv) {


  int rebin = 1;

  //TString folder_in = "HighMassLFVMuTau/FakeRate_SSMtLow";
  TString folder_in = "HighMassLFVMuTau/SignalRegion_CR100";
  TString name_out = "histos_fakerate_MtLow";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

  vector<TFile*> DY_files;
  TFile* file_in_DY_lowmass = new TFile(folder_in+"/Arranged_DY/DY_inclusive.root", "R");            DY_files.push_back(file_in_DY_lowmass);
  TFile* file_in_DY_400to500 = new TFile(folder_in+"/Arranged_DY/DY_400to500.root", "R");            DY_files.push_back(file_in_DY_400to500);
  TFile* file_in_DY_500to700 = new TFile(folder_in+"/Arranged_DY/DY_500to700.root", "R");	     DY_files.push_back(file_in_DY_500to700);
  TFile* file_in_DY_700to800 = new TFile(folder_in+"/Arranged_DY/DY_700to800.root", "R");	     DY_files.push_back(file_in_DY_700to800);
  TFile* file_in_DY_800to1000 = new TFile(folder_in+"/Arranged_DY/DY_800to1000.root", "R");	     DY_files.push_back(file_in_DY_800to1000);
  TFile* file_in_DY_1000to1500 = new TFile(folder_in+"/Arranged_DY/DY_1000to1500.root", "R");	     DY_files.push_back(file_in_DY_1000to1500);
  TFile* file_in_DY_1500to2000 = new TFile(folder_in+"/Arranged_DY/DY_1500to2000.root", "R");	     DY_files.push_back(file_in_DY_1500to2000);
  TFile* file_in_DY_2000to3000 = new TFile(folder_in+"/Arranged_DY/DY_2000to3000.root", "R");        DY_files.push_back(file_in_DY_2000to3000);

  vector<TFile*> TT_files;
  TFile* file_in_TT_lowmll = new TFile(folder_in+"/Arranged_TT/TT_inclusive.root", "R");             TT_files.push_back(file_in_TT_lowmll);
  TFile* file_in_TT_500to800 = new TFile(folder_in+"/Arranged_TT/TT_500to800.root", "R");	     TT_files.push_back(file_in_TT_500to800);
  TFile* file_in_TT_800to1200 = new TFile(folder_in+"/Arranged_TT/TT_800to1200.root", "R");	     TT_files.push_back(file_in_TT_800to1200);
  TFile* file_in_TT_1200to1800 = new TFile(folder_in+"/Arranged_TT/TT_1200to1800.root", "R");	     TT_files.push_back(file_in_TT_1200to1800);
  TFile* file_in_TT_1800toInf = new TFile(folder_in+"/Arranged_TT/TT_1800toInf.root", "R");          TT_files.push_back(file_in_TT_1800toInf);

  vector<TFile*> WW_files;
  TFile* file_in_WW_lowmll = new TFile(folder_in+"/Arranged_WW/WW_inclusive.root", "R");             WW_files.push_back(file_in_WW_lowmll);
  TFile* file_in_WW_200to600 = new TFile(folder_in+"/Arranged_WW/WW_200to600.root", "R");	     WW_files.push_back(file_in_WW_200to600);
  TFile* file_in_WW_600to1200 = new TFile(folder_in+"/Arranged_WW/WW_600to1200.root", "R");	     WW_files.push_back(file_in_WW_600to1200);
  TFile* file_in_WW_1200to2500 = new TFile(folder_in+"/Arranged_WW/WW_1200to2500.root", "R");	     WW_files.push_back(file_in_WW_1200to2500);
  TFile* file_in_WW_2500toInf = new TFile(folder_in+"/Arranged_WW/WW_2500toInf.root", "R");          WW_files.push_back(file_in_WW_2500toInf);

  TFile* file_in_WZ = new TFile(folder_in+"/Arranged_WZ/WZ.root", "R");
  TFile* file_in_ZZ = new TFile(folder_in+"/Arranged_ZZ/ZZ.root", "R");

  TFile* file_in_ST = new TFile(folder_in+"/Arranged_ST/ST.root", "R");

  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");
  

  vector<TString> vars_TH2;
  vars_TH2.push_back("taupt_ratio_pass");
  vars_TH2.push_back("taupt_ratio_fail");

  vector<TString> Mth;
  Mth.push_back("MtLow_OS");
  Mth.push_back("MtLow_SS");

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
  taun.push_back("realtau");


  //cross-sections
  vector<double> xs_DY;
  double xs_DY_lowmass = 6225.42;          xs_DY.push_back(xs_DY_lowmass);   
  double xs_DY_400to500 = 0.4064;	   xs_DY.push_back(xs_DY_400to500);  
  double xs_DY_500to700 = 0.241;	   xs_DY.push_back(xs_DY_500to700);  
  double xs_DY_700to800 = 0.03606;	   xs_DY.push_back(xs_DY_700to800);  
  double xs_DY_800to1000 = 0.03032;	   xs_DY.push_back(xs_DY_800to1000); 
  double xs_DY_1000to1500 = 0.01924;	   xs_DY.push_back(xs_DY_1000to1500);
  double xs_DY_1500to2000 = 0.002181;	   xs_DY.push_back(xs_DY_1500to2000);
  double xs_DY_2000to3000 = 0.0005129;     xs_DY.push_back(xs_DY_2000to3000);

  vector<double> xs_TT;
  double xs_TT_lowmll = 85.5/*831.76*/;      xs_TT.push_back(xs_TT_lowmll);   
  double xs_TT_500to800 = 0.319;             xs_TT.push_back(xs_TT_500to800); 
  double xs_TT_800to1200 = 3.196e-2;	     xs_TT.push_back(xs_TT_800to1200); 
  double xs_TT_1200to1800 = 2.987e-3;	     xs_TT.push_back(xs_TT_1200to1800); 
  double xs_TT_1800toInf = 1.711e-4;         xs_TT.push_back(xs_TT_1800toInf); 

  vector<double> xs_WW;
  double xs_WW_lowm = /*63.21*/12.178;       xs_WW.push_back(xs_WW_lowm);
  double xs_WW_200to600 = 1.39;              xs_WW.push_back(xs_WW_200to600); 
  double xs_WW_600to1200 = 5.7e-2;	     xs_WW.push_back(xs_WW_600to1200); 
  double xs_WW_1200to2500 = 3.6e-3;	     xs_WW.push_back(xs_WW_1200to2500); 
  double xs_WW_2500toInf = 5.4e-5;           xs_WW.push_back(xs_WW_2500toInf); 

  double xs_ST = 38.09;
  double xs_WZ = 22.82;
  double xs_ZZ = 10.32;
  double xs_signal = 20;

  TString var_in, var_out;

  file_out->cd();
  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int m = 0; m<Mth.size(); ++m) {
      for (unsigned int j = 0; j<systs.size(); ++j) {
	for (unsigned int k = 0; k<eta.size(); ++k) {
	  for (unsigned int l = 0; l<taun.size(); ++l) {
	    var_in = vars_TH2[i]+"_"+systs[j]+"_"+Mth[m]+"_"+eta[k]+"_"+taun[l];
	    var_out = var_in;
	    
	    cout << endl << endl <<var_in << endl;
            
	    vector<TH2D*> h_DY_vector;
	    for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
	      h_DY_vector.push_back( MC_histo_TH2(systs[j]+"/"+var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin) ); 
	    }
	    TH2D* h_DY = (TH2D*) h_DY_vector[0]->Clone("DY_"+var_out);
	    for (unsigned int iBin = 1; iBin<DY_files.size(); ++iBin) {
	      h_DY->Add(h_DY_vector[iBin]);
	      delete h_DY_vector[iBin];
	    }
	    delete h_DY_vector[0];
	    h_DY->Write();
	    delete h_DY;
            
	    //vector<TH2D*> h_WJets_vector;
            //for (unsigned int iBin = 0; iBin<WJets_files.size(); ++iBin) {
	    //  h_WJets_vector.push_back( MC_histo_TH2(systs[j]+"/"+var_in, WJets_files[iBin], xs_WJets[iBin], N_WJets[iBin], rebin) ); 
            //}
            //TH2D* h_WJets = (TH2D*) h_WJets_vector[0]->Clone("WJets_"+var_out);
            //for (unsigned int iBin = 1; iBin<WJets_files.size(); ++iBin) {
	    //  h_WJets->Add(h_WJets_vector[iBin]);
            //}
            //h_WJets->Write();

	    vector<TH2D*> h_TT_vector;
	    for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
	      h_TT_vector.push_back( MC_histo_TH2(systs[j]+"/"+var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin) ); 
	    }
	    TH2D* h_TT = (TH2D*) h_TT_vector[0]->Clone("TT_"+var_out);
	    for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
	      h_TT->Add(h_TT_vector[iBin]);
	      delete h_TT_vector[iBin];
	    }
	    delete h_TT_vector[0];
	    h_TT->Write();
	    delete h_TT;
            
	    vector<TH2D*> h_WW_vector;
	    for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
	      h_WW_vector.push_back( MC_histo_TH2(systs[j]+"/"+var_in, WW_files[iBin], file_in_data, xs_WW[iBin], rebin) ); 
	    }
	    TH2D* h_WW = (TH2D*) h_WW_vector[0]->Clone("WW_"+var_out);
	    for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
	      h_WW->Add(h_WW_vector[iBin]);
	      delete h_WW_vector[iBin];
	    }
	    delete h_WW_vector[0];
            
	    TH2D* h_WZ = MC_histo_TH2(systs[j]+"/"+var_in, file_in_WZ, file_in_data, xs_WZ, rebin);
	    TH2D* h_ZZ = MC_histo_TH2(systs[j]+"/"+var_in, file_in_ZZ, file_in_data, xs_ZZ, rebin);
	    TH2D* h_VV = (TH2D*) h_WW->Clone("VV_"+var_out);
	    h_VV->Add(h_WZ);
	    h_VV->Add(h_ZZ);
	    h_VV->Write();
	    delete h_VV;
	    delete h_WZ;
	    delete h_ZZ;
	    delete h_WW;
            
	    TH2D* h_ST = MC_histo_TH2(systs[j]+"/"+var_in, file_in_ST, file_in_data, xs_ST, rebin);
	    h_ST->SetName("ST_"+var_out);
	    h_ST->Write();
	    delete h_ST;
	    
	    TH2D* h_data = (TH2D*) file_in_data -> Get(systs[j]+"/"+var_in);//Data is, by definition, normalized
	    h_data -> SetName("data_"+var_out);
	    h_data->RebinX(rebin);
	    h_data->RebinY(rebin);
	    h_data->Write();
	    delete h_data;
	  }
	}
      }
    }
  }
  file_out->Close();


  return 0;
}
