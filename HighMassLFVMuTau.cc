#define IIHEAnalysis_cxx
#include "SF_and_systematics.cc"
#include "meta.h"
#include "TDirectory.h"

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



//main analysis loop
void IIHEAnalysis::Loop(string controlregion, string type_of_data, string out_name, Float_t nEvents) {
   if (fChain == 0) return;


   bool signal, data, singlephoton, singlemu, DYinc, WJetsinc, TTinc, WWinc, TT;
   if (type_of_data == "Signal" || type_of_data == "signal") {
     signal = true;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
   }
   else if (type_of_data == "Data" || type_of_data == "data" || type_of_data == "singlephoton" || type_of_data == "SinglePhoton" || type_of_data == "singlemu" || type_of_data == "SingleMu") {
     signal = false;
     data = true;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
   }
   else if (type_of_data == "DYinc") {
     signal = false;
     data = false;
     DYinc = true;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
   }
   else if (type_of_data == "WJetsinc") {
     signal = false;
     data = false;
     DYinc = false;
     WJetsinc = true;
     TTinc = false;
     TT = false;
     WWinc = false;
   }
   else if (type_of_data == "TTinc") {
     signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = true;
     TT = true;
     WWinc = false;
   }
   else if (type_of_data == "TT") {
     signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = true;
     WWinc = false;
   }
   else if (type_of_data == "WWinc") {
     signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = true;
   }
   else {
     signal = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
   }

   if (type_of_data == "singlephoton" || type_of_data == "SinglePhoton") singlephoton = true;
   if (type_of_data == "singlemu" || type_of_data == "SingleMu") singlemu = true;

   cout << type_of_data << endl << endl;
   
   cout << "TT-inclusive:    " << TTinc << endl; 
   cout << "TT-any:          " << TT << endl; 
   cout << "Signal:          " << signal << endl; 
   cout << "DY-inclusive:    " << DYinc << endl; 
   cout << "data:            " << data << endl; 
   cout << "WW-inclusive:    " << WWinc << endl; 
   cout << "Wjets-inclusive: " << WJetsinc << endl; 


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
   else if (controlregion == "CR100") {
     //no sign requirement, iso mu, anti-iso tau, estimate fake tau bg
     CR_number = 100;
   }
   else if (controlregion == "CR101") {
     //no sign requirement, iso mu, anti-iso tau, estimate fake tau bg
     CR_number = 101;
   }
   else if (controlregion == "CR102") {
     //no sign requirement, iso mu, iso tau, Mt>80 GeV, W+Jets enriched CR
     CR_number = 102;
   }
   else if (controlregion == "CR103") {
     //no sign requirement, iso mu, anti-iso tau, Mt>80 GeV, W+Jets enriched CR
     CR_number = 103;
   }

   //string out_name = "out_"+type_of_data+".root";
   TFile* file_out = new TFile(out_name.c_str(),"RECREATE");

   const float mu_mass = 0.10565837;
   const float pi = 3.14159;

   //list here the names and x-axis ranges of all gen-level histos we wish to create :
   vector<TString> histo_gen_names;                vector<int> nBins_gen;     vector<float> x_min_gen,    x_max_gen; 
   histo_gen_names.push_back("gen_tau_pt");       nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(100);
   histo_gen_names.push_back("gen_tau_eta");      nBins_gen.push_back(50);   x_min_gen.push_back(-2.5);  x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_tau_phi");      nBins_gen.push_back(64);   x_min_gen.push_back(-3.2);  x_max_gen.push_back(3.2);
   histo_gen_names.push_back("gen_ev_DR2l");       nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(10);
   histo_gen_names.push_back("gen_ev_M");          nBins_gen.push_back(100);  x_min_gen.push_back(995);   x_max_gen.push_back(1005);
   histo_gen_names.push_back("gen_ev_visDR2l");    nBins_gen.push_back(100);  x_min_gen.push_back(0);     x_max_gen.push_back(10);
   histo_gen_names.push_back("gen_ev_Mvis");       nBins_gen.push_back(200);  x_min_gen.push_back(700);   x_max_gen.push_back(1100);
   histo_gen_names.push_back("gen_ev_ntaus");      nBins_gen.push_back(10);  x_min_gen.push_back(0);   x_max_gen.push_back(10);
   histo_gen_names.push_back("gen_tau_decaymode");    nBins_gen.push_back(3);  x_min_gen.push_back(-0.5); x_max_gen.push_back(2.5);
   histo_gen_names.push_back("gen_tautau_decaymode"); nBins_gen.push_back(9);  x_min_gen.push_back(-0.5); x_max_gen.push_back(8.5);

   vector<TH1F*> hgen;
   for (unsigned int i = 0; i<histo_gen_names.size(); ++i) hgen.push_back( new TH1F(histo_gen_names[i], histo_gen_names[i], nBins_gen[i], x_min_gen[i], x_max_gen[i]) ); 


   //list here the names and x-axis ranges of all reco-level histos we wish to create :
   vector<TString> histo_names;               vector<int> nBins;     vector<float> x_min,   x_max; 
   histo_names.push_back("ev_Mvis");          nBins.push_back(8000); x_min.push_back(0);    x_max.push_back(8000);
   histo_names.push_back("ev_Mtot");          nBins.push_back(8000); x_min.push_back(0);    x_max.push_back(8000);
   histo_names.push_back("tau_pt");           nBins.push_back(2000); x_min.push_back(0);    x_max.push_back(2000);
   histo_names.push_back("tau_eta");          nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("tau_phi");          nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("mu_pt");            nBins.push_back(2000); x_min.push_back(0);    x_max.push_back(2000);
   histo_names.push_back("mu_eta");           nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("mu_phi");           nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);
   histo_names.push_back("ev_DRmutau");       nBins.push_back(100);  x_min.push_back(0);    x_max.push_back(10);
   histo_names.push_back("ev_MET");           nBins.push_back(2000); x_min.push_back(0);    x_max.push_back(2000);
   histo_names.push_back("ev_deltaMET");      nBins.push_back(200);  x_min.push_back(-100); x_max.push_back(100);
   histo_names.push_back("ev_Mcol");          nBins.push_back(8000); x_min.push_back(0);    x_max.push_back(8000);
   histo_names.push_back("ev_Mt");            nBins.push_back(8000); x_min.push_back(0);    x_max.push_back(8000);
   histo_names.push_back("mu_isolation");     nBins.push_back(200);  x_min.push_back(0);    x_max.push_back(0.2);
   histo_names.push_back("weight");           nBins.push_back(1000); x_min.push_back(-5);   x_max.push_back(5);
   histo_names.push_back("sign");             nBins.push_back(5);    x_min.push_back(0);    x_max.push_back(5);
   int OS_number = 1;
   int SS_number = 3;
   histo_names.push_back("njet");             nBins.push_back(10);   x_min.push_back(0);    x_max.push_back(10);
   histo_names.push_back("nbjet");            nBins.push_back(10);   x_min.push_back(0);    x_max.push_back(10);
   //exclusively for TT region now
   int n_TT_plots = histo_names.size();
   histo_names.push_back("bjet_pt");          nBins.push_back(2000); x_min.push_back(0);    x_max.push_back(2000);
   histo_names.push_back("bjet_eta");         nBins.push_back(50);   x_min.push_back(-2.5); x_max.push_back(2.5);
   histo_names.push_back("bjet_phi");         nBins.push_back(64);   x_min.push_back(-3.2); x_max.push_back(3.2);


   vector<TString> taun;
   taun.push_back("realtau"); int j_real = taun.size()-1;
   //taun.push_back("faketau"); int j_fake = taun.size()-1;

   vector<TString> Mth;
   Mth.push_back("MtLow_OS");  int k_low_OS = Mth.size()-1;
   Mth.push_back("MtLow_SS");  int k_low_SS = Mth.size()-1;
   Mth.push_back("MtHigh");    int k_high   = Mth.size()-1;
   Mth.push_back("MtLow_TT");  int k_low_TT= Mth.size()-1;
   Mth.push_back("MtHigh_TT"); int k_high_TT= Mth.size()-1;


   //put all systematics here
   vector<TString> systs;
   systs.push_back("nominal");

   vector<TString> specialSyst;

   vector<TString> systs_aux = GetSys();
   for (unsigned int iAux=0; iAux<systs_aux.size(); ++iAux) {
     systs.push_back(systs_aux[iAux]+"_up");
     systs.push_back(systs_aux[iAux]+"_down");
   }

   vector<TString> special_aux;
   special_aux.push_back("TES");
   special_aux.push_back("MES");
   special_aux.push_back("mres");
   for (unsigned int iAux=0; iAux<special_aux.size(); ++iAux) {
     specialSyst.push_back(special_aux[iAux]+"_up");
     specialSyst.push_back(special_aux[iAux]+"_down");
   }
   //end systematics


   vector<TH1F*> h[Mth.size()][systs.size()][taun.size()];
   for (unsigned int i = 0; i<histo_names.size(); ++i) {
     for (unsigned int j = 0; j<taun.size(); ++j) {
       for (unsigned int k = 0; k<systs.size(); ++k) {
	 for (unsigned int l = 0; l<Mth.size(); ++l) {
	   //don't create histos for the TT regions in other regions
	   if (l < k_low_TT && i >= n_TT_plots) continue;
	   h[l][k][j].push_back( new TH1F(histo_names[i]+"_"+taun[j]+"_"+systs[k]+"_"+Mth[l], histo_names[i]+"_"+taun[j]+"_"+systs[k]+"_"+Mth[l], nBins[i], x_min[i], x_max[i]) ); 
	   h[l][k][j][i]->Sumw2();
	 }
       }
     }
   }


   vector<TString> h_names;
   h_names.push_back("taupt_ratio_pass");  int iRatioPass  = h_names.size()-1;
   h_names.push_back("taupt_ratio_fail");  int iRatioFail = h_names.size()-1;


   vector<TString> eta;
   eta.push_back("barrel"); int l_barrel = eta.size()-1;
   eta.push_back("endcap"); int l_endcap = eta.size()-1;

   vector<TH2D*> hh[h_names.size()][k_low_SS+1][systs.size()];

   if (CR_number == 100 && !signal) {
     for (unsigned int i = 0; i<h_names.size(); ++i) {
       for (unsigned int j = 0; j<Mth.size(); ++j) {
	 if (j>k_low_SS) continue; 
	 for (unsigned int k = 0; k<systs.size(); ++k) {
	   for (unsigned int l = 0; l<eta.size(); ++l) {
	     TString nname = h_names[i]+"_"+systs[k]+"_"+Mth[j]+"_"+eta[l]+"_"+taun[0];
	     hh[i][j][k].push_back( new TH2D(nname, nname, 500, 0, 1000, 500, 0, 5) );
	     hh[i][j][k][l]->Sumw2();
	   }
	 }
       }
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


      float first_weight = 1;
      double final_weight = 1;
      
      //Do not consider events which should be rejected
      bool reject_event = false, Zmumu = false;
      if (DYinc) {
        TLorentzVector l1_p4, l2_p4, ll_p4;
	l1_p4.SetPxPyPzE(0, 0, 0, 0);
	l2_p4.SetPxPyPzE(0, 0, 0, 0);
	ll_p4.SetPxPyPzE(0, 0, 0, 0);
        int l1_pdgid = 0, l2_pdgid = 0;
        if (print_count < 20) {
          ++print_count;
          //cout << endl << "LHE info" << endl;
        }
	bool found_1 = false, found_2 = false;
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
            //cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (LHE_pdgid->at(iLHE) == 11 || LHE_pdgid->at(iLHE) == 13 || LHE_pdgid->at(iLHE) == 15) {
            l1_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l1_pdgid = LHE_pdgid->at(iLHE);
	    found_1 = true;
          }
          else if (LHE_pdgid->at(iLHE) == -11 || LHE_pdgid->at(iLHE) == -13 || LHE_pdgid->at(iLHE) == -15) {
            l2_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l2_pdgid = LHE_pdgid->at(iLHE);
	    found_2 = true;
          }
	  if (abs(LHE_pdgid->at(iLHE)) == 13) Zmumu = true;
        }
        if ((l1_pdgid == -l2_pdgid) && found_1 && found_2) {
          ll_p4 = l1_p4 + l2_p4;
          if (ll_p4.M() > 200) reject_event = true;
        }
        else {
          //cout << "??" << endl;
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
          //cout << endl << "LHE info" << endl;
        }

	bool found_1 = false, found_2 = false;
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
	    // cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
            l_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l_pdgid = LHE_pdgid->at(iLHE);
	    found_1 = true;
          }
          else if (abs(LHE_pdgid->at(iLHE)) == 12 || abs(LHE_pdgid->at(iLHE)) == 14 || abs(LHE_pdgid->at(iLHE)) == 16) {
            nu_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            nu_pdgid = LHE_pdgid->at(iLHE);
	    found_2 = true;
          }
        }
        if ((abs(l_pdgid) == abs(nu_pdgid)-1) && found_1 && found_2) {
          lnu_p4 = l_p4 + nu_p4;
          if (lnu_p4.Pt() > 100) reject_event = true;
        }
        else {
          //cout << "??" << endl;
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
          cout << endl << jEntry << endl << "LHE info size " << LHE_Pt->size() << endl;
        }

	bool found_1 = false, found_2 = false;
        for (unsigned int iLHE = 0; iLHE < LHE_Pt->size(); ++iLHE) {
          if (print_count < 20) {
            cout << LHE_pdgid->at(iLHE) << "  " << LHE_Pt->at(iLHE) << "  " << LHE_Eta->at(iLHE) << "  " << LHE_Phi->at(iLHE) << "  " << LHE_E->at(iLHE) << endl;
          }
          if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
            l1_p4.SetPtEtaPhiE(LHE_Pt->at(iLHE),LHE_Eta->at(iLHE),LHE_Phi->at(iLHE),LHE_E->at(iLHE));
            l1_pdgid = LHE_pdgid->at(iLHE);
	    found_1 = true;
          }

          for (unsigned int jLHE = 0; jLHE < iLHE; ++jLHE) {
            if (abs(LHE_pdgid->at(iLHE)) == 11 || abs(LHE_pdgid->at(iLHE)) == 13 || abs(LHE_pdgid->at(iLHE)) == 15) {
              l2_p4.SetPtEtaPhiE(LHE_Pt->at(jLHE),LHE_Eta->at(jLHE),LHE_Phi->at(iLHE),LHE_E->at(jLHE));
              l2_pdgid = LHE_pdgid->at(jLHE);
	      found_2 = true;
            }
	    if (!(found_1 && found_2)) continue;
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

      double top_pt_1 = -10, top_pt_2 = -10;
      if(TT) {
	bool find_t1 = false;
	bool find_t2 = false;
	TLorentzVector MC_p4_1(1,0,0,0);
	TLorentzVector MC_p4_2(1,0,0,0);
	for(unsigned iMC=0 ; iMC<LHE_Pt->size() ; ++iMC) {
	  if( (LHE_pdgid->at(iMC) == 6) ) {
	    MC_p4_1.SetPtEtaPhiE(LHE_Pt->at(iMC),LHE_Eta->at(iMC),LHE_Phi->at(iMC),LHE_E->at(iMC)) ;
	    find_t1 = true;
	  }
	  else if( (LHE_pdgid->at(iMC) == -6) ) {
	    MC_p4_2.SetPtEtaPhiE(LHE_Pt->at(iMC),LHE_Eta->at(iMC),LHE_Phi->at(iMC),LHE_E->at(iMC)) ;
	    find_t2 = true;
	  }
	  if(find_t1 && find_t2) {
	    top_pt_1 = MC_p4_1.Pt();
	    top_pt_2 = MC_p4_2.Pt();
	    break;
	  }
	}
      }

      vector<TLorentzVector> tauhp4;
      tauhp4.clear();
      vector<TLorentzVector> anyleptonp4, genmup4, genelep4;
      anyleptonp4.clear();
      genmup4.clear();
      genelep4.clear();
      if (!data) {
	//start loop over all simulated particules
	TLorentzVector p4;
	for (unsigned int iMC = 0; iMC < mc_pt->size(); ++iMC) {
	  if (abs(mc_pdgId->at(iMC)) == 11 || abs(mc_pdgId->at(iMC)) == 13) {
	    if (mc_pt->at(iMC) < 10) continue;
	    p4.SetPxPyPzE(mc_px->at(iMC), mc_py->at(iMC), mc_pz->at(iMC), mc_energy->at(iMC));
	    if (p4.Pt() < 10) continue;
	    if (p4.Pt() > 10000) continue;
	    anyleptonp4.push_back(p4);
	    if (abs(mc_pdgId->at(iMC)) == 13) genmup4.push_back(p4);
	    if (abs(mc_pdgId->at(iMC)) == 11) genelep4.push_back(p4);
	  }
	}

	for (unsigned int iMC = 0; iMC < mc_tau_had_pt->size(); ++iMC) {
	  p4.SetPtEtaPhiE(mc_tau_had_pt->at(iMC), mc_tau_had_eta->at(iMC), mc_tau_had_phi->at(iMC), mc_tau_had_energy->at(iMC));
	  tauhp4.push_back( p4 );
	  anyleptonp4.push_back( p4 );
	}
      }//end is this not-data? condition


      //Is one of the triggers fired?
      bool PassTrigger = false;
      if (data)  if (trig_HLT_Mu50_accept || trig_HLT_TkMu50_accept) PassTrigger = true;
      if (!data) if (trig_HLT_Mu50_accept || trig_HLT_TkMu50_accept) PassTrigger = true;
      if (!PassTrigger) continue;


      if(!trig_Flag_goodVertices_accept) continue;
      if(!trig_Flag_globalSuperTightHalo2016Filter_accept) continue;
      if(!trig_Flag_HBHENoiseFilter_accept) continue;
      if(!trig_Flag_HBHENoiseIsoFilter_accept) continue;
      if(!trig_Flag_EcalDeadCellTriggerPrimitiveFilter_accept) continue;
      if(!trig_Flag_BadPFMuonFilter_accept) continue;
      if(!signal && !trig_Flag_ecalBadCalibReduced) continue;
      if(data && !trig_Flag_eeBadScFilter_accept) continue;


      //electron veto
      bool electron = false;
      for (unsigned int iEle = 0; iEle < gsf_pt->size(); ++iEle) {
	bool heepID = false;
	if (signal) heepID = gsf_VIDHEEP7->at(iEle);
	else heepID = gsf_VID_heepElectronID_HEEPV70->at(iEle);
	if (heepID && gsf_pt->at(iEle) > 40) electron = true;
        if (electron) break;
      }
      if (electron) continue;


      //dimuon veto
      bool dimuon = false;
      for (unsigned int iMu = 0; iMu < mu_ibt_pt->size(); ++iMu) {
        TLorentzVector mu1_p4, mu2_p4;

	if (mu_isHighPtMuon->at(iMu) && mu_ibt_pt->at(iMu) > 15 && fabs(mu_ibt_eta->at(iMu)) < 2.4 && mu_isoTrackerBased03->at(iMu) < 0.2) {
          mu1_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu), mu_ibt_eta->at(iMu), mu_ibt_phi->at(iMu), mu_mass);
          for (unsigned int iMu2= 0; iMu2 < iMu; ++iMu2) {
	    if (mu_isHighPtMuon->at(iMu2) && mu_ibt_pt->at(iMu2) > 15 && fabs(mu_ibt_eta->at(iMu2)) < 2.4 && mu_isoTrackerBased03->at(iMu2) < 0.2) {
              mu2_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu2), mu_ibt_eta->at(iMu2), mu_ibt_phi->at(iMu2), mu_mass);
              if (mu1_p4.DeltaR(mu2_p4) > 0.2) dimuon = true;
            }
            if (dimuon) break;
          }
        }
        if (dimuon) break;
      }
      if (dimuon) continue;


      //bjet pair finding (medium WP for the bjet)                                                                                                                           
      int nbjet = 0;
      float bjetMedium2016 = 0.800;
      float bjet_weight = 1;
      vector<TLorentzVector> bjet_p4;
      for (unsigned int iJet = 0; iJet < jet_pt->size(); ++iJet) {
	TLorentzVector bjet_p4_temp;
	bool looseJet = false;
	if (signal) looseJet = jet_isJetIDLoose->at(iJet);
	else looseJet = jet_isJetIDLoose_2016->at(iJet);
	if (jet_CSVv2->at(iJet) > bjetMedium2016 && jet_pt->at(iJet) > 30 && fabs(jet_eta->at(iJet)) < 2.4 && looseJet) {
	  bjet_p4_temp.SetPxPyPzE(jet_px->at(iJet), jet_py->at(iJet), jet_pz->at(iJet), jet_energy->at(iJet));

	  //bjet should match neither a tau nor a muon
	  bool btau = false;
	  for (unsigned int iTau = 0; iTau < tau_pt->size(); ++iTau) {
	    TLorentzVector tau_temp_p4;
	    tau_temp_p4.SetPtEtaPhiE(tau_pt->at(iTau), tau_eta->at(iTau), tau_phi->at(iTau), tau_energy->at(iTau));
	    if (tau_temp_p4.Pt() < 30.0) continue;
	    if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	    if (tau_decayModeFinding->at(iTau) < 0.5) continue;
	    if (tau_againstMuonLoose3->at(iTau) < 0.5) continue;
	    if (tau_againstElectronLooseMVA6->at(iTau) < 0.5) continue;
	    if (fabs(tau_charge->at(iTau)) != 1) continue;
	    if (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  
	    if (bjet_p4_temp.DeltaR(tau_temp_p4) < 0.5) btau = true;
	    if (btau) break;
	  }
	  if (btau) continue;
	  
	  bool bmu = false;
	  for (unsigned int iMu = 0; iMu < mu_ibt_pt->size(); ++iMu) {
	    TLorentzVector mu_temp_p4;
	    mu_temp_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu), mu_ibt_eta->at(iMu), mu_ibt_phi->at(iMu), mu_mass);
	    if (mu_ibt_pt->at(iMu) < 53.0) continue;
	    if (!mu_isHighPtMuon->at(iMu)) continue;
	    if (fabs(mu_ibt_eta->at(iMu)) > 2.4) continue;
	    if (mu_isoTrackerBased03->at(iMu) > 0.1) continue;
	  
	    if (bjet_p4_temp.DeltaR(mu_temp_p4) < 0.5) bmu = true;
	    if (bmu) break;
	  }
	  if (bmu) continue;
	  ++nbjet;
	  bjet_p4.push_back(bjet_p4_temp);
	}
	//if (nbjet >= 2) break;
      }
      if (!data) bjet_weight = BtagSF_Deepcsv_medium;



      //classify mu-tau pairs by their visible mass
      map<vector<int>, float> Mvis_map;
      vector<vector<int>> mutau_ind_temp;
      for (unsigned int ii = 0; ii < mu_ibt_pt->size(); ++ii) {
        TLorentzVector mu_p4;
        mu_p4.SetPtEtaPhiM(mu_ibt_pt->at(ii), mu_ibt_eta->at(ii), mu_ibt_phi->at(ii), mu_mass);
        for (unsigned int jj = 0; jj < tau_pt->size(); ++jj) {
          TLorentzVector tau_p4;
          tau_p4.SetPtEtaPhiE(tau_pt->at(jj), tau_eta->at(jj), tau_phi->at(jj), tau_energy->at(jj));

          float Mvis = (tau_p4+mu_p4).M();
          vector<int> index;
          index.push_back(ii); //mu index
          index.push_back(jj); //tau index

          if (Mvis != Mvis) Mvis = 0;
          if (Mvis < 0) Mvis = 0;
          Mvis_map[index] = Mvis;
          mutau_ind_temp.push_back(index);
        }
      }


      vector<vector<int>> rest, mutau_ind;
      mutau_ind.clear();
      while (mutau_ind_temp.size()>0) {
        rest.clear();
        float max_mvis = -1;
        vector<int> highest_ind;
        highest_ind.clear();
        for (unsigned int iInd = 0; iInd < mutau_ind_temp.size(); ++iInd) {
          if (Mvis_map[mutau_ind_temp[iInd]] > max_mvis) {
            max_mvis = Mvis_map[mutau_ind_temp[iInd]];
            if (highest_ind.size() > 0) rest.push_back(highest_ind);
            highest_ind = mutau_ind_temp[iInd];
          }
          else {
            rest.push_back(mutau_ind_temp[iInd]);
          }
        }
        mutau_ind.push_back(highest_ind);
        mutau_ind_temp = rest;
      }




      bool filled_histos[Mth.size()][systs.size()];
      for (unsigned int l=0; l<Mth.size(); ++l) for (unsigned int k=0; k<systs.size(); ++k) filled_histos[l][k] = false;
      //start loop over reconstructed mu-tau pairs
      for (unsigned int ii = 0; ii < mutau_ind.size(); ++ii) {
	int iMu = mutau_ind[ii][0];
	int iTau = mutau_ind[ii][1];
	if (mu_ibt_pt->at(iMu) < 45.0) continue;
	if (!mu_isHighPtMuon->at(iMu)) continue;
	if (fabs(mu_ibt_eta->at(iMu)) > 2.4) continue;


	TLorentzVector mu_p4, mu_ibt_transp4;
	mu_p4.SetPtEtaPhiM(mu_ibt_pt->at(iMu), mu_ibt_eta->at(iMu), mu_ibt_phi->at(iMu), mu_mass);
	mu_ibt_transp4.SetPxPyPzE(mu_ibt_px->at(iMu), mu_ibt_py->at(iMu), 0, mu_ibt_pt->at(iMu));

	TLorentzVector tau_p4, tau_TES_p4, vis_p4, met_p4, metmu_p4, total_p4;
	float met_px = MET_T1Txy_Px;
	float met_py = MET_T1Txy_Py;
	float met_pt = MET_T1Txy_Pt;
	tau_p4.SetPtEtaPhiE(tau_pt->at(iTau), tau_eta->at(iTau), tau_phi->at(iTau), tau_energy->at(iTau));
	met_p4.SetPxPyPzE(met_px, met_py, 0, met_pt);
	
	vis_p4 = tau_p4 + mu_p4;
	total_p4 = vis_p4 + met_p4;
	metmu_p4 = met_p4 + mu_p4;
	
	if (tau_pt->at(iTau) < 0) continue;
	if (tau_p4.Pt() < 27.0) continue;
	if (fabs(tau_eta->at(iTau)) > 2.3) continue;
	if (tau_decayModeFinding->at(iTau) < 0.5) continue;
	if (tau_againstMuonLoose3->at(iTau) < 0.5) continue;
	if (tau_againstElectronLooseMVA6->at(iTau) < 0.5) continue;
	if (fabs(tau_charge->at(iTau)) != 1) continue;

	//control regions : sign selection, muon isolation and tau ID
	if (CR_number == -1) {
	  cout << "Error: Control region not recognized" << endl;
	  break;
	}
	
	float reliso = mu_isoTrackerBased03->at(iMu); //use instead sumofpts divided by muon ibt pt


	//MET recalculation because we're using the high-pt muon ID
	TLorentzVector mu_gt_p4, mu_gt_transp4;
	mu_gt_p4.SetPxPyPzE(0, 0, 0, 0);
	mu_gt_transp4.SetPxPyPzE(0, 0, 0, 0);

	float min_dR = 0.2;
	for (unsigned int kk=0; kk<mu_gt_pt->size(); ++kk) {
	  if (!mu_isPFMuon->at(kk)) continue;
	  mu_gt_p4.SetPtEtaPhiM(mu_gt_pt->at(kk), mu_gt_eta->at(kk), mu_gt_phi->at(kk), mu_mass);
	  if ( (abs(mu_gt_p4.Pt()) >= 998.99) && (abs(mu_gt_p4.Pt()) <= 999.01) )continue;
	  if (mu_gt_p4.DeltaR(mu_p4) > min_dR) continue;
	  min_dR = mu_gt_p4.DeltaR(mu_p4);
	  mu_gt_transp4.SetPtEtaPhiM(mu_gt_pt->at(kk), 0, mu_gt_phi->at(kk), mu_mass);
	}
	met_p4 = met_p4 + mu_gt_transp4 - mu_ibt_transp4;


	
	float Mt = -1;
	if (2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) < 0) {
	  Mt = 0;
	}
	else {
	  Mt = sqrt(2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) );
	}

	if (CR_number<100) {
	  //sign selection
	  if (CR_number%2 == 0) {
	    if (tau_charge->at(iTau) * mu_ibt_charge->at(iMu) > 0) continue; //OS selection
	  }
	  else {
	    if (tau_charge->at(iTau) * mu_ibt_charge->at(iMu) < 0) continue; //SS selection
	  }
	  
	  //muon isolation
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
	    if (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }
	}
	else {
	  if (CR_number == 100) {
	    if (reliso > 0.1) continue;
	    //if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	    if (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }	      
	  if (CR_number == 101) {
	    if (reliso > 0.1) continue;
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) continue;
	    if (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }	      
	  if (CR_number == 102) {
	    if (reliso > 0.1) continue;
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	    if (Mt<80) continue;
	  }	      
	  if (CR_number == 103) {
	    if (reliso > 0.1) continue;
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) continue;
	    if (tau_byVLooseIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	    if (Mt<80) continue;
	  }	      
	}


	TString lepton = "", mu_lepton = "";

	//separate histos by tau realness
	int jTauN=j_real;
	bool tau_match = false;
	float dR_threshold = 0.4;

	if (!data) {
	  //fill gen histos to understand wth is going on
	  for (unsigned int iGen = 0; iGen<tauhp4.size(); ++iGen) {
	    hgen[0]->Fill(tauhp4[iGen].Pt(), mc_w_sign);
	    hgen[1]->Fill(tauhp4[iGen].Eta(), mc_w_sign);
	    hgen[2]->Fill(tauhp4[iGen].Phi(), mc_w_sign);
	  }

	  bool ele_match = false;
	  if (genelep4.size() != 0) {
	    for (unsigned int iGen = 0; iGen < genelep4.size(); ++iGen) {
	      if (tau_p4.DeltaR(genelep4[iGen]) < dR_threshold) ele_match = true;
	    }
	  }
	  bool mu_match = false;
	  bool recogen_mu_match = false;
	  if (genmup4.size() != 0) {
	    for (unsigned int iGen = 0; iGen < genmup4.size(); ++iGen) {
	      if (tau_p4.DeltaR(genmup4[iGen]) < dR_threshold) mu_match = true;
	      if (mu_p4.DeltaR(genmup4[iGen]) < dR_threshold) recogen_mu_match = true;
	    }
	  }
	  if (ele_match && mu_match) {
	    cout << "gen electron AND muon both match to same tau !!!" << endl << endl << endl;
	    continue;
	  }
	  if (ele_match) lepton = "ele";
	  if (mu_match)  lepton = "mu";
	  if (recogen_mu_match) mu_lepton = "mu";


	  bool is_not_jet = false;
	  if (anyleptonp4.size() != 0) {
	    for (unsigned int iGen = 0; iGen < anyleptonp4.size(); ++iGen) {
	      if (tau_p4.DeltaR(anyleptonp4[iGen]) < dR_threshold) {
		is_not_jet = true;
		break;
	      }
	    }
	  }
	  if (tauhp4.size() != 0) {
	    for (unsigned int iGen = 0; iGen < tauhp4.size(); ++iGen) {
	      if (tau_p4.DeltaR(tauhp4[iGen]) < dR_threshold) {
		tau_match = true;
		lepton = "tau";
		break;
	      }
	    }
	  }
	  if (is_not_jet) {
	    jTauN=j_real;
	  }
	  else {
	    //jTauN=j_fake;
	    continue;
	  }
	  if (CR_number >= 2) tau_match = false;
	}


	// MATCH TAUS TO AK4 jets
        bool matched_to_reco_jet=false;
        TLorentzVector jet_p4(0.,0.,0.,0.);
        for (unsigned int ijet = 0; ijet < jet_pt->size(); ijet++){
          if(!(fabs(jet_eta->at(ijet)) < 2.3)) continue;
	  bool looseJet = false;
	  if (signal) looseJet = jet_isJetIDLoose->at(ijet);
	  else looseJet = jet_isJetIDLoose_2016->at(ijet);
          if(!looseJet) continue;
          TLorentzVector jet_p4_tmp;
          jet_p4_tmp.SetPxPyPzE(jet_px->at(ijet), jet_py->at(ijet), jet_pz->at(ijet), jet_energy->at(ijet));
          if(!(tau_p4.DeltaR(jet_p4_tmp) < dR_threshold)) continue;
          matched_to_reco_jet=true;
          jet_p4=jet_p4_tmp;
          break;

        }

	if(!(matched_to_reco_jet)) continue;


	float ratio = 0;
	if (jet_p4.Pt() != 0) ratio = tau_p4.Pt()/jet_p4.Pt();

	int lMth = -1;
	if (Mt < 120) {
	  if ( tau_charge->at(iTau)*mu_ibt_charge->at(iMu) < 0) {
	    lMth = k_low_OS;
	  }
	  else {
	    lMth = k_low_SS;
	  }
	}
	else {
	  lMth = k_high;
	}


	float nVert = -1, mcWeight = 1;
	if (!data) {
	  nVert = mc_trueNumInteractions;
	  if (mc_weight > 0) mcWeight = 1;
	  else if (mc_weight < 0) mcWeight = -1;
	  else mcWeight = 0;
	}
	cout << "mc weight: " << mcWeight << endl;

	map<TString, float> weightsSys;
	//weights and systematics
	weightsSys = GetWeightSys(CR_number, nVert, tau_p4, ratio, mu_p4, lepton, top_pt_1, top_pt_2);
	first_weight = weightsSys["nominal"]*mcWeight;
	h[lMth][0][jTauN][13]->Fill(first_weight);
	final_weight = first_weight;


	if (first_weight != first_weight) {
	  cout << "Not a number!!!!!!!" << endl;
	  continue;
	}

	
	float dR = tau_p4.DeltaR(mu_p4);
	if (dR < 0.5) continue;

	int sign_number = -1;
	if ( tau_charge->at(iTau)*mu_ibt_charge->at(iMu) < 0) {
	  sign_number = OS_number;
	}
	else {
	  sign_number = SS_number;
	}
	


	TString eta_string = GetEtaString(tau_p4.Eta());
	int l_eta = -1;
	if (eta_string == "barrel") {
	  l_eta = l_barrel;
	}
	else if (eta_string == "endcap") {
	  l_eta = l_endcap;
	}
	else {
	  continue;
	}


	//TH2's for the fake rate
	int iRatio = -1;
	//Tau histos
	if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) > 0.5) {
	  iRatio  = iRatioPass;
	}
	else {
	  iRatio  = iRatioFail;
	}



	cout << endl << "PU: " << mc_trueNumInteractions << endl;

	TLorentzVector tau_p4_copy = tau_p4, mu_p4_copy = mu_p4, met_p4_copy = met_p4;
	for (unsigned int k_syst=0; k_syst<systs.size(); ++k_syst) {
	  TString argument = systs[k_syst];
	  for (unsigned int iSpecial=0; iSpecial<specialSyst.size(); ++iSpecial) {
	    if (systs[k_syst] == specialSyst[iSpecial]) {
	      argument = "nominal";
	      vector<TLorentzVector> newp4 = GetScaleVariation(systs[k_syst], lepton, mu_lepton, tau_p4_copy, mu_p4_copy, mu_ibt_charge->at(iMu), met_p4_copy);
	      tau_p4 = newp4[0];
	      mu_p4 = newp4[1];
	      met_p4 = newp4[2];
	      break;
	    }
	    else {
	      tau_p4 = tau_p4_copy;
	      mu_p4 = mu_p4_copy;
	      met_p4 = met_p4_copy;
	    }
	  }
	  
	  final_weight = weightsSys[argument]*mcWeight;

	  cout << "systematic: " << systs[k_syst] << "  weight: " << final_weight << endl;
	  cout << "tau: " << lepton << "  pt: " << tau_p4.Pt() << endl;
	  cout << "mu: " << mu_lepton << "  pt: " << mu_p4.Pt() << endl << endl;

	  //tau and mu pt selections
	  if (tau_p4.Pt() < 30.0) continue;
	  if (mu_p4.Pt() < 53.0) continue;
	  
	  //Mt recalculation
	  Mt = sqrt(2 * ( mu_p4.Pt() * met_p4.Pt()  - mu_p4.Px()*met_p4.Px() - mu_p4.Py()*met_p4.Py() ) );

	  lMth = -1;
	  bool isLowTTCR = false, isHighTTCR = false;
	  TString sign_string = ""; 
	  if (Mt < 120) {
	    if ( tau_charge->at(iTau)*mu_ibt_charge->at(iMu) < 0) {
	      lMth = k_low_OS;
	      sign_string = "OS";
	      if (nbjet >= 2) isLowTTCR = true;
	    }
	    else {
	      lMth = k_low_SS;
	      sign_string = "SS";
	    }
	  }
	  else {
	    lMth = k_high;
	    if (tau_charge->at(iTau)*mu_ibt_charge->at(iMu) < 0) {
	      sign_string = "OS";
	      if (nbjet >= 2) isHighTTCR = true;
	    }
	    else {
	      sign_string = "SS";
	    }
	  }



	  float Mcol = GetCollinearMass(tau_p4, mu_p4, met_p4);


	  int l_value = lMth;
	  if (CR_number == 100) {
	    if ( (l_value < k_low_SS + 1) && !signal) {
	      hh[iRatio][l_value][k_syst][l_eta]->Fill(tau_p4.Pt(), ratio, final_weight);
	    }
	    if (tau_byTightIsolationMVArun2v1DBoldDMwLT->at(iTau) < 0.5) continue;
	  }
	  
	  bool stopFilling = false;
	  int n_fuel = 0;
	  while (!stopFilling && n_fuel<2) {
	    ++n_fuel;


	    //only fill histos once for each region and systematic
	    if (filled_histos[l_value][k_syst]) continue;
	    filled_histos[l_value][k_syst] = true;

	    
	    h[l_value][k_syst][jTauN][0]->Fill(vis_p4.M(), final_weight);
	    h[l_value][k_syst][jTauN][1]->Fill(total_p4.M(), final_weight);
	    h[l_value][k_syst][jTauN][2]->Fill(tau_p4.Pt(), final_weight);
	    h[l_value][k_syst][jTauN][3]->Fill(tau_p4.Eta(), final_weight);
	    h[l_value][k_syst][jTauN][4]->Fill(tau_p4.Phi(), final_weight);
	    h[l_value][k_syst][jTauN][5]->Fill(mu_p4.Pt(), final_weight);
	    h[l_value][k_syst][jTauN][6]->Fill(mu_p4.Eta(), final_weight);
	    h[l_value][k_syst][jTauN][7]->Fill(mu_p4.Phi(), final_weight);
	    h[l_value][k_syst][jTauN][8]->Fill(dR, final_weight);
	    h[l_value][k_syst][jTauN][9]->Fill(met_p4.Pt(), final_weight);
	    h[l_value][k_syst][jTauN][10]->Fill(met_p4.Px()-met_px, final_weight);
	    h[l_value][k_syst][jTauN][11]->Fill(Mcol, final_weight);
	    h[l_value][k_syst][jTauN][12]->Fill(Mt, final_weight);
	    h[l_value][k_syst][jTauN][13]->Fill(reliso, final_weight);
	    h[l_value][k_syst][jTauN][14]->Fill(final_weight, 1);
	    h[l_value][k_syst][jTauN][15]->Fill(sign_number, final_weight);
	    h[l_value][k_syst][jTauN][16]->Fill(jet_n, final_weight);
	    h[l_value][k_syst][jTauN][17]->Fill(nbjet, final_weight);
	    //special TT plots
	    if (l_value == k_high_TT || l_value == k_low_TT) {
	      stopFilling = true;
	      for (unsigned int iBJet = 0; iBJet < bjet_p4.size(); ++iBJet) {
		h[l_value][k_syst][jTauN][18]->Fill(bjet_p4[iBJet].Pt(), final_weight);
		h[l_value][k_syst][jTauN][19]->Fill(bjet_p4[iBJet].Eta(), final_weight);
		h[l_value][k_syst][jTauN][20]->Fill(bjet_p4[iBJet].Phi(), final_weight);
	      }
	    }
	    if (l_value == lMth) {
	      if (isHighTTCR) {
	    	l_value = k_high_TT;
	    	final_weight *= bjet_weight;
	      }
	      else if (isLowTTCR) {
	    	l_value = k_low_TT;
	    	final_weight *= bjet_weight;
	      } 
	      else {
	    	stopFilling = true;
	      }
	    }
	  }
	}
      }//loop over mu-tau pairs
   }//loop over events

   
   TH1F* h_total_events =  new TH1F("weighted_events", "weighted_events", 1, 0, 1);
   h_total_events->Fill(0.5, nEvents);
   file_out->cd();
   h_total_events->Write();

   vector<TDirectory*> d_sys;
   for (unsigned int k = 0; k<systs.size(); ++k) {
     d_sys.push_back( file_out->mkdir( systs[k] ) );
     d_sys[k]->cd();
     for (unsigned int i = 0; i<histo_names.size(); ++i) for (unsigned int j = 0; j<taun.size(); ++j) for (unsigned int l = 0; l<Mth.size(); ++l) if (!(l < k_low_TT && i >= n_TT_plots)) h[l][k][j][i]->Write();
     if (CR_number == 100 && !signal) for (unsigned int i = 0; i<h_names.size(); ++i) for (unsigned int j = 0; j<k_low_SS+1; ++j) for (unsigned int l = 0; l<eta.size(); ++l) hh[i][j][k][l]->Write();
     d_sys[k]->Close();
   }
   cout << "end" << endl;

   for (unsigned int i = 0; i<hgen.size(); ++i) hgen[i]->Write();
   file_out->Close();
 
}
