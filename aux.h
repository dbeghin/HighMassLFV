#include "TH1.h"
#include "TH2.h"
#include <vector>
#include <iostream>
#include "TString.h"
#include "TLorentzVector.h"
#include "PU_reWeighting.cc"
#include "PU_reWeighting_ZPrime.cc"
#include "GeneralizedEndpoint.cc"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"

using namespace std;


TLorentzVector CorrectMET(TLorentzVector met_p4, TLorentzVector p1_p4, TLorentzVector p2_p4) {
  TLorentzVector mmet_p4;

  float met_px = met_p4.Px() + p1_p4.Px() - p2_p4.Px();
  float met_py = met_p4.Py() + p1_p4.Py() - p2_p4.Py();
  float met_pt = pow(pow(met_px, 2) + pow(met_py, 2), 0.5);
  mmet_p4.SetPxPyPzE(met_px, met_py, 0, met_pt);

  return mmet_p4;
}



float GetSignalWeight(float cosAngle, float mupt_ratio) { 
  if (fabs(cosAngle) > 1) {
    cout << endl << endl << "impossible cosine value" << endl << endl;
    return 0;
  }
  if (mupt_ratio >= 1) mupt_ratio = 0.9;

  TString hname = "HeavyHiggsOverRPV_600_cosapt_mu";


  TFile* signalweights_file = new TFile("Reweighting/signalweights_unfactorised.root","R");
  TH2F* hh = (TH2F*) signalweights_file->Get(hname);
  int iBin = hh->FindBin(cosAngle, mupt_ratio);
  double base_SF = hh->GetBinContent(iBin);
  //double error = hh->GetBinError(iBin);
  
  signalweights_file->Close("R");
  return base_SF;
}

float norm_F(float x, float y){

  return sqrt(x*x+y*y);

}

vector<TString> GetTauDMs() {
  vector<TString> tauDecayModes;
  tauDecayModes.push_back("dm0");
  tauDecayModes.push_back("dm1");
  tauDecayModes.push_back("dm10");
  tauDecayModes.push_back("dm11");
  
  return tauDecayModes;
}

vector<TString> GetSys() {
  vector<TString> systematics;
  vector<TString> tauDecayModes = GetTauDMs();
  for (unsigned int iDM=0; iDM < tauDecayModes.size(); ++iDM) {
    systematics.push_back("TrueTES"+tauDecayModes[iDM]);
  }
  systematics.push_back("FakeEleTESdm0");
  systematics.push_back("FakeEleTESdm1");
  systematics.push_back("FakeMuTESdm0");
  systematics.push_back("FakeMuTESdm1");
  systematics.push_back("MES");
  systematics.push_back("mres");
  systematics.push_back("METJetEn");
  systematics.push_back("METJetRes");
  systematics.push_back("METUnclustered");
  systematics.push_back("minbias");
  systematics.push_back("muonID");
  systematics.push_back("muonIso");
  //systematics.push_back("tracking");
  systematics.push_back("trigger");
  systematics.push_back("tauID");
  systematics.push_back("eletauFR");
  systematics.push_back("mutauFR");
  systematics.push_back("FRstat");
  //systematics.push_back("FRsys");
  systematics.push_back("topPt");
  systematics.push_back("topQscale");
  systematics.push_back("topPDF");
  systematics.push_back("WWPDF");
  systematics.push_back("prefiring");

  return systematics;
}


vector<TString> GetSignalSysVeto() {
  vector<TString> systematics;
  systematics.push_back("FRstat");
  systematics.push_back("FRsys");
  systematics.push_back("topPt");
  systematics.push_back("topQscale");
  systematics.push_back("topPDF");
  systematics.push_back("WWPDF");

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


double GetPUWeight(int PU, TString signal_name, TString var) {
  double pu_reweight = 0;
  if (!signal_name.Contains("ZPrime")) {
    if (var == "nom") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup", "Data_pileup_normalized");
    else if (var == "up") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup","Data_pileup_normalized_UP");
    else if (var == "down") pu_reweight = PU_2016::MC_pileup_weight(PU, "MC_pileup","Data_pileup_normalized_DOWN");
    else cout << "PU reweight error" << endl;
  }
  else {
    string mc_nickname = signal_name.Data();

    if (var == "nom") pu_reweight = PU_ZPrime::MC_pileup_weight(PU, mc_nickname, "Data_2016BtoH");
    else if (var == "up") pu_reweight = PU_ZPrime::MC_pileup_weight(PU, mc_nickname, "Data_2016BtoH_high");
    else if (var == "down") pu_reweight = PU_ZPrime::MC_pileup_weight(PU, mc_nickname, "Data_2016BtoH_low");
  }

  //cout << "ZPrime signal? " << signal_name << " PU reweight PU: " << PU << "  weight: " << pu_reweight << endl;

  return pu_reweight;
}


double GetHighPtIDWeight(TLorentzVector mu_p4, TString var) {
  float mu_p = mu_p4.P();
  float mu_eta = mu_p4.Eta();

  //scale factors from twiki https://twiki.cern.ch/twiki/bin/viewauth/CMS/HighPtMuonReferenceRun2
  double weight_nom = 0;
  double error = 0;
  if (mu_p >= 50 && mu_p < 100) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.9914;
      error = 0.0008;
    }
    else {
      weight_nom = 1;
      error = 0;
    }
  }
  else if (mu_p >= 100 && mu_p < 150) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.9936;
      error = 0.0009;
    }
    else {
      weight_nom = 0.993;
      error = 0.001;
    }
  }
  else if (mu_p >= 150 && mu_p < 200) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.993;
      error = 0.001;
    }
    else {
      weight_nom =  0.991;
      error = 0.001;
    }
  }
  else if (mu_p >= 200 && mu_p < 300) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.993;
      error = 0.002;
    }
    else {
      weight_nom = 0.985;
      error = 0.001;
    }
  }
  else if (mu_p >= 300 && mu_p < 400) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.990;
      error = 0.004;
    }
    else {
      weight_nom = 0.981;
      error = 0.002;
    }
  }
  else if (mu_p >= 400 && mu_p < 600) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.990;
      error = 0.003;
    }
    else {
      weight_nom = 0.979;
      error = 0.004;
    }
  }
  else if (mu_p >= 600 && mu_p < 1500) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.989;
      error = 0.004;
    }
    else {
      weight_nom = 0.978;
      error = 0.005;
    }
  }
  else if (mu_p >= 1500) {
    if (fabs(mu_eta) < 1.6) {
      weight_nom = 0.8;
      error = 0.3;
    }
    else {
      weight_nom = 0.9;
      error = 0.2;
    }
  }
  
  double weight = 0;
  if (var == "nom") {
    weight = weight_nom;
  }
  else if (var == "up") {
    weight = weight_nom + error;
  }
  else if (var == "down") {
    weight = weight_nom - error;
  }

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
  int bin_in = Iso_histo_1->FindBin(mu_eta, mu_pt);
  double tkLooseISO_sf_1 = Iso_histo_1->GetBinContent(bin_in);
  double error_1 = Iso_histo_1->GetBinError(bin_in);

  TH2F* Iso_histo_2 = (TH2F*) Iso_file_2->Get("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt");
  bin_in = Iso_histo_2->FindBin(mu_eta, mu_pt);
  double tkLooseISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //TkLoose iso : < 0.1
  double error_2 = Iso_histo_2->GetBinError(bin_in);

  double tkLooseISO_sf = (lumi_1*tkLooseISO_sf_1 + lumi_2*tkLooseISO_sf_2) / (lumi_1 + lumi_2);
  double error = (lumi_1*error_1 + lumi_2*error_2) / (lumi_1 + lumi_2);
  double weight_plus = tkLooseISO_sf + error;
  double weight_minus = tkLooseISO_sf - error;

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
  
  //scale factors from twiki https://twiki.cern.ch/twiki/bin/viewauth/CMS/HighPtMuonReferenceRun2
  double weight_nom = 0;
  double error = 0;
  if (mu_pt >= 52 && mu_pt < 55) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.980;
      error = 0.007;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.938;
      error = 0.006;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 0.996;
      error = 0.01;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.934;
      error = 0.03;
    }
  }
  else if (mu_pt >= 55 && mu_pt < 60) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.983;
      error = 0.004;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.958;
      error = 0.005;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 1.005;
      error = 0.009;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.954;
      error = 0.02;
    }
  }
  else if (mu_pt >= 60 && mu_pt < 120) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.980;
      error = 0.002;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.956;
      error = 0.002;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 0.990;
      error = 0.003;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.948;
      error = 0.007;
    }
  }
  else if (mu_pt >= 120 && mu_pt < 200) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.980;
      error = 0.003;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.948;
      error = 0.005;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 0.988;
      error = 0.005;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.935;
      error = 0.01;
    }
  }
  else if (mu_pt >= 200 && mu_pt < 300) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.980;
      error = 0.007;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.927;
      error = 0.01;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 0.977;
      error = 0.01;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.888;
      error = 0.03;
    }
  }
  else if (mu_pt >= 300 && mu_pt < 500) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.962;
      error = 0.01;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.911;
      error = 0.03;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 1.010;
      error = 0.02;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 0.953;
      error = 0.07;
    }
  }
  else if (mu_pt >= 500) {
    if (fabs(mu_eta)<0.9) {
      weight_nom = 0.983;
      error = 0.05;
    }
    else if (fabs(mu_eta)>=0.9 && fabs(mu_eta)<1.2) {
      weight_nom = 0.824;
      error = 0.1;
    }
    else if (fabs(mu_eta)>=1.2 && fabs(mu_eta)<2.1) {
      weight_nom = 1.020;
      error = 0.1;
    }
    else if (fabs(mu_eta)>=2.1 && fabs(mu_eta)<2.4) {
      weight_nom = 1.134;
      error = 0.5;
    }
  }

  double weight = 0;
  if (var == "nom") {
    weight = weight_nom;
  }
  else if (var == "up") {
    weight = weight_nom + error;
  }
  else if (var == "down") {
    weight = weight_nom - error;
  }

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
    TFile* TauID_file = new TFile("Reweighting/TauID_SF_pt_DeepTau2017v2p1VSjet_2016Legacy.root","R");

    if (tau_pt >= 1000) tau_pt = 999;

    TF1* central_fct = (TF1*) TauID_file->Get("Tight_cent");
    TF1* up_fct = (TF1*) TauID_file->Get("Tight_up");
    TF1* down_fct = (TF1*) TauID_file->Get("Tight_down");

    double base_weight = central_fct->Eval(tau_pt);
    double factor_up   = up_fct->Eval(tau_pt);
    double factor_down = down_fct->Eval(tau_pt);

    TauID_file->Close("R");

    double weight = 0;
    if (var=="nom") weight = base_weight;
    else if (var=="up") weight = factor_up;
    else if (var=="down") weight = factor_down;
    return weight;
  }
}


double GetEleTauFR(float eta, TString lepton, TString var) {
  if (lepton != "ele") {
    return 1;
  }
  else {
    double weight = 1;

    TFile* TauID_file = new TFile("Reweighting/TauID_SF_eta_DeepTau2017v2p1VSe_2016Legacy.root","R");
    
    TH1F* histo = (TH1F*) TauID_file->Get("Loose");
    int bin_in = histo->FindBin(fabs(eta));

    double base_weight = histo->GetBinContent(bin_in);
    double error   = histo->GetBinError(bin_in);

    TauID_file->Close("R");

    if (var=="nom") weight = base_weight;
    else if (var=="up") weight = base_weight+error;
    else if (var=="down") weight = base_weight-error;

    return weight;
  }
}


double GetMuTauFR(float eta, TString lepton, TString var) {
  //Loose WP
  if (lepton != "mu") {
    return 1;
  }
  else {
    double weight = 1;
    
    TFile* TauID_file = new TFile("Reweighting/TauID_SF_eta_DeepTau2017v2p1VSmu_2016Legacy.root","R");
    
    TH1F* histo = (TH1F*) TauID_file->Get("Tight");
    int bin_in = histo->FindBin(fabs(eta));

    double base_weight = histo->GetBinContent(bin_in);
    double error   = histo->GetBinError(bin_in);

    TauID_file->Close("R");

    if (var=="nom") weight = base_weight;
    else if (var=="up") weight = base_weight+error;
    else if (var=="down") weight = base_weight-error;

    return weight;
  }
}


double FakeRate_unfactorised(int CR_number, double taupt, double taueta, double ratio, TString sys, TString var) {
  if (taupt >= 1000) taupt = 999;
  if (ratio >= 2) ratio = 1.9;

  if (CR_number == 100 || CR_number == 102) return 1;
  if (sys == "FRsys") return 1;
  TString prestring = "";
  if (sys == "FRstat") prestring = "nominal";
  else if (sys == "eletrigger") return 1;
  else if (sys == "topPt") return 1;
  else {
    if (var == "nom") return 1;
    else prestring = sys+"_"+var;
  }

  TString eta_string = GetEtaString(taueta);
  if (eta_string == "") {
    return 0;
  }


  TString hname = prestring+"_"+eta_string;
  if (taupt > 150) {
    hname += "_taupt_150_1000";
  }
  else {
    hname += "_taupt_0_150";
  }
  //hname += "_taupt_50_1000";


  TFile* fake_file = new TFile("Reweighting/fakerate_unfactorised_MtLow.root","R");
  TH2F* h_taupt = (TH2F*) fake_file->Get("FakeRateByTauPtAndRatio_"+hname);
  int iBin = h_taupt->FindBin(taupt, ratio);
  double base_SF = h_taupt->GetBinContent(iBin);
  double error = h_taupt->GetBinError(iBin);
  
  double weight = 0;
  if (sys == "FRstat") {
    if (var=="nom") weight = base_SF;
    else if (var=="up") weight = base_SF+error;
    else if (var=="down") weight = base_SF-error;
  }
  else {
    float weight_normal = FakeRate_unfactorised(CR_number, taupt, taueta, ratio, "FRstat", "nom");
    if (weight_normal !=0) weight = base_SF/weight_normal;
  }


  fake_file->Close("R");
  return weight;
}



double FakeRate_DY(int CR_number, double taupt, double taueta, double ratio, TString sys, TString var) {
  if (taupt >= 1000) taupt = 999;
  if (taupt <= 30) taupt = 31;
  if (ratio >= 2) ratio = 1.9;
  if (CR_number == 100 || CR_number == 102) return 1;
  
  //TString eta_string = GetEtaString(taueta);
  //TString hname = "eta_"+eta_string;

  TString hname = "total";
  if (taupt > 150) {
    hname += "_taupt_150_1000";
  }
  else {
    hname += "_taupt_0_150";
  }
  
  TFile* fake_file = new TFile("Reweighting/fakerate_unfactorised_DY.root","R");
  TH1F* h_taupt = (TH1F*) fake_file->Get("FakeRateByTauPtAndRatio_"+hname);
  int iBin = h_taupt->FindBin(taupt, ratio);
  double DY_SF = h_taupt->GetBinContent(iBin);
  double DY_error = h_taupt->GetBinError(iBin);
  double norm_SF = FakeRate_unfactorised(CR_number,taupt,taueta,ratio,"FRstat","nom");
  
  double weight = 0;
  //if (var=="nom") weight = DY_SF;
  //else if (var=="up") {
  //  weight = DY_SF+DY_error;
  //}
  //else if (var=="down") {
  //  weight = DY_SF-DY_error;
  //}


  if (var=="nom") weight = 1;
  else if (var=="up") {
    if (norm_SF != 0) weight = DY_SF/norm_SF;
  }
  else if (var=="down") {
    if (norm_SF != 0) weight = (2*norm_SF-DY_SF)/norm_SF;
  }
  fake_file->Close("R");
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


double GetTopQscale(TString process, float Mll, TString var){
  if (Mll <0 || process != "TT" || var=="nom") {
    return 1;
  }
  else {
    //double unc = 0.135 - 5.981*pow(10,-5)*Mll + 1.807*pow(10,-7)*pow(Mll,2) - 1.815*pow(10,-10)*pow(Mll,3) + 7.875*pow(10,-14)*pow(Mll,4) - 1.229*pow(10,-17)*pow(Mll,5);
    double unc = .007 - 1.238*pow(10,-5)*Mll + 9.69*pow(10,-9)*pow(Mll,2);
    
    double weight = 0;
    if (var=="nom") weight = 1;
    else if (var=="up") weight = 1+fabs(unc);
    else if (var=="down") weight = 1-fabs(unc);
    
    return weight;
  }
}

double GetTopPDF(TString process, float Mll, TString var){
  if (Mll <0 || process != "TT" || var=="nom") {
    return 1;
  }
  else {
    //double unc = 0.49 - 0.0007795*Mll + 1.59*pow(10,-6)*pow(Mll,2) - 1.166*pow(10,-9)*pow(Mll,3) + 3.93*pow(10,-13)*pow(Mll,4) - 4.72*pow(10,-17)*pow(Mll,5);
    double unc = .07 - 0.0001739*Mll + 1.383*pow(10,-7)*pow(Mll,2);

    double weight = 0;
    if (var=="nom") weight = 1;
    else if (var=="up") weight = 1+fabs(unc);
    else if (var=="down") weight = 1-fabs(unc);
    
    return weight;
  }
}

double GetWWPDF(TString process, float Mll, TString var){
  if (Mll <0 || process != "WW" || var=="nom") {
    return 1;
  }
  else {
    double unc = 1-(0.993 - 2.001*pow(10,-4)*Mll + 2.838*pow(10,-8)*pow(Mll,2));
    
    double weight = 0;
    if (var=="nom") weight = 1;
    else if (var=="up") weight = 1+fabs(unc);
    else if (var=="down") weight = 1-fabs(unc);
    
    return weight;
  }
}


double GetPrefiringWeight(TString var, vector<double> w_pref) {

  double weight = 1;
  if (var == "nom") weight = w_pref[0];
  else if (var == "up") weight = w_pref[1];
  else if (var == "down") weight = w_pref[2];

  return weight;
}


double GeneralWeightFunction(int CR_number, TString sys, int n_vert, TLorentzVector tau_p4, float ratio, TLorentzVector mu_p4, TString lepton, float top_pt_1, float top_pt_2, float Mll, vector<double> w_pref, TString process, TString var) {
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
      //systematics with their own weights
      if (sys == "minbias") weight = GetPUWeight(n_vert,process,var);
      else if (sys == "muonID") weight = GetHighPtIDWeight(mu_p4,var);
      else if (sys == "muonIso") weight = GetTkLooseIsoWeight(mu_pt,mu_eta,var);
      else if (sys == "tracking") weight = GetTrackingWeight(mu_pt,mu_eta,var);
      else if (sys == "trigger") weight = GetTriggerWeight(mu_pt,mu_eta,var);
      else if (sys == "tauID") weight = GetTightTauIDWeight(tau_pt,lepton,var);
      else if (sys == "eletauFR") weight = GetEleTauFR(tau_eta,lepton,var);
      else if (sys == "mutauFR") weight = GetMuTauFR(tau_eta,lepton,var);
      else if (sys == "topPt") weight = GetTopPtWeight(top_pt_1,top_pt_2,var); 
      else if (sys == "eletrigger") weight = GetEleTriggerWeight(mu_pt,mu_eta); 
      else if (sys == "topQscale") weight = GetTopQscale(process,Mll,var); 
      else if (sys == "topPDF") weight = GetTopPDF(process,Mll,var); 
      else if (sys == "WWPDF") weight = GetWWPDF(process,Mll,var); 
      else if (sys == "prefiring") weight = GetPrefiringWeight(var,w_pref);

      //multiply by the FR corresponding to each systematic
      //fake rate is 1 when weight is nominal
      //when there is systematics up/down variation, fake rate value is corrected accordingly
      weight *= FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);

      if (sys.Contains("FRstat")) weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      //shape systematics also affect fake rate
      else if (sys.Contains("TES")) weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "MES") weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "mres") weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      else if (sys.Contains("METJet")) weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      else if (sys.Contains("METUnclustered")) weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
    }
    else {
      //this is data
      weight = 1;
      if (sys == "FRstat") weight = FakeRate_unfactorised(CR_number,tau_pt,tau_eta,ratio,sys,var);
      else if (sys == "FRsys") weight = FakeRate_DY(CR_number,tau_pt,tau_eta,ratio,sys,var);
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

  double METproj=(MET.Px()*tau.Px()+MET.Py()*tau.Py())/tau.Pt();
  //if (METproj < 0) cout << "METproj: " << METproj << endl;
  double xth=1;
  if(METproj>0) xth=tau.Pt()/(tau.Pt()+METproj);
  else xth = 1;
  double mass_vis=(tau+mu).M();
  double mcol = 0;
  if (mass_vis != mass_vis) mass_vis=0;
  if (mass_vis<0) mass_vis = 0;

  mcol=mass_vis/sqrt(xth);

  return mcol;
}


double GetCollinearMass_old(TLorentzVector tau, TLorentzVector mu,  TLorentzVector MET) {

  double METproj=fabs(MET.Px()*tau.Px()+MET.Py()*tau.Py())/tau.Pt();
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


pair<TLorentzVector,TLorentzVector> TauEnergyScale(TLorentzVector tau_p4, int tau_DM, TLorentzVector met_p4, TString var) {
  TLorentzVector tau_TES_p4;
  tau_TES_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_TES_p4;
  met_TES_p4.SetPxPyPzE(0,0,0,0);
  
  TFile* ES_file_lowpt = new TFile("Reweighting/TauES_dm_DeepTau2017v2p1VSjet_2016Legacy.root","R");
  TH1F* h_ES_low = (TH1F*) ES_file_lowpt->Get("tes");
  h_ES_low->SetName("tes_low");
  TFile* ES_file_highpt = new TFile("Reweighting/TauES_dm_DeepTau2017v2p1VSjet_2016Legacy_ptgt100.root","R");
  TH1F* h_ES_high = (TH1F*) ES_file_highpt->Get("tes");
  h_ES_high->SetName("tes_high");

  int iBin = h_ES_low->FindBin(tau_DM);
  float scale_nom = h_ES_low->GetBinContent(iBin);
  float error_scale = 0;
  if (tau_p4.Pt() > 170) {
    error_scale = h_ES_high->GetBinError(iBin);
  }
  else {
    error_scale = (h_ES_high->GetBinError(iBin) - h_ES_low->GetBinError(iBin))/(170-34)*tau_p4.Pt()/scale_nom + (h_ES_low->GetBinError(iBin)*170 - h_ES_high->GetBinError(iBin)*34)/(170-34);
    //cout << tau_p4.Pt()/scale_nom << " GeV  " << h_ES_low->GetBinError(iBin) << " " << error_scale << " " << h_ES_high->GetBinError(iBin) << endl;
  }

  float rescale = 1;
  if (var=="nom")       rescale = scale_nom;
  else if (var=="up")   rescale = (scale_nom + error_scale)/scale_nom;
  else if (var=="down") rescale = (scale_nom - error_scale)/scale_nom;
  tau_TES_p4 = tau_p4*rescale;

  met_TES_p4 = CorrectMET(met_p4, tau_p4, tau_TES_p4);

  pair<TLorentzVector,TLorentzVector> tes;
  tes.first = tau_TES_p4;
  tes.second = met_TES_p4;

  ES_file_lowpt->Close("R");
  ES_file_highpt->Close("R");

  return tes;
}


pair<TLorentzVector,TLorentzVector> FakeEleTauEnergyScale(TLorentzVector tau_p4, int tau_DM, TLorentzVector met_p4, TString var) {
  TLorentzVector tau_TES_p4;
  tau_TES_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_TES_p4;
  met_TES_p4.SetPxPyPzE(0,0,0,0);

  pair<TLorentzVector,TLorentzVector> tes;
  tes.first = tau_p4;
  tes.second = met_p4;

  if (tau_DM > 1) return tes;
  
  TFile* ES_file = new TFile("Reweighting/TauFES_eta-dm_DeepTau2017v2p1VSe_2016Legacy.root","R");
  TGraphAsymmErrors* g_ES = (TGraphAsymmErrors*) ES_file->Get("fes");

  float scale_nom = 1;
  float error_up = 0;
  float error_down = 0;
  TString eta_string = GetEtaString(tau_p4.Eta());
  if (tau_DM == 0 && eta_string == "barrel") {
    scale_nom  =          g_ES->Eval(0.5);
    error_up   = g_ES->GetErrorYhigh(0);
    error_down =  g_ES->GetErrorYlow(0);
  }
  else if (tau_DM == 1 && eta_string == "barrel") {
    scale_nom  =          g_ES->Eval(1.5);
    error_up   = g_ES->GetErrorYhigh(1);
    error_down =  g_ES->GetErrorYlow(1);
  }
  else if (tau_DM == 0 && eta_string == "endcap") {
    scale_nom  =          g_ES->Eval(2.5);
    error_up   = g_ES->GetErrorYhigh(2);
    error_down =  g_ES->GetErrorYlow(2);
  }
  else if (tau_DM == 1 && eta_string == "endcap") {
    scale_nom  =          g_ES->Eval(3.5);
    error_up   = g_ES->GetErrorYhigh(3);
    error_down =  g_ES->GetErrorYlow(3);
  }

  float rescale = 1;
  if (var=="nom")       rescale = scale_nom;
  else if (var=="up")   rescale = (scale_nom + error_up)/scale_nom;
  else if (var=="down") rescale = (scale_nom - error_down)/scale_nom;
  tau_TES_p4 = tau_p4*rescale;

  met_TES_p4 = CorrectMET(met_p4, tau_p4, tau_TES_p4);

  tes.first = tau_TES_p4;
  tes.second = met_TES_p4;

  ES_file->Close("R");

  return tes;
}




pair<TLorentzVector,TLorentzVector> FakeMuTauEnergyScale(TLorentzVector tau_p4, TLorentzVector met_p4, TString var) {
  TLorentzVector tau_TES_p4;
  tau_TES_p4.SetPxPyPzE(0,0,0,0);
  TLorentzVector met_TES_p4;
  met_TES_p4.SetPxPyPzE(0,0,0,0);
  
  float scale_nom = 1;
  float error_scale = 0.01;

  float rescale = 1;
  if (var=="nom")       rescale = scale_nom;
  else if (var=="up")   rescale = scale_nom + error_scale;
  else if (var=="down") rescale = scale_nom - error_scale;
  tau_TES_p4 = tau_p4*rescale;

  met_TES_p4 = CorrectMET(met_p4, tau_p4, tau_TES_p4);

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
  
  float smearing = 0;
  if (fabs(mu_p4.Eta()) < 1.2) {
    if (mu_p4.Pt() < 200) smearing = 0.003;
    else if (mu_p4.Pt() < 500) smearing = 0.005;
    else smearing = 0.01;
  }
  else {
    if (mu_p4.Pt() < 200) smearing = 0.006;
    else if (mu_p4.Pt() < 500) smearing = 0.01;
    else smearing = 0.02;
  }

  double scale = gRandom->Gaus(0, smearing);

  if (var=="nom")  mu_res_p4 = mu_p4;
  else if (var=="up")   mu_res_p4 = mu_p4*(1+scale);
  else if (var=="down") mu_res_p4 = mu_p4*(1-scale);

  met_res_p4 = CorrectMET(met_p4, mu_p4, mu_res_p4);

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
  met_mes_p4 = CorrectMET(met_p4, mu_p4, mu_mes_p4);

  pair<TLorentzVector,TLorentzVector> mes;
  mes.first = mu_mes_p4;
  mes.second = met_mes_p4;
  return mes;
}


vector<TLorentzVector> GetScaleVariation(TString syst, TString tau_gen, TString mu_gen, TLorentzVector tau_p4, int tau_DM, TLorentzVector mu_p4, int mu_charge, TLorentzVector met_p4, vector<TLorentzVector> MET_scale_p4) {
  TLorentzVector tau_newp4, mu_newp4, met_newp4;

  TString var = "nom";
  if (syst.Contains("up")) var = "up";
  else if (syst.Contains("down")) var = "down";

  tau_newp4 = tau_p4, met_newp4 = met_p4, mu_newp4 = mu_p4;
  //cout << syst << "  tau_DM: " << tau_DM << endl;

  if (syst.Contains("TrueTES") && tau_gen == "tau") {
    if (var == "nom") {
      tau_newp4 = TauEnergyScale(tau_p4,tau_DM,met_p4,var).first, met_newp4 = TauEnergyScale(tau_p4,tau_DM,met_p4,var).second;
    }
    else {
      bool diagDM = false;
      if (tau_DM == 0  && syst.Contains("dm0_"))  diagDM = true;
      if (tau_DM == 1  && syst.Contains("dm1_"))  diagDM = true;
      if (tau_DM == 10 && syst.Contains("dm10_")) diagDM = true;
      if (tau_DM == 11 && syst.Contains("dm11_")) diagDM = true;
      if (diagDM) {
	tau_newp4 = TauEnergyScale(tau_p4,tau_DM,met_p4,var).first, met_newp4 = TauEnergyScale(tau_p4,tau_DM,met_p4,var).second;
      }
    }
  }
  else if (syst.Contains("FakeEleTES") && tau_gen == "ele") {
    if (var == "nom") {
      tau_newp4 = FakeEleTauEnergyScale(tau_p4,tau_DM,met_p4,var).first, met_newp4 = FakeEleTauEnergyScale(tau_p4,tau_DM,met_p4,var).second;
    }
    else {
      bool diagDM = false;
      if (tau_DM == 0  && syst.Contains("dm0_"))  diagDM = true;
      if (tau_DM == 1  && syst.Contains("dm1_"))  diagDM = true;
      if (diagDM) {
	tau_newp4 = FakeEleTauEnergyScale(tau_p4,tau_DM,met_p4,var).first, met_newp4 = FakeEleTauEnergyScale(tau_p4,tau_DM,met_p4,var).second;
      }
    }
  }
  else if (syst.Contains("FakeMuTES") && tau_gen == "mu") {
    if (var == "nom") {
      tau_newp4 = FakeMuTauEnergyScale(tau_p4,met_p4,var).first, met_newp4 = FakeMuTauEnergyScale(tau_p4,met_p4,var).second;
    }
    else {
      bool diagDM = false;
      if (tau_DM == 0  && syst.Contains("dm0_"))  diagDM = true;
      if (tau_DM == 1  && syst.Contains("dm1_"))  diagDM = true;
      if (diagDM) tau_newp4 = FakeMuTauEnergyScale(tau_p4,met_p4,var).first, met_newp4 = FakeMuTauEnergyScale(tau_p4,met_p4,var).second;
    }
  }
  else if (syst.Contains("MES")) {
    if (mu_gen != "mu") var = "nom";
    mu_newp4 = MuEnergyScale(mu_p4,mu_charge,met_p4,var).first, met_newp4 = MuEnergyScale(mu_p4,mu_charge,met_p4,var).second;
  }
  else if (syst.Contains("mres")) {
    if (mu_gen != "mu") var = "nom";
    mu_newp4 = MuResolution(mu_p4,met_p4,var).first, met_newp4 = MuResolution(mu_p4,met_p4,var).second;
  }
  else if (syst.Contains("METJetEn_up")) {
    met_newp4 = MET_scale_p4[0];
  }
  else if (syst.Contains("METJetEn_down")) {
    met_newp4 = MET_scale_p4[1];
  }
  else if (syst.Contains("METJetRes_up")) {
    met_newp4 = MET_scale_p4[2];
  }
  else if (syst.Contains("METJetRes_down")) {
    met_newp4 = MET_scale_p4[3];
  }
  else if (syst.Contains("METUnclustered_up")) {
    met_newp4 = MET_scale_p4[4];
  }
  else if (syst.Contains("METUnclustered_down")) {
    met_newp4 = MET_scale_p4[5];
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
