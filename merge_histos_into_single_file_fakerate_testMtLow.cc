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


TH2F* MC_histo_TH2(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin, double base_lumi) {

  cout << file_in->GetName() << endl;

  TH1F* h_events_data = (TH1F*) file_in_d->Get("weighted_events");
  double full_data = 7.86454e+08;
  double succ_data_ratio = 1;//h_events_data->Integral()/full_data;
  cout << "succesfull data ratio " << succ_data_ratio << endl;
  double lumi = base_lumi * pow(10,3) * succ_data_ratio; //luminosity in pb^-1

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



int main(int argc, char** argv) {


  int rebin = 1;
  string eera = *(argv + 1);
  TString era = eera;

  TString folder_in = "HighMassLFVMuTau/FakeRate_MtLow"; 
  TString name_out = "histos_fakerate_flavorMtLow";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

  vector<TFile*> DY_files;
  TFile* file_in_DY_lowmass = new TFile(folder_in+"/Arranged_DY/DY_inclusive.root", "R");            DY_files.push_back(file_in_DY_lowmass);
  TFile* file_in_DY_100to200 = new TFile(folder_in+"/Arranged_DY/DY_100to200.root", "R");            DY_files.push_back(file_in_DY_100to200);
  TFile* file_in_DY_200to400 = new TFile(folder_in+"/Arranged_DY/DY_200to400.root", "R");            DY_files.push_back(file_in_DY_200to400);
  TFile* file_in_DY_400to500 = new TFile(folder_in+"/Arranged_DY/DY_400to500.root", "R");            DY_files.push_back(file_in_DY_400to500);
  TFile* file_in_DY_500to700 = new TFile(folder_in+"/Arranged_DY/DY_500to700.root", "R");	     DY_files.push_back(file_in_DY_500to700);
  TFile* file_in_DY_700to800 = new TFile(folder_in+"/Arranged_DY/DY_700to800.root", "R");	     DY_files.push_back(file_in_DY_700to800);
  TFile* file_in_DY_800to1000 = new TFile(folder_in+"/Arranged_DY/DY_800to1000.root", "R");	     DY_files.push_back(file_in_DY_800to1000);
  TFile* file_in_DY_1000to1500 = new TFile(folder_in+"/Arranged_DY/DY_1000to1500.root", "R");	     DY_files.push_back(file_in_DY_1000to1500);
  TFile* file_in_DY_1500to2000 = new TFile(folder_in+"/Arranged_DY/DY_1500to2000.root", "R");	     DY_files.push_back(file_in_DY_1500to2000);
  TFile* file_in_DY_2000to3000 = new TFile(folder_in+"/Arranged_DY/DY_2000to3000.root", "R");        DY_files.push_back(file_in_DY_2000to3000);


  //TFile* file_in_WJets = new TFile(folder_in+"/Arranged_WJets/WJets.root", "R");
  //vector<TFile*> WJets_files;
  //TFile* file_in_WJets_lowpt = new TFile(folder_in+"/Arranged_WJets/WJets_inclusive.root", "R");     WJets_files.push_back(file_in_WJets_lowpt);
  //TFile* file_in_WJets_100to250 = new TFile(folder_in+"/Arranged_WJets/WJets_100to250.root", "R");   WJets_files.push_back(file_in_WJets_100to250);
  //TFile* file_in_WJets_250to400 = new TFile(folder_in+"/Arranged_WJets/WJets_250to400.root", "R");   WJets_files.push_back(file_in_WJets_250to400);
  //TFile* file_in_WJets_400to600 = new TFile(folder_in+"/Arranged_WJets/WJets_400to600.root", "R");   WJets_files.push_back(file_in_WJets_400to600);
  //TFile* file_in_WJets_600toInf = new TFile(folder_in+"/Arranged_WJets/WJets_600toInf.root", "R");   WJets_files.push_back(file_in_WJets_600toInf);
  
  vector<TFile*> TT_files;
  TFile* file_in_TT_lowmll = new TFile(folder_in+"/Arranged_TT/TT_2l2nu.root", "R");             TT_files.push_back(file_in_TT_lowmll);
  TFile* file_in_TT_500to800 = new TFile(folder_in+"/Arranged_TT/TT_500to800.root", "R");	     TT_files.push_back(file_in_TT_500to800);
  TFile* file_in_TT_800to1200 = new TFile(folder_in+"/Arranged_TT/TT_800to1200.root", "R");	     TT_files.push_back(file_in_TT_800to1200);
  TFile* file_in_TT_1200to1800 = new TFile(folder_in+"/Arranged_TT/TT_1200to1800.root", "R");	     TT_files.push_back(file_in_TT_1200to1800);
  TFile* file_in_TT_1800toInf = new TFile(folder_in+"/Arranged_TT/TT_1800toInf.root", "R");          TT_files.push_back(file_in_TT_1800toInf);
  
  vector<TFile*> VV_files;
  VV_files.push_back( new TFile(folder_in+"/Arranged_WW/WW_inclusive.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WW/WW_200to600.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WW/WW_600to1200.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WW/WW_1200to2500.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WW/WW_2500toInf.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WZ/WZ_2l2q.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_WZ/WZ_3lnu.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_ZZ/ZZ_2l2nu.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_ZZ/ZZ_2l2q.root", "R") );
  VV_files.push_back( new TFile(folder_in+"/Arranged_ZZ/ZZ_4l.root", "R") );
  
  TFile* file_in_ST = new TFile(folder_in+"/Arranged_ST/ST.root", "R");
  
  
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


  double base_lumi=0;
  if (era=="B") base_lumi=5.79;
  else if (era=="C") base_lumi=2.57;
  else if (era=="D") base_lumi=4.25;
  else if (era=="E") base_lumi=4.01;
  else if (era=="F") base_lumi=3.10;
  else if (era=="G") base_lumi=7.54;
  else if (era=="H") base_lumi=8.60;
  else if (era=="BtoF") base_lumi=19.8;
  else if (era=="GH") base_lumi=16.1;
  else if (era=="all") base_lumi=35.9;
  

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");
  dms.push_back("DM11");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> flavor;
  flavor.push_back("MtLow_OS");
  flavor.push_back("MtLow_SS");


  //cross-sections
  float kNNLO = 1.023;

  vector<double> xs_DY;
  double xs_DY_lowmass   = 6077.22;          xs_DY.push_back(xs_DY_lowmass);
  double xs_DY_100to200  = kNNLO*226.6;      xs_DY.push_back(xs_DY_100to200);
  double xs_DY_200to400  = kNNLO*7.77;       xs_DY.push_back(xs_DY_200to400);
  double xs_DY_400to500  = kNNLO*0.4065;     xs_DY.push_back(xs_DY_400to500);
  double xs_DY_500to700  = kNNLO*0.2334;     xs_DY.push_back(xs_DY_500to700);
  double xs_DY_700to800  = kNNLO*0.03614;    xs_DY.push_back(xs_DY_700to800);
  double xs_DY_800to1000 = kNNLO*0.03047;    xs_DY.push_back(xs_DY_800to1000);
  double xs_DY_1000to1500= kNNLO*0.01636;    xs_DY.push_back(xs_DY_1000to1500);
  double xs_DY_1500to2000= kNNLO*0.00218;    xs_DY.push_back(xs_DY_1500to2000);
  double xs_DY_2000to3000= kNNLO*0.0005156;  xs_DY.push_back(xs_DY_2000to3000);
  double xs_DY_3000toInf = kNNLO*0.0005156;  xs_DY.push_back(xs_DY_3000toInf);


  vector<double> xs_TT;
  xs_TT.push_back(831.76*0.105); //2l2nu
  double xs_TT_500to800 = 0.326;             xs_TT.push_back(xs_TT_500to800);
  double xs_TT_800to1200 = 3.26e-2;         xs_TT.push_back(xs_TT_800to1200);
  double xs_TT_1200to1800 = 3.05e-3;        xs_TT.push_back(xs_TT_1200to1800);
  double xs_TT_1800toInf = 1.74e-4;         xs_TT.push_back(xs_TT_1800toInf);

  vector<double> xs_VV;
  double xs_WW_lowm = 12.178;                xs_VV.push_back(xs_WW_lowm);
  double xs_WW_200to600 = 1.39;              xs_VV.push_back(xs_WW_200to600);
  double xs_WW_600to1200 = 5.7e-2;           xs_VV.push_back(xs_WW_600to1200);
  double xs_WW_1200to2500 = 3.6e-3;          xs_VV.push_back(xs_WW_1200to2500);
  double xs_WW_2500toInf = 5.4e-5;           xs_VV.push_back(xs_WW_2500toInf);
  double xs_WZ_2l2q = 5.595;                 xs_VV.push_back(xs_WZ_2l2q);
  double xs_WZ_3lnu = 5.052;                 xs_VV.push_back(xs_WZ_3lnu);
  double xs_ZZ_2l2nu = 0.564;                xs_VV.push_back(xs_ZZ_2l2nu);
  double xs_ZZ_2l2q = 3.22;                  xs_VV.push_back(xs_ZZ_2l2q);
  double xs_ZZ_4l = 1.212;                   xs_VV.push_back(xs_ZZ_4l);


  double xs_ST = 38.09;
  double xs_WZ = 22.82;
  double xs_ZZ = 10.32;
  double xs_signal = 20;

  TString var_in = "";

  file_out->cd();
  //options = is it the DY Sig?, variable name, which file to get the histo from, process cross-section

  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int j = 0; j<dms.size(); ++j) {
      for (unsigned int k = 0; k<eta.size(); ++k) {
	for (unsigned int l = 0; l<flavor.size(); ++l) {
	  var_in = vars_TH2[i]+"_"+dms[j]+"_"+flavor[l]+"_"+eta[k];
	  cout << endl << endl <<var_in << endl;
        	    
	  vector<TH2F*> h_DY_vector;
	  for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
	    //if (iBin>0) continue; //FIXME
	    h_DY_vector.push_back( MC_histo_TH2(var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin, base_lumi) ); 
	  }
	  TH2F* h_DY = (TH2F*) h_DY_vector[0]->Clone("DY_"+var_in);
	  for (unsigned int iBin = 1; iBin<h_DY_vector.size(); ++iBin) {
	    h_DY->Add(h_DY_vector[iBin]);
	    delete h_DY_vector[iBin];
	  }
	  h_DY->Write();
	  delete h_DY;
        
          vector<TH2F*> h_TT_vector;
          for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
            //if (iBin > 0) continue;
            h_TT_vector.push_back( MC_histo_TH2(var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin, base_lumi) ); 
          }
          TH2F* h_TT = (TH2F*) h_TT_vector[0]->Clone("TT_"+var_in);
          for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
            h_TT->Add(h_TT_vector[iBin]);
	    delete h_TT_vector[iBin];
          }
          h_TT->Write();
	  delete h_TT;
          
            
          vector<TH2F*> h_VV_vector;
          for (unsigned int iBin = 0; iBin<VV_files.size(); ++iBin) {
            if (iBin>0) continue;
            h_VV_vector.push_back( MC_histo_TH2(var_in, VV_files[iBin], file_in_data, xs_VV[iBin], rebin, base_lumi) ); 
          }
          TH2F* h_VV = (TH2F*) h_VV_vector[0]->Clone("VV_"+var_in);
          for (unsigned int iBin = 1; iBin<h_VV_vector.size(); ++iBin) {
            h_VV->Add(h_VV_vector[iBin]);
	    delete h_VV_vector[iBin];
          }
          h_VV->Write();
	  delete h_VV;
          
          TH2F* h_ST = MC_histo_TH2(var_in, file_in_ST, file_in_data, xs_ST, rebin, base_lumi);
	  h_ST->SetName("ST_"+var_in);
          h_ST->Write();
	  delete h_ST;
          
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
