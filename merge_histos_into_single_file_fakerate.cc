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



int main(int argc, char** argv) {


  int rebin = 2;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/FakeRate"; //FIXME
  //TString folder_in = "HighMassLFVMuTau/FakeRate_EleTau";
  TString name_out = "histos_fakerate_DY";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");

  vector<TFile*> DY_files;
  TFile* file_in_DY_lowmass = new TFile(folder_in+"/Arranged_DY/DY_inclusive.root", "R");            DY_files.push_back(file_in_DY_lowmass);
  //TFile* file_in_DY_400to500 = new TFile(folder_in+"/Arranged_DY/DY_400to500.root", "R");            DY_files.push_back(file_in_DY_400to500);
  //TFile* file_in_DY_500to700 = new TFile(folder_in+"/Arranged_DY/DY_500to700.root", "R");	     DY_files.push_back(file_in_DY_500to700);
  //TFile* file_in_DY_700to800 = new TFile(folder_in+"/Arranged_DY/DY_700to800.root", "R");	     DY_files.push_back(file_in_DY_700to800);
  //TFile* file_in_DY_800to1000 = new TFile(folder_in+"/Arranged_DY/DY_800to1000.root", "R");	     DY_files.push_back(file_in_DY_800to1000);
  //TFile* file_in_DY_1000to1500 = new TFile(folder_in+"/Arranged_DY/DY_1000to1500.root", "R");	     DY_files.push_back(file_in_DY_1000to1500);
  //TFile* file_in_DY_1500to2000 = new TFile(folder_in+"/Arranged_DY/DY_1500to2000.root", "R");	     DY_files.push_back(file_in_DY_1500to2000);
  //TFile* file_in_DY_2000to3000 = new TFile(folder_in+"/Arranged_DY/DY_2000to3000.root", "R");        DY_files.push_back(file_in_DY_2000to3000);


  //TFile* file_in_WJets = new TFile(folder_in+"/Arranged_WJets/WJets.root", "R");
  //vector<TFile*> WJets_files;
  //TFile* file_in_WJets_lowpt = new TFile(folder_in+"/Arranged_WJets/WJets_inclusive.root", "R");     WJets_files.push_back(file_in_WJets_lowpt);
  //TFile* file_in_WJets_100to250 = new TFile(folder_in+"/Arranged_WJets/WJets_100to250.root", "R");   WJets_files.push_back(file_in_WJets_100to250);
  //TFile* file_in_WJets_250to400 = new TFile(folder_in+"/Arranged_WJets/WJets_250to400.root", "R");   WJets_files.push_back(file_in_WJets_250to400);
  //TFile* file_in_WJets_400to600 = new TFile(folder_in+"/Arranged_WJets/WJets_400to600.root", "R");   WJets_files.push_back(file_in_WJets_400to600);
  //TFile* file_in_WJets_600toInf = new TFile(folder_in+"/Arranged_WJets/WJets_600toInf.root", "R");   WJets_files.push_back(file_in_WJets_600toInf);
  //
  //vector<TFile*> TT_files;
  //TFile* file_in_TT_lowmll = new TFile(folder_in+"/Arranged_TT/TT_inclusive.root", "R");             TT_files.push_back(file_in_TT_lowmll);
  //TFile* file_in_TT_500to800 = new TFile(folder_in+"/Arranged_TT/TT_500to800.root", "R");	     TT_files.push_back(file_in_TT_500to800);
  //TFile* file_in_TT_800to1200 = new TFile(folder_in+"/Arranged_TT/TT_800to1200.root", "R");	     TT_files.push_back(file_in_TT_800to1200);
  //TFile* file_in_TT_1200to1800 = new TFile(folder_in+"/Arranged_TT/TT_1200to1800.root", "R");	     TT_files.push_back(file_in_TT_1200to1800);
  //TFile* file_in_TT_1800toInf = new TFile(folder_in+"/Arranged_TT/TT_1800toInf.root", "R");          TT_files.push_back(file_in_TT_1800toInf);
  //
  //vector<TFile*> WW_files;
  //TFile* file_in_WW_lowmll = new TFile(folder_in+"/Arranged_WW/WW_inclusive.root", "R");             WW_files.push_back(file_in_WW_lowmll);
  //TFile* file_in_WW_200to600 = new TFile(folder_in+"/Arranged_WW/WW_200to600.root", "R");	     WW_files.push_back(file_in_WW_200to600);
  //TFile* file_in_WW_600to1200 = new TFile(folder_in+"/Arranged_WW/WW_600to1200.root", "R");	     WW_files.push_back(file_in_WW_600to1200);
  //TFile* file_in_WW_1200to2500 = new TFile(folder_in+"/Arranged_WW/WW_1200to2500.root", "R");	     WW_files.push_back(file_in_WW_1200to2500);
  //TFile* file_in_WW_2500toInf = new TFile(folder_in+"/Arranged_WW/WW_2500toInf.root", "R");          WW_files.push_back(file_in_WW_2500toInf);
  //
  //TFile* file_in_WZ = new TFile(folder_in+"/Arranged_WZ/WZ.root", "R");
  //TFile* file_in_ZZ = new TFile(folder_in+"/Arranged_ZZ/ZZ.root", "R");
  //
  //TFile* file_in_ST = new TFile(folder_in+"/Arranged_ST/ST.root", "R");
  //
  //
  //TFile* file_in_signal = new TFile(folder_in+"/Arranged_RPV/RPV.root", "R");
  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");
  
  

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
    	    
    vector<TH1F*> h_DY_vector;
    for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
      h_DY_vector.push_back( MC_histo(var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin) ); 
    }
    TH1F* h_DY = (TH1F*) h_DY_vector[0]->Clone("DY_"+var_in);
    for (unsigned int iBin = 1; iBin<h_DY_vector.size(); ++iBin) {
      h_DY->Add(h_DY_vector[iBin]);
    }
    h_DY->Write();
    cout << h_DY << endl;
    
    
    //vector<TH1F*> h_TT_vector;
    //for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
    //  //if (iBin > 0) continue;
    //  h_TT_vector.push_back( MC_histo(var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin) ); 
    //}
    //TH1F* h_TT = (TH1F*) h_TT_vector[0]->Clone("TT_"+var_in);
    //for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
    //  h_TT->Add(h_TT_vector[iBin]);
    //}
    //h_TT->Write();
    //
    //  
    //vector<TH1F*> h_WW_vector;
    //for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
    //  if (iBin>0) continue;
    //  h_WW_vector.push_back( MC_histo(var_in, WW_files[iBin], file_in_data, xs_WW[iBin], rebin) ); 
    //}
    //TH1F* h_WW = (TH1F*) h_WW_vector[0]->Clone("WW_"+var_in);
    //for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
    //  h_WW->Add(h_WW_vector[iBin]);
    //}
    //  
    //TH1F* h_WZ = MC_histo(var_in, file_in_WZ, file_in_data, xs_WZ, rebin);
    //TH1F* h_ZZ = MC_histo(var_in, file_in_ZZ, file_in_data, xs_ZZ, rebin);
    //TH1F* h_VV = (TH1F*) h_WW->Clone("VV_"+var_in);
    //h_VV->Add(h_WZ);
    //h_VV->Add(h_ZZ);
    ////h_VV -> SetName("VV_"+var_in);
    //h_VV->Write();
    //
    //TH1F* h_ST = MC_histo(var_in, file_in_ST, file_in_data, xs_ST, rebin);
    //h_ST->SetName("ST_"+var_in);
    //h_ST->Write();
    
    //TH1F* h_data = (TH1F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
    //h_data -> SetName("data_"+var_in);
    //h_data->Rebin(rebin);
    //h_data->Write();
  }    


  for (unsigned int i = 0; i<vars_TH2.size(); ++i) {
    for (unsigned int j = 0; j<dms.size(); ++j) {
      for (unsigned int k = 0; k<eta.size(); ++k) {
	for (unsigned int l = 0; l<flavor.size(); ++l) {
	  var_in = vars_TH2[i]+"_"+dms[j]+"_"+flavor[l]+"_"+eta[k];
	  cout << endl << endl <<var_in << endl;
        	    
	  vector<TH2F*> h_DY_vector;
	  for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
	    //if (iBin>0) continue; //FIXME
	    h_DY_vector.push_back( MC_histo_TH2(var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin) ); 
	  }
	  TH2F* h_DY = (TH2F*) h_DY_vector[0]->Clone("DY_"+var_in);
	  for (unsigned int iBin = 1; iBin<h_DY_vector.size(); ++iBin) {
	    h_DY->Add(h_DY_vector[iBin]);
	  }
	  h_DY->Write();
	  cout << h_DY << endl;
        
        
          //vector<TH2F*> h_TT_vector;
          //for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
          //  //if (iBin > 0) continue;
          //  h_TT_vector.push_back( MC_histo_TH2(var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin) ); 
          //}
          //TH2F* h_TT = (TH2F*) h_TT_vector[0]->Clone("TT_"+var_in);
          //for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
          //  h_TT->Add(h_TT_vector[iBin]);
          //}
          //h_TT->Write();
          //
          //  
          //vector<TH2F*> h_WW_vector;
          //for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
          //  if (iBin>0) continue;
          //  h_WW_vector.push_back( MC_histo_TH2(var_in, WW_files[iBin], file_in_data, xs_WW[iBin], rebin) ); 
          //}
          //TH2F* h_WW = (TH2F*) h_WW_vector[0]->Clone("WW_"+var_in);
          //for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
          //  h_WW->Add(h_WW_vector[iBin]);
          //}
          //  
          //TH2F* h_WZ = MC_histo_TH2(var_in, file_in_WZ, file_in_data, xs_WZ, rebin);
          //TH2F* h_ZZ = MC_histo_TH2(var_in, file_in_ZZ, file_in_data, xs_ZZ, rebin);
          //TH2F* h_VV = (TH2F*) h_WW->Clone("VV_"+var_in);
          //h_VV->Add(h_WZ);
          //h_VV->Add(h_ZZ);
          ////h_VV -> SetName("VV_"+var_in);
          //h_VV->Write();
          //
          //TH2F* h_ST = MC_histo_TH2(var_in, file_in_ST, file_in_data, xs_ST, rebin);
	  //h_ST->SetName("ST_"+var_in);
          //h_ST->Write();
          
          //TH2F* h_data = (TH2F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
          //h_data -> SetName("data_"+var_in);
          //h_data->RebinX(rebin);
          //h_data->RebinY(rebin);
          //h_data->Write();
	}
      }
    }
  }
  file_out->Close();


  return 0;
}
