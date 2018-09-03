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

  double lumi = 35.9 * pow(10,3); //luminosity in pb^-1

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


  int rebin = 10;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/FakeRate";
  TString name_out = "histos_signal";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

  vector<TString> mass;
  mass.push_back("500");
  mass.push_back("1000");
  mass.push_back("1600");
  //mass.push_back("2000");
  mass.push_back("3000");


  vector<TFile*> files_in;
  for (unsigned int i=0; i<mass.size(); ++i) {
    files_in.push_back( new TFile("Signal_"+mass[i]+".root", "R") );
  }


  vector<TString> vars;
  vars.push_back("ev_Mvis_realtau_MtHigh");          
  vars.push_back("ev_Mcol_realtau_MtHigh");          
  vars.push_back("ev_Mtot_realtau_MtHigh");          


  //cross-sections
  vector<double> xs;
  xs.push_back(1.);//500
  xs.push_back(1.);//1000
  xs.push_back(1.);//1600
  //xs.push_back(1.);//2000
  xs.push_back(1.);//3000

  
  //Nevets
  vector<long> N;
  N.push_back(10000);//500
  N.push_back(10000);//1000
  N.push_back(10000);//1600
  //N.push_back(100);//2000
  N.push_back(10000);//3000



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
