#include <iostream>
#include <fstream>
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

using namespace std;


TH1F* MC_histo_sig(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {

  cout << var << endl;
  cout << file_in->GetName() << endl;

  TH1F* h_events_data = (TH1F*) file_in_d->Get("weighted_events");
  double full_data = 7.86454e+08;
  double succ_data_ratio = 1; //h_events_data->Integral()/full_data;
  cout << "succesfull data ratio " << succ_data_ratio << endl;
  double lumi = 35.9 * pow(10,3) * succ_data_ratio; //luminosity in pb^-1

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();
  delete h_events;

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


TH1F* MC_histo_zprime(TString var, TString weightID, TFile* file_in, double xs, int rebin) {
  cout << var << endl;
  cout << file_in->GetName() << endl;

  TH1F* h_SOWbase = (TH1F*) file_in->Get("sumOfPDFWeights_0");
  double SOWbase = h_SOWbase->Integral();
  delete h_SOWbase;

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();
  delete h_events;

  TH1F* h_before = (TH1F*) file_in->Get("mcWeight_before");
  double Nbefore = h_before->Integral();
  delete h_before;

  TH1F* h_after = (TH1F*) file_in->Get("mcWeight_after");
  double Nafter = h_after->Integral();
  delete h_after;

  double lumi = 35.9 * pow(10,3);

  double w = 0;
  TH1F* h_SOWsys = (TH1F*) file_in->Get("sumOfPDFWeights_"+weightID);
  double SOWsys = h_SOWsys->Integral();
  delete h_SOWsys;
  //Weight
  if (SOWsys != 0) w = xs*lumi/Nevents * Nbefore/Nafter * SOWbase/SOWsys;

  cout << "Events in data/MC " << w << endl;

  TH1F* hh;
  hh = (TH1F*) file_in -> Get(var);

  hh -> Scale(w);
  hh -> Rebin(rebin);

  return hh;
}



int main(int argc, char** argv) {


  int rebin = 1;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/SignalRegion_CR100";
  //TString folder_in = "HighMassLFVMuTau/MassResolution";
  TString name_out = "histos_signal";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");




                                       //cross-sections in pb           
  vector<TString> signal;              vector<double> xs;	      
  signal.push_back("ZPrime_500");      xs.push_back(9.56     );       
  signal.push_back("ZPrime_600");      xs.push_back(5.03     );      
  signal.push_back("ZPrime_700");      xs.push_back(2.83     ); 
  signal.push_back("ZPrime_800");      xs.push_back(1.704    ); 
  signal.push_back("ZPrime_900");      xs.push_back(1.075    ); 
  signal.push_back("ZPrime_1000");     xs.push_back(0.7141   );  
  signal.push_back("ZPrime_1100");     xs.push_back(0.4775   );  
  signal.push_back("ZPrime_1200");     xs.push_back(0.329    ); 
  signal.push_back("ZPrime_1300");     xs.push_back(0.234    ); 
  signal.push_back("ZPrime_1400");     xs.push_back(0.1675   );  
  signal.push_back("ZPrime_1500");     xs.push_back(0.1226   );  
  signal.push_back("ZPrime_1600");     xs.push_back(0.09071  );  
  signal.push_back("ZPrime_1700");     xs.push_back(0.06808  );  
  signal.push_back("ZPrime_1800");     xs.push_back(0.05166  );  
  signal.push_back("ZPrime_1900");     xs.push_back(0.03912  );  
  signal.push_back("ZPrime_2000");     xs.push_back(0.03027  );  
  signal.push_back("ZPrime_2200");     xs.push_back(0.01847  );  
  signal.push_back("ZPrime_2400");     xs.push_back(0.01147  );  
  signal.push_back("ZPrime_2600");     xs.push_back(0.007258 );  
  signal.push_back("ZPrime_2800");     xs.push_back(0.004695 );  
  signal.push_back("ZPrime_3000");     xs.push_back(0.003079 );  
  signal.push_back("ZPrime_3500");     xs.push_back(0.001163 );  
  signal.push_back("ZPrime_4000");     xs.push_back(0.0004841);  
  signal.push_back("ZPrime_4500");     xs.push_back(0.0002196);  
  signal.push_back("ZPrime_5000");     xs.push_back(0.0001113);  
  signal.push_back("ZPrime_5500");     xs.push_back(6.238e-05); 	 
  signal.push_back("ZPrime_6000");     xs.push_back(3.896e-05); 	 
  				       
  signal.push_back("QBH_200");         xs.push_back(24304.548    );
  signal.push_back("QBH_400");         xs.push_back(1794.1462    );
  signal.push_back("QBH_600");         xs.push_back(334.19232    );
  signal.push_back("QBH_800");         xs.push_back(94.06898     );
  signal.push_back("QBH_1000");        xs.push_back(33.19272     );
  signal.push_back("QBH_1200");        xs.push_back(13.331938    );
  signal.push_back("QBH_1400");        xs.push_back(5.958376     );
  signal.push_back("QBH_1600");        xs.push_back(2.8127452    );
  signal.push_back("QBH_1800");        xs.push_back(1.4207024    );
  signal.push_back("QBH_2000");        xs.push_back(0.74266324   );
  signal.push_back("QBH_2500");        xs.push_back(0.16715634   );
  signal.push_back("QBH_3000");        xs.push_back(0.0423081    );
  signal.push_back("QBH_3500");        xs.push_back(0.011538935  );
  signal.push_back("QBH_4000");        xs.push_back(0.003249312  );
  signal.push_back("QBH_4500");        xs.push_back(0.000944568  );
  signal.push_back("QBH_5000");        xs.push_back(0.00027294006);
  signal.push_back("QBH_5500");        xs.push_back(7.84E-05     );
  signal.push_back("QBH_6000");        xs.push_back(2.20E-05     );
  signal.push_back("QBH_7000");        xs.push_back(1.59E-06     );
  signal.push_back("QBH_9000");        xs.push_back(4.49E-09     );
  signal.push_back("QBH_10000");       xs.push_back(1.60E-10     );
  
  signal.push_back("RPV_l001_200");    xs.push_back(773.72);
  signal.push_back("RPV_l001_300");    xs.push_back(212.55);
  signal.push_back("RPV_l001_400");    xs.push_back(81.57);
  signal.push_back("RPV_l001_500");    xs.push_back(37.73);
  signal.push_back("RPV_l001_600");    xs.push_back(19.66);
  signal.push_back("RPV_l001_700");    xs.push_back(11.130);
  signal.push_back("RPV_l001_800");    xs.push_back(6.6938);
  signal.push_back("RPV_l001_900");    xs.push_back(4.2154);
  signal.push_back("RPV_l001_1000");   xs.push_back(2.7521);
  signal.push_back("RPV_l001_1200");   xs.push_back(1.27272);
  signal.push_back("RPV_l001_1400");   xs.push_back(0.63690);
  signal.push_back("RPV_l001_1600");   xs.push_back(0.3369363);
  signal.push_back("RPV_l001_1800");   xs.push_back(0.185585);
  signal.push_back("RPV_l001_2000");   xs.push_back(0.105317);
  signal.push_back("RPV_l001_2500");   xs.push_back(2.7834e-02);
  signal.push_back("RPV_l001_3000");   xs.push_back(7.879e-03);
  signal.push_back("RPV_l001_3500");   xs.push_back(2.2935e-03);
  signal.push_back("RPV_l01_3500");    xs.push_back(2.2935e-01);
  signal.push_back("RPV_l001_4000");   xs.push_back(6.7061e-04);
  signal.push_back("RPV_l01_4000");    xs.push_back(0.06704);
  signal.push_back("RPV_l02_4000");    xs.push_back(0.2682463);
  signal.push_back("RPV_l05_4000");    xs.push_back(1.);
  signal.push_back("RPV_l001_4500");   xs.push_back(1.9445e-04);
  signal.push_back("RPV_l02_4500");    xs.push_back(1.);
  signal.push_back("RPV_l001_5000");   xs.push_back(5.5668e-05);
  signal.push_back("RPV_l02_5000");    xs.push_back(1.);
  signal.push_back("RPV_l001_5500");   xs.push_back(1.);
  signal.push_back("RPV_l02_5500");    xs.push_back(1.);
  signal.push_back("RPV_l02_6000");    xs.push_back(1.);
  signal.push_back("RPV_l05_6000");    xs.push_back(1.);
  signal.push_back("RPV_l001_6500");   xs.push_back(1.);
  signal.push_back("RPV_l02_6500");    xs.push_back(1.);
  signal.push_back("RPV_l05_6500");    xs.push_back(1.);


  vector<TFile*> files_in;
  for (unsigned int i=0; i<signal.size(); ++i) {
    files_in.push_back( new TFile(folder_in+"/Arranged_signal/"+signal[i]+".root", "R") );
  }

  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");


  vector<float> xpoints_pt {/*0, 10, 20,*/ 30, 40, 50, 60, 70, 80, 90, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 450, 500, 600, 800, 1000};
  vector<float> xpoints_Mcol {/*0, 10, 20,*/ 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 6000, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800, 6900, 7000, 7100, 7200, 7300, 7400, 7500, 7600, 7700, 7800, 7900, 8000};

  vector<TString> vars              ,        vars_out;                              vector<bool> simpleRebin;      vector<int> reb;   vector<vector<float>> xpoints;	 
  //vars.push_back("ev_Mvis_realtau_");        vars_out.push_back("ev_Mvis_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);	 
  vars.push_back("ev_Mcol_realtau_");        vars_out.push_back("ev_Mcol_");	    simpleRebin.push_back(true);   reb.push_back(100); xpoints.push_back(xpoints_pt);	 
  //vars.push_back("ev_Mtot_realtau_");        vars_out.push_back("ev_Mtot_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);	 
  //vars.push_back("tau_pt_realtau_");         vars_out.push_back("tau_pt_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("tau_eta_realtau_");        vars_out.push_back("tau_eta_");	    simpleRebin.push_back(true);   reb.push_back(2);  xpoints.push_back(xpoints_pt);	 
  //vars.push_back("tau_phi_realtau_");        vars_out.push_back("tau_phi_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);	 
  //vars.push_back("mu_pt_realtau_");          vars_out.push_back("mu_pt_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("mu_eta_realtau_");         vars_out.push_back("mu_eta_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("mu_phi_realtau_");         vars_out.push_back("mu_phi_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("ev_DRmutau_realtau_");     vars_out.push_back("ev_DRmutau_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("ev_Mt_realtau_");          vars_out.push_back("ev_Mt_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("ev_MET_realtau_");         vars_out.push_back("ev_MET_");	    simpleRebin.push_back(false);  reb.push_back(200);  xpoints.push_back(xpoints_Mcol); 
  //vars.push_back("mu_isolation_realtau_");   vars_out.push_back("mu_isolation_");   simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("sign_realtau_");           vars_out.push_back("sign_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
  //vars.push_back("ev_Nvertex_realtau_");     vars_out.push_back("ev_Nvertex_");	    simpleRebin.push_back(false);  reb.push_back(1);  xpoints.push_back(xpoints_pt);  	 
				


  vector<TString> Mth;
  //Mth.push_back("MtLow_OS");
  //Mth.push_back("MtLow_SS");
  Mth.push_back("MtHigh");
  //Mth.push_back("MtLow_TT");
  //Mth.push_back("MtHigh_TT");

  vector<TString> systs;
  systs.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  systs_aux.push_back("PDF");
  vector<TString> systs_signalveto = GetSignalSysVeto();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    bool bVeto = false;
    for (unsigned int iVeto=0; iVeto<systs_signalveto.size(); ++iVeto) {
      if (systs_aux[iAux] == systs_signalveto[iVeto]) {
  	bVeto = true;
  	break;
      }
    }
    if (bVeto) continue;
    if (systs_aux[iAux] == "topPt") continue;
    systs.push_back(systs_aux[iAux]+"_up");
    systs.push_back(systs_aux[iAux]+"_down");
  }

  TString var_in, var_out;
  float xs_same = 1;

  file_out->cd();
  for (unsigned int i = 0; i<systs.size(); ++i) {
    TDirectory* dir = file_out->mkdir( systs[i] );
    dir->cd();
    for (unsigned int k = 0; k<vars.size(); ++k) {
      for (unsigned int l = 0; l<Mth.size(); ++l) {
	var_in = systs[i]+"/"+vars[k]+systs[i]+"_"+Mth[l];
	var_out = vars_out[k]+Mth[l];
    
	for (unsigned int j = 0; j<signal.size(); ++j) {
	  if (signal[j].Contains("ZPrime")) {
            TString weight_ID = "median";
            if (systs[i] == "PDF_up") weight_ID = "up";
            else if (systs[i] == "PDF_down") weight_ID = "down";
            TH1F* h = MC_histo_zprime(var_in, weight_ID, files_in[j], xs_same, rebin);
            h->SetName(signal[j]+"_"+var_out);
            h->Write();
            delete h;
          }
          else {
            TH1F* h = MC_histo_sig(var_in, files_in[j], file_in_data, xs_same, rebin);
            h->SetName(signal[j]+"_"+var_out);
            h->Write();
            delete h;
          }
	}          
      }
    }
    dir->Close();
  }    
  file_out->Close();


  


  return 0;
}
