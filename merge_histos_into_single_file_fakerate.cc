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

  //h -> Sumw2();
  h -> Scale(w);
  h -> Rebin(rebin);
  
  return h;

}


TH2F* MC_histo_TH2(TString var, TFile* file_in, double xs, long Nevents, int rebin) {

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
  
  TH2F* h;
  h = (TH2F*) file_in -> Get(var);

  //h -> Sumw2();
  h -> Scale(w);
  h -> RebinX(rebin);
  h -> RebinY(rebin);
  
  return h;

}



int main(int argc, char** argv) {


  int rebin = 2;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/FakeRate"; //FIXME
  //TString folder_in = "HighMassLFVMuTau/FakeRate_EleTau";
  TString name_out = "histos_fakerate";

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

  TFile* file_in_faketau = new TFile("HighMassLFVMuTau/QCDWJets_CR0.root", "R");

  TFile* file_in_QCD = new TFile(folder_in+"/Arranged_QCD/QCD.root", "R");
  vector<TFile*> QCD_files;
  TFile* file_in_QCD_15to30 = new TFile(folder_in+"/Arranged_QCD/QCD_15to30.root", "R");              QCD_files.push_back(file_in_QCD_15to30);
  TFile* file_in_QCD_30to50 = new TFile(folder_in+"/Arranged_QCD/QCD_30to50.root", "R");              QCD_files.push_back(file_in_QCD_30to50);
  TFile* file_in_QCD_50to80 = new TFile(folder_in+"/Arranged_QCD/QCD_50to80.root", "R");              QCD_files.push_back(file_in_QCD_50to80);
  TFile* file_in_QCD_80to120 = new TFile(folder_in+"/Arranged_QCD/QCD_80to120.root", "R");            QCD_files.push_back(file_in_QCD_80to120);
  TFile* file_in_QCD_120to170 = new TFile(folder_in+"/Arranged_QCD/QCD_120to170.root", "R");          QCD_files.push_back(file_in_QCD_120to170);
  TFile* file_in_QCD_170to300 = new TFile(folder_in+"/Arranged_QCD/QCD_170to300.root", "R");          QCD_files.push_back(file_in_QCD_170to300);
  TFile* file_in_QCD_300to470 = new TFile(folder_in+"/Arranged_QCD/QCD_300to470.root", "R");          QCD_files.push_back(file_in_QCD_300to470);
  TFile* file_in_QCD_470to600 = new TFile(folder_in+"/Arranged_QCD/QCD_470to600.root", "R");          QCD_files.push_back(file_in_QCD_470to600);
  TFile* file_in_QCD_600to800 = new TFile(folder_in+"/Arranged_QCD/QCD_600to800.root", "R");          QCD_files.push_back(file_in_QCD_600to800);
  TFile* file_in_QCD_800to1000 = new TFile(folder_in+"/Arranged_QCD/QCD_800to1000.root", "R");        QCD_files.push_back(file_in_QCD_800to1000);
  TFile* file_in_QCD_1000to1400 = new TFile(folder_in+"/Arranged_QCD/QCD_1000to1400.root", "R");      QCD_files.push_back(file_in_QCD_1000to1400);
  TFile* file_in_QCD_1400to1800 = new TFile(folder_in+"/Arranged_QCD/QCD_1400to1800.root", "R");      QCD_files.push_back(file_in_QCD_1400to1800);
  TFile* file_in_QCD_1800to2400 = new TFile(folder_in+"/Arranged_QCD/QCD_1800to2400.root", "R");      QCD_files.push_back(file_in_QCD_1800to2400);
  TFile* file_in_QCD_2400to3200 = new TFile(folder_in+"/Arranged_QCD/QCD_2400to3200.root", "R");      QCD_files.push_back(file_in_QCD_2400to3200);
  

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

  TFile* file_in_ST_top = new TFile(folder_in+"/Arranged_ST_top/ST_top.root", "R");
  TFile* file_in_ST_antitop = new TFile(folder_in+"/Arranged_ST_antitop/ST_antitop.root", "R");


  TFile* file_in_signal = new TFile(folder_in+"/Arranged_RPV/RPV.root", "R");
  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");
  
  

  vector<TString> vars;
  vars.push_back("ev_Mvis");          
  vars.push_back("mu_pt");            
  vars.push_back("mu_eta");           
  vars.push_back("mu_phi");           

  vector<TString> vars_TH2;
  vars_TH2.push_back("taupt_jetpt_pass");
  vars_TH2.push_back("taupt_jetpt_fail");
  

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");


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

  vector<double> xs_WJets;
  double xs_WJets_lowpt = 61526.7;         xs_WJets.push_back(xs_WJets_lowpt);	 
  double xs_WJets_100to250 = 627.1;        xs_WJets.push_back(xs_WJets_100to250);
  double xs_WJets_250to400 = 21.8;	   xs_WJets.push_back(xs_WJets_250to400);
  double xs_WJets_400to600 = 2.635;	   xs_WJets.push_back(xs_WJets_400to600);
  double xs_WJets_600toInf = 0.4102;       xs_WJets.push_back(xs_WJets_600toInf);

  double xs_QCD_muenriched = 720648000*0.00042;
  //double xs_QCD_inc = 1953000;
  vector<double> xs_QCD;
  double xs_QCD_15to30 = /*0;*/1.822*pow(10,9);    xs_QCD.push_back(xs_QCD_15to30);
  double xs_QCD_30to50 = /*0;*/1.387*pow(10,8);    xs_QCD.push_back(xs_QCD_30to50);
  double xs_QCD_50to80 = /*0;*/1.913*pow(10,7);    xs_QCD.push_back(xs_QCD_50to80);
  double xs_QCD_80to120 = 2.736*pow(10,6);   xs_QCD.push_back(xs_QCD_80to120);
  double xs_QCD_120to170 = 4.663*pow(10,5);  xs_QCD.push_back(xs_QCD_120to170);
  double xs_QCD_170to300 = 1.172*pow(10,5);  xs_QCD.push_back(xs_QCD_170to300);
  double xs_QCD_300to470 = 7.76*pow(10,3);   xs_QCD.push_back(xs_QCD_300to470);
  double xs_QCD_470to600 = 640.5;	     xs_QCD.push_back(xs_QCD_470to600);
  double xs_QCD_600to800 = 185.9;	     xs_QCD.push_back(xs_QCD_600to800);
  double xs_QCD_800to1000 = 32;		     xs_QCD.push_back(xs_QCD_800to1000);
  double xs_QCD_1000to1400 = 9.37;	     xs_QCD.push_back(xs_QCD_1000to1400);
  double xs_QCD_1400to1800 = 0.838;	     xs_QCD.push_back(xs_QCD_1400to1800);
  double xs_QCD_1800to2400 = 0.112;	     xs_QCD.push_back(xs_QCD_1800to2400);
  double xs_QCD_2400to3200 = 6.74*pow(10,-3); xs_QCD.push_back(xs_QCD_2400to3200);

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


  double N_QCD_muenriched = 22093506;//9951232;//check metadata
  vector<double> N_QCD;
  long N_QCD_15to30 = 37585689;              N_QCD.push_back(N_QCD_15to30);
  long N_QCD_30to50 = 9979945;               N_QCD.push_back(N_QCD_30to50);
  long N_QCD_50to80 = 9954259;               N_QCD.push_back(N_QCD_50to80);
  long N_QCD_80to120 = 7608728+6986638;      N_QCD.push_back(N_QCD_80to120);
  long N_QCD_120to170 = 5504047+6324260;     N_QCD.push_back(N_QCD_120to170);
  long N_QCD_170to300 = 6855630+6799704;     N_QCD.push_back(N_QCD_170to300);
  long N_QCD_300to470 = 4150323+14771394;    N_QCD.push_back(N_QCD_300to470);
  long N_QCD_470to600 = 3866704;             N_QCD.push_back(N_QCD_470to600);
  long N_QCD_600to800 = 3810427+9496102;     N_QCD.push_back(N_QCD_600to800);
  long N_QCD_800to1000 = 13715052;           N_QCD.push_back(N_QCD_800to1000);
  long N_QCD_1000to1400 = 2829635+6539554;   N_QCD.push_back(N_QCD_1000to1400);
  long N_QCD_1400to1800 = 312291+2310326;    N_QCD.push_back(N_QCD_1400to1800);
  long N_QCD_1800to2400 = 397083+1549881;    N_QCD.push_back(N_QCD_1800to2400);
  long N_QCD_2400to3200 = 398491+550068;     N_QCD.push_back(N_QCD_2400to3200); 


  vector<double> N_TT;
  double N_TT_lowmll = 75897555;              N_TT.push_back(N_TT_lowmll);   
  double N_TT_500to800 = 199979;	      N_TT.push_back(N_TT_500to800); 
  double N_TT_800to1200 = 199773;	      N_TT.push_back(N_TT_800to1200);
  double N_TT_1200to1800 = 199956;	      N_TT.push_back(N_TT_1200to1800);
  double N_TT_1800toInf = 40816;	      N_TT.push_back(N_TT_1800toInf);

  vector<double> N_WW;
  double N_WW_lowm = 993997;                      N_WW.push_back(N_WW_lowm);
  double N_WW_200to600 = 0.326;             N_WW.push_back(N_WW_200to600); 
  double N_WW_600to1200 = 5.66665e-2;	     N_WW.push_back(N_WW_600to1200); 
  double N_WW_1200to2500 = 3.557e-3;	     N_WW.push_back(N_WW_1200to2500); 
  double N_WW_2500toInf = 5.395e-5;         N_WW.push_back(N_WW_2500toInf); 

  double N_ST_top = 3256548;
  double N_ST_antitop = 3256309;

  double N_WZ = 921116;
  double N_ZZ = 990051;


  double N_signal = 14994;
  TString var_in;

  file_out->cd();
  //options = is it the DY Sig?, variable name, which file to get the histo from, process cross-section
  for (unsigned int i = 0; i<vars.size(); ++i) {
    var_in = vars[i];
    cout << endl << endl <<var_in << endl;
    	    
    vector<TH1F*> h_DY_vector;
    for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
      //if (iBin>0) continue; //FIXME
      h_DY_vector.push_back( MC_histo(var_in, DY_files[iBin], xs_DY[iBin], N_DY[iBin], rebin) ); 
    }
    TH1F* h_DY = (TH1F*) h_DY_vector[0]->Clone("DY_"+var_in);
    for (unsigned int iBin = 1; iBin<h_DY_vector.size(); ++iBin) {
      h_DY->Add(h_DY_vector[iBin]);
    }
    h_DY->Write();
    cout << h_DY << endl;
    
    /*vector<TH1F*> h_QCD_vector;
    for (unsigned int iBin = 0; iBin<QCD_files.size(); ++iBin) {
      h_QCD_vector.push_back( MC_histo(var_in, QCD_files[iBin], xs_QCD[iBin], N_QCD[iBin], rebin) ); 
    }
    TH1F* h_QCD = (TH1F*) h_QCD_vector[0]->Clone("QCD_"+var_in);
    for (unsigned int iBin = 1; iBin<QCD_files.size(); ++iBin) {
      h_QCD->Add(h_QCD_vector[iBin]);
    }
    h_QCD->Write();*/
    
    /*vector<TH1F*> h_WJets_vector;
    for (unsigned int iBin = 0; iBin<WJets_files.size(); ++iBin) {
      h_WJets_vector.push_back( MC_histo(var_in, WJets_files[iBin], xs_WJets[iBin], N_WJets[iBin], rebin) ); 
    }
    TH1F* h_WJets = (TH1F*) h_WJets_vector[0]->Clone("WJets_"+var_in);
    for (unsigned int iBin = 1; iBin<WJets_files.size(); ++iBin) {
      h_WJets->Add(h_WJets_vector[iBin]);
    }
    h_WJets->Write();*/
    
    vector<TH1F*> h_TT_vector;
    for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
      //if (iBin > 0) continue;
      h_TT_vector.push_back( MC_histo(var_in, TT_files[iBin], xs_TT[iBin], N_TT[iBin], rebin) ); 
    }
    TH1F* h_TT = (TH1F*) h_TT_vector[0]->Clone("TT_"+var_in);
    for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
      h_TT->Add(h_TT_vector[iBin]);
    }
    h_TT->Write();
    
      
    vector<TH1F*> h_WW_vector;
    for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
      if (iBin>0) continue;
      h_WW_vector.push_back( MC_histo(var_in, WW_files[iBin], xs_WW[iBin], N_WW[iBin], rebin) ); 
    }
    TH1F* h_WW = (TH1F*) h_WW_vector[0]->Clone("WW_"+var_in);
    for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
      h_WW->Add(h_WW_vector[iBin]);
    }
      
    TH1F* h_WZ = MC_histo(var_in, file_in_WZ, xs_WZ, N_WZ, rebin);
    TH1F* h_ZZ = MC_histo(var_in, file_in_ZZ, xs_ZZ, N_ZZ, rebin);
    TH1F* h_VV = (TH1F*) h_WW->Clone("VV_"+var_in);
    h_VV->Add(h_WZ);
    h_VV->Add(h_ZZ);
    //h_VV -> SetName("VV_"+var_in);
    h_VV->Write();
    
    TH1F* h_ST_top = MC_histo(var_in, file_in_ST_top, xs_ST, N_ST_top, rebin);
    TH1F* h_ST_antitop = MC_histo(var_in, file_in_ST_antitop, xs_ST, N_ST_antitop, rebin);
    TH1F* h_ST = (TH1F*) h_ST_top->Clone("ST_"+var_in);
    h_ST->Add(h_ST_antitop);
    h_ST->Write();
    
    TH1F* h_data = (TH1F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
    h_data -> SetName("data_"+var_in);
    h_data->Rebin(rebin);
    h_data->Write();
  }    


  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int j = 0; j<dms.size(); ++j) {
      for (unsigned int k = 0; k<eta.size(); ++k) {
        var_in = vars_TH2[i]+"_"+dms[j]+"_"+eta[k];
        cout << endl << endl <<var_in << endl;
        	    
        vector<TH2F*> h_DY_vector;
        for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
          //if (iBin>0) continue; //FIXME
          h_DY_vector.push_back( MC_histo_TH2(var_in, DY_files[iBin], xs_DY[iBin], N_DY[iBin], rebin) ); 
        }
        TH2F* h_DY = (TH2F*) h_DY_vector[0]->Clone("DY_"+var_in);
        for (unsigned int iBin = 1; iBin<h_DY_vector.size(); ++iBin) {
          h_DY->Add(h_DY_vector[iBin]);
        }
        h_DY->Write();
        cout << h_DY << endl;
        
        /*vector<TH2F*> h_QCD_vector;
        for (unsigned int iBin = 0; iBin<QCD_files.size(); ++iBin) {
          h_QCD_vector.push_back( MC_histo_TH2(var_in, QCD_files[iBin], xs_QCD[iBin], N_QCD[iBin], rebin) ); 
        }
        TH2F* h_QCD = (TH2F*) h_QCD_vector[0]->Clone("QCD_"+var_in);
        for (unsigned int iBin = 1; iBin<QCD_files.size(); ++iBin) {
          h_QCD->Add(h_QCD_vector[iBin]);
        }
        h_QCD->Write();*/
        
        /*vector<TH2F*> h_WJets_vector;
        for (unsigned int iBin = 0; iBin<WJets_files.size(); ++iBin) {
          h_WJets_vector.push_back( MC_histo_TH2(var_in, WJets_files[iBin], xs_WJets[iBin], N_WJets[iBin], rebin) ); 
        }
        TH2F* h_WJets = (TH2F*) h_WJets_vector[0]->Clone("WJets_"+var_in);
        for (unsigned int iBin = 1; iBin<WJets_files.size(); ++iBin) {
          h_WJets->Add(h_WJets_vector[iBin]);
        }
        h_WJets->Write();*/
        
        vector<TH2F*> h_TT_vector;
        for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
          //if (iBin > 0) continue;
          h_TT_vector.push_back( MC_histo_TH2(var_in, TT_files[iBin], xs_TT[iBin], N_TT[iBin], rebin) ); 
        }
        TH2F* h_TT = (TH2F*) h_TT_vector[0]->Clone("TT_"+var_in);
        for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
          h_TT->Add(h_TT_vector[iBin]);
        }
        h_TT->Write();
        
          
        vector<TH2F*> h_WW_vector;
        for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
          if (iBin>0) continue;
          h_WW_vector.push_back( MC_histo_TH2(var_in, WW_files[iBin], xs_WW[iBin], N_WW[iBin], rebin) ); 
        }
        TH2F* h_WW = (TH2F*) h_WW_vector[0]->Clone("WW_"+var_in);
        for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
          h_WW->Add(h_WW_vector[iBin]);
        }
          
        TH2F* h_WZ = MC_histo_TH2(var_in, file_in_WZ, xs_WZ, N_WZ, rebin);
        TH2F* h_ZZ = MC_histo_TH2(var_in, file_in_ZZ, xs_ZZ, N_ZZ, rebin);
        TH2F* h_VV = (TH2F*) h_WW->Clone("VV_"+var_in);
        h_VV->Add(h_WZ);
        h_VV->Add(h_ZZ);
        //h_VV -> SetName("VV_"+var_in);
        h_VV->Write();
        
        TH2F* h_ST_top = MC_histo_TH2(var_in, file_in_ST_top, xs_ST, N_ST_top, rebin);
        TH2F* h_ST_antitop = MC_histo_TH2(var_in, file_in_ST_antitop, xs_ST, N_ST_antitop, rebin);
        TH2F* h_ST = (TH2F*) h_ST_top->Clone("ST_"+var_in);
        h_ST->Add(h_ST_antitop);
        h_ST->Write();
        
        TH2F* h_data = (TH2F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
        h_data -> SetName("data_"+var_in);
        h_data->RebinX(rebin);
        h_data->RebinY(rebin);
        h_data->Write();
      }
    }
  }
  file_out->Close();


  return 0;
}
