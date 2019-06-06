#include "TH1.h"
#include "TH2.h"
#include <vector>
#include <iostream>
#include "TString.h"
#include "TLorentzVector.h"
#include "PU_reWeighting.cc"
#include "GeneralizedEndpoint.cc"
#include "TGraphErrors.h"

using namespace std;

float norm_F(float x, float y){

  return sqrt(x*x+y*y);

}

vector<TString> GetSys() {
  vector<TString> systematics;
  systematics.push_back("TES");
  systematics.push_back("MES");
  systematics.push_back("mres");
  systematics.push_back("minbias");
  systematics.push_back("muonID");
  systematics.push_back("muonIso");
  systematics.push_back("tracking");
  systematics.push_back("trigger");
  systematics.push_back("tauID");
  systematics.push_back("eletauFR");
  systematics.push_back("mutauFR");
  systematics.push_back("FRstat");
  systematics.push_back("FRsys");
  systematics.push_back("topPt");

  return systematics;
}


TString GetEtaString(float eta) {
  TString eta_string = "";
  if (fabs(eta) < 1.46) {
    eta_string = "barrel";
  }
  else if (fabs(eta) > 1.56) {
    eta_string = "endcap";
  }
  else {
    eta_string = "";
  }
  return eta_string;
}


//TString GetEtaString(float eta) {
//  TString eta_string = "";
//  if (fabs(eta) < 1.) {
//    eta_string = "barrel";
//  }
//  else if (fabs(eta) < 1.46) {
//    eta_string = "transition";
//  }
//  else if (fabs(eta) > 1.56) {
//    eta_string = "endcap";
//  }
//  else {
//    eta_string = "";
//  }
//  return eta_string;
//}


double GetPUWeight(int PU, TString var) {
  double pu_reweight = 0;
  if (var == "nom") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup", "Data_pileup_normalized");
  else if (var == "up") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup","Data_pileup_normalized_UP");
  else if (var == "down") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup","Data_pileup_normalized_DOWN");
  else cout << "PU reweight error" << endl;

  return pu_reweight;
}


double GetHighPtIDWeight(TLorentzVector mu_p4, TString var) {
  TFile* ID_file_1 = new TFile("Reweighting/RunBCDEF_SF_ID.root","R");
  TFile* ID_file_2 = new TFile("Reweighting/RunGH_SF_ID.root","R");

  float mu_pt = mu_p4.Pt();
  float mu_eta = mu_p4.Eta();

  //scale factors
  //ID
  TH2F* ID_histo_1 = (TH2F*) ID_file_1->Get("NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt");
  int nbinsY = ID_histo_1->GetNbinsY();
  if (mu_pt > ID_histo_1->GetYaxis()->GetBinLowEdge(nbinsY) + ID_histo_1->GetYaxis()->GetBinWidth(nbinsY)) mu_pt = ID_histo_1->GetYaxis()->GetBinLowEdge(nbinsY) + ID_histo_1->GetYaxis()->GetBinWidth(nbinsY)/2.0;
  int bin_in = ID_histo_1->FindBin(mu_eta, mu_pt);
  double highPtID_sf_1 = ID_histo_1->GetBinContent(bin_in);
  double error_1 = pow(ID_histo_1->GetBinError(bin_in),2);
  double error_highpt = 0;
  if (var == "down") {
    if (fabs(mu_eta) < 1.6) {
      if (mu_p4.P() > 100) {
	//data eff modeled as flat, divide it by MC to get SF, get the difference wrt to the nominal SF
	error_highpt = fabs( ( 0.9936 - 3.71e-6*mu_p4.P() ) - highPtID_sf_1); 
      }
    }
    else if (fabs(mu_eta) < 2.4) {
      if (mu_p4.P() > 200) {
	//data and MC eff vary here, get the difference wrt to the nominal SF
	error_highpt = fabs( ( 0.9784 - 4.73e-5*mu_p4.P() ) / ( 0.9908 - 1.26e-5*mu_p4.P() ) - highPtID_sf_1); 
      }
    }
  }
  error_1 += pow(error_highpt,2);
  error_1 = sqrt(error_1);
  double lumi_1 = 20.0; //luminosity of Runs BCDEF         


  TH2F* ID_histo_2 = (TH2F*) ID_file_2->Get("NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt");
  bin_in = ID_histo_2->FindBin(mu_eta, mu_pt);
  double highPtID_sf_2 = ID_histo_2->GetBinContent(bin_in);
  double error_2 = pow(ID_histo_2->GetBinError(bin_in),2);
  error_highpt = 0;
  if (var == "down") {
    if (fabs(mu_eta) < 1.6) {
      if (mu_p4.P() > 100) {
	//data eff modeled as flat, divide it by MC to get SF, get the difference wrt to the nominal SF
	error_highpt = fabs(  ( 0.9936 - 3.71e-6*mu_p4.P() ) - highPtID_sf_2); 
      }
    }
    else if (fabs(mu_eta) < 2.4) {
      if (mu_p4.P() > 200) {
	//data and MC eff vary here, get the difference wrt to the nominal SF
	error_highpt = fabs( ( 0.9784 - 4.73e-5*mu_p4.P() ) / ( 0.9908 - 1.26e-5*mu_p4.P() ) - highPtID_sf_2); 
      }
    }
  }
  error_2 += pow(error_highpt,2);
  error_2 = sqrt(error_2);
  double lumi_2 = 16.0; //luminosity of Runs GH            

  double highPtID_sf = (lumi_1*highPtID_sf_1 + lumi_2*highPtID_sf_2) / (lumi_1 + lumi_2);
  double error = (lumi_1*error_1 + lumi_2*error_2) / (lumi_1 + lumi_2);

  double weight = 0;
  if (var == "nom") {
    weight = highPtID_sf;
  }
  else if (var == "up") {
    weight = highPtID_sf + error;
  }
  else if (var == "down") {
    weight = highPtID_sf - error;
  }

  ID_file_1->Close("R");
  ID_file_2->Close("R");

  return weight;
}


double GetTkLooseIsoWeight(float mu_pt, float mu_eta, TString var) {
  TFile* Iso_file_1 = new TFile("Reweighting/RunBCDEF_SF_ISO.root","R");
  TFile* Iso_file_2 = new TFile("Reweighting/RunGH_SF_ISO.root","R");
  double lumi_1 = 20.0; //luminosity of Runs BCDEF         
  double lumi_2 = 16.0; //luminosity of Runs GH            

  TH2F* Iso_histo_1 = (TH2F*) Iso_file_1->Get("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt");
  int nbinsY = Iso_histo_1->GetNbinsY();
  if (mu_pt > Iso_histo_1->GetYaxis()->GetBinLowEdge(nbinsY) + Iso_histo_1->GetYaxis()->GetBinWidth(nbinsY)) mu_pt = Iso_histo_1->GetYaxis()->GetBinLowEdge(nbinsY) + Iso_histo_1->GetYaxis()->GetBinWidth(nbinsY)/2.0;
  cout << "mu_pt < 120 GeV? " << mu_pt << endl;
  int bin_in = Iso_histo_1->FindBin(mu_eta, mu_pt);
  double tkLooseISO_sf_1 = Iso_histo_1->GetBinContent(bin_in);

  TH2F* Iso_histo_2 = (TH2F*) Iso_file_2->Get("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt");
  bin_in = Iso_histo_2->FindBin(mu_eta, mu_pt);
  double tkLooseISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //TkLoose iso : < 0.1

  double tkLooseISO_sf = (lumi_1*tkLooseISO_sf_1 + lumi_2*tkLooseISO_sf_2) / (lumi_1 + lumi_2);
  double weight_plus = (1+sqrt(pow(.005,2) + pow(.02,2)))*tkLooseISO_sf; //stat 0.5% syst 2%
  double weight_minus = (1-sqrt(pow(.005,2) + pow(.02,2)))*tkLooseISO_sf; //stat 0.5% syst 2%

  double weight = 0;
  if (var=="nom") weight = tkLooseISO_sf;
  else if (var=="up") weight = weight_plus;
  else if (var=="down") weight = weight_minus;

  Iso_file_1->Close("R");
  Iso_file_2->Close("R");
  return weight;
}


double GetTrackingWeight(float mu_pt, float mu_eta, TString var) {
  if (mu_pt > 120) mu_pt = 119;
  TFile* Tracker_file = new TFile("Reweighting/Tracking_EfficienciesAndSF_BCDEFGH.root","R");
  TGraphErrors* Tracker_graph = (TGraphErrors*) Tracker_file->Get("ratio_eff_eta3_dr030e030_corr");
  double lumi_1 = 20.0; //luminosity of Runs BCDEF         
  double lumi_2 = 16.0; //luminosity of Runs GH            

  double tracker_sf = Tracker_graph->Eval(mu_eta); //evaluate the graph function at this value of eta
  double weight_other = 0;
  double factor = 1;
  if (mu_pt < 300) factor = .005;
  else factor = .01;

  if (var=="up") weight_other = tracker_sf*(1+factor);
  else if (var=="down") weight_other = tracker_sf*(1-factor);

  double weight = 0;
  if (var=="nom") weight = tracker_sf;
  else if (var=="up") weight = weight_other;
  else if (var=="down") weight = weight_other;
  
  Tracker_file->Close("R");
  return weight;
}


double GetTriggerWeight(float mu_pt, float mu_eta, TString var) {
  TFile* Trigger_file_1 = new TFile("Reweighting/EfficienciesAndSF_RunBtoF.root","R");
  TFile* Trigger_file_2 = new TFile("Reweighting/EfficienciesAndSF_Period4.root","R");
  double lumi_1 = 20.0; //luminosity of Runs BCDEF         
  double lumi_2 = 16.0; //luminosity of Runs GH            

  TH1F* Trigger_histo_1 = (TH1F*) Trigger_file_1->Get("Mu50_OR_TkMu50_EtaBins/eta_ratio");
  int bin_in = Trigger_histo_1->FindBin(mu_eta);
  double trigger_sf_1 = Trigger_histo_1->GetBinContent(bin_in);

  TH1F* Trigger_histo_2 = (TH1F*) Trigger_file_2->Get("Mu50_OR_TkMu50_EtaBins/eta_ratio");
  bin_in = Trigger_histo_2->FindBin(mu_eta);
  double trigger_sf_2 = Trigger_histo_2->GetBinContent(bin_in);

  double trigger_sf =  (lumi_1*trigger_sf_1 + lumi_2*trigger_sf_2) / (lumi_1 + lumi_2);
  double weight_other = 0;
  double factor_up = 1;
  double factor_down = 1;
  if (mu_pt < 300) factor_up = .02, factor_down = .02; //normal syst
  else  factor_up = .02, factor_down = 0.06;
  //add prefiring syst
  factor_up = 1+sqrt(pow(factor_up,2) + pow(.02,2));
  factor_down = 1-sqrt(pow(factor_down,2) + pow(.02,2));

  if (var=="up") weight_other = trigger_sf*factor_up;
  else if (var=="down") weight_other = trigger_sf*factor_down;

  double weight = 0;
  if (var=="nom") weight = trigger_sf;
  else if (var=="up") weight = trigger_sf*factor_up;
  else if (var=="down") weight = trigger_sf*factor_down;

  Trigger_file_1->Close("R");
  Trigger_file_2->Close("R");
  return weight;

}


double GetEleTriggerWeight(float ele_pt, float ele_eta) {
  if (ele_pt > 1000) ele_pt = 999;
  TFile* Trigger_file = new TFile("/user/amkalsi/CMSSW_8_0_17/src/Plots/LowMTRegion/python/data/TriggerEff.root","R");

  TH1F* Trigger_histo = (TH1F*) Trigger_file->Get("hEff_Ele27OR115OR175");
  int bin_in = Trigger_histo->FindBin(fabs(ele_eta), ele_pt);
  double trigger_sf = Trigger_histo->GetBinContent(bin_in);

  Trigger_file->Close("R");
  return trigger_sf;

}



double GetTightTauIDWeight(float tau_pt, TString lepton, TString var) {
  if (lepton != "tau") {
    return 1;
  }
  else {
    double base_weight = 0.95;
    double factor_up   = 1+sqrt( pow(.05,2) + pow(0.05*tau_pt/1000,2) );
    double factor_down = 1-sqrt( pow(.05,2) + pow(-0.35*tau_pt/1000,2) );

    double weight = 0;
    if (var=="nom") weight = base_weight;
    else if (var=="up") weight = base_weight*factor_up;
    else if (var=="down") weight = base_weight*factor_down;
    return weight;
  }
}


double GetEleTauFR(float eta, TString lepton, TString var) {
  if (lepton != "ele") {
    return 1;
  }
  else {
    double weight = 1;
    
    if (var=="nom") {
      if (fabs(eta) < 1.460) weight = 1.21;
      else if (fabs(eta) > 1.558) weight = 1.38;
    }
    else if (var=="up") {
      if (fabs(eta) < 1.460) weight = 1.27;
      else if (fabs(eta) > 1.558) weight = 1.42;
    }
    else if (var=="down") {
      if (fabs(eta) < 1.460) weight = 1.15;
      else if (fabs(eta) > 1.558) weight = 1.34;
    }

    return weight;
  }
}


double GetMuTauFR(float eta, TString lepton, TString var) {
  if (lepton != "mu") {
    return 1;
  }
  else {
    double weight = 1;
    
    if (var=="nom") {
      if (fabs(eta) < 0.4) weight = 1.47;
      else if (fabs(eta) < 0.8 && fabs(eta) > 0.4) weight = 1.55;
      else if (fabs(eta) < 1.2 && fabs(eta) > 0.8) weight = 1.33;
      else if (fabs(eta) < 1.7 && fabs(eta) > 1.2) weight = 1.72;
      else if (fabs(eta) < 2.3 && fabs(eta) > 1.7) weight = 2.50;
    }
    else if (var=="up") {
      if (fabs(eta) < 0.4) weight = 1.63;
      else if (fabs(eta) < 0.8 && fabs(eta) > 0.4) weight = 1.85;
      else if (fabs(eta) < 1.2 && fabs(eta) > 0.8) weight = 1.38;
      else if (fabs(eta) < 1.7 && fabs(eta) > 1.2) weight = 2.26;
      else if (fabs(eta) < 2.3 && fabs(eta) > 1.7) weight = 3.13;
    }
    else if (var=="down") {
      if (fabs(eta) < 0.4) weight = 1.31;
      else if (fabs(eta) < 0.8 && fabs(eta) > 0.4) weight = 1.25;
      else if (fabs(eta) < 1.2 && fabs(eta) > 0.8) weight = 1.28;
      else if (fabs(eta) < 1.7 && fabs(eta) > 1.2) weight = 1.18;
      else if (fabs(eta) < 2.3 && fabs(eta) > 1.7) weight = 1.87;
    }

    return weight;
  }
}


double FakeRate_unfactorised(double taupt, double taueta, double ratio, TString sys, TString var) {
  if (taupt >= 1000) taupt = 999;
  if (ratio >= 2) ratio = 1.9;
  cout << "fake rate: " << sys << "  var: " << var << endl;
  if (sys != "FRstat" && sys != "FRsys") var = "nom";
  cout << "fake rate: " << sys << "  var: " << var << endl;

  TFile* fake_file = new TFile("Reweighting/fakerate_unfactorised_MtLow.root","R");

  TString eta_string = GetEtaString(taueta);
  if (eta_string == "") {
    fake_file->Close("R");
    return 0;
  }

  TString hname = sys+"_"+eta_string;
  if (taupt > 150) {
    hname += "_taupt_150_1000";
  }
  else {
    hname += "_taupt_0_150";
  }

  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPtAndRatio_"+hname);
  int iBin = h_taupt->FindBin(taupt, ratio);
  double base_SF = h_taupt->GetBinContent(iBin);
  double error = h_taupt->GetBinError(iBin);
  
  double weight = 0;
  if (var=="nom") weight = base_SF;
  else if (var=="up") weight = base_SF+error;
  else if (var=="down") weight = base_SF-error;

  fake_file->Close("R");

  return weight;
}



double FakeRate_DY(double taupt, double taueta, double ratio, TString sys, TString var) {
  if (taupt >= 1000) taupt = 999;
  if (ratio >= 2) ratio = 1.9;
  cout << "fake rate: " << sys << "  var: " << var << endl;
  if (sys != "FRstat" && sys != "FRsys") var = "nom";
  cout << "fake rate: " << sys << "  var: " << var << endl;
  
  TFile* fake_file = new TFile("Reweighting/fakerate_unfactorised_DY.root","R");
  
  //TString eta_string = GetEtaString(taueta);
  //TString hname = "eta_"+eta_string;

  TString hname = "total";
  if (taupt > 150) {
    hname += "_taupt_150_1000";
  }
  else {
    hname += "_taupt_0_150";
  }
  
  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPtAndRatio_"+hname);
  int iBin = h_taupt->FindBin(taupt, ratio);
  double DY_SF = h_taupt->GetBinContent(iBin);
  double norm_SF = FakeRate_unfactorised(taupt,taueta,ratio,sys,"nom");
  
  double weight = 0;
  if (var=="nom") weight = 1;
  else if (var=="up") {
    if (norm_SF != 0) weight = DY_SF/norm_SF;
  }
  else if (var=="down") {
    if (norm_SF != 0) weight = (2*norm_SF-DY_SF)/norm_SF;
  }
  return weight;
}



double GetTopPtWeightUnc(float top_pt_in) {
  double weight = 0.0;
  if (top_pt_in < 0.0) {
    weight = 0.0;
  } else if (top_pt_in < 150.0) {
    weight = 0.045;
  } else if (top_pt_in < 1000.0) {
    weight = 0.04 * top_pt_in/1000.0 + 0.045;
  } else if (top_pt_in < 1100.0) {
    weight = 0.09;
  } else if (top_pt_in < 1200.0) {
    weight = 0.1;
  } else if (top_pt_in < 1400.0) {
    weight = 0.12;
  } else if (top_pt_in < 1600.0) {
    weight = 0.14;
  } else if (top_pt_in < 1800.0) {
    weight = 0.155;
  } else if (top_pt_in < 2000.0) {
    weight = 0.18;
  } else if (top_pt_in < 2200.0) {
    weight = 0.2;
  } else if (top_pt_in < 2600.0) {
    weight = 0.243;
  } else if (top_pt_in < 3000.0) {
    weight = 0.34;
  } else if (top_pt_in > 2999.9) {
    weight = 0.34;
  }
  return weight;
}



double GetTopPtWeight(float top_pt_1, float top_pt_2, TString var){
  if (top_pt_1 < 0 || top_pt_2 <0) {
    return 1;
  }
  else {
    double tmp_t1 = exp(0.0615-0.0005*top_pt_1);
    double tmp_t2 = exp(0.0615-0.0005*top_pt_2);
    double tmp_t1_uncer = GetTopPtWeightUnc(top_pt_1);
    double tmp_t2_uncer = GetTopPtWeightUnc(top_pt_2);
    
    double w_top_up = sqrt(tmp_t1*(1.0 + tmp_t1_uncer)*tmp_t2*(1.0 + tmp_t2_uncer) );
    double w_top_down = sqrt(tmp_t1*(1.0 - tmp_t1_uncer)*tmp_t2*(1.0 - tmp_t2_uncer) );
    double w_top_nom = sqrt(tmp_t1 * tmp_t2);
    
    double weight = 0;
    if (var=="nom") weight = w_top_nom;
    else if (var=="up") weight = w_top_up;
    else if (var=="down") weight = w_top_down;
    
    return weight;
  }
}


double GeneralWeightFunction(TString sys, int n_vert, TLorentzVector tau_p4, float ratio, TLorentzVector mu_p4, TString lepton, float top_pt_1, float top_pt_2, TString var) {
  vector<TString> systematics = GetSys();

  bool match = false;
  for (unsigned int i=0; i<systematics.size(); ++i) {
    if (sys==systematics[i]) {
      match = true;
      break;
    }
  }

  if (!match) {
    cout << endl << endl <<  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "systematic [[ " << sys << " ]] not recognized" << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl << endl;
    return 0;
  }
  else {
    float mu_pt = mu_p4.Pt();
    float mu_eta = mu_p4.Eta();
    float tau_pt = tau_p4.Pt();
    float tau_eta = tau_p4.Eta();

    float weight = 0;

    if (n_vert>=0) {
      //this is MC
      if (sys == "minbias") weight = GetPUWeight(n_vert,var);
      else if (sys == "muonID") weight = GetHighPtIDWeight(mu_p4,var);
      else if (sys == "muonIso") weight = GetTkLooseIsoWeight(mu_pt,mu_eta,var);
      else if (sys == "tracking") weight = GetTrackingWeight(mu_pt,mu_eta,var);
      else if (sys == "trigger") weight = GetTriggerWeight(mu_pt,mu_eta,var);
      else if (sys == "tauID") weight = GetTightTauIDWeight(tau_pt,lepton,var);
      else if (sys == "eletauFR") weight = GetEleTauFR(tau_eta,lepton,var);
      else if (sys == "mutauFR") weight = GetMuTauFR(tau_eta,lepton,var);
      else if (sys == "FRstat") weight = FakeRate_unfactorised(tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "FRsys") weight = FakeRate_DY(tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "topPt") weight = GetTopPtWeight(top_pt_1,top_pt_2,var); 
      else if (sys == "eletrigger") weight = GetEleTriggerWeight(mu_pt,mu_eta); 
    }
    else {
      //this is data
      weight = 1;
      if (sys == "FRstat") weight = FakeRate_unfactorised(tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "FRsys") weight = FakeRate_DY(tau_pt,tau_eta,ratio,sys,var);
    }

    if (weight != weight) weight = 0;
    if (weight < 0) weight = 0;
    return weight;
  }
}


double FakeRate_noratio(double taupt, TString eta) {
  double SF=0.2;
  if (taupt >= 1000) taupt = 999;

  TFile* fake_file = new TFile("Reweighting/fakerate_MtLow.root","R");

  double reweight = 0;

  TString hname = "eta_"+eta;
  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPt_"+hname);
  int iBin = h_taupt->FindBin(taupt);
  double base_SF = h_taupt->GetBinContent(iBin);
  
  SF = base_SF;
  reweight = SF;

  return reweight;
}



double FakeRateHigh_unfactorised(double taupt, double ratio, TString eta) {
  double SF=0.2;
  if (taupt >= 1000) taupt = 999;
  if (ratio >= 2) ratio = 1.9;

  TFile* fake_file = new TFile("Reweighting/fakerate_unfactorised_MtLow.root","R");

  double reweight = 0;

  TString hname = "eta_"+eta;
  if (taupt > 150) {
    hname += "_taupt_150_1000";
  }
  else {
    hname += "_taupt_0_150";
  }

  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPtAndRatio_"+hname);
  int iBin = h_taupt->FindBin(taupt, ratio);
  double base_SF = h_taupt->GetBinContent(iBin)+h_taupt->GetBinError(iBin);
  
  SF = base_SF;
  reweight = SF;

  return reweight;
}





double FakeRate_factorised(double taupt, double ratio, TString eta) {
  double SF=0.2;
  if (taupt >= 1000) taupt = 999;
  if (ratio >= 2) ratio = 1.9;

  TFile* fake_file = new TFile("Reweighting/fakerate_MtLow.root","R");

  double reweight = 0;

  TString hname = "eta_"+eta;
  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPt_"+hname);
  int iBin = h_taupt->FindBin(taupt);
  double base_SF = h_taupt->GetBinContent(iBin);
  
  TH1F* h_corr = (TH1F*) fake_file->Get("RatioCorrectionFactor_"+hname);
  iBin = h_corr->FindBin(ratio);
  double corr_factor = h_corr->GetBinContent(iBin);

  SF = corr_factor*base_SF;
  //cout << endl << "taupt " << taupt << endl;
  //cout << "ratio " << ratio << endl;
  //cout << eta << endl;
  //cout << base_SF << "*" << corr_factor << endl;
  reweight = SF;

  return reweight;
}


double FakeRate_SSMtLow(double taupt, double jetpt, TString eta) {
  double SF=0.2;

  TFile* fake_file = new TFile("Reweighting/fakerate_SSMtLow.root","R");

  TString sector_string = "_taupt_";
  
  if (jetpt >= 1000) jetpt = 999;
  if (taupt >= 1000) taupt = 999;

  if ( (taupt > 0) && (taupt < 300) ) {
    sector_string += "0_300_jetpt_";
    
    if ( (jetpt > 0) && (jetpt < 150) )          sector_string += "0_150";
    else if ( (jetpt >= 150) && (jetpt < 300) )  sector_string += "150_300";
    else if ( (jetpt >= 300) && (jetpt < 1000) ) sector_string += "300_1000";
  }
  else if ( (taupt > 300) && (taupt < 1000) ) {
    sector_string += "300_1000_jetpt_";
    
    if ( (jetpt > 0) && (jetpt < 300) )          sector_string += "0_300";
    else if ( (jetpt >= 300) && (jetpt < 1000) ) sector_string += "300_1000";
  }

  double reweight = 0;

  TString hname = "hratio_data_eta_"+eta+"_taupt_jetpt_pass" + sector_string;
  TH2F* h_fake = (TH2F*) fake_file->Get(hname);
  int iBin = h_fake->FindBin(taupt, jetpt);
  SF = h_fake->GetBinContent(iBin);
  if (SF != 1) reweight = SF/(1-SF);

  return reweight;

}


double GetCollinearMass(TLorentzVector tau, TLorentzVector mu,  TLorentzVector MET) {

  double METproj=fabs((MET.Px()*tau.Px()+MET.Py()*tau.Py())/tau.Pt());
  double xth=1;
  if((tau.Pt()+METproj)!=0) xth=tau.Pt()/(tau.Pt()+METproj);
  double mass_vis=(tau+mu).M();
  double mcol = 0;
  if (mass_vis != mass_vis) mass_vis=0;
  if (mass_vis<0) mass_vis = 0;

  if (xth>0) mcol=mass_vis/sqrt(xth);
  else mcol=0;

  return mcol;
}


pair<TLorentzVector,TLorentzVector> TauEnergyScale(TLorentzVector tau_p4, TLorentzVector met_p4, TString var) {
  TLorentzVector tau_TES_p4;
  tau_TES_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_TES_p4;
  met_TES_p4.SetPxPyPzE(0,0,0,0);
  
  float scale = 0.03; //3 percent for high pt taus
  if (var=="nom")  tau_TES_p4 = tau_p4;
  else if (var=="up")   tau_TES_p4 = tau_p4*(1+scale);
  else if (var=="down") tau_TES_p4 = tau_p4*(1-scale);

  met_TES_p4 = met_p4 - tau_TES_p4 + tau_p4;

  pair<TLorentzVector,TLorentzVector> tes;
  tes.first = tau_TES_p4;
  tes.second = met_TES_p4;
  return tes;
}


pair<TLorentzVector,TLorentzVector> MuResolution(TLorentzVector mu_p4, TLorentzVector met_p4, TString var) {
  TLorentzVector mu_res_p4;
  mu_res_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_res_p4;
  met_res_p4.SetPxPyPzE(0,0,0,0);
  
  float scale = 0;
  TString eta_string = GetEtaString(mu_p4.Eta());
  if (eta_string == "barrel") {
    if (mu_p4.Pt() < 200) scale = 0.003;
    else if (mu_p4.Pt() < 500) scale = 0.005;
    else scale = 0.01;
  }
  else if (eta_string == "endcap") {
    if (mu_p4.Pt() < 200) scale = 0.006;
    else if (mu_p4.Pt() < 500) scale = 0.01;
    else scale = 0.02;
  }

  if (var=="nom")  mu_res_p4 = mu_p4;
  else if (var=="up")   mu_res_p4 = mu_p4*(1+scale);
  else if (var=="down") mu_res_p4 = mu_p4*(1-scale);

  met_res_p4 = met_p4 - mu_res_p4 + mu_p4;

  pair<TLorentzVector,TLorentzVector> mes;
  mes.first = mu_res_p4;
  mes.second = met_res_p4;
  return mes;
}


pair<TLorentzVector,TLorentzVector> MuEnergyScale(TLorentzVector mu_p4, int mu_charge, TLorentzVector met_p4, TString var) {
  TLorentzVector mu_mes_p4;
  mu_mes_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_mes_p4;
  met_mes_p4.SetPxPyPzE(0,0,0,0);
  
  int mode = 0, verbose = 1;

  if (var=="nom")  mode = 0;
  else if (var=="up")   mode = 1;
  else if (var=="down") mode = 2;

  GeneralizedEndpoint* g = new GeneralizedEndpoint();
  float mu_mes_pt = 0;
  float mu_mass = 0.105;
  mu_mes_pt = g->GeneralizedEndpointPt(mu_p4.Pt(), mu_charge, mu_p4.Eta(), mu_p4.Phi(), mode, verbose);
  mu_mes_p4.SetPtEtaPhiM(mu_mes_pt,mu_p4.Eta(),mu_p4.Phi(),mu_mass);
  met_mes_p4 = met_p4 - mu_mes_p4 + mu_p4;

  pair<TLorentzVector,TLorentzVector> mes;
  mes.first = mu_mes_p4;
  mes.second = met_mes_p4;
  return mes;
}


vector<TLorentzVector> GetScaleVariation(TString syst, TString tau_gen, TString mu_gen, TLorentzVector tau_p4, TLorentzVector mu_p4, int mu_charge, TLorentzVector met_p4) {
  TLorentzVector tau_newp4, mu_newp4, met_newp4;

  TString var = "nom";
  if (syst.Contains("up")) var = "up";
  else if (syst.Contains("down")) var = "down";

  if (syst.Contains("TES")) {
    if (tau_gen != "tau") var = "nom";
    tau_newp4 = TauEnergyScale(tau_p4,met_p4,var).first, met_newp4 = TauEnergyScale(tau_p4,met_p4,var).second;
    mu_newp4 = mu_p4;
  }
  else if (syst.Contains("MES")) {
    if (mu_gen != "mu") var = "nom";
    mu_newp4 = MuEnergyScale(mu_p4,mu_charge,met_p4,var).first, met_newp4 = MuEnergyScale(mu_p4,mu_charge,met_p4,var).second;
    tau_newp4 = tau_p4;
  }
  else if (syst.Contains("mres")) {
    if (mu_gen != "mu") var = "nom";
    mu_newp4 = MuResolution(mu_p4,met_p4,var).first, met_newp4 = MuResolution(mu_p4,met_p4,var).second;
    tau_newp4 = tau_p4;
  }

  vector<TLorentzVector> newp4;
  newp4.push_back(tau_newp4);
  newp4.push_back(mu_newp4);
  newp4.push_back(met_newp4);

  return newp4;
}


double GetReweight_mumu(int PU, float mu1_pt, float mu1_eta, float mu2_pt, float mu2_eta) {
  if (mu1_pt >= 120) mu1_pt = 100;
  if (mu2_pt >= 120) mu2_pt = 100;
  //scale factor files that need to be open
  TFile* ID_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_id.root","R");
  TFile* ID_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_id.root","R");
  TFile* Iso_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_iso.root","R");
  TFile* Iso_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_iso.root","R");
  TFile* Trigger_file_1 = new TFile("Reweighting/EfficienciesAndSF_RunBtoF.root","R");
  TFile* Trigger_file_2 = new TFile("Reweighting/EfficienciesAndSF_Period4.root","R");
  TFile* Tracker_file = new TFile("Reweighting/Tracking_EfficienciesAndSF_BCDEFGH.root","R");


  //PU reweight
  double pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup", "Data_pileup_normalized");


  //scale factors
  //ID
  TH2F* ID_histo_1 = (TH2F*) ID_file_1->Get("MC_NUM_MediumID2016_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  int bin_in = ID_histo_1->FindBin(mu1_pt, fabs(mu1_eta));
  double mediumID2016_sf_1 = ID_histo_1->GetBinContent(bin_in);
  double lumi_1 = 20.0; //luminosity of Runs BCDEF

  TH2F* ID_histo_2 = (TH2F*) ID_file_2->Get("MC_NUM_MediumID2016_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  bin_in = ID_histo_2->FindBin(mu1_pt, fabs(mu1_eta));
  double mediumID2016_sf_2 = ID_histo_2->GetBinContent(bin_in);
  double lumi_2 = 16.0; //luminosity of Runs GH                

  double mediumID2016_sf_mu1 = (lumi_1*mediumID2016_sf_1 + lumi_2*mediumID2016_sf_2) / (lumi_1 + lumi_2);


  bin_in = ID_histo_1->FindBin(mu2_pt, fabs(mu2_eta));
  mediumID2016_sf_1 = ID_histo_1->GetBinContent(bin_in);

  bin_in = ID_histo_2->FindBin(mu2_pt, fabs(mu2_eta));
  mediumID2016_sf_2 = ID_histo_2->GetBinContent(bin_in);


  double mediumID2016_sf_mu2 = (lumi_1*mediumID2016_sf_1 + lumi_2*mediumID2016_sf_2) / (lumi_1 + lumi_2);
  double mediumID2016_sf = mediumID2016_sf_mu1*mediumID2016_sf_mu2;


  //Isolation
  TH2F* Iso_histo_1 = (TH2F*) Iso_file_1->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  bin_in = Iso_histo_1->FindBin(mu1_pt, fabs(mu1_eta));
  double tightISO_sf_1 = Iso_histo_1->GetBinContent(bin_in); //Tight iso : < 0.15

  TH2F* Iso_histo_2 = (TH2F*) Iso_file_2->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  bin_in = Iso_histo_2->FindBin(mu1_pt, fabs(mu1_eta));
  double tightISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //Tight iso : < 0.15

  double tightISO_sf_mu1 = (lumi_1*tightISO_sf_1 + lumi_2*tightISO_sf_2) / (lumi_1 + lumi_2);


  bin_in = Iso_histo_1->FindBin(mu2_pt, fabs(mu2_eta));
  tightISO_sf_1 = Iso_histo_1->GetBinContent(bin_in); //Tight iso : < 0.15

  bin_in = Iso_histo_2->FindBin(mu2_pt, fabs(mu2_eta));
  tightISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //Tight iso : < 0.15

  double tightISO_sf_mu2 = (lumi_1*tightISO_sf_1 + lumi_2*tightISO_sf_2) / (lumi_1 + lumi_2);
  double tightISO_sf = tightISO_sf_mu1 * tightISO_sf_mu2;


  //Trigger
  //Careful, this is using IsoMu24 eff., ehich is different from IsoMu22
  TH2F* Trigger_histo_data_1 = (TH2F*) Trigger_file_1->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesDATA/pt_abseta_DATA");
  bin_in = Trigger_histo_data_1->FindBin(mu1_pt, fabs(mu1_eta));
  double eff_data_mu1 = Trigger_histo_data_1->GetBinContent(bin_in);
  bin_in = Trigger_histo_data_1->FindBin(mu2_pt, fabs(mu2_eta));
  double eff_data_mu2 = Trigger_histo_data_1->GetBinContent(bin_in);
  double eff_data = 1 - (1-eff_data_mu1) * (1-eff_data_mu2);

  TH2F* Trigger_histo_MC_1 = (TH2F*) Trigger_file_1->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/pt_abseta_MC");
  bin_in = Trigger_histo_MC_1->FindBin(mu1_pt, fabs(mu1_eta));
  double eff_MC_mu1 = Trigger_histo_MC_1->GetBinContent(bin_in);
  bin_in = Trigger_histo_MC_1->FindBin(mu2_pt, fabs(mu2_eta));
  double eff_MC_mu2 = Trigger_histo_MC_1->GetBinContent(bin_in);
  double eff_MC = 1 - (1-eff_MC_mu1) * (1-eff_MC_mu2);
  double trigger_sf_1 = eff_data/eff_MC;


  TH2F* Trigger_histo_data_2 = (TH2F*) Trigger_file_2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesDATA/pt_abseta_DATA");
  bin_in = Trigger_histo_data_2->FindBin(mu1_pt, fabs(mu1_eta));
  eff_data_mu1 = Trigger_histo_data_2->GetBinContent(bin_in);
  bin_in = Trigger_histo_data_2->FindBin(mu2_pt, fabs(mu2_eta));
  eff_data_mu2 = Trigger_histo_data_2->GetBinContent(bin_in);
  eff_data = 1 - (1-eff_data_mu1) * (1-eff_data_mu2);

  TH2F* Trigger_histo_MC_2 = (TH2F*) Trigger_file_2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/pt_abseta_MC");
  bin_in = Trigger_histo_MC_2->FindBin(mu1_pt, fabs(mu1_eta));
  eff_MC_mu1 = Trigger_histo_MC_2->GetBinContent(bin_in);
  bin_in = Trigger_histo_MC_2->FindBin(mu2_pt, fabs(mu2_eta));
  eff_MC_mu2 = Trigger_histo_MC_2->GetBinContent(bin_in);
  eff_MC = 1 - (1-eff_MC_mu1) * (1-eff_MC_mu2);
  double trigger_sf_2 = eff_data/eff_MC;

  double trigger_sf = (lumi_1*trigger_sf_1 + lumi_2*trigger_sf_2) / (lumi_1 + lumi_2);


  //Tracking
  TGraphErrors* Tracker_graph = (TGraphErrors*) Tracker_file->Get("ratio_eff_eta3_dr030e030_corr");
  double tracker_sf_mu1 = Tracker_graph->Eval(mu1_eta); //evaluate the graph function at this value of eta
  double tracker_sf_mu2 = Tracker_graph->Eval(mu2_eta); //evaluate the graph function at this value of eta
  double tracker_sf = tracker_sf_mu1 * tracker_sf_mu2;

  double reweight = mediumID2016_sf * tightISO_sf * trigger_sf * tracker_sf * pu_reweight;


  ID_file_1->Close("R");
  ID_file_2->Close("R");
  Iso_file_1->Close("R");
  Iso_file_2->Close("R");
  Trigger_file_1->Close("R");
  Trigger_file_2->Close("R");
  Tracker_file->Close("R");


  return reweight;
}
