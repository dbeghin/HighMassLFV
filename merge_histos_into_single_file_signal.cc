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


TH1F* MC_histo(TString var, TFile* file_in, double xs, long Nevents, int rebin) {

  cout << file_in->GetName() << endl;

  double lumi = 35.9*pow(10, 3); //luminosity in pb^-1

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

  h -> Sumw2();
  h -> Scale(w);
  h -> Rebin(rebin);
  
  return h;

}



int main(int argc, char** argv) {


  int rebin = 1;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/SignalRegion_CR100";
  TString name_out = "histos_signal";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

                            //cross-sections             //Nevents	     
  vector<TString> mass;     vector<double> xs;	         vector<long> N;    
  mass.push_back("500");    xs.push_back(9.56); 	 N.push_back(47195);
  mass.push_back("600");    xs.push_back(5.03 ); 	 N.push_back(49995);
  mass.push_back("700");    xs.push_back(2.83	   ); 	 N.push_back(48194);
  mass.push_back("800");    xs.push_back(1.704	   ); 	 N.push_back(49992);
  mass.push_back("900");    xs.push_back(1.075	   ); 	 N.push_back(49988);
  mass.push_back("1000");   xs.push_back(0.7141   ); 	 N.push_back(44789);
  mass.push_back("1100");   xs.push_back(0.4775   ); 	 N.push_back(49186);
  mass.push_back("1200");   xs.push_back(0.329	   ); 	 N.push_back(45189);
  mass.push_back("1300");   xs.push_back(0.234	   ); 	 N.push_back(42392);
  mass.push_back("1400");   xs.push_back(0.1675   ); 	 N.push_back(49979);
  mass.push_back("1500");   xs.push_back(0.1226   ); 	 N.push_back(40985);
  mass.push_back("1600");   xs.push_back(0.09071  ); 	 N.push_back(49282);
  mass.push_back("1700");   xs.push_back(0.06808  ); 	 N.push_back(45479);
  mass.push_back("1800");   xs.push_back(0.05166  ); 	 N.push_back(49986);
  mass.push_back("1900");   xs.push_back(0.03912  ); 	 N.push_back(49972);
  mass.push_back("2000");   xs.push_back(0.03027  ); 	 N.push_back(49975);
  mass.push_back("2200");   xs.push_back(0.01847  ); 	 N.push_back(43679);
  mass.push_back("2400");   xs.push_back(0.01147  ); 	 N.push_back(49974);
  mass.push_back("2600");   xs.push_back(0.007258 ); 	 N.push_back(49272);
  mass.push_back("2800");   xs.push_back(0.004695 ); 	 N.push_back(49971);
  mass.push_back("3000");   xs.push_back(0.003079 ); 	 N.push_back(49977);
  mass.push_back("3500");   xs.push_back(0.001163 ); 	 N.push_back(49976);
  mass.push_back("4000");   xs.push_back(0.0004841); 	 N.push_back(44972);
  mass.push_back("4500");   xs.push_back(0.0002196); 	 N.push_back(39271);
  mass.push_back("5000");   xs.push_back(0.0001113); 	 N.push_back(49965);
  //mass.push_back("5500");   xs.push_back(1.); 	 N.push_back(41173);
  //mass.push_back("6000");   xs.push_back(1.); 	 N.push_back(49979);


  vector<TFile*> files_in;
  for (unsigned int i=0; i<mass.size(); ++i) {
    files_in.push_back( new TFile(folder_in+"/Arranged_Signal/Signal_"+mass[i]+".root", "R") );
  }


  vector<TString> vars;
  vars.push_back("ev_Mvis_realtau_MtHigh");          
  vars.push_back("ev_Mcol_realtau_MtHigh");          
  vars.push_back("ev_Mtot_realtau_MtHigh");          


  TString var_in;

  file_out->cd();
  //options = is it the DY Sig?, variable name, which file to get the histo from, process cross-section
  for (unsigned int i = 0; i<vars.size(); ++i) {
    var_in = vars[i];
    cout << endl << endl <<var_in << endl;
    
    for (unsigned int j = 0; j<mass.size(); ++j) {
      TH1F* h = MC_histo(var_in, files_in[j], xs[j], N[j], rebin);
      h->SetName(mass[j]+"_"+var_in);
      h->Write();
    }          
  }    
  file_out->Close();


  return 0;
}
