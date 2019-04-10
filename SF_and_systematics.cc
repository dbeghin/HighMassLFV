#define IIHEAnalysis_cxx
#include "IIHEAnalysis_2016.h"
#include "TString.h"
#include <iostream>
#include <vector>
#include "aux.h"

using namespace std;


map<TString, float> GetWeightSys(int n_vert, TLorentzVector tau_p4, float ratio, TLorentzVector mu_p4, TString lepton, float top_pt) {

  vector<TString> systematics = GetSys();      
  
  vector<TString> variations;
  variations.push_back("up");
  variations.push_back("down");


  float mu_pt = mu_p4.Pt();
  float mu_eta = mu_p4.Eta();
  float tau_pt = tau_p4.Pt();
  float tau_eta = tau_p4.Eta();

  float nom_weight = 1;
  nom_weight *= GetTrackingWeight(mu_eta,"nom");
  //nom_weight *= GetPUWeight(n_vert,"nom");
  //nom_weight *= GetHighPtIDWeight(mu_pt,mu_eta,"nom");
  //nom_weight *= GetTkLooseIDWeight(mu_pt,mu_eta,"nom");
  //nom_weight *= GetTriggerWeight(mu_pt,mu_eta,"nom");
  //nom_weight *= GetTightTauIDSF(tau_pt,"nom");
  //nom_weight *= GetEleTauFR(tau_eta,lepton,"nom");
  //nom_weight *= GetMuTauFR(tau_eta,lepton,"nom");
  //nom_weight *= FakeRate_unfactorised(tau_pt,tau_eta,ratio,"nom");
  //nom_weight *= FakeRate_DY(tau_pt,tau_eta,ratio,"nom");
  //nom_weight *= GetTopPtWeight(top_pt,"nom");


  map<TString, float> simple_weights;
  for (unsigned int i = 0; i<systematics.size(); ++i) {
    if (systematics[i]=="TES") continue; //Tau energy scale is special, it's not a change in event weight
    TString key_nom = systematics[i]+"_nom";
    simple_weights[key_nom] = GeneralWeightFunction(systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt,"nom");
    nom_weight *= simple_weights[key_nom];

    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key = systematics[i]+"_"+variations[j];
      simple_weights[key] = GeneralWeightFunction(systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt,variations[j]);
    }

  }


  map<TString, float> weights;
  weights["nominal"] = nom_weight;
  for (unsigned int i = 0; i<systematics.size(); ++i) {
    if (systematics[i]=="TES") continue; //Tau energy scale is special, it's not a change in event weight
    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key_nom = systematics[i]+"_nom";
      TString key = systematics[i]+"_"+variations[j];
      weights[key] = nom_weight/simple_weights[key_nom]*simple_weights[key];
    }
  }


  return weights;

}

