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


TH2D* MC_histo_TH2(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {
  cout << file_in->GetName() << endl;

  TH1F* h_events_data = (TH1F*) file_in_d->Get("weighted_events");
  double full_data = 7.86454e+08;
  double succ_data_ratio = h_events_data->Integral()/full_data;
  cout << "succesfull data ratio " << succ_data_ratio << endl;

  double lumi = 35.9 * pow(10,3) * succ_data_ratio; //luminosity in pb^-1

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();

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

  TFile* file_in_WJets = new TFile(folder_in+"/Arranged_WJets/WJets.root", "R");
  vector<TFile*> WJets_files;
  TFile* file_in_WJets_lowpt = new TFile(folder_in+"/Arranged_WJets/WJets_inclusive.root", "R");     WJets_files.push_back(file_in_WJets_lowpt);
  TFile* file_in_WJets_100to250 = new TFile(folder_in+"/Arranged_WJets/WJets_100to250.root", "R");   WJets_files.push_back(file_in_WJets_100to250);
  TFile* file_in_WJets_250to400 = new TFile(folder_in+"/Arranged_WJets/WJets_250to400.root", "R");   WJets_files.push_back(file_in_WJets_250to400);
  TFile* file_in_WJets_400to600 = new TFile(folder_in+"/Arranged_WJets/WJets_400to600.root", "R");   WJets_files.push_back(file_in_WJets_400to600);
  TFile* file_in_WJets_600toInf = new TFile(folder_in+"/Arranged_WJets/WJets_600toInf.root", "R");   WJets_files.push_back(file_in_WJets_600toInf);

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
  //vars_TH2.push_back("taupt_mupt_pass");
  //vars_TH2.push_back("taupt_mupt_fail");

  vector<TString> Mth;
  Mth.push_back("MtLow_OS");
  Mth.push_back("MtLow_SS");
  //Mth.push_back("MtHigh");

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> taun;
  taun.push_back("realtau");
  //taun.push_back("faketau");


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

  vector<double> xs_WJets;
  double xs_WJets_lowpt = 61526.7;         xs_WJets.push_back(xs_WJets_lowpt);	 
  double xs_WJets_100to250 = 627.1;        xs_WJets.push_back(xs_WJets_100to250);
  double xs_WJets_250to400 = 21.8;	   xs_WJets.push_back(xs_WJets_250to400);
  double xs_WJets_400to600 = 2.635;	   xs_WJets.push_back(xs_WJets_400to600);
  double xs_WJets_600toInf = 0.4102;       xs_WJets.push_back(xs_WJets_600toInf);

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

  //Nevents
  vector<double> N_DY;
  double N_DY_lowmass = 56653095 + 24936833;  N_DY.push_back(N_DY_lowmass);   
  double N_DY_400to500 = 151168;	      N_DY.push_back(N_DY_400to500);  
  double N_DY_500to700 = 144087;	      N_DY.push_back(N_DY_500to700);  
  double N_DY_700to800 = 136874;	      N_DY.push_back(N_DY_700to800);  
  double N_DY_800to1000 = 131568;	      N_DY.push_back(N_DY_800to1000); 
  double N_DY_1000to1500 = 119990;	      N_DY.push_back(N_DY_1000to1500);
  double N_DY_1500to2000 = 111668;	      N_DY.push_back(N_DY_1500to2000);
  double N_DY_2000to3000 = 101266;	      N_DY.push_back(N_DY_2000to3000);

  vector<double> N_WJets;
  double N_WJets_lowpt = 57025279 + 29584813;                         N_WJets.push_back(N_WJets_lowpt);	 
  double N_WJets_100to250 = 3605796 + 3645002 + 28601158 + 7436491;   N_WJets.push_back(N_WJets_100to250);
  double N_WJets_250to400 = 369311 + 369256 + 3696492;		      N_WJets.push_back(N_WJets_250to400);
  double N_WJets_400to600 = 364427 + 376433;			      N_WJets.push_back(N_WJets_400to600);
  double N_WJets_600toInf = 396336 + 396394;                          N_WJets.push_back(N_WJets_600toInf);

  vector<double> N_TT;
  double N_TT_lowmll = 75897555;              N_TT.push_back(N_TT_lowmll);   
  double N_TT_500to800 = 199979;	      N_TT.push_back(N_TT_500to800); 
  double N_TT_800to1200 = 199773;	      N_TT.push_back(N_TT_800to1200);
  double N_TT_1200to1800 = 199956;	      N_TT.push_back(N_TT_1200to1800);
  double N_TT_1800toInf = 40816;	      N_TT.push_back(N_TT_1800toInf);

  vector<double> N_WW;
  double N_WW_lowm = 1021000;                N_WW.push_back(N_WW_lowm);
  double N_WW_200to600 = 199991;             N_WW.push_back(N_WW_200to600); 
  double N_WW_600to1200 = 74997;             N_WW.push_back(N_WW_600to1200); 
  double N_WW_1200to2500 = 99992;            N_WW.push_back(N_WW_1200to2500); 
  double N_WW_2500toInf = 13968;             N_WW.push_back(N_WW_2500toInf); 

  double N_ST = 3256548 + 3256309;

  double N_WZ = 921116;
  double N_ZZ = 990051;


  double N_signal = 14994;
  TString var_in, var_out;

  file_out->cd();
  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int m = 0; m<Mth.size(); ++m) {
      for (unsigned int j = 0; j<dms.size(); ++j) {
	for (unsigned int k = 0; k<eta.size(); ++k) {
	  for (unsigned int l = 0; l<taun.size(); ++l) {
	    var_in = vars_TH2[i]+"_"+Mth[m]+"_"+dms[j]+"_"+eta[k]+"_"+taun[l];
	    var_out = var_in;
	    
	    cout << endl << endl <<var_in << endl;
            
	    vector<TH2D*> h_DY_vector;
	    for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
	      h_DY_vector.push_back( MC_histo_TH2(var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin) ); 
	    }
	    TH2D* h_DY = (TH2D*) h_DY_vector[0]->Clone("DY_"+var_out);
	    for (unsigned int iBin = 1; iBin<DY_files.size(); ++iBin) {
	      h_DY->Add(h_DY_vector[iBin]);
	    }
	    h_DY->Write();
            
	    //vector<TH2D*> h_WJets_vector;
            //for (unsigned int iBin = 0; iBin<WJets_files.size(); ++iBin) {
	    //  h_WJets_vector.push_back( MC_histo_TH2(var_in, WJets_files[iBin], xs_WJets[iBin], N_WJets[iBin], rebin) ); 
            //}
            //TH2D* h_WJets = (TH2D*) h_WJets_vector[0]->Clone("WJets_"+var_out);
            //for (unsigned int iBin = 1; iBin<WJets_files.size(); ++iBin) {
	    //  h_WJets->Add(h_WJets_vector[iBin]);
            //}
            //h_WJets->Write();

	    vector<TH2D*> h_TT_vector;
	    for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
	      h_TT_vector.push_back( MC_histo_TH2(var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin) ); 
	    }
	    TH2D* h_TT = (TH2D*) h_TT_vector[0]->Clone("TT_"+var_out);
	    for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
	      h_TT->Add(h_TT_vector[iBin]);
	    }
	    h_TT->Write();
            
	    vector<TH2D*> h_WW_vector;
	    for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
	      h_WW_vector.push_back( MC_histo_TH2(var_in, WW_files[iBin], file_in_data, xs_WW[iBin], rebin) ); 
	    }
	    TH2D* h_WW = (TH2D*) h_WW_vector[0]->Clone("WW_"+var_out);
	    for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
	      h_WW->Add(h_WW_vector[iBin]);
	    }
            
	    TH2D* h_WZ = MC_histo_TH2(var_in, file_in_WZ, file_in_data, xs_WZ, rebin);
	    TH2D* h_ZZ = MC_histo_TH2(var_in, file_in_ZZ, file_in_data, xs_ZZ, rebin);
	    TH2D* h_VV = (TH2D*) h_WW->Clone("VV_"+var_out);
	    h_VV->Add(h_WZ);
	    h_VV->Add(h_ZZ);
	    h_VV->Write();
            
	    TH2D* h_ST = MC_histo_TH2(var_in, file_in_ST, file_in_data, xs_ST, rebin);
	    h_ST->SetName("ST_"+var_out);
	    h_ST->Write();
	    
	    TH2D* h_data = (TH2D*) file_in_data -> Get(var_in);//Data is, by definition, normalized
	    h_data -> SetName("data_"+var_out);
	    h_data->RebinX(rebin);
	    h_data->RebinY(rebin);
	    h_data->Write();
	  }
	}
      }
    }
  }
  file_out->Close();


  return 0;
}
