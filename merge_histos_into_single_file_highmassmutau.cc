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
  cout << "xs: " << xs << "  Nevents: " << Nevents << endl;
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
  TFile* file_in_DY_100to200 = new TFile(folder_in+"/Arranged_DY/DY_100to200.root", "R");            DY_files.push_back(file_in_DY_100to200);
  TFile* file_in_DY_200to400 = new TFile(folder_in+"/Arranged_DY/DY_200to400.root", "R");            DY_files.push_back(file_in_DY_200to400);
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
  vars.push_back("ev_deltaMET");       
  vars.push_back("ev_Mt");        
  vars.push_back("ev_MET"); 
  vars.push_back("ev_Mcol"); 
  vars.push_back("mu_isolation"); 
  vars.push_back("sign");
  vars.push_back("ev_Nvertex");
  vars.push_back("njet");
  vars.push_back("nbjet");
  //exclusively for TT region now                                                                                                                                                        
  int n_TT_plots = vars.size();
  vars.push_back("bjet_pt");
  vars.push_back("bjet_eta");
  vars.push_back("bjet_phi");


  vector<TString> taun;
  taun.push_back("realtau");

  vector<TString> Mth;
  Mth.push_back("MtLow_OS");
  Mth.push_back("MtLow_SS");
  Mth.push_back("MtHigh");
  Mth.push_back("MtLow_TT");  int k_low_TT= Mth.size()-1;
  Mth.push_back("MtHigh_TT");

  vector<TString> systs;              
  systs.push_back("nominal");
  vector<TString> systs_aux = GetSys();
  for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
    if (systs_aux[iAux] == "topPt") continue;
    systs.push_back(systs_aux[iAux]+"_up");
    systs.push_back(systs_aux[iAux]+"_down");
  }

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
  //double xs_DY_3000toInf = kNNLO*0.0005156;  xs_DY.push_back(xs_DY_3000toInf);


  vector<double> xs_TT;
  //xs_TT.push_back(831.76*0.438); //semilep
  //xs_TT.push_back(831.76*0.457); //had 
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

  double xs_ST = 38.94;
  double xs_WZ = 23.4;
  double xs_ZZ = 10.16;
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

	  if (l < k_low_TT && i >= n_TT_plots) continue;

          var_in = systs[k]+"/"+vars[i]+"_"+taun[j]+"_"+systs[k]+"_"+Mth[l];
          //var_in = systs[k]+"/"+vars[i]+"_"+taun[j]+"_"+systs[k]+Mth[l];
	  if (CR == "CR100" || CR == "CR102") {
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
	  delete h_DY;
          for (unsigned int iBin = 0; iBin<DY_files.size(); ++iBin) delete h_DY_vector[iBin];
          
      	  
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
	  delete h_TT;
          for (unsigned int iBin = 0; iBin<TT_files.size(); ++iBin) delete h_TT_vector[iBin];
          
            
          vector<TH1F*> h_VV_vector;
          for (unsigned int iBin = 0; iBin<VV_files.size(); ++iBin) {
	    //if (iBin > 0) break;
            h_VV_vector.push_back( MC_histo(var_in, VV_files[iBin], file_in_data, xs_VV[iBin], rebin) ); 
          }
          TH1F* h_VV = (TH1F*) h_VV_vector[0]->Clone("VV_"+var_out);
          for (unsigned int iBin = 1; iBin<h_VV_vector.size(); ++iBin) {
            h_VV->Add(h_VV_vector[iBin]);
          }
          h_VV->Write();
	  delete h_VV;
          for (unsigned int iBin = 0; iBin<VV_files.size(); ++iBin) delete h_VV_vector[iBin];
          
          TH1F* h_ST = MC_histo(var_in, file_in_ST, file_in_data, xs_ST, rebin);
          h_ST->SetName("ST_"+var_out);
          h_ST->Write();
	  delete h_ST;
	  
          
          TH1F* h_data = (TH1F*) file_in_data -> Get(var_in);//Data is, by definition, normalized
          h_data -> SetName("data_"+var_out);
          h_data->Rebin(rebin);
          h_data->Write();
	  delete h_data;

	  if (CR == "CR100" || CR == "CR102") {
	    var_in = systs[k]+"_"+vars[i]+"_"+Mth[l];
	    cout << file_in_faketau->GetName() << endl;
	    TH1F* h_faketaus = (TH1F*) file_in_faketau -> Get("faketau_"+var_in);
	    h_faketaus->Write();
	    delete h_faketaus;
	  }
	}
      }
    }
    dir->Close();
  }
  cout << "almost" << endl;
  file_out->Close();
  cout << "end" << endl;
  //for (unsigned int iFile=0; iFile<DY_files.size(); ++iFile) DY_files[iFile]->Close();
  ////for (unsigned int iFile=0; iFile<WJets_files.size(); ++iFile) WJets_files[iFile]->Close();
  //for (unsigned int iFile=0; iFile<TT_files.size(); ++iFile) TT_files[iFile]->Close();
  //for (unsigned int iFile=0; iFile<VV_files.size(); ++iFile) VV_files[iFile]->Close();
  //file_in_faketau->Close();
  //
  //file_in_ST->Close();
  //
  //file_in_data->Close();
  cout << "done" << endl;


  return 0;
}
