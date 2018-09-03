#define IIHEAnalysis_cxx
#include "IIHEAnalysis_old.h"
//#include <TH1.h>
#include <TLorentzVector.h>
//#include <TCanvas.h>
#include "TString.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  string out = *(argv + 1);
  string out_name= out;
  string in = *(argv + 2);
  string inname= in;
  string phase_in = *(argv + 3);
  string phase= phase_in;
  string type_in = *(argv + 4);
  string type= type_in;
  TFile *fIn = TFile::Open(inname.c_str());
  TH1F* hCounter = (TH1F*) fIn->Get("h1");
  TH1F* hCounter2 = (TH1F*) fIn->Get("h2");
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop(phase, type, out_name, hCounter, hCounter2);
  return 0;
}

void IIHEAnalysis::Loop(string phase, string type_of_data, string out_name, TH1F* hCounter, TH1F* hCounter2) {
   if (fChain == 0) return;

   bool DY, data;
   if (type_of_data == "DYinc" || type_of_data == "DYhighM" || type_of_data == "DY") {
     DY = true;
     data = false;
   }
   else if (type_of_data == "Data" || type_of_data == "data") {
     DY = false;
     data = true;
   }
   else {
     DY = false;
     data = false;
   }

   //string out_name = "out_"+type_of_data+".root";
   TFile* file_out = new TFile(out_name.c_str(),"RECREATE");

   const float mu_mass = 0.10565837;


   //list here the names and x-axis ranges of all reco-level histos we wish to create :
   vector<TString> histo_names;             vector<int> nBins;     vector<float> x_min,   x_max; 
   histo_names.push_back("mu_pt");          nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(100);
   histo_names.push_back("mu_eta");         nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("mu_phi");         nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("mu1_pt");         nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(100);
   histo_names.push_back("mu1_eta");        nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("mu1_phi");        nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("mu2_pt");         nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(100);
   histo_names.push_back("mu2_eta");        nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("mu2_phi");        nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("ev_DRmumu");      nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(10);
   histo_names.push_back("ev_Mt_raw");      nBins.push_back(150);  x_min.push_back(0);    x_max.push_back(150);
   histo_names.push_back("ev_Mt");          nBins.push_back(150);  x_min.push_back(0);    x_max.push_back(150);
   histo_names.push_back("ev_Mvis");        nBins.push_back(40);   x_min.push_back(70);   x_max.push_back(110);
   histo_names.push_back("ev_METmumass");   nBins.push_back(150);  x_min.push_back(0);    x_max.push_back(150);
   histo_names.push_back("ev_MET");         nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(100);
   histo_names.push_back("ev_METphi");      nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("ev_Nvertex");     nBins.push_back(81);   x_min.push_back(-0.5); x_max.push_back(80.5);

   vector<TH1F*> h;
   for (unsigned int i = 0; i<histo_names.size(); ++i) {
     h.push_back( new TH1F(histo_names[i], histo_names[i], nBins[i], x_min[i], x_max[i]) ); 
   }

   vector<TString> htau_names;              vector<int> nBins_tau;     vector<float> x_min_tau,   x_max_tau; 
   htau_names.push_back("taupt_pass");      nBins_tau.push_back(1000); x_min_tau.push_back(0);    x_max_tau.push_back(1000);
   htau_names.push_back("taupt_fail");      nBins_tau.push_back(1000); x_min_tau.push_back(0);    x_max_tau.push_back(1000);
   htau_names.push_back("tau_MVA");         nBins_tau.push_back(200);  x_min_tau.push_back(-1);   x_max_tau.push_back(1);

   vector<TString> dms;
   dms.push_back("DM0");
   dms.push_back("DM1");
   dms.push_back("DM10");

   vector<TString> eta;
   eta.push_back("barrel");
   eta.push_back("endcap");

   vector<TString> trigger;
   trigger.push_back("tautrfired");
   trigger.push_back("tautrindiff");

   vector<TH1F*> htau[histo_names.size()][dms.size()][eta.size()];
   for (unsigned int i = 0; i<htau_names.size(); ++i) {
     for (unsigned int j = 0; j<dms.size(); ++j) {
       for (unsigned int k = 0; k<eta.size(); ++k) {
	 for (unsigned int l = 0; l<trigger.size(); ++l) {
	   htau[i][j][k].push_back( new TH1F(htau_names[i]+"_"+dms[j]+"_"+eta[k]+"_"+trigger[l], htau_names[i]+"_"+dms[j]+"_"+eta[k]+"_"+trigger[l], nBins_tau[i], x_min_tau[i], x_max_tau[i]) ); 
	 }
       }
     }
   }


   TH1F* h_reweight = new TH1F("h_r", "h_r", 100, -2, 2);

   Long64_t nEntries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   int print_count = 0;
   //start loop over all events
   for (Long64_t jEntry = 0; jEntry < nEntries; ++jEntry) {
      Long64_t iEntry = LoadTree(jEntry);
      if (iEntry < 0) break;
      if (jEntry % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", jEntry, nEntries);

      nb = fChain->GetEntry(jEntry);
      nbytes += nb;

      float final_weight = 1;

      
      bool gen_mutau = false;
      bool DYtomumu = false;
      TLorentzVector gentau_p4;
      if (DY) {
	int moth_ind = -1, tau1_ind = -1, tau2_ind = -1, mu1_ind = -1, mu2_ind = -1, tau1_dm = -1, tau2_dm = -1;
	TLorentzVector mu1_p4, mu2_p4, tau1_p4, tau2_p4, tau1_visp4, tau2_visp4, gen_mumu_p4, nutau1_p4, nutau2_p4;
	//start loop over all simulated particules
	for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {

          //Find 2 mus whose mothers are either photons or Z bosons
          if (abs(mc_pdgId->at(iMC)) == 13) {
            moth_ind = mc_mother_index->at(iMC).at(0);
            if (moth_ind < 0) continue;
            if (abs(mc_pdgId->at(moth_ind)) == 22 || abs(mc_pdgId->at(moth_ind)) == 23) {
              if (mu1_ind == -1) {
                mu1_ind = iMC;
                mu1_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
                if (mu1_p4.Pt() < 15) mu1_ind = -1;
              }
              else if (mu1_ind > -1 && mu2_ind == -1) {
                mu2_ind = iMC;
                mu2_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
                if (mu2_p4.Pt() < 15) mu2_ind = -1;
              }
              else if (mu1_ind > -1 && mu2_ind > -1) {
                cout << "!!!!!!!!!!!!!!!" << endl << "Error : more than 2 mus" << endl << "!!!!!!!!!!!!!!!" << endl << endl;
                break;
              }//end locating 1st and 2nd gen taus
            }//end condition on taus' mothers
          }//end condition on particle's id = tau

          //Find 2 taus whose mothers are either photons or Z bosons
          if (abs(mc_pdgId->at(iMC)) == 15) {
            moth_ind = mc_mother_index->at(iMC).at(0);
            if (moth_ind < 0) continue;
            if (abs(mc_pdgId->at(moth_ind)) == 22 || abs(mc_pdgId->at(moth_ind)) == 23) {
              if (tau1_ind == -1) {
                tau1_ind = iMC;
                tau1_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
                if (tau1_p4.Pt() < 15) tau1_ind = -1;
              }
              else if (tau1_ind > -1 && tau2_ind == -1) {
                tau2_ind = iMC;
                tau2_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
                if (tau2_p4.Pt() < 15) tau2_ind = -1;
              }
              else if (tau1_ind > -1 && tau2_ind > -1) {
                cout << "!!!!!!!!!!!!!!!" << endl << "Error : more than 2 taus" << endl << "!!!!!!!!!!!!!!!" << endl << endl;
                break;
              }//end locating 1st and 2nd gen taus
            }//end condition on taus' mothers
          }//end condition on particle's id = tau

          //check whether the gen particle is a neutrino
          bool neutrino;
          if (abs(mc_pdgId->at(iMC)) == 12 || abs(mc_pdgId->at(iMC)) == 14 || abs(mc_pdgId->at(iMC)) == 16) {
            neutrino = true;
          }
          else {
            neutrino = false;
          }

          if (!neutrino) {
            //Find one decay product of the tau (neutrinos excluded because they complicate things) and deduce the decay mode
            if (mc_mother_index->at(iMC).at(0) == tau1_ind) {
              if (abs(mc_pdgId->at(iMC)) == 11) {
                tau1_dm = 0;
                tau1_visp4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
              }
              else if (abs(mc_pdgId->at(iMC)) == 13) {
                tau1_dm = 1;
                tau1_visp4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
              }
              else if (abs(mc_pdgId->at(iMC)) < 7 || abs(mc_pdgId->at(iMC)) > 999) {
                tau1_dm = 2;
              }
            }
            else if (mc_mother_index->at(iMC).at(0) == tau2_ind) {
              if (abs(mc_pdgId->at(iMC)) == 11) {
                tau2_dm = 0;
                tau2_visp4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
              }
              else if (abs(mc_pdgId->at(iMC)) == 13) {
                tau2_dm = 1;
                tau2_visp4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
              }
              else if (abs(mc_pdgId->at(iMC)) < 7 || abs(mc_pdgId->at(iMC)) > 999) {
                tau1_dm = 2;
              }//end condition on gen particle's pdgId                                                                                                          
            }//end condition on mother's index                                                                                                                                                                   
	  }//neutrino condition
          else if (abs(mc_pdgId->at(iMC)) == 16) {
            //If we have a tau neutrino, we want to save its 4-momentum, so we can calculate the visible 4-momentum of the hadronic tau later
            if (mc_mother_index->at(iMC).at(0) == tau1_ind) {
              nutau1_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
            }
            else if (mc_mother_index->at(iMC).at(0) == tau2_ind) {
              nutau2_p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
            }
          }//end neutrino condition	  
	}//loop over sim particules

	if (tau1_ind != -1 && tau2_ind != -1) {
	  if (print_count < 20) {
	    ++print_count;
	    cout << endl;
	    for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) cout << iMC << "  PDG ID " << mc_pdgId->at(iMC) << "  Mother PDG ID " << mc_mother_index->at(iMC).at(0) << "  Pt " << mc_pt->at(iMC) << endl;
	    cout << endl << endl;
	  }
	  
	  if (tau1_dm == -1) tau1_dm = 2;
          if (tau2_dm == -1) tau2_dm = 2;

	  if (tau1_dm == 2 && tau2_dm == 1) {
	    gen_mutau = true;
	    gentau_p4 = tau1_p4 - nutau1_p4;
	  }
	  else if (tau1_dm == 1 && tau2_dm == 2) {
	    gen_mutau = true;
	    gentau_p4 = tau2_p4 - nutau2_p4;
	  }
	    
	  /*gen_mumu_p4 = mu1_p4 + mu2_p4;

	    if (gen_mumu_p4.M() > 50) DYtomumu = true;*/
	}
      }//end is this DY? condition



      //Is one of the triggers fired?
      bool PassMuonTrigger = false;
      if (trig_HLT_IsoMu24_accept || trig_HLT_IsoTkMu24_accept || trig_HLT_VLooseIsoPFTau140_Trk50_eta2p1_accept) PassMuonTrigger = true;
      if (!PassMuonTrigger) continue;


      //start muon counting loop
      int Nmu = 0;
      for (unsigned int iMu = 0; iMu < mu_gt_pt->size(); ++iMu) {
        if(mu_isPFMuon->at(iMu) && mu_gt_pt->at(iMu) > 10 && fabs(mu_gt_eta->at(iMu)) < 2.4 && fabs(mu_gt_dxy_firstPVtx->at(iMu)) < 0.045 && fabs(mu_gt_dz_firstPVtx->at(iMu)) < 0.2 && mu_pfIsoDbCorrected04->at(iMu) < 0.3 && mu_isMediumMuon->at(iMu)) ++Nmu;
        if (Nmu > 2) break;
      }
      if (Nmu > 2) continue; //2nd muon veto

      //electron veto
      bool electron = false;
      for (unsigned int iEle = 0; iEle < gsf_pt->size(); ++iEle) {
        if (gsf_VIDLoose->at(iEle) && gsf_pt->at(iEle) > 10 && fabs(gsf_eta->at(iEle)) < 2.5 && fabs(gsf_dxy_firstPVtx->at(iEle)) < 0.045 && fabs(gsf_dz_firstPVtx->at(iEle)) < 0.2 && gsf_passConversionVeto->at(iEle) && gsf_nLostInnerHits->at(iEle) <= 1 && gsf_relIso->at(iEle) < 0.3) electron = true;
        if (electron) break;
      }
      if (electron) continue;

      //bjet veto (medium WP for the bjet)                                                                                                                           
      /*bool bjet = false;
      for (unsigned int iJet = 0; iJet < jet_pt->size(); ++iJet) {
        if (jet_CSVv2->at(iJet) > 0.800 && jet_pt->at(iJet) > 20 && fabs(jet_eta->at(iJet)) < 2.4) bjet = true;
        if (bjet) break;
      }
      if (bjet) continue;*/



      //Sort muons, taus, by increasing isolation/decreasing pt                                                                                                                   
      float iso = 1.5, pt = 0.0;
      int lowest = -1;
      vector<int> orderedMu, orderedTau;
      vector<int> rest, rest2;
      bool interesting = false;


      //sorting muons                                                                                                                                                             
      for (unsigned int ii = 0; ii < mu_gt_pt->size(); ++ii) {
        rest.push_back(ii);
      }
      while (rest.size()>0) {
        rest2.clear();
        lowest = -1;
        iso = 10000000;
        pt = 0;
        for (unsigned int ii = 0; ii < rest.size(); ++ii) {
          if (mu_pfIsoDbCorrected04->at(rest[ii]) < iso) {
            iso = mu_pfIsoDbCorrected04->at(rest[ii]);
            pt = mu_gt_pt->at(rest[ii]);
            if (lowest > -1) rest2.push_back(lowest);
            lowest = rest[ii];
          }
          else if (mu_pfIsoDbCorrected04->at(rest[ii]) == iso && mu_gt_pt->at(rest[ii]) > pt) {
            pt = mu_gt_pt->at(rest[ii]);
            if (lowest > -1) rest2.push_back(lowest);
            lowest = rest[ii];
            interesting = true;//just a flag to see if this actually happens                                                                                                      
          }
          else {
            rest2.push_back(rest[ii]);
          }
        }
        orderedMu.push_back(lowest);
        rest = rest2;
      }




      //start loop over reconstructed muons
      bool found_mumu_pair = false;
      for (unsigned int ii = 0; ii < orderedMu.size(); ++ii) {
	if (found_mumu_pair) break;
	int iMu1 = orderedMu[ii];
	//start 2nd loop over reconstructed mus
	for (unsigned int jj = 0; jj < ii; ++jj) {
	  if (found_mumu_pair) break;
	  int iMu2 = orderedMu[jj];

	  if (mu_gt_pt->at(iMu1) < 26.0) continue;
	  if (fabs(mu_gt_eta->at(iMu1)) > 2.4) continue;
	  if (!mu_isPFMuon->at(iMu1)) continue;
	  if (!mu_isMediumMuon->at(iMu1)) continue; //medium ID
	  if (fabs(mu_gt_dxy_firstPVtx->at(iMu1)) > 0.045) continue;
	  if (fabs(mu_gt_dz_firstPVtx->at(iMu1)) > 0.2) continue;
	  float reliso = mu_pfIsoDbCorrected04->at(iMu1);
	  if (reliso > 0.15) continue;

	  if (mu_gt_pt->at(iMu2) < 26.0) continue;
	  if (fabs(mu_gt_eta->at(iMu2)) > 2.4) continue;
	  if (!mu_isPFMuon->at(iMu2)) continue; //medium ID
	  if (!mu_isMediumMuon->at(iMu2)) continue; //medium ID
	  if (fabs(mu_gt_dxy_firstPVtx->at(iMu2)) > 0.045) continue;
	  if (fabs(mu_gt_dz_firstPVtx->at(iMu2)) > 0.2) continue;
	  reliso = mu_pfIsoDbCorrected04->at(iMu2);
	  if (reliso > 0.15) continue;

	  TLorentzVector mu1_p4, mu2_p4, total_p4, met_p4, metmu_p4;
	  mu1_p4.SetPtEtaPhiM(mu_gt_pt->at(iMu1), mu_gt_eta->at(iMu1), mu_gt_phi->at(iMu1), mu_mass);
	  mu2_p4.SetPtEtaPhiM(mu_gt_pt->at(iMu2), mu_gt_eta->at(iMu2), mu_gt_phi->at(iMu2), mu_mass);
	  total_p4 = mu1_p4 + mu2_p4;

	  if (total_p4.M() < 70 || total_p4.M() > 110) continue;

	  if (!data) {
	    final_weight = mc_w_sign*GetReweight_mumu(mc_trueNumInteractions, mu1_p4.Pt(), mu1_p4.Eta(), mu2_p4.Pt(), mu2_p4.Eta());
	  }
	  
	  met_p4.SetPtEtaPhiM(MET_T1Txy_Pt, 0, MET_T1Txy_phi, 0);
	  metmu_p4 = met_p4 + mu1_p4;

	  //check if there's a DY match
	  bool DY_match = false;
	  if (DY) {
	    //we need to have a mumu pair at gen-level, Mu1 must have same sign and be DeltaR-compatible with one gen mu
	    if (gen_mutau &&  /*tau_charge->at(iTau)*gentau_charge > 0  &&*/  (mu1_p4.DeltaR(gentau_p4) < 0.5 || mu2_p4.DeltaR(gentau_p4) < 0.5) ){
	      DY_match = true;
	    }
	  }

	  if (final_weight != final_weight) continue;


	  float dR = mu1_p4.DeltaR(mu2_p4);
	  h[9]->Fill(dR, final_weight);
	  if (dR < 0.5) continue;

	  h_reweight->Fill(final_weight);

	  found_mumu_pair = true;

	  for (unsigned int iTau = 0; iTau < tau_pt->size(); ++iTau) {
	    if (tau_pt->at(iTau) < 30.0) continue;
	    if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	    if (tau_decayModeFinding->at(iTau) < 0.5) continue;
	    if (tau_againstMuonTight3->at(iTau) < 0.5) continue;
	    if (tau_againstElectronVLooseMVA6->at(iTau) < 0.5) continue;

	    TLorentzVector tau_p4;
	    tau_p4.SetPxPyPzE(tau_px->at(iTau), tau_py->at(iTau), tau_pz->at(iTau), tau_energy->at(iTau));
	    if (tau_p4.DeltaR(mu1_p4) < 0.5) continue;
	    if (tau_p4.DeltaR(mu2_p4) < 0.5) continue;
	    
	    //mu histos
	    h[0]->Fill(mu_gt_pt->at(iMu1), final_weight);
	    h[0]->Fill(mu_gt_pt->at(iMu2), final_weight);

	    h[1]->Fill(mu_gt_eta->at(iMu1), final_weight);
	    h[1]->Fill(mu_gt_eta->at(iMu2), final_weight);

	    h[2]->Fill(mu_gt_phi->at(iMu1), final_weight);
	    h[2]->Fill(mu_gt_phi->at(iMu2), final_weight);
	    

	    //Mu1 histos
	    h[3]->Fill(mu_gt_pt->at(iMu1), final_weight);
	    h[4]->Fill(mu_gt_eta->at(iMu1), final_weight);
	    h[5]->Fill(mu_gt_phi->at(iMu1), final_weight);


	    //Mu2 histos
	    h[6]->Fill(mu_gt_pt->at(iMu2), final_weight);
	    h[7]->Fill(mu_gt_eta->at(iMu2), final_weight);
	    h[8]->Fill(mu_gt_phi->at(iMu2), final_weight);

	    
	    //misc. hitos
	    h[12]->Fill(total_p4.M(), final_weight);
	    h[14]->Fill(met_p4.Pt(), final_weight);
	    h[15]->Fill(met_p4.Phi(), final_weight);
	    h[16]->Fill(pv_n, final_weight);

	    int j_dm = -1, k_eta = -1, l_trigger = 1;
	    if (tau_decayMode->at(iTau) == 0) {
	      j_dm = 0;
	    }
	    else if (tau_decayMode->at(iTau) == 1) {
	      j_dm = 1;
	    }
	    else if (tau_decayMode->at(iTau) == 10) {
	      j_dm = 2;
	    }

	    if (fabs(tau_eta->at(iTau)) < 1.5) {
	      k_eta = 0;
	    }
	    else {
	      k_eta = 1;
	    }

	    if (trig_HLT_VLooseIsoPFTau140_Trk50_eta2p1_accept) {
	      l_trigger = 0;
	    }

	    //Tau histos
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) htau[0][j_dm][k_eta][l_trigger]->Fill(tau_pt->at(iTau), final_weight);
	    if ((tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) && (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5)) htau[1][j_dm][k_eta][l_trigger]->Fill(tau_pt->at(iTau), final_weight);
	    htau[2][j_dm][k_eta][l_trigger]->Fill(tau_byIsolationMVArun2v1DBoldDMwLTraw->at(iTau), final_weight);
	  }//loop over taus
	}//loop over mus
      }//loop over muons
   }//loop over events

   file_out->cd();
   //hCounter->Write();
   //hCounter2->Write();
   h_reweight->Write();
   for (unsigned int i = 0; i<histo_names.size(); ++i) h[i]->Write();
   for (unsigned int i=0; i<htau_names.size(); ++i) for (unsigned int j=0; j<dms.size(); ++j) for (unsigned int k=0; k<eta.size(); ++k) for (unsigned int l=0; l<trigger.size(); ++l) htau[i][j][k][l]->Write();
   file_out->Close();

}
