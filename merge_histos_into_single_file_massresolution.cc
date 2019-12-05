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


using namespace std;


TH1F* MC_histo(TString var, TFile* file_in, double xs, int rebin) {

  cout << file_in->GetName() << endl;

  double lumi = 35.9 * pow(10,3); //luminosity in pb^-1                                                                                                                                                           

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();

  double e_Nevents = pow(Nevents,0.5);
  double e_xs = 0.01*xs;

  //Weight                                                                                                                                                                                                                          
  double w = 0;
  if (Nevents != 0) w = xs*lumi/Nevents;
  cout << "Events in data/events in MC " << w << endl;

  TH1F* h;
  h = (TH1F*) file_in -> Get(var);

  h -> Scale(w);
  h -> Rebin(rebin);

  return h;

}


int main(int argc, char** argv) {


  int rebin = 1;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/MassResolution";
  TString name_out = "histos_massresolution";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

                            //cross-sections           
  vector<TString> mass;     vector<double> xs;	      
  mass.push_back("500");    xs.push_back(9.56     );       
  mass.push_back("600");    xs.push_back(5.03     );      
  mass.push_back("700");    xs.push_back(2.83	  ); 
  mass.push_back("800");    xs.push_back(1.704	  ); 
  mass.push_back("900");    xs.push_back(1.075	  ); 
  mass.push_back("1000");   xs.push_back(0.7141   );  
  mass.push_back("1100");   xs.push_back(0.4775   );  
  mass.push_back("1200");   xs.push_back(0.329	  ); 
  mass.push_back("1300");   xs.push_back(0.234	  ); 
  mass.push_back("1400");   xs.push_back(0.1675   );  
  mass.push_back("1500");   xs.push_back(0.1226   );  
  mass.push_back("1600");   xs.push_back(0.09071  );  
  mass.push_back("1700");   xs.push_back(0.06808  );  
  mass.push_back("1800");   xs.push_back(0.05166  );  
  mass.push_back("1900");   xs.push_back(0.03912  );  
  mass.push_back("2000");   xs.push_back(0.03027  );  
  mass.push_back("2200");   xs.push_back(0.01847  );  
  mass.push_back("2400");   xs.push_back(0.01147  );  
  mass.push_back("2600");   xs.push_back(0.007258 );  
  mass.push_back("2800");   xs.push_back(0.004695 );  
  mass.push_back("3000");   xs.push_back(0.003079 );  
  mass.push_back("3500");   xs.push_back(0.001163 );  
  mass.push_back("4000");   xs.push_back(0.0004841);  
  mass.push_back("4500");   xs.push_back(0.0002196);  
  mass.push_back("5000");   xs.push_back(0.0001113);  
  //mass.push_back("5500");   xs.push_back(1.); 	 
  //mass.push_back("6000");   xs.push_back(1.); 	 


  vector<TFile*> files_in;
  for (unsigned int i=0; i<mass.size(); ++i) {
    files_in.push_back( new TFile(folder_in+"/Arranged_ZPrime/ZPrime_"+mass[i]+".root", "R") );
  }

  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");

  vector<TString> vars  ,               vars_out;
  vars.push_back("Mgen");               vars_out.push_back("Mgen");         
  vars.push_back("Mvis");         	vars_out.push_back("Mvis");         
  vars.push_back("Mtot");         	vars_out.push_back("Mtot");         
  vars.push_back("Mcol_nodisc");  	vars_out.push_back("Mcol_nodisc");  
  vars.push_back("Mvis_res");     	vars_out.push_back("Mvis_res");     
  vars.push_back("Mtot_res");     	vars_out.push_back("Mtot_res");     
  vars.push_back("Mcol_res");     	vars_out.push_back("Mcol_res");     
  vars.push_back("Mcol_elevloose");	vars_out.push_back("Mcol_elevloose");
  vars.push_back("Mcol_eleloose");	vars_out.push_back("Mcol_eleloose");
  vars.push_back("Mcol_elemedium");	vars_out.push_back("Mcol_elemedium");
  vars.push_back("Mcol_eletight"); 	vars_out.push_back("Mcol_eletight");
  vars.push_back("Mcol_muloose"); 	vars_out.push_back("Mcol_muloose"); 
  vars.push_back("Mcol_mutight"); 	vars_out.push_back("Mcol_mutight"); 
  vars.push_back("Mcol_final");	  	vars_out.push_back("Mcol_final");	  
  vars.push_back("Mcol_tightest");	vars_out.push_back("Mcol_tightest");

  TString var_in, var_out;

  file_out->cd();
  for (unsigned int i = 0; i<vars.size(); ++i) {
    var_in = vars[i];
    var_out = vars_out[i];
    cout << endl << endl <<var_in << endl;
    
    for (unsigned int j = 0; j<mass.size(); ++j) {
      TH1F* h = MC_histo(var_in, files_in[j], xs[j], rebin);
      h->SetName(mass[j]+"_"+var_out);
      h->Write();
    }          
  }    
  file_out->Close();


  return 0;
}
