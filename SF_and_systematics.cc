#define IIHEAnalysis_cxx
#include "IIHEAnalysis_2016.h"

using namespace std;


map<TString, float> GetWeightSys(int CR_number, int n_vert, TLorentzVector tau_p4, float ratio, TLorentzVector mu_p4, TString lepton, float top_pt_1, float top_pt_2) {

  vector<TString> systematics = GetSys();      

  
  vector<TString> variations;
  variations.push_back("up");
  variations.push_back("down");


  float mu_pt = mu_p4.Pt();
  float mu_eta = mu_p4.Eta();
  float tau_pt = tau_p4.Pt();
  float tau_eta = tau_p4.Eta();

  float nom_weight = 1;


  map<TString, float> simple_weights;
  for (unsigned int i = 0; i<systematics.size(); ++i) {

    TString key_nom = systematics[i]+"_nom";
    simple_weights[key_nom] = GeneralWeightFunction(CR_number,systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt_1,top_pt_2,"nom");
    nom_weight *= simple_weights[key_nom];
    if (simple_weights[key_nom] == 0) cout << "Weight is ZERO: " << systematics[i] << endl;

    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key = systematics[i]+"_"+variations[j];
      simple_weights[key] = GeneralWeightFunction(CR_number,systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt_1,top_pt_2,variations[j]);
    }

  }


  map<TString, float> weights;
  weights["nominal"] = nom_weight;
  for (unsigned int i = 0; i<systematics.size(); ++i) {

    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key_nom = systematics[i]+"_nom";
      TString key = systematics[i]+"_"+variations[j];
      float temp_weight= 0;
      if (simple_weights[key_nom] != 0) temp_weight = nom_weight/simple_weights[key_nom]*simple_weights[key];
      weights[key] = temp_weight;
    }
  }


  return weights;

}

