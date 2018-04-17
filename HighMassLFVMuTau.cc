#define IIHEAnalysis_cxx
#include "IIHEAnalysis_old.h"
//#include <TH1.h>
//#include <TLorentzVector.h>
//#include <TCanvas.h>
#include "TString.h"
#include <iostream>
#include <vector>

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
  TH1F* hCounter = (TH1F*) fIn->Get("h1");
  TH1F* hCounter2 = (TH1F*) fIn->Get("h2");
  TTree* tree = (TTree*) fIn->Get("IIHEAnalysis");

  IIHEAnalysis* a = new IIHEAnalysis(tree);
  a->Loop(controlregion, type, out_name, hCounter, hCounter2);
  return 0;
}

void IIHEAnalysis::Loop(string controlregion, string type_of_data, string out_name, TH1F* hCounter, TH1F* hCounter2) {
   if (fChain == 0) return;


   bool Signal, data, DYinc, WJetsinc, TTinc, WWinc;
   if (type_of_data == "Signal" || type_of_data == "signal") {
     Signal = true;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     WWinc = false;
   }
   else if (type_of_data == "Data" || type_of_data == "data") {
     Signal = false;
     data = true;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     WWinc = false;
   }
   else if (type_of_data == "DYinc") {
     Signal = false;
     data = false;
     DYinc = true;
     WJetsinc = false;
     TTinc = false;
     WWinc = false;
   }
   else if (type_of_data == "WJetsinc") {
     Signal = false;
     data = false;
     DYinc = false;
     WJetsinc = true;
     TTinc = false;
     WWinc = false;
   }
   else if (type_of_data == "TTinc") {
     Signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = true;
     WWinc = false;
   }
   else if (type_of_data == "WW") {
     Signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     WWinc = true;
   }
   else {
     Signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     WWinc = false;
   }


   int CR_number = -1;
   if (controlregion == "CR0") {
     //OS, iso mu, iso tau = signal region
     CR_number = 0;
   }
   else if (controlregion == "CR1") {
     //SS, iso mu, iso tau
     CR_number = 1;
   }
   else if (controlregion == "CR2") {
     //OS, iso mu, anti-iso tau
     CR_number = 2;
   }
   else if (controlregion == "CR3") {
     //SS, iso mu, anti-iso tau
     CR_number = 3;
   }
   else if (controlregion == "CR4") {
     //OS, anti-iso mu, anti-iso tau
     CR_number = 4;
   }
   else if (controlregion == "CR5") {
     //SS, anti-iso mu, anti-iso tau
     CR_number = 5;
   }
   else if (controlregion == "CR7") {
     //SS, iso mu, anti-iso tau, mt restricted
     CR_number = 7;
   }
   else if (controlregion == "CR9") {
     //SS, iso mu, iso tau, mt restricted
     CR_number = 9;
   }


   //string out_name = "out_"+type_of_data+".root";
   TFile* file_out = new TFile(out_name.c_str(),"RECREATE");

   const float mu_mass = 0.10565837;
   const float pi = 3.14159;

   //list here the names and x-axis ranges of all gen-level histos we wish to create :
   vector<TString> histo_gen_names;                vector<int> nBins_gen;     vector<float> x_min_gen,    x_max_gen; 
   histo_gen_names.push_back("gen_tauh_pt");       nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(100);
   histo_gen_names.push_back("gen_tauh_eta");      nBins_gen.push_back(50);   x_min_gen.push_back(-2.5);  x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_tauh_phi");      nBins_gen.push_back(64);   x_min_gen.push_back(-3.2);  x_max_gen.push_back(3.2);
   histo_gen_names.push_back("gen_tauh_vispt");    nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(100);
   histo_gen_names.push_back("gen_tauh_viseta");   nBins_gen.push_back(50);   x_min_gen.push_back(-2.5);  x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_tauh_visphi");   nBins_gen.push_back(64);   x_min_gen.push_back(-3.2);  x_max_gen.push_back(3.2);
   histo_gen_names.push_back("gen_taumu_pt");      nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(100);
   histo_gen_names.push_back("gen_taumu_eta");     nBins_gen.push_back(50);   x_min_gen.push_back(-2.5);  x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_taumu_phi");     nBins_gen.push_back(64);   x_min_gen.push_back(-3.2);  x_max_gen.push_back(3.2);
   histo_gen_names.push_back("gen_taumu_vispt");   nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(100);
   histo_gen_names.push_back("gen_taumu_viseta");  nBins_gen.push_back(50);   x_min_gen.push_back(-2.5);  x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_taumu_visphi");  nBins_gen.push_back(64);   x_min_gen.push_back(-3.2);  x_max_gen.push_back(3.2);
   histo_gen_names.push_back("gen_ev_DR2l");       nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(10);
   histo_gen_names.push_back("gen_ev_M");          nBins_gen.push_back(100);  x_min_gen.push_back(995);   x_max_gen.push_back(1005);
   histo_gen_names.push_back("gen_ev_visDR2l");    nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(10);
   histo_gen_names.push_back("gen_ev_Mvis");       nBins_gen.push_back(200);  x_min_gen.push_back(700);   x_max_gen.push_back(1100);
   histo_gen_names.push_back("gen_ev_MwithMET");   nBins_gen.push_back(200);  x_min_gen.push_back(700);   x_max_gen.push_back(1100);
   histo_gen_names.push_back("gen_tau_decaymode");    nBins_gen.push_back(3);  x_min_gen.push_back(-0.5); x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_tautau_decaymode"); nBins_gen.push_back(9);  x_min_gen.push_back(-0.5); x_max_gen.push_back(8.5);

   vector<TH1F*> hgen;
   for (unsigned int i = 0; i<histo_gen_names.size(); ++i) hgen.push_back( new TH1F(histo_gen_names[i], histo_gen_names[i], nBins_gen[i], x_min_gen[i], x_max_gen[i]) ); 


   //list here the names and x-axis ranges of all reco-level histos we wish to create :
   vector<TString> histo_names;               vector<int> nBins;     vector<float> x_min,   x_max; 
   histo_names.push_back("ev_Mvis");          nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(1000);
   histo_names.push_back("ev_Mtot");          nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(1000);
   histo_names.push_back("tau_pt");           nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(500);
   histo_names.push_back("tau_eta");          nBins.push_back(100);  x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("tau_phi");          nBins.push_back(100);  x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("mu_pt");            nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(500);
   histo_names.push_back("mu_eta");           nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("mu_phi");           nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("ev_DRmutau");       nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(10);
   histo_names.push_back("ev_DeltaPhimutau"); nBins.push_back(64);   x_min.push_back(0);    x_max.push_back(3.2);
   histo_names.push_back("ev_DeltaPhiMETtau");nBins.push_back(64);   x_min.push_back(0);    x_max.push_back(3.2);
   histo_names.push_back("ev_Mt_raw");        nBins.push_back(150);  x_min.push_back(0);    x_max.push_back(150);
   histo_names.push_back("ev_MET");           nBins.push_back(1000);  x_min.push_back(0);    x_max.push_back(1000);
   histo_names.push_back("ev_weight");        nBins.push_back(400);  x_min.push_back(-2);   x_max.push_back(2);
   histo_names.push_back("ev_deltaMET");      nBins.push_back(200);  x_min.push_back(-100); x_max.push_back(100);
   histo_names.push_back("ev_Mcol");          nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(1000);
   histo_names.push_back("ev_Mt");            nBins.push_back(1000); x_min.push_back(0);    x_max.push_back(1000);

   vector<TString> pseudorapidity;
   pseudorapidity.push_back("taubarrel");
   pseudorapidity.push_back("tauendcap");

   vector<TH1F*> h[pseudorapidity.size()];
   for (unsigned int i = 0; i<histo_names.size(); ++i) {
     for (unsigned int j = 0; j<pseudorapidity.size(); ++j) {
       h[j].push_back( new TH1F(histo_names[i]+"_"+pseudorapidity[j], histo_names[i]+"_"+pseudorapidity[j], nBins[i], x_min[i], x_max[i]) ); 
       h[j][i]->Sumw2();
     }
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

      float first_weight = 1;//fChain->GetLeaf("event_weight")->GetValue();
      double final_weight = first_weight;
      

      //Do not consider events which should be rejected
      bool reject_event = false;
      if (DYinc) {
        TLorentzVector l1_p4, l2_p4, ll_p4;
	l1_p4.SetPxPyPzE(0, 0, 0, 0);
	l2_p4.SetPxPyPzE(0, 0, 0, 0);
	ll_p4.SetPxPyPzE(0, 0, 0, 0);
        int l1_pdgid = 0, l2_pdgid = 0;
        if (print_count < 20) {
          ++print_count;
          cout << endl << "LHE info" << endl;
        }
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
            cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (LHE_pdgid->at(iLHE) == 11 || LHE_pdgid->at(iLHE) == 13 || LHE_pdgid->at(iLHE) == 15) {
            l1_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l1_pdgid = LHE_pdgid->at(iLHE);
          }
          else if (LHE_pdgid->at(iLHE) == -11 || LHE_pdgid->at(iLHE) == -13 || LHE_pdgid->at(iLHE) == -15) {
            l2_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l2_pdgid = LHE_pdgid->at(iLHE);
          }
        }
        if (l1_pdgid == -l2_pdgid) {
          ll_p4 = l1_p4 + l2_p4;
          if (ll_p4.M() > 400) reject_event = true;
        }
        else {
          cout << "??" << endl;
        }
      }//close is this DY inclusive question
      else if (WJetsinc) {
        TLorentzVector l_p4, nu_p4, lnu_p4;
	l_p4.SetPxPyPzE(0, 0, 0, 0);
	nu_p4.SetPxPyPzE(0, 0, 0, 0);
	lnu_p4.SetPxPyPzE(0, 0, 0, 0);
        int l_pdgid = 0, nu_pdgid = 0;
        if (print_count < 20) {
          ++print_count;
          cout << endl << "LHE info" << endl;
        }
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
            cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
            l_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l_pdgid = LHE_pdgid->at(iLHE);
          }
          else if (abs(LHE_pdgid->at(iLHE)) == 12 || abs(LHE_pdgid->at(iLHE)) == 14 || abs(LHE_pdgid->at(iLHE)) == 16) {
            nu_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            nu_pdgid = LHE_pdgid->at(iLHE);
          }
        }
        if (l_pdgid == nu_pdgid-1) {
          lnu_p4 = l_p4 + nu_p4;
          if (lnu_p4.Pt() > 100) reject_event = true;
        }
        else {
          cout << "??" << endl;
        }
      }//close the is this WJets inclusive question
      else if (TTinc || WWinc) {
        TLorentzVector l1_p4, l2_p4, ll_p4;
	l1_p4.SetPxPyPzE(0, 0, 0, 0);
	l2_p4.SetPxPyPzE(0, 0, 0, 0);
	ll_p4.SetPxPyPzE(0, 0, 0, 0);
        int l1_pdgid = 0, l2_pdgid = 0;
        if (print_count < 20) {
          ++print_count;
          cout << endl << "LHE info" << endl;
        }
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
            cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
            l1_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l1_pdgid = LHE_pdgid->at(iLHE);
          }

          for (unsigned int jLHE = 0; jLHE < iLHE; ++jLHE) {
            if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
              l2_p4.SetPtEtaPhiE(LHE_Pt->at(jLHE),LHE_Eta->at(jLHE),LHE_Phi->at(iLHE),LHE_E->at(jLHE));
              l2_pdgid = LHE_pdgid->at(jLHE);
            }
            ll_p4 = l1_p4 + l2_p4;
	    if (TTinc) {
	      if (ll_p4.M() > 500) reject_event = true;
	    }
	    else if (WWinc) {
	      if (ll_p4.M() > 200) reject_event = true;
	    }	      
	    if (reject_event) break;
          }
	  if (reject_event) break;
        }
      }
      //close the is this TT inclusive question
      if (reject_event) continue;

      
      vector<TLorentzVector> tauhp4;
      tauhp4.clear();
      if (!data) {
	//start loop over all simulated particules
	int moth_ind = -1;
	vector<int> tau_ind;
	tau_ind.clear();
	TLorentzVector mup4, nutaup4, totalp4, totalvisp4, totalvismetp4, p4;
	vector<TLorentzVector> taup4, tauvisp4;
	taup4.clear();
	tauvisp4.clear();
	bool foundmu = false, foundtau = false;
	vector<bool> tauh;
	for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {

	  moth_ind = mc_mother_index->at(iMC).at(0);
	  if (abs(mc_pdgId->at(iMC)) == 15) {
	    p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
	    if (p4.Pt() > 20) {
	      foundtau = true;
	      taup4.push_back(p4);
	      tauvisp4.push_back(p4);
	      tau_ind.push_back(iMC);
	      tauh.push_back(true);
	    }
	  }
	  if (moth_ind < 0) continue;
	  if (abs(mc_pdgId->at(moth_ind)) == 15) {
	    for (unsigned int iTau = 0; iTau<tau_ind.size(); ++iTau) {
	      if (moth_ind == tau_ind[iTau]) {
		if (abs(mc_pdgId->at(iMC)) == 16) {
		  nutaup4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
		  tauvisp4[iTau] = taup4[iTau] - nutaup4;
		}
		else if (abs(mc_pdgId->at(iMC)) == 11 || abs(mc_pdgId->at(iMC)) == 13) {
		  //not a hadron
		  tauh[iTau] = false;
		}
	      }
	    }
	  }
	}
	for (unsigned int iTau = 0; iTau<tau_ind.size(); ++iTau) {
	  if (tauh[iTau]) tauhp4.push_back(tauvisp4[iTau]);
	}

	if (print_count < 20 && foundtau) {
	  ++print_count;
	  cout << endl;
	  for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) cout << iMC << "  " << mc_pdgId->at(iMC) << "  " << mc_mother_index->at(iMC).at(0) << "  " << mc_pt->at(iMC) << endl;
	}
      }//end is this not-data? condition



      //Is one of the triggers fired?
      bool PassMuonTrigger = false;
      if (trig_HLT_Mu50_accept || trig_HLT_TkMu50_accept) PassMuonTrigger = true;
      //if (trig_HLT_IsoMu27_accept || trig_HLT_IsoTkMu27_accept) PassMuonTrigger = true;
      if (!PassMuonTrigger) continue;


      //start muon counting loop
      /*int Nmu = 0;
      for (unsigned int iMu = 0; iMu < mu_gt_pt->size(); ++iMu) {
        if(mu_isPFMuon->at(iMu) && mu_gt_pt->at(iMu) > 20 && fabs(mu_gt_eta->at(iMu)) < 2.4 && fabs(mu_gt_dxy_firstPVtx->at(iMu)) < 0.045 && fabs(mu_gt_dz_firstPVtx->at(iMu)) < 0.2 && mu_pfIsoDbCorrected04->at(iMu) < 0.3 && mu_isMediumMuon->at(iMu)) ++Nmu;
        if (Nmu > 1) break;
      }
      if (Nmu > 1) continue; //2nd muon veto                                                                                                                                                                

      //electron veto
      bool electron = false;
      for (unsigned int iEle = 0; iEle < gsf_pt->size(); ++iEle) {
	if (gsf_VIDLoose->at(iEle) && gsf_pt->at(iEle) > 20 && fabs(gsf_eta->at(iEle)) < 2.5 && fabs(gsf_dxy_firstPVtx->at(iEle)) < 0.045 && fabs(gsf_dz_firstPVtx->at(iEle)) < 0.2 && gsf_passConversionVeto->at(iEle) && gsf_nLostInnerHits->at(iEle) <= 1 && gsf_relIso->at(iEle) < 0.3) electron = true;
        if (electron) break;
      }
      if (electron) continue;*/

      //bjet veto (medium WP for the bjet)                                                                                                                           
      /*bool bjet = false;
      for (unsigned int iJet = 0; iJet < jet_pt->size(); ++iJet) {
        if (jet_CSVv2->at(iJet) > 0.800 && jet_pt->at(iJet) > 20 && fabs(jet_eta->at(iJet)) < 2.4) bjet = true;
        if (bjet) break;
      }
      if (bjet) continue;*/



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
	if (mu_ibt_pt->at(iMu) < 53.0) continue;
	//if (mu_ibt_pt->at(iMu) < 30.0) continue;
	if (!mu_isHighPtMuon->at(iMu)) continue;
	//if (!mu_isPFMuon->at(iMu)) continue;
	if (fabs(mu_ibt_eta->at(iMu)) > 2.4) continue;
	bool goodGlob = mu_isGlobalMuon->at(iMu) && mu_gt_normalizedChi2->at(iMu) < 3 && mu_combinedQuality_chi2LocalPosition->at(iMu) < 12 && mu_combinedQuality_trkKink->at(iMu) < 20;
	bool isMedium2016 = mu_isLooseMuon->at(iMu) && mu_innerTrack_validFraction->at(iMu) > 0.49 && mu_segmentCompatibility->at(iMu) > (goodGlob ? 0.303 : 0.451);
	//if (!mu_isMediumMuon->at(iMu)) continue;
	//if (!isMedium2016) continue;
	//if (fabs(mu_ibt_dxy_firstPVtx->at(iMu)) > 0.2) continue;
	//if (fabs(mu_ibt_dz_firstPVtx->at(iMu)) > 0.5) continue;


	TLorentzVector mu_p4, mu_ibt_transp4;
	mu_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu), mu_ibt_eta->at(iMu), mu_ibt_phi->at(iMu), mu_mass);
	mu_ibt_transp4.SetPxPyPzE(mu_ibt_px->at(iMu), mu_ibt_py->at(iMu), 0, mu_ibt_pt->at(iMu));

	//start loop over reconstructed taus
	for (unsigned int jj = 0; jj < orderedTau.size(); ++jj) {
	  if (filled_histos) break;//if we've filled the histos once, break out of the loop
	  int iTau = orderedTau[jj];

	  TLorentzVector tau_p4, tau_TES_p4, vis_p4, met_p4, metmu_p4, total_p4;
	  float met_px = MET_Px;
	  float met_py = MET_Py;
	  float met_pt = MET_Pt;
	  tau_p4.SetPtEtaPhiE(tau_pt->at(iTau), tau_eta->at(iTau), tau_phi->at(iTau), tau_energy->at(iTau));
	  met_p4.SetPxPyPzE(met_px, met_py, 0, met_pt);
	  
	  vis_p4 = tau_p4 + mu_p4;
	  total_p4 = vis_p4 + met_p4;
	  metmu_p4 = met_p4 + mu_p4;
	  
	  if (tau_p4.Pt() < 30.0) continue;
	  if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	  if (tau_decayModeFinding->at(iTau) < 0.5) continue;
	  if (tau_againstMuonTight3->at(iTau) < 0.5) continue;
	  if (tau_againstElectronVLooseMVA6->at(iTau) < 0.5) continue;
	  //if (tau_ptLeadChargedCand->at(iTau) < 5) continue;
	  //if (fabs(tau_dz->at(iTau)) > 0.2) continue;
	  if (fabs(tau_charge->at(iTau)) != 1) continue;

	  //control regions : sign selection, muon isolation and tau ID
	  if (CR_number == -1) {
	    cout << "Error: Control region not recognized" << endl;
	    break;
	  }
	  
	  //sign selection
	  if (CR_number%2 == 0) {
	    if (tau_charge->at(iTau) * mu_ibt_charge->at(iMu) > 0) continue; //OS selection
	  }
	  else {
	    if (tau_charge->at(iTau) * mu_ibt_charge->at(iMu) < 0) continue; //SS selection
	  }

	  //muon isolation
	  float reliso = mu_isoTrackerBased03->at(iMu); //use instead sumofpts divided by muon ibt pt
	  //float reliso = mu_pfIsoDbCorrected04->at(iMu);
	  if (CR_number < 4 || CR_number >= 7) {
	    if (reliso > 0.1) continue;
	  }
	  else {
	    if (reliso < 0.1) continue;
	  }
	    
	  //tau isolation
	  if (CR_number < 2 || CR_number == 9) {
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }
	  else {
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) continue;
	    if (tau_byLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }

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
	  //cout << "MET before corr. " << met_p4.Pt() << endl;
	  met_p4 = met_p4 + mu_gt_transp4 - mu_ibt_transp4;
	  //cout << "MET after corr. " << met_p4.Pt() << endl << endl;

	  //separate histos by tau eta
	  int jEta=0;
	  if (fabs(tau_p4.Eta()) > 1.444) jEta=1;

	  if (!data) {
	    bool tau_match = false;
	    if (tauhp4.size() != 0) {
	      for (unsigned int iGen = 0; iGen < tauhp4.size(); ++iGen) {
		if (tau_p4.DeltaR(tauhp4[iGen]) < 0.5) tau_match = true;
	      }
	    }
	    if (CR_number >= 2) tau_match = false;
	    float reweight = GetReweight_highmass(mc_trueNumInteractions, mu_p4.Pt(), mu_p4.Eta(), tau_match);
	    h[jEta][13]->Fill(reweight);
	    final_weight = GetReweight_highmass(mc_trueNumInteractions, mu_p4.Pt(), mu_p4.Eta(), tau_match) * 1.0 * mc_w_sign;
	  }
	  

	  if (CR_number >= 7) {
	    float Mt = -1;
	    if (2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) < 0) {
              Mt = 0;
            }
            else {
              Mt = sqrt(2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) );
            }
	    h[jEta][16]->Fill(Mt, final_weight);
	    if (Mt < 80 || Mt > 120) continue;
	  }

	  //Pzeta calculation
	  float norm_zeta= norm_F( tau_p4.Px()/tau_p4.Pt()+mu_p4.Px()/mu_p4.Pt(), tau_p4.Py()/tau_p4.Pt()+mu_p4.Py()/mu_p4.Pt() );
	  //cout << norm_zeta << endl;
	  float x_zeta= (tau_p4.Px()/tau_p4.Pt()+mu_p4.Px()/mu_p4.Pt())/norm_zeta;
	  float y_zeta= (tau_p4.Py()/tau_p4.Pt()+mu_p4.Py()/mu_p4.Pt())/norm_zeta;
	  float p_zeta_mis=met_p4.Px()*x_zeta+met_p4.Py()*y_zeta;
	  float pzeta_vis=(tau_p4.Px()+mu_p4.Px())*x_zeta+(tau_p4.Py()+mu_p4.Py())*y_zeta;
	  float cut_zeta= p_zeta_mis-0.85*pzeta_vis;


	  if (final_weight != final_weight) {
	    cout << "Not a number!!!!!!!" << endl;
	    continue;
	  }



	  
	  float dR = tau_p4.DeltaR(mu_p4);
	  float dphi_mutau = 100, dphi_METtau = 100;
	  float large_phi = 100;
	  float small_phi = 100;
	  if (tau_p4.Phi() > mu_p4.Phi()) {
	    large_phi = tau_p4.Phi();
	    small_phi = mu_p4.Phi();
	  }
	  else {
	    small_phi = tau_p4.Phi();
	    large_phi = mu_p4.Phi();
	  }
	  if (fabs(large_phi-small_phi) > fabs(large_phi-(small_phi+2*pi))) {
	    dphi_mutau = fabs(large_phi-(small_phi+2*pi));
	  }
	  else {
	    dphi_mutau = fabs(large_phi-small_phi);
	  }

	  if (tau_p4.Phi() > met_p4.Phi()) {
	    large_phi = tau_p4.Phi();
	    small_phi = met_p4.Phi();
	  }
	  else {
	    small_phi = tau_p4.Phi();
	    large_phi = met_p4.Phi();
	  }
	  if (fabs(large_phi-small_phi) > fabs(large_phi-(small_phi+2*pi))) {
	    dphi_METtau = fabs(large_phi-(small_phi+2*pi));
	  }
	  else {
	    dphi_METtau = fabs(large_phi-small_phi);
	  }


	  float Mcol = GetCollinearMass(tau_p4, mu_p4, met_p4);


	  h[jEta][9]->Fill(dphi_mutau, final_weight);
	  h[jEta][10]->Fill(dphi_METtau, final_weight);

	  if (dR < 0.5) continue;
	  filled_histos = true;
	  h[jEta][0]->Fill(vis_p4.M(), final_weight);
	  h[jEta][1]->Fill(total_p4.M(), final_weight);
	  h[jEta][2]->Fill(tau_p4.Pt(), final_weight);
	  h[jEta][3]->Fill(tau_p4.Eta(), final_weight);
	  h[jEta][4]->Fill(tau_p4.Phi(), final_weight);
	  h[jEta][5]->Fill(mu_p4.Pt(), final_weight);
	  h[jEta][6]->Fill(mu_p4.Eta(), final_weight);
	  h[jEta][7]->Fill(mu_p4.Phi(), final_weight);
	  h[jEta][8]->Fill(dR, final_weight);
	  //h[jEta][11]->Fill(Mt, final_weight);
	  h[jEta][12]->Fill(met_p4.Pt(), final_weight);
	  //h[jEta][13]->Fill(mc_w_sign);
	  h[jEta][14]->Fill(met_p4.Px()-met_px, final_weight);
	  h[jEta][15]->Fill(Mcol, final_weight);

	  //if (cut_zeta < -25) continue;

	}//loop over taus
      }//loop over muons
   }//loop over events

   file_out->cd();
   for (unsigned int i = 0; i<histo_names.size(); ++i) for (unsigned int j = 0; j<pseudorapidity.size(); ++j) h[j][i]->Write();
   for (unsigned int i = 0; i<hgen.size(); ++i) hgen[i]->Write();
   file_out->Close();
}
