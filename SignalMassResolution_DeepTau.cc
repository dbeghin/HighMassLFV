#define IIHEAnalysis_cxx
#include "IIHEAnalysis_2016.h"
#include "meta.h"

using namespace std;

int main(int argc, char** argv) {
  string out = *(argv + 1);
  string out_name= out;
  string in = *(argv + 2);
  string inname= in;
  string controlregion_in = *(argv + 3);
  string controlregion= controlregion_in;
  string type_in = *(argv + 4);
  string type= type_in;
  TFile *fIn = TFile::Open(inname.c_str());
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  TTree* mmeta = (TTree*) fIn->Get("meta");
  meta* m = new meta(mmeta);
  Float_t nEvents = m->Loop(type);

  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop(controlregion, type, out_name, nEvents);
  return 0;
}


//Get weighted events
Float_t meta::Loop(string type_of_data) {
  if (fChain == 0) return -1;

  bool data;
  if (type_of_data == "Data" || type_of_data == "data" || type_of_data == "singlephoton" || type_of_data == "SinglePhoton" || type_of_data == "singlemu" || type_of_data == "SingleMu") {
    data = true;
  }
  else {
    data = false;
  }

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  Float_t nEvents = -1;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if (data) {
      nEvents = nEventsRaw;
    }
    else {
      nEvents = mc_nEventsWeighted;
    }
    break;
  }
  return nEvents;
}


void IIHEAnalysis::Loop(string controlregion, string type_of_data, string out_name, Float_t nEvents) {
   if (fChain == 0) return;


   //string out_name = "out_"+type_of_data+".root";
   TFile* file_out = new TFile(out_name.c_str(),"RECREATE");

   const float mu_mass = 0.10565837;
   const float pi = 3.14159;

   //list here the names and x-axis ranges of all gen-level histos we wish to create :
   vector<TString> histo_names;         vector<int> nBins;      vector<float> x_min,    x_max; 
   histo_names.push_back("Mgen");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mvis_nodisc");  nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mtot_nodisc");  nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mcol_nodisc");  nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mvis_res");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("Mtot_res");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("Mcol_res");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("oldMcol_res");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("Mvis_wdisc");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mtot_wdisc");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mcol_wdisc");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("oldMcol_wdisc");    nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("tauMETdphi_wdisc"); nBins.push_back(32);  x_min.push_back(0);     x_max.push_back(3.2);
   histo_names.push_back("Mcol_dphiplus");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("oldMcol_dphiplus");    nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mcol_res_dphiplus");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("tauMETdphi_dphiplus"); nBins.push_back(32);  x_min.push_back(0);     x_max.push_back(3.2);
   histo_names.push_back("Mcol_dphiminus");       nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("oldMcol_dphiminus");    nBins.push_back(8000);  x_min.push_back(0);     x_max.push_back(8000);
   histo_names.push_back("Mcol_res_dphiminus");   nBins.push_back(2000);  x_min.push_back(-1);    x_max.push_back(1);
   histo_names.push_back("tauMETdphi_dphiminus"); nBins.push_back(32);  x_min.push_back(0);     x_max.push_back(3.2);
   histo_names.push_back("tau_eta");              nBins.push_back(100);  x_min.push_back(-5);     x_max.push_back(5);
   histo_names.push_back("mu_eta");               nBins.push_back(100);  x_min.push_back(-5);     x_max.push_back(5);

   vector<TH1F*> h;
   for (unsigned int i = 0; i<histo_names.size(); ++i) {
     h.push_back( new TH1F(histo_names[i], histo_names[i], nBins[i], x_min[i], x_max[i]) ); 
     h[i]->Sumw2();
   }



   Long64_t nEntries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   long n1=0, n2=0;
   int print_count = 0;
   //start loop over all events
   for (Long64_t jEntry = 0; jEntry < nEntries; ++jEntry) {
      Long64_t iEntry = LoadTree(jEntry);
      if (iEntry < 0) break;
      if (jEntry % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", jEntry, nEntries);

      nb = fChain->GetEntry(jEntry);
      nbytes += nb;

      float mcWeight = 1;
      if (mc_weight > 0) mcWeight = 1;
      else if (mc_weight < 0) mcWeight = -1;
      else mcWeight = 0;
      double final_weight = mcWeight;
      

      //start loop over all simulated particules
      int moth_ind = -1, tau_ind = -1;
      TLorentzVector gen_mup4, gen_taup4, gen_totalp4, p4, pp4;

      for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
	moth_ind = mc_mother_index->at(iMC).at(0);
	if (abs(mc_pdgId->at(iMC)) == 15) {
	  p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
	  if (p4.Pt() > 10000) continue;
	  gen_taup4 = p4;
	  tau_ind = iMC;
	}
      }


      vector<TLorentzVector> tauhp4;
      for (unsigned int iMC = 0; iMC < mc_tau_had_pt->size(); ++iMC) {
	//cout << mc_tau_had_pt->at(iMC) << " " << mc_tau_had_eta->at(iMC) << " " << mc_tau_had_phi->at(iMC) << " " << mc_tau_had_energy->at(iMC) << endl;
	p4.SetPtEtaPhiE(mc_tau_had_pt->at(iMC), mc_tau_had_eta->at(iMC), mc_tau_had_phi->at(iMC), mc_tau_had_energy->at(iMC));
	float DRthr = 0.2;
	bool hadtau = true;
	for (unsigned int iMC2 = 0; iMC2 < mc_tau_pt->size(); ++iMC2) {
	  pp4.SetPtEtaPhiE(mc_tau_pt->at(iMC2), mc_tau_eta->at(iMC2), mc_tau_phi->at(iMC2), mc_tau_energy->at(iMC2));
	  if ((p4.DeltaR(pp4) < DRthr) && (mc_tau_decay->at(iMC2) == 0 || mc_tau_decay->at(iMC2) == 1)) hadtau = false;
	}
	if (!hadtau) continue;
	tauhp4.push_back( p4 );
      }


      vector<TLorentzVector> tauh2p4;
      tauh2p4.clear();
      vector<TLorentzVector> taup4, tauvisp4, anyleptonp4, genmup4, genelep4;
      taup4.clear();
      tauvisp4.clear();
      anyleptonp4.clear();
      genmup4.clear();
      genelep4.clear();
      vector<int> vec_tau_ind, tau_dm;
      vec_tau_ind.clear(), tau_dm.clear();
      //start loop over all simulated particules
      moth_ind = -1;
      TLorentzVector mup4, nutaup4, totalp4, totalvisp4, totalvismetp4;
      bool foundmu = false, foundtau = false;
      vector<bool> tauh;
      int n_nus = 0;
      for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
        if (abs(mc_pdgId->at(iMC)) == 15) {
          p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
	  if (p4.Pt() > 10000) continue;
	  foundtau = true;
	  taup4.push_back(p4);
	  tauvisp4.push_back(p4);
	  vec_tau_ind.push_back(iMC);
	  tauh.push_back(true);
	  tau_dm.push_back(2);
        }
        if (abs(mc_pdgId->at(iMC)) == 11 || abs(mc_pdgId->at(iMC)) == 13) {
          if (mc_pt->at(iMC) < 10) continue;
          p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
          if (p4.Pt() < 10) continue;
          if (p4.Pt() > 10000) continue;
          anyleptonp4.push_back(p4);
          if (abs(mc_pdgId->at(iMC)) == 13) {
	    genmup4.push_back(p4);
	    h[22]->Fill(p4.Eta(), final_weight);
	  }
          if (abs(mc_pdgId->at(iMC)) == 11) genelep4.push_back(p4);
        }
      }
      //find out about the tau daughters
      for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
        moth_ind = mc_mother_index->at(iMC).at(0);
        if (moth_ind < 0) continue;
        if (abs(mc_pdgId->at(moth_ind)) == 15) {
          for (unsigned int iTau = 0; iTau<vec_tau_ind.size(); ++iTau) {
            if (moth_ind == vec_tau_ind[iTau]) {
	      if (abs(mc_pdgId->at(iMC)) == 16) {
		nutaup4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
		tauvisp4[iTau] = taup4[iTau] - nutaup4;
		++n_nus;
	      }
	      else if (abs(mc_pdgId->at(iMC)) == 11) {
		//not a hadron
		tauh[iTau] = false;
		tau_dm[iTau] = 0;
	      }
	      else if (abs(mc_pdgId->at(iMC)) == 13) {
		//not a hadron
		tauh[iTau] = false;
		tau_dm[iTau] = 1;
	      }
	    }
          }
        }
      }
      for (unsigned int iTau = 0; iTau<vec_tau_ind.size(); ++iTau) {
        if (tauh[iTau]) {
	  tauh2p4.push_back(tauvisp4[iTau]);
	  anyleptonp4.push_back(tauvisp4[iTau]);
	  h[21]->Fill(tauvisp4[iTau].Eta(), final_weight);
	}
      }
      


      for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
	moth_ind = mc_mother_index->at(iMC).at(0);
	if (abs(mc_pdgId->at(iMC))  == 13) {
	  if (mc_pt->at(iMC) < 10) continue;
	  p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
	  if (p4.Pt() < 10) continue;
	  if (p4.Pt() > 10000) continue;
	  if (moth_ind == tau_ind) continue;
	  gen_mup4 = p4;
	}
      }
      if (print_count < 20) {
	++print_count;
	cout << endl;
	for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) cout << iMC << "  " << mc_pdgId->at(iMC) << "  " << mc_mother_index->at(iMC).at(0) << "  " << mc_pt->at(iMC) << "  " << mc_eta->at(iMC) << "  " << mc_phi->at(iMC) << endl;
      }
      gen_totalp4 = gen_taup4 + gen_mup4;
      h[0]->Fill(gen_totalp4.M(), final_weight);


      bool PassMuonTrigger = false;
      if (trig_HLT_Mu50_accept || trig_HLT_TkMu50_accept) PassMuonTrigger = true;
      //if (trig_HLT_IsoMu27_accept || trig_HLT_IsoTkMu27_accept) PassMuonTrigger = true;
      if (!PassMuonTrigger) continue;




      //Sort muons, taus by decreasing pt
      float pt = 0.0;
      int highest = -1;
      vector<int> orderedMu, orderedTau;
      vector<int> rest, rest2;



      //sorting muons
      for (unsigned int ii = 0; ii < mu_ibt_pt->size(); ++ii) {
        rest.push_back(ii);
      }
      while (rest.size()>0) {
	rest2.clear();
	highest = -1;
        pt = -10000;
        for (unsigned int ii = 0; ii < rest.size(); ++ii) {
          if (mu_ibt_pt->at(rest[ii]) > pt) {
            pt = mu_ibt_pt->at(rest[ii]);
            if (highest > -1) rest2.push_back(highest);
            highest = rest[ii];
          }
          else {
	    rest2.push_back(rest[ii]);
	  }
        }
        orderedMu.push_back(highest);
        rest = rest2;
      }

      //sorting taus
      rest.clear();
      for (unsigned int ii = 0; ii < tau_pt->size(); ++ii) {
        rest.push_back(ii);
      }
      while (rest.size()>0) {
	rest2.clear();
	highest = -1;
        pt = -10000;
        for (unsigned int ii = 0; ii < rest.size(); ++ii) {
          if (tau_pt->at(rest[ii]) > pt) {
            pt = tau_pt->at(rest[ii]);
            if (highest > -1) rest2.push_back(highest);
            highest = rest[ii];
          }
          else {
	    rest2.push_back(rest[ii]);
	  }
        }
        orderedTau.push_back(highest);
        rest = rest2;
      }



      //start loop over reconstructed muons
      bool filled_histos = false;
      for (unsigned int ii = 0; ii < orderedMu.size(); ++ii) {
	if (filled_histos) break;//if we've filled the histos once, break out of the loop
	int iMu = orderedMu[ii];
	bool goodGlob = mu_isGlobalMuon->at(iMu) && mu_gt_normalizedChi2->at(iMu) < 3 && mu_combinedQuality_chi2LocalPosition->at(iMu) < 12 && mu_combinedQuality_trkKink->at(iMu) < 20;
	bool isMedium2016 = mu_isLooseMuon->at(iMu) && mu_innerTrack_validFraction->at(iMu) > 0.49 && mu_segmentCompatibility->at(iMu) > (goodGlob ? 0.303 : 0.451);


	TLorentzVector mu_p4, mu_ibt_transp4;
	mu_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu), mu_ibt_eta->at(iMu), mu_ibt_phi->at(iMu), mu_mass);
	mu_ibt_transp4.SetPxPyPzE(mu_ibt_px->at(iMu), mu_ibt_py->at(iMu), 0, mu_ibt_pt->at(iMu));

	//start loop over reconstructed taus
	for (unsigned int jj = 0; jj < orderedTau.size(); ++jj) {
	  if (filled_histos) break;//if we've filled the histos once, break out of the loop
	  int iTau = orderedTau[jj];

	  TLorentzVector tau_p4, tau_TES_p4, vis_p4, met_p4, metmu_p4, total_p4;
	  float met_px = MET_FinalCollection_Px;
	  float met_py = MET_FinalCollection_Py;
	  float met_pt = MET_FinalCollection_Pt;
	  tau_p4.SetPtEtaPhiE(tau_pt->at(iTau), tau_eta->at(iTau), tau_phi->at(iTau), tau_energy->at(iTau));
	  met_p4.SetPxPyPzE(met_px, met_py, 0, met_pt);
	  
	  vis_p4 = tau_p4 + mu_p4;
	  total_p4 = vis_p4 + met_p4;
	  metmu_p4 = met_p4 + mu_p4;
	  
	  if (mu_ibt_pt->at(iMu) < 53.0) continue;
	  if (!mu_isHighPtMuon->at(iMu)) continue;
	  if (fabs(mu_ibt_eta->at(iMu)) > 2.4) continue;
	  if (tau_pt->at(iTau) < 0) continue;
	  if (tau_p4.Pt() < 50.0) continue;
	  if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	  if (tau_decayModeFindingNewDMs->at(iTau) < 0.5) continue;
	  if (tau_decayMode->at(iTau) == 5 || tau_decayMode->at(iTau) == 6) continue;
	  if (fabs(tau_charge->at(iTau)) != 1) continue;

	  float reliso = mu_isoTrackerBased03->at(iMu); //use instead sumofpts divided by muon ibt pt

	  if (reliso > 0.1) continue;

	  bool tau_match = false;
	  for (unsigned int iGen=0; iGen<tauhp4.size(); ++iGen) {
	    if (tau_p4.DeltaR(tauhp4[iGen]) < 0.4) {
	      tau_match = true;
	      break;
	    }
	  }
	  if (tauh2p4.size() == 0) continue;
	  if (!tau_match) continue;


	  //electron veto
	  bool electron = false;
	  for (unsigned int iEle = 0; iEle < gsf_pt->size(); ++iEle) {
	    if (gsf_VID_heepElectronID_HEEPV70->at(iEle) && gsf_pt->at(iEle) > 40) electron = true;
	    if (electron) break;
	  }
	  if (electron) continue;

	  
	  //mu-to-tau veto
	  bool extra_muon = false;
	  for (unsigned int jMu = 0; jMu < mu_ibt_pt->size(); ++jMu) {
	    if (mu_ibt_pt->at(jMu) < 30) continue;
	    if (!mu_isHighPtMuon->at(jMu)) continue;
	    if (fabs(mu_ibt_eta->at(jMu)) > 2.4) continue;
	    if (mu_isoTrackerBased03->at(jMu) > 0.1) continue;
	    TLorentzVector extra_mu_p4;
	    extra_mu_p4.SetPtEtaPhiM(mu_ibt_pt->at(jMu), mu_ibt_eta->at(jMu), mu_ibt_phi->at(jMu), mu_mass);
	    if (tau_p4.DeltaR(extra_mu_p4) > 0.2) continue;
	    extra_muon = true;
	    break;
	  }
	  if (extra_muon) continue;


	  //MET recalculation because we're using the high-pt muon ID
	  TLorentzVector mu_gt_p4, mu_gt_transp4;
	  mu_gt_p4.SetPxPyPzE(0, 0, 0, 0);
	  mu_gt_transp4.SetPxPyPzE(0, 0, 0, 0);

	  float min_dR = 0.2;
	  for (unsigned int kk=0; kk<mu_gt_pt->size(); ++kk) {
	    if (!mu_isPFMuon->at(kk)) continue;
	    mu_gt_p4.SetPtEtaPhiM(mu_gt_pt->at(kk), mu_gt_eta->at(kk), mu_gt_phi->at(kk), mu_mass);
	    if (mu_gt_p4.DeltaR(mu_p4) > min_dR) continue;
	    min_dR = mu_gt_p4.DeltaR(mu_p4);
	    mu_gt_transp4.SetPxPyPzE(mu_gt_px->at(kk), mu_gt_py->at(kk), 0, mu_gt_pt->at(kk));
	  }
	  met_p4 = met_p4 + mu_gt_transp4 - mu_ibt_transp4;

	  float Mt = -1;
	  if (2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) < 0) {
	    Mt = 0;
	  }
	  else {
	    Mt = sqrt(2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) );
	  }

	  if (Mt < 120) continue;
	  

	  if (final_weight != final_weight) {
	    cout << "Not a number!!!!!!!" << endl;
	    continue;
	  }

	  float dR = tau_p4.DeltaR(mu_p4);

	  float Mcol = GetCollinearMass(tau_p4, mu_p4, met_p4);
	  float Mcol_old = GetCollinearMass_old(tau_p4, mu_p4, met_p4);

	  if (dR < 0.5) continue;
	  filled_histos = true;

	  h[1]->Fill(vis_p4.M(), final_weight);
	  h[2]->Fill(total_p4.M(), final_weight);
	  h[3]->Fill(Mcol, final_weight);

	  if (tau_byLooseDeepTau2017v2p1VSe->at(iTau) < 0.5) continue;
	  if (tau_byLooseDeepTau2017v2p1VSmu->at(iTau) < 0.5) continue;
	  if (tau_byTightDeepTau2017v2p1VSjet->at(iTau) < 0.5) continue;
	  float Mres = (vis_p4.M() - gen_totalp4.M())/gen_totalp4.M();
	  h[4]->Fill(Mres, final_weight);

	  Mres = (total_p4.M() - gen_totalp4.M())/gen_totalp4.M();
	  h[5]->Fill(Mres, final_weight);
	  
	  Mres = (Mcol - gen_totalp4.M())/gen_totalp4.M();
	  h[6]->Fill(Mres, final_weight);

	  Mres = (Mcol_old - gen_totalp4.M())/gen_totalp4.M();
	  h[7]->Fill(Mres, final_weight);

	  h[8]->Fill(vis_p4.M(), final_weight);
	  h[9]->Fill(total_p4.M(), final_weight);
	  h[10]->Fill(Mcol, final_weight);
	  h[11]->Fill(Mcol_old, final_weight);

	  float tauMET_dphi = 0;
	  if (fabs(tau_p4.Phi() - met_p4.Phi()) < pi) {
	    tauMET_dphi = fabs(tau_p4.Phi() - met_p4.Phi());
	  }
	  else {
	    tauMET_dphi = 2*pi - fabs(tau_p4.Phi() - met_p4.Phi());
	  }

	  Mres = (Mcol - gen_totalp4.M())/gen_totalp4.M();
	  h[12]->Fill(tauMET_dphi, final_weight);
	  if (tauMET_dphi < pi/2) {
	    h[13]->Fill(Mcol, final_weight);
	    h[14]->Fill(Mcol_old, final_weight);
	    h[15]->Fill(Mres, final_weight);
	    h[16]->Fill(tauMET_dphi, final_weight);
	  }
	  else {
	    h[17]->Fill(Mcol, final_weight);
	    h[18]->Fill(Mcol_old, final_weight);
	    h[19]->Fill(Mres, final_weight);
	    h[20]->Fill(tauMET_dphi, final_weight);
	  }	    

	}//loop over taus
      }//loop over muons
   }//loop over events


   TH1F* h_total_events =  new TH1F("weighted_events", "weighted_events", 1, 0, 1);
   h_total_events->Fill(0.5, nEvents);
   file_out->cd();
   h_total_events->Write();
   for (unsigned int i = 0; i<histo_names.size(); ++i) h[i]->Write();
   file_out->Close();
}
