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

  Float_t nEvents = -1;

  if (type=="ZPrime" || type=="Zprime" || type=="zprime" || type=="QBH") {
    TH1F* h_total_events = (TH1F*) fIn->Get("weighted_events");
    nEvents = h_total_events->Integral();
  }
  else {
    TTree* mmeta = (TTree*) fIn->Get("meta");
    meta* m = new meta(mmeta);
    nEvents = m->Loop(type);
  }

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


   bool signal, data, singlephoton, singlemu, DYinc, WJetsinc, TTinc, WWinc, TT, WW, zprime;
   TString process = "";
   if (type_of_data == "Signal" || type_of_data == "signal" || type_of_data == "ZPrime" || type_of_data == "Zprime" || type_of_data == "zprime" || type_of_data == "QBH" || type_of_data == "RPV") {
     signal = true;
     if (type_of_data ==  "ZPrime" || type_of_data == "Zprime" || type_of_data == "zprime") zprime = true;
     else zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = false;
   }
   else if (type_of_data == "Data" || type_of_data == "data" || type_of_data == "singlephoton" || type_of_data == "SinglePhoton" || type_of_data == "singlemu" || type_of_data == "SingleMu") {
     signal = false;
     zprime = false;
     data = true;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = false;
   }
   else if (type_of_data == "DYinc") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = true;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = false;
   }
   else if (type_of_data == "WJetsinc") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = true;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = false;
   }
   else if (type_of_data == "TTinc") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = true;
     TT = true;
     WWinc = false;
     WW = false;
     process = "TT";
   }
   else if (type_of_data == "TT") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = true;
     WWinc = false;
     WW = false;
     process = "TT";
   }
   else if (type_of_data == "WWinc") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = true;
     WW = true;
     process = "WW";
   }
   else if (type_of_data == "WW") {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = true;
     process = "WW";
   }
   else {
     signal = false;
     zprime = false;
     data = false;
     DYinc = false;
     WJetsinc = false;
     TTinc = false;
     TT = false;
     WWinc = false;
     WW = false;
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




   vector<LHAPDF::PDF*> pdf;
   vector<TH1F*> h_PDFweight;
   vector<TH1F*> h_sumPDF;
   TH1F* h_mcWeightBefore = new TH1F("mcWeight_before", "mcWeight_before", 1, 0, 1);
   TH1F* h_mcWeightAfter = new TH1F("mcWeight_after", "mcWeight_after", 1, 0, 1);
   TH1F* h_acceptance = new TH1F("acceptance", "acceptance", 1, 0, 1);
   if (zprime) {
     for(unsigned int in = 0; in < 101; ++in){
       pdf.push_back(LHAPDF::mkPDF("NNPDF31_nnlo_as_0118", in));
       TString PDFname = "PDFweights_"+std::to_string(in);
       h_PDFweight.push_back( new TH1F(PDFname, PDFname, 20000, -1000, 1000));
       PDFname = "sumOfPDFWeights_"+std::to_string(in);
       h_sumPDF.push_back(    new TH1F(PDFname, PDFname, 1, 0, 1));
     }
   }
   //end systematics


   

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

      float nVert = -1, mcWeight = 1;
      if (!data) {
	nVert = mc_trueNumInteractions;
	if (mc_weight > 0) mcWeight = 1;
	else if (mc_weight < 0) mcWeight = -1;
	else mcWeight = 0;
      }
      cout << "mc weight: " << mcWeight << endl;

      //LHAPDF weights
      vector<float> pdfweights;                                                                                                                                                                 
      if (zprime) {
	double xfx1_3 = pdf.at(0)->xfxQ(mc_id_first,  mc_x_first, mc_scalePDF);
	double xfx2_3 = pdf.at(0)->xfxQ(mc_id_second,  mc_x_second, mc_scalePDF);
	double w03 = xfx1_3*xfx2_3;
	double lhaweight3 = 0.0;
	pdfweights.clear();	
	bool bHighWeight = false;
        h_mcWeightBefore->Fill(0.5,mcWeight);
        for(unsigned int iPDF = 1; iPDF < pdf.size(); iPDF++){
          double      xfx_1_3 = pdf.at(iPDF)->xfxQ(mc_id_first, mc_x_first, mc_scalePDF);
          double      xfx_2_3 = pdf.at(iPDF)->xfxQ(mc_id_second, mc_x_second, mc_scalePDF);
          double w_new3 = xfx_1_3*xfx_2_3;
          lhaweight3  = w_new3/w03;
          if (fabs(lhaweight3) > 100) std::cout << "rew: " << lhaweight3 <<  "  w03: " << w03 << "  w_new3: " << w_new3 << std::endl;
          if (fabs(lhaweight3) > 10) {
	    bHighWeight = true;
	    break;
	  }
        }
        if (bHighWeight) continue;
	h_mcWeightAfter->Fill(0.5,mcWeight);

        for(unsigned int iPDF = 0; iPDF < pdf.size(); iPDF++){
          double      xfx_1_3 = pdf.at(iPDF)->xfxQ(mc_id_first, mc_x_first, mc_scalePDF);
          double      xfx_2_3 = pdf.at(iPDF)->xfxQ(mc_id_second, mc_x_second, mc_scalePDF);
          double w_new3 = xfx_1_3*xfx_2_3;
          lhaweight3  = w_new3/w03;
          h_sumPDF[iPDF]->Fill(0.5,w_new3);
          h_PDFweight[iPDF]->Fill(lhaweight3,1);

          pdfweights.push_back(lhaweight3);
        }
	
	//acceptance
        TLorentzVector l1_p4, l2_p4, pp4;
	l1_p4.SetPxPyPzE(0, 0, 0, 0);
	l2_p4.SetPxPyPzE(0, 0, 0, 0);
	bool found_mutau = false;
	for (unsigned int iMu = 0; iMu < mc_pt->size(); ++iMu) {
	  if (mc_pt->at(iMu) < 10) continue;
	  if (abs(mc_pdgId->at(iMu)) != 13) continue;
	  if (mc_status->at(iMu) > 1) continue;
	  l1_p4.SetPxPyPzE(mc_px->at(iMu), mc_py->at(iMu), mc_pz->at(iMu), mc_energy->at(iMu));
	  if (l1_p4.Pt() < 45) continue;
	  if (fabs(l1_p4.Eta()) > 2.4) continue;
  
	  for (unsigned int iTau = 0; iTau < mc_tau_had_pt->size(); ++iTau) {
	    if (mc_tau_had_status->at(iTau) > 2) continue;
	    l2_p4.SetPtEtaPhiE(mc_tau_had_pt->at(iTau), mc_tau_had_eta->at(iTau), mc_tau_had_phi->at(iTau), mc_tau_had_energy->at(iTau));
	    if (l2_p4.Pt() < 45) continue;
	    if (fabs(l2_p4.Eta()) > 2.3) continue;
	    if (l1_p4.DeltaR(l2_p4) < 0.5) continue;
	    float DRthr = 0.2;
	    bool hadtau = true;
	    for (unsigned int iMC2 = 0; iMC2 < mc_tau_pt->size(); ++iMC2) {
	      pp4.SetPtEtaPhiE(mc_tau_pt->at(iMC2), mc_tau_eta->at(iMC2), mc_tau_phi->at(iMC2), mc_tau_energy->at(iMC2));
	      if ((l2_p4.DeltaR(pp4) < DRthr) && (mc_tau_decay->at(iMC2) == 0 || mc_tau_decay->at(iMC2) == 1)) hadtau = false;
	    }
	    if (!hadtau) continue;

	    found_mutau = true;
	    break;
	  }
	  if (found_mutau) break;
	}
	if (found_mutau) h_acceptance->Fill(0.5, mcWeight);
      }


   }//loop over events

   
   TH1F* h_total_events =  new TH1F("weighted_events", "weighted_events", 1, 0, 1);
   h_total_events->Fill(0.5, nEvents);
   file_out->cd();
   h_total_events->Write();
   h_mcWeightBefore->Write();
   h_mcWeightAfter->Write();
   h_acceptance->Write();

   for (unsigned int iPDF=0; iPDF<h_PDFweight.size(); ++iPDF) {
     h_PDFweight[iPDF]->Write();
     h_sumPDF[iPDF]->Write();
   }


   //for (unsigned int i = 0; i<hgen.size(); ++i) hgen[i]->Write();
   file_out->Close();
 
}
