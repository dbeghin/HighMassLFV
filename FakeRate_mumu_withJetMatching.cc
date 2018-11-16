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
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop(phase, type, out_name, hCounter, hCounter2);
  return 0;
}


void IIHEAnalysis::Loop(string phase, string type_of_data, string out_name) {
  if (fChain == 0) return;

  bool DY, data;
  if (type_of_data == "DYinc" || type_of_data == "DYhighM" || type_of_data == "DY") {
    DY = true;
    data = false;
  }
  else if (type_of_data == "Data" || type_of_data == "data" || type_of_data == "singlemu" || type_of_data == "SingleMu") {
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
  histo_names.push_back("mu_pt");          nBins.push_back(500);  x_min.push_back(0);    x_max.push_back(500);
  histo_names.push_back("mu_eta");         nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
  histo_names.push_back("mu_phi");         nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
  histo_names.push_back("mu1_pt");         nBins.push_back(500);  x_min.push_back(0);    x_max.push_back(500);
  histo_names.push_back("mu1_eta");        nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
  histo_names.push_back("mu1_phi");        nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
  histo_names.push_back("mu2_pt");         nBins.push_back(500);  x_min.push_back(0);    x_max.push_back(500);
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
    h[i]->Sumw2();
  }

  vector<TString> h_names;              vector<int> nBins2;     vector<float> x_min2,   x_max2; 
  h_names.push_back("taupt_pass");      nBins2.push_back(1000); x_min2.push_back(0);    x_max2.push_back(1000);
  h_names.push_back("taupt_fail");      nBins2.push_back(1000); x_min2.push_back(0);    x_max2.push_back(1000);
  h_names.push_back("jetpt_pass");      nBins2.push_back(1000); x_min2.push_back(0);    x_max2.push_back(1000);
  h_names.push_back("jetpt_fail");      nBins2.push_back(1000); x_min2.push_back(0);    x_max2.push_back(1000);
  h_names.push_back("ptratio_pass");    nBins2.push_back(2000); x_min2.push_back(0);    x_max2.push_back(20);
  h_names.push_back("ptratio_fail");    nBins2.push_back(2000); x_min2.push_back(0);    x_max2.push_back(20);
  h_names.push_back("tau_MVA");         nBins2.push_back(200);  x_min2.push_back(-1);   x_max2.push_back(1);

  vector<TString> dms;
  dms.push_back("DM0");
  dms.push_back("DM1");
  dms.push_back("DM10");

  vector<TString> eta;
  eta.push_back("barrel");
  eta.push_back("endcap");

  vector<TString> trigger;                int indiff_number, fired_number;
  trigger.push_back("tautrfired");        fired_number = 0;
  trigger.push_back("tautrindiff");       indiff_number = 1;

  vector<TString> pt_range;
  pt_range.push_back("tau_pt_30_50");
  pt_range.push_back("tau_pt_50_100");
  pt_range.push_back("tau_pt_100");

  vector<TH1F*> hh[h_names.size()][dms.size()][eta.size()][trigger.size()];

  for (unsigned int i = 0; i<h_names.size(); ++i) {
    for (unsigned int j = 0; j<dms.size(); ++j) {
      for (unsigned int k = 0; k<eta.size(); ++k) {
        for (unsigned int l = 0; l<trigger.size(); ++l) {
          for (unsigned int m = 0; m<pt_range.size(); ++m) {
            TString nname = h_names[i]+"_"+dms[j]+"_"+eta[k]+"_"+trigger[l]+"_"+pt_range[m];
            hh[i][j][k][l].push_back( new TH1F(nname, nname, nBins2[i], x_min2[i], x_max2[i]) ); 
            hh[i][j][k][l][m]->Sumw2();

          }
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
    if (jEntry % 10 == 0) cout << endl << "Processed events: " << jEntry << " of " << nEntries;

    nb = fChain->GetEntry(jEntry);
    nbytes += nb;

    float final_weight = 1;



    //Is one of the triggers fired?
    bool PassMuonTrigger = false;
    if (trig_HLT_IsoMu24_accept || trig_HLT_IsoTkMu24_accept) PassMuonTrigger = true;
    if (!PassMuonTrigger) continue;


    //start muon counting loop
    //FIXME
    /*int Nmu = 0;
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
    if (electron) continue;*/


    vector<TLorentzVector> gen_tauh_p4;
    gen_tauh_p4.clear();
    if(!data) {  
      for (unsigned int iGen = 0; iGen < mc_px->size(); iGen++){

        TLorentzVector gen_part, gen_part2, gen_part3;
        gen_part.SetPxPyPzE(mc_px->at(iGen),mc_py->at(iGen),mc_pz->at(iGen),mc_energy->at(iGen));
        bool isTau = abs(mc_pdgId->at(iGen))==15  ? true : false ;
        unsigned int  moth_ind = mc_mother_index->at(iGen).at(0);
        bool ishadronicdecay(false);
        int neutrino = 0;
        bool leptau= false;
        if( isTau) {

          for (unsigned int iGen2 = 0; iGen2 < mc_px->size(); iGen2++){

            gen_part2.SetPxPyPzE(mc_px->at(iGen2),mc_py->at(iGen2),mc_pz->at(iGen2),mc_energy->at(iGen2));
            if(fabs(mc_pdgId->at(iGen2))==11 || fabs(mc_pdgId->at(iGen2))==13) {
              if((mc_mother_index->at(iGen2).at(0)) > 0) {
                if(fabs(mc_pdgId->at(mc_mother_index->at(iGen2).at(0))) == 15 && int(mc_mother_index->at(iGen2).at(0)) == int(iGen) ) {
                  leptau=true;
                  break;
                }
              }
            }
          }
        }

        if(!(leptau)) {
          for (unsigned int iGen2 = 0; iGen2 < mc_px->size(); iGen2++){
            gen_part3.SetPxPyPzE(mc_px->at(iGen2),mc_py->at(iGen2),mc_pz->at(iGen2),mc_energy->at(iGen2));
            if(fabs(mc_pdgId->at(iGen2))== 16 || fabs(mc_pdgId->at(iGen2))== 12 || fabs(mc_pdgId->at(iGen2))== 14 ) {
              if((mc_mother_index->at(iGen2).at(0)) > 0) {
                if(fabs(mc_pdgId->at(mc_mother_index->at(iGen2).at(0))) == 15. && int(mc_mother_index->at(iGen2).at(0)) == int(iGen) ) {


                  neutrino++ ;
                }
              }
            }
          }
        }
        if(neutrino =! 1) continue;
        gen_part = gen_part - gen_part3; // subtracting neutrino 4 momentum
        gen_tauh_p4.push_back(gen_part);
      }
    }




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
        //if (fabs(mu_gt_dxy_firstPVtx->at(iMu1)) > 0.045) continue;
        //if (fabs(mu_gt_dz_firstPVtx->at(iMu1)) > 0.2) continue;
        float reliso = mu_pfIsoDbCorrected04->at(iMu1);
        if (reliso > 0.15) continue;

        if (mu_gt_pt->at(iMu2) < 26.0) continue;
        if (fabs(mu_gt_eta->at(iMu2)) > 2.4) continue;
        if (!mu_isPFMuon->at(iMu2)) continue; //medium ID
        if (!mu_isMediumMuon->at(iMu2)) continue; //medium ID
        //if (fabs(mu_gt_dxy_firstPVtx->at(iMu2)) > 0.045) continue;
        //if (fabs(mu_gt_dz_firstPVtx->at(iMu2)) > 0.2) continue;
        reliso = mu_pfIsoDbCorrected04->at(iMu2);
        if (reliso > 0.15) continue;

        TLorentzVector mu1_p4, mu2_p4, total_p4, met_p4, metmu_p4;
        mu1_p4.SetPtEtaPhiM(mu_gt_pt->at(iMu1), mu_gt_eta->at(iMu1), mu_gt_phi->at(iMu1), mu_mass);
        mu2_p4.SetPtEtaPhiM(mu_gt_pt->at(iMu2), mu_gt_eta->at(iMu2), mu_gt_phi->at(iMu2), mu_mass);
        total_p4 = mu1_p4 + mu2_p4;

        if (total_p4.M() < 70 || total_p4.M() > 110) continue;

        if (mu_gt_charge->at(iMu1) * mu_gt_charge->at(iMu2) > 0) continue; //SS veto

        if (!data) {
          final_weight = mc_w_sign*GetReweight_mumu(mc_trueNumInteractions, mu1_p4.Pt(), mu1_p4.Eta(), mu2_p4.Pt(), mu2_p4.Eta());
        }

        met_p4.SetPtEtaPhiM(MET_T1Txy_Pt, 0, MET_T1Txy_phi, 0);
        metmu_p4 = met_p4 + mu1_p4;


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
          if (tau_againstMuonTight3->at(iTau) < 0.5) continue;  //FIXME
          if (tau_againstElectronVLooseMVA6->at(iTau) < 0.5) continue;
          //if (tau_againstMuonLoose3->at(iTau) < 0.5) continue;  //FIXME
          //if (tau_againstElectronTightMVA6->at(iTau) < 0.5) continue;
          TLorentzVector tau_p4;
          tau_p4.SetPxPyPzE(tau_px->at(iTau), tau_py->at(iTau), tau_pz->at(iTau), tau_energy->at(iTau));

          // taus NOT matched to gen taus
          bool is_not_gen_matched= false;
          for (unsigned int iGen = 0; iGen < gen_tauh_p4.size(); ++iGen) {
                  if(tau_p4.DeltaR(gen_tauh_p4[iGen]) < 0.2) continue;
            is_not_gen_matched=true;
          }
          if(!data && !is_not_gen_matched) continue; // reject reco tau which matches to gen tau


          if (tau_p4.DeltaR(mu1_p4) < 0.5) continue;
          if (tau_p4.DeltaR(mu2_p4) < 0.5) continue;

          // MATCH TAUS TO AK4 jets
          bool matched_to_reco_jet=false;
          TLorentzVector jet_p4(0.,0.,0.,0.);
          for (unsigned int ijet = 0; ijet < jet_pt->size(); ijet++){
            if(!(fabs(jet_eta->at(ijet)) < 2.3)) continue;
            if(!(jet_isJetIDLoose->at(ijet))) continue;
            TLorentzVector jet_p4_tmp;
            jet_p4_tmp.SetPxPyPzE(jet_px->at(ijet), jet_py->at(ijet), jet_pz->at(ijet), jet_energy->at(ijet));
            if(!(tau_p4.DeltaR(jet_p4_tmp) < 0.2)) continue;
            matched_to_reco_jet=true;
            jet_p4=jet_p4_tmp;
            break;

          }

          if(!(matched_to_reco_jet)) continue;

          // NEED PT< ETA and DM sperateuons, lets talk!!!

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

          int j_dm = -1, k_eta = -1, k_pt = -1;
          bool bTautrigger = false;
          if (tau_decayMode->at(iTau) == 0) {
            j_dm = 0;
          }
          else if (tau_decayMode->at(iTau) == 1 || tau_decayMode->at(iTau) == 2) {
            j_dm = 1;
          }
          else if (tau_decayMode->at(iTau) == 10 || tau_decayMode->at(iTau) == 11) {
            j_dm = 2;
          }
          // ETA CHANGE
          if (fabs(tau_eta->at(iTau)) < 1.46) {
            k_eta = 0;
          }
          else if (fabs(tau_eta->at(iTau)) > 1.56) {
            k_eta = 1;
          }
          else {
            continue;
          }

          //PT Range

          if(tau_pt->at(iTau) >= 30. && tau_pt->at(iTau) < 50.) {
            k_pt=0;

          } else if (tau_pt->at(iTau) >= 50. && tau_pt->at(iTau) < 100.) {

            k_pt=1;
          } else {
            k_pt=2;
          }
          if (trig_HLT_VLooseIsoPFTau140_Trk50_eta2p1_accept) {
            bTautrigger = true;
          }
          // =----- TO DO +++++++>>>>>>>>>>>   ADD JET PT histo and its ratio
          double ptratio = 1.;
          if (jet_p4.Pt() != 0) ptratio = tau_pt->at(iTau)/jet_p4.Pt();
          
          //Tau histos
          if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) {
            hh[0][j_dm][k_eta][indiff_number][k_pt]->Fill(tau_pt->at(iTau), final_weight);
	    hh[2][j_dm][k_eta][indiff_number][k_pt]->Fill(jet_p4.Pt(), final_weight);
	    hh[4][j_dm][k_eta][indiff_number][k_pt]->Fill(ptratio, final_weight);

            if (bTautrigger) hh[0][j_dm][k_eta][fired_number][k_pt]->Fill(tau_byIsolationMVArun2v1DBoldDMwLTraw->at(iTau), final_weight);
          }
          if ((tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) && (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5)) {
            hh[1][j_dm][k_eta][indiff_number][k_pt]->Fill(tau_pt->at(iTau), final_weight);
	    hh[3][j_dm][k_eta][indiff_number][k_pt]->Fill(jet_p4.Pt(), final_weight);
	    hh[5][j_dm][k_eta][indiff_number][k_pt]->Fill(ptratio, final_weight);

            if (bTautrigger) hh[1][j_dm][k_eta][fired_number][k_pt]->Fill(tau_byIsolationMVArun2v1DBoldDMwLTraw->at(iTau), final_weight);
          }
          hh[6][j_dm][k_eta][indiff_number][k_pt]->Fill(tau_byIsolationMVArun2v1DBoldDMwLTraw->at(iTau), final_weight);

          if (bTautrigger) hh[6][j_dm][k_eta][fired_number][k_pt]->Fill(tau_byIsolationMVArun2v1DBoldDMwLTraw->at(iTau), final_weight);
        }//loop over taus
      }//loop over mus
    }//loop over muons
  }//loop over events

  file_out->cd();
  h_reweight->Write();
  for (unsigned int i = 0; i<histo_names.size(); ++i) h[i]->Write();
  for (unsigned int i=0; i<h_names.size(); ++i) for (unsigned int j=0; j<dms.size(); ++j) for (unsigned int k=0; k<eta.size(); ++k) for (unsigned int l=0; l<trigger.size(); ++l) for (unsigned int m=0; m<pt_range.size(); ++m)   hh[i][j][k][l][m]->Write();

  file_out->Close();

}
