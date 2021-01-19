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
  signal.push_back("RPV_l001_4000");   xs.push_back(6.7061e-04);
  signal.push_back("RPV_l001_4500");   xs.push_back(1.9445e-04);
  signal.push_back("RPV_l001_5000");   xs.push_back(5.5668e-05);


  vector<TFile*> files_in;
  for (unsigned int i=0; i<signal.size(); ++i) {
    files_in.push_back( new TFile(folder_in+"/Arranged_signal/"+signal[i]+".root", "R") );
  }


  vector<TString> vars  ,                  vars_out;
  vars.push_back("Mgen");                  vars_out.push_back("Mgen");         
  vars.push_back("Mvis_nodisc");           vars_out.push_back("Mvis_nodisc");         
  vars.push_back("Mtot_nodisc");           vars_out.push_back("Mtot_nodisc");         
  vars.push_back("Mcol_nodisc");  	   vars_out.push_back("Mcol_nodisc");  
  vars.push_back("Mvis_res");     	   vars_out.push_back("Mvis_res");     
  vars.push_back("Mtot_res");     	   vars_out.push_back("Mtot_res");     
  vars.push_back("Mcol_res");     	   vars_out.push_back("Mcol_res");     
  vars.push_back("oldMcol_res");     	   vars_out.push_back("oldMcol_res");     
  vars.push_back("Mvis_wdisc");            vars_out.push_back("Mvis_wdisc");         
  vars.push_back("Mtot_wdisc");            vars_out.push_back("Mtot_wdisc");         
  vars.push_back("Mcol_wdisc");  	   vars_out.push_back("Mcol_wdisc");  
  vars.push_back("oldMcol_wdisc");  	   vars_out.push_back("oldMcol_wdisc");  
  vars.push_back("Mcol_dphiplus");         vars_out.push_back("Mcol_dphiplus");        
  vars.push_back("oldMcol_dphiplus");      vars_out.push_back("oldMcol_dphiplus");    
  vars.push_back("Mcol_res_dphiplus");     vars_out.push_back("Mcol_res_dphiplus");    
  vars.push_back("tauMETdphi_dphiplus");   vars_out.push_back("tauMETdphi_dphiplus"); 
  vars.push_back("Mcol_dphiminus");        vars_out.push_back("Mcol_dphiminus");      
  vars.push_back("oldMcol_dphiminus");     vars_out.push_back("oldMcol_dphiminus");   
  vars.push_back("Mcol_res_dphiminus");    vars_out.push_back("Mcol_res_dphiminus");    
  vars.push_back("tauMETdphi_dphiminus");  vars_out.push_back("tauMETdphi_dphiminus"); 
  vars.push_back("tau_eta");               vars_out.push_back("gentau_eta"); 
  vars.push_back("tau_eta");               vars_out.push_back("genmu_eta"); 

  TString var_in, var_out;

  file_out->cd();
  for (unsigned int i = 0; i<vars.size(); ++i) {
    var_in = vars[i];
    var_out = vars_out[i];
    cout << endl << endl <<var_in << endl;
    
    for (unsigned int j = 0; j<signal.size(); ++j) {
      TH1F* h = MC_histo(var_in, files_in[j], 1, rebin);
      h->SetName(signal[j]+"_"+var_out);
      h->Write();
    }          
  }    
  file_out->Close();


  return 0;
}
