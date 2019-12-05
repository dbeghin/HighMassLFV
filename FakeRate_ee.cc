#define IIHEAnalysis_cxx
//#include "aux.h"
#include "IIHEAnalysis_2017.h"
#include "meta.h"

using namespace std;

int main(int argc, char** argv) {
  string out = *(argv + 1);
  string out_name= out;
  string in = *(argv + 2);
  string inname= in;
  string mc_in = *(argv + 3);
  string mc_nickname= mc_in;
  string phase_in = *(argv + 4);
  string phase= phase_in;
  string type_in = *(argv + 5);
  string type= type_in;
  TFile *fIn = TFile::Open(inname.c_str());
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  TTree* mmeta = (TTree*) fIn->Get("meta");
  meta* m = new meta(mmeta);
  Float_t nEvents = m->Loop(type);

  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop(phase, type, out_name, mc_nickname, nEvents);
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


/////////////////////////////!\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//Currently set up to calculate fake rates in the mutau region
////////////////////////////!\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//main analysis loop
void IIHEAnalysis::Loop(string controlregion, string type_of_data, string out_name, string mc_nick, Float_t nEvents) {
   if (fChain == 0) return;

   bool DY, data;
   if (type_of_data == "DYinc" || type_of_data == "DYhighM" || type_of_data == "DY") {
     DY = true;
     data = false;
   }
   else   if (type_of_data == "Data" || type_of_data == "data" || type_of_data == "singlephoton" || type_of_data == "SinglePhoton" || type_of_data == "singlemu" || type_of_data == "SingleMu") {
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
   histo_names.push_back("ele_pt");          nBins.push_back(500);  x_min.push_back(0);    x_max.push_back(500);
   histo_names.push_back("ele_eta");         nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("ele_phi");         nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("ev_DRee");         nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(10);
   histo_names.push_back("ev_Mvis");         nBins.push_back(40);   x_min.push_back(70);   x_max.push_back(110);
   histo_names.push_back("ev_Nvertex");      nBins.push_back(81);   x_min.push_back(-0.5); x_max.push_back(80.5);

   vector<TH1F*> h;
   for (unsigned int i = 0; i<histo_names.size(); ++i) {
     h.push_back( new TH1F(histo_names[i], histo_names[i], nBins[i], x_min[i], x_max[i]) ); 
     h[i]->Sumw2();
   }

   vector<TString> htau_names;                
   htau_names.push_back("taupt_ratio_pass");  
   htau_names.push_back("taupt_ratio_fail");  

   vector<TString> dms;
   dms.push_back("DM0");
   dms.push_back("DM1");
   dms.push_back("DM10");

   vector<TString> eta;
   eta.push_back("barrel");
   eta.push_back("endcap");


   vector<TH2D*> hh[htau_names.size()][dms.size()];

   for (unsigned int i = 0; i<htau_names.size(); ++i) {
     for (unsigned int k = 0; k<dms.size(); ++k) {
       for (unsigned int l = 0; l<eta.size(); ++l) {
	 TString nname = htau_names[i]+"_"+dms[k]+"_"+eta[l];
	 hh[i][k].push_back( new TH2D(nname, nname, 1000, 0, 1000, 1000, 0, 10) );
	 hh[i][k][l]->Sumw2();
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
      //if (jEntry % 10 == 0) cout << endl << "Processed events: " << jEntry << " of " << nEntries;

      nb = fChain->GetEntry(jEntry);
      nbytes += nb;

      float final_weight = 1;

      //Is one of the triggers fired?
      bool PassTrigger = false;
      if (trig_HLT_Ele27_WPTight_Gsf_accept) PassTrigger = true;
      if (!PassTrigger) continue;

      if(!trig_Flag_goodVertices_accept) continue;
      if(!trig_Flag_globalSuperTightHalo2016Filter_accept) continue;
      if(!trig_Flag_HBHENoiseFilter_accept) continue;
      if(!trig_Flag_HBHENoiseIsoFilter_accept) continue;
      if(!trig_Flag_EcalDeadCellTriggerPrimitiveFilter_accept) continue;
      if(!trig_Flag_BadPFMuonFilter_accept) continue;
      if(!trig_Flag_ecalBadCalibReduced) continue;
      if(data && !trig_Flag_eeBadScFilter_accept) continue;



      //generated level
      TLorentzVector p4, pp4;
      vector<TLorentzVector> tauhp4;
      tauhp4.clear();
      vector<TLorentzVector> anyleptonp4;
      anyleptonp4.clear();
      if (!data) {
        //start loop over all simulated particules
        for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
          if (abs(mc_pdgId->at(iMC)) == 11 || abs(mc_pdgId->at(iMC)) == 13) {
            if (mc_pt->at(iMC) < 10) continue;
            p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
            if (p4.Pt() < 10) continue;
            if (p4.Pt() > 10000) continue;
            anyleptonp4.push_back(p4);
          }
        }//end MC loop

        for (unsigned int iMC = 0; iMC < mc_tau_had_pt->size(); ++iMC) {
          p4.SetPtEtaPhiE(mc_tau_had_pt->at(iMC), mc_tau_had_eta->at(iMC), mc_tau_had_phi->at(iMC), mc_tau_had_energy->at(iMC));
          float DRthr = 0.4;
          bool hadtau = true;
          for (unsigned int iMC2 = 0; iMC2 < mc_tau_pt->size(); ++iMC2) {
            pp4.SetPtEtaPhiE(mc_tau_pt->at(iMC2), mc_tau_eta->at(iMC2), mc_tau_phi->at(iMC2), mc_tau_energy->at(iMC2));
            if ((p4.DeltaR(pp4) < DRthr) && (mc_tau_decay->at(iMC2) == 0 || mc_tau_decay->at(iMC2) == 1)) hadtau = false;
          }
          if (!hadtau) continue;
          tauhp4.push_back( p4 );
          anyleptonp4.push_back( p4 );
        }
      }//end gen-level



      //start loop over reconstructed muons
      bool found_ee_pair = false;
      for (unsigned int iEle1 = 0; iEle1 < gsf_pt->size(); ++iEle1) {
	if (found_ee_pair) break;
	//start 2nd loop over reconstructed mus
	for (unsigned int iEle2 = 0; iEle2 < iEle1; ++iEle2) {
	  if (found_ee_pair) break;

	  if (gsf_pt->at(iEle1) < 30.0) continue;
	  if (fabs(gsf_sc_eta->at(iEle1)) > 2.5) continue;
	  if (!gsf_VID_cutBasedElectronID_Fall17_94X_V1_tight->at(iEle1)) continue;

	  if (gsf_pt->at(iEle2) < 30.0) continue;
	  if (fabs(gsf_sc_eta->at(iEle2)) > 2.5) continue;
	  if (!gsf_VID_cutBasedElectronID_Fall17_94X_V1_tight->at(iEle2)) continue;

	  TLorentzVector ele1_p4, ele2_p4, total_p4;
	  ele1_p4.SetPtEtaPhiM(gsf_pt->at(iEle1), gsf_sc_eta->at(iEle1), gsf_sc_phi->at(iEle1), 0);
	  ele2_p4.SetPtEtaPhiM(gsf_pt->at(iEle2), gsf_sc_eta->at(iEle2), gsf_sc_phi->at(iEle2), 0);
	  total_p4 = ele1_p4 + ele2_p4;

	  if (total_p4.M() < 70 || total_p4.M() > 110) continue;

	  if (gsf_charge->at(iEle1) * gsf_charge->at(iEle2) > 0) continue; //SS veto

	  if (!data) {
            final_weight = mc_w_sign;
          }
	  

	  if (final_weight != final_weight) continue;


	  float dR = ele1_p4.DeltaR(ele2_p4);
	  h[3]->Fill(dR, final_weight);
	  if (dR < 0.5) continue;

	  h_reweight->Fill(final_weight);

	  found_ee_pair = true;

	  for (unsigned int iTau = 0; iTau < tau_pt->size(); ++iTau) {
	    if (tau_pt->at(iTau) < 30.0) continue;
	    if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	    if (tau_decayModeFinding->at(iTau) < 0.5) continue;
	    if (tau_againstMuonLoose3->at(iTau) < 0.5) continue;  //FIXME
	    if (tau_againstElectronLooseMVA6->at(iTau) < 0.5) continue;
	    //if (tau_againstMuonLoose3->at(iTau) < 0.5) continue;  //FIXME
	    //if (tau_againstElectronTightMVA6->at(iTau) < 0.5) continue;

	    TLorentzVector tau_p4;
	    tau_p4.SetPxPyPzE(tau_px->at(iTau), tau_py->at(iTau), tau_pz->at(iTau), tau_energy->at(iTau));
	    if (tau_p4.DeltaR(ele1_p4) < 0.5) continue;
	    if (tau_p4.DeltaR(ele2_p4) < 0.5) continue;


	    //match to gen-level
	    if (!data) {
	      bool gen_lepton = false;
	      for (unsigned int iMC = 0; iMC < anyleptonp4.size(); ++iMC) {
		if (tau_p4.DeltaR(anyleptonp4[iMC]) < 0.4) {
		  gen_lepton = true;
		  break;
		}
	      }
	      if (!gen_lepton) continue;
	    }

	    
	    // MATCH TAUS TO AK4 jets
	    bool matched_to_reco_jet=false;
	    TLorentzVector jet_p4(0.,0.,0.,0.);
	    for (unsigned int ijet = 0; ijet < jet_pt->size(); ijet++){
	      if(!(fabs(jet_eta->at(ijet)) < 2.3)) continue;
	      if(!(jet_isJetID_2017->at(ijet))) continue;
	      TLorentzVector jet_p4_tmp;
	      jet_p4_tmp.SetPxPyPzE(jet_px->at(ijet), jet_py->at(ijet), jet_pz->at(ijet), jet_energy->at(ijet));
	      if(!(tau_p4.DeltaR(jet_p4_tmp) < 0.2)) continue;
	      matched_to_reco_jet=true;
	      jet_p4=jet_p4_tmp;
	      break;

	    }

	    if(!(matched_to_reco_jet)) continue;
	    float ratio = 0;
	    if (jet_p4.Pt() != 0) ratio = tau_p4.Pt()/jet_p4.Pt();


	    //ele histos
	    h[0]->Fill(gsf_pt->at(iEle1), final_weight);
	    h[0]->Fill(gsf_pt->at(iEle2), final_weight);

	    h[1]->Fill(gsf_sc_eta->at(iEle1), final_weight);
	    h[1]->Fill(gsf_sc_eta->at(iEle2), final_weight);

	    h[2]->Fill(gsf_sc_phi->at(iEle1), final_weight);
	    h[2]->Fill(gsf_sc_phi->at(iEle2), final_weight);
	    

	    //misc. hitos
	    h[4]->Fill(total_p4.M(), final_weight);
	    h[5]->Fill(pv_n, final_weight);

	    int j_dm = -1, k_eta = -1;
	    bool bTautrigger = false;
	    if (tau_decayMode->at(iTau) == 0) {
	      j_dm = 0;
	    }
	    else if (tau_decayMode->at(iTau) == 1) {
	      j_dm = 1;
	    }
	    else if (tau_decayMode->at(iTau) == 10) {
	      j_dm = 2;
	    }

	    TString eta_string = GetEtaString(tau_eta->at(iTau));
	    if (eta_string == "barrel") {
	      k_eta = 0;
	    }
	    else if (eta_string == "endcap"){
	      k_eta = 1;
	    }
	    if (j_dm == -1 || k_eta == -1) continue;

	    cout << eta_string << endl;
	    cout << "vvloose " << tau_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017->at(iTau) << endl;
	    //Tau histos
	    if (tau_byTightIsolationMVArun2017v2DBoldDMwLT2017->at(iTau) > 0.5) {
	      hh[0][j_dm][k_eta]->Fill(tau_pt->at(iTau), ratio, final_weight);
	    }
	    if ((tau_byTightIsolationMVArun2017v2DBoldDMwLT2017->at(iTau) < 0.5) && (tau_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017->at(iTau) > 0.5)) {
	      hh[1][j_dm][k_eta]->Fill(tau_pt->at(iTau), ratio, final_weight);
	    }
	  }//loop over taus
	}//loop over eles
      }//loop over eles
   }//loop over events

   TH1F* h_total_events =  new TH1F("weighted_events", "weighted_events", 1, 0, 1);
   h_total_events->Fill(0.5, nEvents);
   file_out->cd();
   h_total_events->Write();
   h_reweight->Write();
   for (unsigned int i = 0; i<histo_names.size(); ++i) h[i]->Write();
   for (unsigned int i=0; i<htau_names.size(); ++i) for (unsigned int j=0; j<dms.size(); ++j) for (unsigned int k=0; k<eta.size(); ++k) hh[i][j][k]->Write();
   file_out->Close();

}
