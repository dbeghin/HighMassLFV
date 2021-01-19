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


TH1F* MC_histo(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {

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
  if (Nevents !=0 ) {
    w = xs*lumi/Nevents;
  }
  cout << "Events in data/events in MC " << w << endl;
  
  TH1F* h;
  h = (TH1F*) file_in -> Get(var);

  //h -> Sumw2();
  h -> Scale(w);
  h -> Rebin(rebin);
  
  return h;

}


TH2F* MC_histo_TH2(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {

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
  if (Nevents !=0 ) {
    w = xs*lumi/Nevents;
  }
  cout << "Events in data/events in MC " << w << endl;
  
  TH2F* h;
  h = (TH2F*) file_in -> Get(var);

  //h -> Sumw2();
  h -> Scale(w);
  h -> RebinX(rebin);
  h -> RebinY(rebin);
  
  return h;

}



int main(/*int argc, char** argv*/) {


  int rebin = 1;
  //string eera = *(argv + 1);
  //TString era = eera;

  TString folder_in = "HighMassLFVMuTau/FakeRate_mumu"; 
  TString name_out = "histos_fakerate_DY";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");
  //TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data_"+era+".root", "R");
  

  vector<TString> vars;
  vars.push_back("ev_Mvis");          
  vars.push_back("mu_pt");            
  vars.push_back("mu_eta");           
  vars.push_back("mu_phi");           

  vector<TString> vars_TH2;
  vars_TH2.push_back("taupt_ratio_pass");
  vars_TH2.push_back("taupt_ratio_fail");
  

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");
  dms.push_back("DM11");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> flavor;
  flavor.push_back("quark");
  flavor.push_back("gluon");


  //cross-sections
  vector<double> xs_DY;
  double xs_DY_lowmass = 6225.4;           xs_DY.push_back(xs_DY_lowmass);   
  double xs_DY_400to500 = 0.4064;	   xs_DY.push_back(xs_DY_400to500);  
  double xs_DY_500to700 = 0.241;	   xs_DY.push_back(xs_DY_500to700);  
  double xs_DY_700to800 = 0.03606;	   xs_DY.push_back(xs_DY_700to800);  
  double xs_DY_800to1000 = 0.03032;	   xs_DY.push_back(xs_DY_800to1000); 
  double xs_DY_1000to1500 = 0.01924;	   xs_DY.push_back(xs_DY_1000to1500);
  double xs_DY_1500to2000 = 0.002181;	   xs_DY.push_back(xs_DY_1500to2000);
  double xs_DY_2000to3000 = 0.0005129;     xs_DY.push_back(xs_DY_2000to3000);

  vector<double> xs_TT;
  double xs_TT_lowmll = 831.76;              xs_TT.push_back(xs_TT_lowmll);   
  double xs_TT_500to800 = 0.326;             xs_TT.push_back(xs_TT_500to800); 
  double xs_TT_800to1200 = 5.66665e-2;	     xs_TT.push_back(xs_TT_800to1200); 
  double xs_TT_1200to1800 = 3.557e-3;	     xs_TT.push_back(xs_TT_1200to1800); 
  double xs_TT_1800toInf = 5.395e-5;         xs_TT.push_back(xs_TT_1800toInf); 

  vector<double> xs_WW;
  double xs_WW_lowm = 63.21;                 xs_WW.push_back(xs_WW_lowm);
  double xs_WW_200to600 = 1.39;              xs_WW.push_back(xs_WW_200to600); 
  double xs_WW_600to1200 = 5.7e-2;	     xs_WW.push_back(xs_WW_600to1200); 
  double xs_WW_1200to2500 = 3.6e-3;	     xs_WW.push_back(xs_WW_1200to2500); 
  double xs_WW_2500toInf = 5.4e-5;           xs_WW.push_back(xs_WW_2500toInf); 

  double xs_ST = 38.09;
  double xs_WZ = 22.82;
  double xs_ZZ = 10.32;
  double xs_signal = 20;

  TString var_in = "";

  file_out->cd();
  //options = is it the DY Sig?, variable name, which file to get the histo from, process cross-section
  for (unsigned int i = 0; i<vars.size(); ++i) {
    var_in = vars[i];
    cout << endl << endl <<var_in << endl;
    	    
    
    TH1F* h_data = (TH1F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
    h_data -> SetName("data_"+var_in);
    h_data->Rebin(rebin);
    h_data->Write();
    delete h_data;
  }    


  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int j = 0; j<dms.size(); ++j) {
      for (unsigned int k = 0; k<eta.size(); ++k) {
	for (unsigned int l = 0; l<flavor.size(); ++l) {
	  var_in = vars_TH2[i]+"_"+dms[j]+"_"+flavor[l]+"_"+eta[k];
	  cout << endl << endl <<var_in << endl;
        	    
          TH2F* h_data = (TH2F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
          h_data -> SetName("data_"+var_in);
          h_data->RebinX(rebin);
          h_data->RebinY(rebin);
          h_data->Write();
	  delete h_data;
	}
      }
    }
  }
  file_out->Close();


  return 0;
}
