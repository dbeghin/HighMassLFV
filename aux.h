#include "TH1.h"
#include "TH2.h"
#include <vector>
#include <iostream>
#include "TString.h"
#include "TLorentzVector.h"

using namespace std;

float norm_F(float x, float y){

  return sqrt(x*x+y*y);

}



float top_reweighting_uncertainty(float top_pt_in){
  float weight = 0.0;
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




double FakeRate_unfactorised(double taupt, double ratio, TString eta) {
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
  double base_SF = h_taupt->GetBinContent(iBin);
  
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



double FakeRate_mumu(double taupt, double jetpt) {
  double SF=0.2;

  TFile* fake_file = new TFile("Reweighting/fakerate.root","R");

  TString sector_string = "_taupt_";
  
  if (jetpt >= 1000) jetpt = 999;
  if (taupt >= 1000) taupt = 999;

  if ( (taupt > 0) && (taupt < 300) ) {
    if ( (jetpt > 0) && (jetpt < 120) )          sector_string += "0_300_jetpt_0_120";
    else if ( (jetpt >= 120) && (jetpt < 300) )  sector_string += "0_300_jetpt_120_300";
    else if ( (jetpt >= 300) && (jetpt < 1000) ) sector_string += "0_1000_jetpt_300_1000";
  }
  else if ( (taupt > 300) && (taupt < 1000) ) {
    if ( (jetpt > 0) && (jetpt < 300) )          sector_string += "300_1000_jetpt_0_300";
    else if ( (jetpt >= 300) && (jetpt < 1000) ) sector_string += "0_1000_jetpt_300_1000";
  }

  double reweight = 0;

  TString hname = "hratio_data_total_taupt_jetpt_pass" + sector_string;
  TH2F* h_fake = (TH2F*) fake_file->Get(hname);
  int iBin = h_fake->FindBin(taupt, jetpt);
  SF = h_fake->GetBinContent(iBin);
  if (SF != 1) reweight = SF/(1-SF);

  return reweight;

}



double topPtReweight(double pt) {
  double aa = 1.08872;
  double bb = 0.0119998;
  double cc = 0.895139;

  double w_top = exp(-aa-bb*pt) + cc;

  return w_top;
}


double GetCollinearMass(TLorentzVector tau, TLorentzVector mu,  TLorentzVector MET) {

  double METproj=fabs((MET.Px()*tau.Px()+MET.Py()*tau.Py())/tau.Pt());
  double xth=1;
  if((tau.Pt()+METproj)!=0) xth=tau.Pt()/(tau.Pt()+METproj);
  double mass_vis=(tau+mu).M();
  return mass_vis/sqrt(xth);
}


double GetLepToTauFR(TString lep, double eta) {

  double reweight = 1;
  if (lep == "mu") {
    if (fabs(eta) < 0.4) reweight = 1.47;
    else if (fabs(eta) < 0.8 && fabs(eta) > 0.4) reweight = 1.55;
    else if (fabs(eta) < 1.2 && fabs(eta) > 0.8) reweight = 1.33;
    else if (fabs(eta) < 1.7 && fabs(eta) > 1.2) reweight = 1.72;
    else if (fabs(eta) < 2.3 && fabs(eta) > 1.7) reweight = 2.50;
  }
  else if (lep == "ele") {
    if (fabs(eta) < 1.460) reweight = 1.21;
    else if (fabs(eta) > 1.558) reweight = 1.38;
  }
  return reweight;

}


double GetReweight(int PU, float mu_pt, float mu_eta) {
  //scale factor files that need to be open
  TFile* ID_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_id.root","R");
  TFile* ID_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_id.root","R");
  TFile* Iso_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_iso.root","R");
  TFile* Iso_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_iso.root","R");
  TFile* Trigger_file_1 = new TFile("Reweighting/EfficienciesAndSF_RunBtoF.root","R");
  TFile* Trigger_file_2 = new TFile("Reweighting/EfficienciesAndSF_Period4.root","R");
  TFile* Tracker_file = new TFile("Reweighting/Tracking_EfficienciesAndSF_BCDEFGH.root","R");


  //PU reweight
  double pu_reweight = PU_reReco_Morind17::MC_pileup_weight(PU,0,"all");


  //scale factors
  //ID           
  TH2F* ID_histo_1 = (TH2F*) ID_file_1->Get("MC_NUM_MediumID2016_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  int bin_in = ID_histo_1->FindBin(mu_pt, fabs(mu_eta));
  double mediumID2016_sf_1 = ID_histo_1->GetBinContent(bin_in);
  double lumi_1 = 20.0; //luminosity of Runs BCDEF             

  TH2F* ID_histo_2 = (TH2F*) ID_file_2->Get("MC_NUM_MediumID2016_DEN_genTracks_PAR_pt_eta/pt_abseta_ratio");
  bin_in = ID_histo_2->FindBin(mu_pt, fabs(mu_eta));
  double mediumID2016_sf_2 = ID_histo_2->GetBinContent(bin_in);
  double lumi_2 = 16.0; //luminosity of Runs GH                

  double mediumID2016_sf = (lumi_1*mediumID2016_sf_1 + lumi_2*mediumID2016_sf_2) / (lumi_1 + lumi_2);


  //Isolation
  TH2F* Iso_histo_1 = (TH2F*) Iso_file_1->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  bin_in = Iso_histo_1->FindBin(mu_pt, fabs(mu_eta));
  double tightISO_sf_1 = Iso_histo_1->GetBinContent(bin_in); //Tight iso : < 0.15.

  TH2F* Iso_histo_2 = (TH2F*) Iso_file_2->Get("TightISO_MediumID_pt_eta/pt_abseta_ratio");
  bin_in = Iso_histo_2->FindBin(mu_pt, fabs(mu_eta));
  double tightISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //Tight iso : < 0.15

  double tightISO_sf = (lumi_1*tightISO_sf_1 + lumi_2*tightISO_sf_2) / (lumi_1 + lumi_2);


  //Trigger
  //Careful, this uses IsoMu24 trigger, different from IsoMu22 trigger
  TH2F* Trigger_histo_1 = (TH2F*) Trigger_file_1->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/pt_abseta_ratio");
  bin_in = Trigger_histo_1->FindBin(mu_pt, fabs(mu_eta));
  double trigger_sf_1 = Trigger_histo_1->GetBinContent(bin_in);

  TH2F* Trigger_histo_2 = (TH2F*) Trigger_file_2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/pt_abseta_ratio");
  bin_in = Trigger_histo_2->FindBin(mu_pt, fabs(mu_eta));
  double trigger_sf_2 = Trigger_histo_2->GetBinContent(bin_in);

  double trigger_sf =  (lumi_1*trigger_sf_1 + lumi_2*trigger_sf_2) / (lumi_1 + lumi_2);


  //Tracking
  TGraphErrors* Tracker_graph = (TGraphErrors*) Tracker_file->Get("ratio_eff_eta3_dr030e030_corr");
  double tracker_sf = Tracker_graph->Eval(mu_eta); //evaluate the graph function at this value of eta

  if (mediumID2016_sf == 0) mediumID2016_sf = 1.0; 
  if (tightISO_sf == 0) tightISO_sf = 1.0; 
  if (trigger_sf == 0) trigger_sf = 1.0; 
  if (tracker_sf == 0) tracker_sf = 1.0; 
  if (pu_reweight == 0) pu_reweight = 1.0; 
  
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


double GetReweight_highmass(int PU, float mu_pt, float mu_eta, bool tau_match, bool singlephoton) {
  //scale factor files that need to be open
  TFile* ID_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_id.root","R");
  TFile* ID_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_id.root","R");
  TFile* Iso_file_1 = new TFile("Reweighting/EfficienciesAndSF_BCDEF_iso.root","R");
  TFile* Iso_file_2 = new TFile("Reweighting/EfficienciesAndSF_GH_iso.root","R");
  TFile* Trigger_file_1 = new TFile("Reweighting/EfficienciesAndSF_RunBtoF.root","R");
  TFile* Trigger_file_2 = new TFile("Reweighting/EfficienciesAndSF_Period4.root","R");
  TFile* Tracker_file = new TFile("Reweighting/Tracking_EfficienciesAndSF_BCDEFGH.root","R");


  //PU reweight
  double pu_reweight = PU_reReco_Morind17::MC_pileup_weight(PU,0,"all");


  //scale factors
  //ID
  TH2F* ID_histo_1 = (TH2F*) ID_file_1->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
  int bin_in = ID_histo_1->FindBin(fabs(mu_eta), mu_pt);
  double highPtID_sf_1 = ID_histo_1->GetBinContent(bin_in);
  double lumi_1 = 20.0; //luminosity of Runs BCDEF         

  TH2F* ID_histo_2 = (TH2F*) ID_file_2->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
  bin_in = ID_histo_2->FindBin(fabs(mu_eta), mu_pt);
  double highPtID_sf_2 = ID_histo_2->GetBinContent(bin_in);
  double lumi_2 = 16.0; //luminosity of Runs GH            

  double highPtID_sf = (lumi_1*highPtID_sf_1 + lumi_2*highPtID_sf_2) / (lumi_1 + lumi_2);


  //Isolation                                                                                                                                                                                                                                                                                                                                                                     
  TH2F* Iso_histo_1 = (TH2F*) Iso_file_1->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
  bin_in = Iso_histo_1->FindBin(fabs(mu_eta), mu_pt);
  double tkLooseISO_sf_1 = Iso_histo_1->GetBinContent(bin_in);

  TH2F* Iso_histo_2 = (TH2F*) Iso_file_2->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
  bin_in = Iso_histo_2->FindBin(fabs(mu_eta), mu_pt);
  double tkLooseISO_sf_2 = Iso_histo_2->GetBinContent(bin_in); //Tight iso : < 0.15

  double tkLooseISO_sf = (lumi_1*tkLooseISO_sf_1 + lumi_2*tkLooseISO_sf_2) / (lumi_1 + lumi_2);


  //Trigger
  //Careful, this uses IsoMu24 trigger, different from IsoMu22 trigger
  TH2F* Trigger_histo_1 = (TH2F*) Trigger_file_1->Get("Mu50_OR_TkMu50_PtEtaBins/pt_abseta_ratio");
  bin_in = Trigger_histo_1->FindBin(mu_pt, fabs(mu_eta));
  double trigger_sf_1 = Trigger_histo_1->GetBinContent(bin_in);

  TH2F* Trigger_histo_2 = (TH2F*) Trigger_file_2->Get("Mu50_OR_TkMu50_PtEtaBins/pt_abseta_ratio");
  bin_in = Trigger_histo_2->FindBin(mu_pt, fabs(mu_eta));
  double trigger_sf_2 = Trigger_histo_2->GetBinContent(bin_in);

  double trigger_sf =  (lumi_1*trigger_sf_1 + lumi_2*trigger_sf_2) / (lumi_1 + lumi_2);


  //Tracking
  TGraphErrors* Tracker_graph = (TGraphErrors*) Tracker_file->Get("ratio_eff_eta3_dr030e030_corr");
  double tracker_sf = Tracker_graph->Eval(mu_eta); //evaluate the graph function at this value of eta

  if (highPtID_sf == 0) highPtID_sf = 1.0;
  if (tkLooseISO_sf == 0) tkLooseISO_sf = 1.0;
  if (trigger_sf == 0) trigger_sf = 1.0;
  if (tracker_sf == 0) tracker_sf = 1.0;
  if (pu_reweight == 0) pu_reweight = 1.0;

  if (singlephoton) trigger_sf = 1.0;
  double reweight = highPtID_sf * tkLooseISO_sf * trigger_sf * tracker_sf * pu_reweight;
  if (tau_match) reweight = reweight*0.95;


  ID_file_1->Close("R");
  ID_file_2->Close("R");
  Iso_file_1->Close("R");
  Iso_file_2->Close("R");
  Trigger_file_1->Close("R");
  Trigger_file_2->Close("R");
  Tracker_file->Close("R");


  return reweight;
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
  double pu_reweight = PU_reReco_Morind17::MC_pileup_weight(PU,0,"all");


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



