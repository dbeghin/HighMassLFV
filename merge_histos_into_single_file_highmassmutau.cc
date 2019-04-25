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
#include "aux.h"
#include "TDirectory.h"


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
  string CR = *(argv + 1);

  TString folder_in = "";
  TString name_out = "";
  if (CR == "CR0" || CR == "CR00") {
    folder_in = "HighMassLFVMuTau/OSisomuisotau_CR0"; 
  }
  else if (CR == "CR1") {
    folder_in = "HighMassLFVMuTau/SSisomuisotau_CR1"; 
  }
  else if (CR == "CR2") {
    folder_in = "HighMassLFVMuTau/OSisomuantitau_CR2"; 
  }
  else if (CR == "CR3") {
    folder_in = "HighMassLFVMuTau/SSisomuantitau_CR3"; 
  }
  else if (CR == "CR4") {
    folder_in = "HighMassLFVMuTau/OSantimuantitau_CR4"; 
  }
  else if (CR == "CR5") {
    folder_in = "HighMassLFVMuTau/SSantimuantitau_CR5";
  }
  else if (CR == "CR7") {
    folder_in = "HighMassLFVMuTau/Wjets_CR7";
  }
  else if (CR == "CR9") {
    folder_in = "HighMassLFVMuTau/Wjets_CR9";
  }
  else if (CR == "CR100") {
    folder_in = "HighMassLFVMuTau/SignalRegion_CR100";
  }
  else if (CR == "CR101") {
    folder_in = "HighMassLFVMuTau/Faketaus_CR101";
    //folder_in = "HighMassLFVMuTau/Faketaus_CR101_unfactorised";
    //folder_in = "HighMassLFVMuTau/Faketaus_CR101_factorised";
  }
  else if (CR == "CR102") {
    folder_in = "HighMassLFVMuTau/WjetsTest_CR102";
  }
  else if (CR == "CR102b") {
    folder_in = "HighMassLFVMuTau/WjetsTest_CR102";
  }
  else if (CR == "CR103") {
    folder_in = "HighMassLFVMuTau/WjetsTest_CR103";
  }
  else {
    cout << "unrecognised argument!!!" << endl;
  }
  name_out = "histos_highmassmutau_"+CR;

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

  TFile* file_in_faketau;
  if (CR == "CR100") {
    file_in_faketau = new TFile("HighMassLFVMuTau/Faketaus_CR0.root", "R");
  }
  else if (CR == "CR102") {
    file_in_faketau = new TFile("HighMassLFVMuTau/Faketaus_CR102.root", "R");
  }

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


  TFile* file_in_signal = new TFile(folder_in+"/Arranged_RPV/RPV.root", "R");
  TFile* file_in_data = new TFile(folder_in+"/Arranged_data/data.root", "R");
  

  vector<TString> vars;
  vars.push_back("ev_Mvis");          
  vars.push_back("ev_Mtot");          
  vars.push_back("tau_pt");           
  vars.push_back("tau_eta");          
  vars.push_back("tau_phi");          
  vars.push_back("mu_pt");            
  vars.push_back("mu_eta");           
  vars.push_back("mu_phi");           
  vars.push_back("ev_DRmutau");       
  vars.push_back("ev_DeltaPhimutau"); 
  vars.push_back("ev_DeltaPhiMETtau");
  vars.push_back("ev_Mt");        
  vars.push_back("ev_MET"); 
  vars.push_back("ev_Mcol"); 
  vars.push_back("mu_isolation"); 
  vars.push_back("sign");
  //if (CR == "CR7") vars.push_back("ev_Mt"); 
  //if (CR == "CR9") vars.push_back("ev_Mt"); 


  vector<TString> taun;
  taun.push_back("realtau");
  if (CR!="CR100") taun.push_back("faketau");

  vector<TString> Mth;
  Mth.push_back("MtLow_OS");
  Mth.push_back("MtLow_SS");
  Mth.push_back("MtHigh");
  Mth.push_back("MtLow_TT");
  Mth.push_back("MtHigh_TT"); //FIXME

  vector<TString> systs;              
  systs.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    systs.push_back(systs_aux[iAux]+"_up");
    systs.push_back(systs_aux[iAux]+"_down");
  }

  //cross-sections
  vector<double> xs_DY;
  double xs_DY_lowmass = 6225.42;          xs_DY.push_back(xs_DY_lowmass);   
  //double xs_DY_lowmass = 5765.4;           xs_DY.push_back(xs_DY_lowmass);   
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
  double xs_TT_lowmll = 87.31;               xs_TT.push_back(xs_TT_lowmll);   
  double xs_TT_500to800 = 0.326;             xs_TT.push_back(xs_TT_500to800); 
  double xs_TT_800to1200 = 3.26e-2;	     xs_TT.push_back(xs_TT_800to1200); 
  double xs_TT_1200to1800 = 3.05e-3;	     xs_TT.push_back(xs_TT_1200to1800); 
  double xs_TT_1800toInf = 1.74e-4;          xs_TT.push_back(xs_TT_1800toInf); 

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

  TString var_in, var_out;

  file_out->cd();
  //options = is it the DY Sig?, variable name, which file to get the histo from, process cross-section
  for (unsigned int k = 0; k<systs.size(); ++k) {
    TDirectory* dir = file_out->mkdir( systs[k] );
    dir->cd();
    for (unsigned int i = 0; i<vars.size(); ++i) {
      for (unsigned int j = 0; j<taun.size(); ++j) {
	for (unsigned int l = 0; l<Mth.size(); ++l) {
          //var_in = systs[k]+"/"+vars[i]+"_"+taun[j]+"_"+systs[k]+"_"+Mth[l];
          var_in = systs[k]+"/"+vars[i]+"_"+taun[j]+"_"+systs[k]+Mth[l];
	  if (CR == "CR100") {
	    var_out = systs[k]+"_"+vars[i]+"_"+Mth[l];
	  }
	  else {
	    var_out = vars[i]+"_"+taun[j]+"_"+systs[k]+"_"+Mth[l];
	  }
          
          cout << endl << endl <<var_in << endl;
          
          vector<TH1F*> h_DY_vector;
          for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) {
            h_DY_vector.push_back( MC_histo(var_in, DY_files[iBin], file_in_data, xs_DY[iBin], rebin) ); 
          }
          TH1F* h_DY = (TH1F*) h_DY_vector[0]->Clone("DY_"+var_out);
          for (unsigned int iBin = 1; iBin<DY_files.size(); ++iBin) {
	    //if (iBin > 2) continue;
            h_DY->Add(h_DY_vector[iBin]);
          }
          h_DY->Write();
          
          if (CR == "CR00") {
            TH1F* h_faketau = (TH1F*) file_in_faketau->Get("faketau_"+var_in);
            h_faketau->Write();
          }
          else {
            
            if (CR == "CR0") {
              TH1F* h_WJets = (TH1F*) file_in_WJets->Get("WJets_"+var_in);
              h_WJets->Write();
            }
            else {
              //vector<TH1F*> h_WJets_vector;
              //for (unsigned int iBin = 0; iBin<WJets_files.size(); ++iBin) {
	      //  h_WJets_vector.push_back( MC_histo(var_in, WJets_files[iBin], xs_WJets[iBin], N_WJets[iBin], rebin) ); 
              //}
              //TH1F* h_WJets = (TH1F*) h_WJets_vector[0]->Clone("WJets_"+var_out);
              //for (unsigned int iBin = 1; iBin<WJets_files.size(); ++iBin) {
	      //  h_WJets->Add(h_WJets_vector[iBin]);
              //}
              //h_WJets->Write();
            }
          }
      	  
          vector<TH1F*> h_TT_vector;
          for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) {
	    //if (iBin == 1 || iBin == 2) continue;
            h_TT_vector.push_back( MC_histo(var_in, TT_files[iBin], file_in_data, xs_TT[iBin], rebin) ); 
          }
          TH1F* h_TT = (TH1F*) h_TT_vector[0]->Clone("TT_"+var_out);
          for (unsigned int iBin = 1; iBin<h_TT_vector.size(); ++iBin) {
            h_TT->Add(h_TT_vector[iBin]);
          }
          h_TT->Write();
          
            
          vector<TH1F*> h_WW_vector;
          for (unsigned int iBin = 0; iBin<WW_files.size(); ++iBin) {
	    //if (iBin > 0) break;
            h_WW_vector.push_back( MC_histo(var_in, WW_files[iBin], file_in_data, xs_WW[iBin], rebin) ); 
          }
          TH1F* h_WW = (TH1F*) h_WW_vector[0]->Clone("WW_"+var_out);
          for (unsigned int iBin = 1; iBin<h_WW_vector.size(); ++iBin) {
            h_WW->Add(h_WW_vector[iBin]);
          }
          
            
          TH1F* h_WZ = MC_histo(var_in, file_in_WZ, file_in_data, xs_WZ, rebin);
          TH1F* h_ZZ = MC_histo(var_in, file_in_ZZ, file_in_data, xs_ZZ, rebin);
          TH1F* h_VV = (TH1F*) h_WW->Clone("VV_"+var_out);
          h_VV->Add(h_WZ);
          h_VV->Add(h_ZZ);
          //h_VV -> SetName("VV_"+var_in);
          h_VV->Write();
          
          TH1F* h_ST = MC_histo(var_in, file_in_ST, file_in_data, xs_ST, rebin);
          h_ST->SetName("ST_"+var_out);
          h_ST->Write();
	  
	  if (CR != "CR101") {
	    //TH1F* h_signal = MC_histo(var_in, file_in_signal, xs_signal, N_signal, rebin);
	    //h_signal->SetName("Signal_"+var_out);
	    //h_signal->Write();
	    //FIXME
	  }
          
          TH1F* h_data = (TH1F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
          h_data -> SetName("data_"+var_out);
          h_data->Rebin(rebin);
          h_data->Write();

	  if (j>0) continue;
          var_in = systs[k]+"_"+vars[i]+"_"+Mth[l];
	  cout << file_in_faketau->GetName() << endl;
	  TH1F* h_faketaus = (TH1F*) file_in_faketau -> Get("faketau_"+var_in);
	  h_faketaus->Write();
	}
      }
    }
    dir->Close();
  }
  cout << "done" << endl;
  for (unsigned int iFile=0; iFile<DY_files.size(); ++iFile) DY_files[iFile]->Close();
  for (unsigned int iFile=0; iFile<WJets_files.size(); ++iFile) WJets_files[iFile]->Close();
  for (unsigned int iFile=0; iFile<TT_files.size(); ++iFile) TT_files[iFile]->Close();
  for (unsigned int iFile=0; iFile<WW_files.size(); ++iFile) WW_files[iFile]->Close();
  file_in_faketau->Close();

  file_in_ST->Close();
  file_in_WZ->Close();
  file_in_ZZ->Close();

  file_in_data->Close();

  file_out->Close();

  return 0;
}
