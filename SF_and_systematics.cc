#define IIHEAnalysis_cxx
#include "IIHEAnalysis_2016.h"

using namespace std;


map<TString, float> GetWeightSys(int CR_number, int n_vert, TLorentzVector tau_p4, float ratio, TLorentzVector mu_p4, TString lepton, float top_pt_1, float top_pt_2) {

  vector<TString> systematics = GetSys();      
  vector<TString> ignore;
  ignore.push_back("TES");

  vector<TString> setto1;
  if (CR_number == 100 || CR_number == 102) {
    setto1.push_back("FRstat");
    setto1.push_back("FRsys");
  }
  
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

    bool bIgnore = false;
    for (unsigned int kk=0; kk<ignore.size(); ++kk) {
      if (systematics[i] == ignore[kk]) {
	bIgnore = true;
	break;
      }
    }
    if (bIgnore) continue;

    bool bSetto1 = false;
    for (unsigned int kk=0; kk<setto1.size(); ++kk) {
      if (systematics[i] == setto1[kk]) {
	bSetto1 = true;
	break;
      }
    }

    TString key_nom = systematics[i]+"_nom";
    simple_weights[key_nom] = GeneralWeightFunction(systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt_1,top_pt_2,"nom");
    if (bSetto1) simple_weights[key_nom] = 1;
    nom_weight *= simple_weights[key_nom];


    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key = systematics[i]+"_"+variations[j];
      simple_weights[key] = GeneralWeightFunction(systematics[i],n_vert,tau_p4,ratio,mu_p4,lepton,top_pt_1,top_pt_2,variations[j]);
      if (bSetto1) simple_weights[key] = 1;
    }

  }


  map<TString, float> weights;
  weights["nominal"] = nom_weight;
  for (unsigned int i = 0; i<systematics.size(); ++i) {

    bool bIgnore = false;
    for (unsigned int kk=0; kk<ignore.size(); ++kk) {
      if (systematics[i] == ignore[kk]) {
	bIgnore = true;
	break;
      }
    }
    if (bIgnore) continue;

    for (unsigned int j = 0; j<variations.size(); ++j) {
      TString key_nom = systematics[i]+"_nom";
      TString key = systematics[i]+"_"+variations[j];
      weights[key] = nom_weight/simple_weights[key_nom]*simple_weights[key];
    }
  }


  return weights;

}

