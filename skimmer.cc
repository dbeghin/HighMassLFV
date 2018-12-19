#include <iostream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "vector"
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>
#include <sys/stat.h>
#include <TVector3.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

using namespace std;

int main(int argc, char** argv) {
  string out = *(argv + 1);
  string in = *(argv + 2);
  TString out_name= out;
  TString in_name = in;


  TFile *file_in=TFile::Open(in_name, "R");

  TTree* oldtree = (TTree*) file_in->Get("IIHEAnalysis");



  //IIHEAnalysis *ev_event   = new IIHEAnalysis();
  //oldtree->SetBranchAddress("ev_event",&ev_event);
  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("ev_event",1);
  oldtree->SetBranchStatus("ev_run",1);
  oldtree->SetBranchStatus("ev_luminosityBlock",1);
  oldtree->SetBranchStatus("trig_HLT_Photon50_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon33_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon75_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon90_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon120_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon175_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon200_accept",1);
  oldtree->SetBranchStatus("gsf_sc_eta",1);
  oldtree->SetBranchStatus("gsf_sc_phi",1);
  oldtree->SetBranchStatus("gsf_sigmaIetaIeta",1);
  oldtree->SetBranchStatus("gsf_hadronicOverEm",1);
  oldtree->SetBranchStatus("gsf_nLostInnerHits",1);
  oldtree->SetBranchStatus("gsf_dxy_firstPVtx",1);
  oldtree->SetBranchStatus("gsf_dz_firstPVtx",1);
  oldtree->SetBranchStatus("trig_HLT_DoubleEle33_CaloIdL_MW_hltEle33CaloIdLMWPMS2Filter_eta",1);
  oldtree->SetBranchStatus("trig_HLT_DoubleEle33_CaloIdL_MW_hltEle33CaloIdLMWPMS2Filter_phi",1);
  oldtree->SetBranchStatus("gsf_caloEnergy",1);
  oldtree->SetBranchStatus("gsf_eta",1);
  oldtree->SetBranchStatus("gsf_dr03EcalRecHitSumEt",1);
  oldtree->SetBranchStatus("gsf_dr03HcalDepth1TowerSumEt",1);
  oldtree->SetBranchStatus("ev_fixedGridRhoFastjetAll",1);
  oldtree->SetBranchStatus("gsf_sc_energy",1);
  oldtree->SetBranchStatus("trig_HLT_DoubleEle33_CaloIdL_MW_hltEle33CaloIdLMWPMS2Filter_et",1);
  oldtree->SetBranchStatus("gsf_dr03TkSumPtHEEP7",1);
  oldtree->SetBranchStatus("gsf_sc_et",1);
  oldtree->SetBranchStatus("gsf_et",1);
  oldtree->SetBranchStatus("gsf_ecaldrivenSeed",1);
  oldtree->SetBranchStatus("gsf_deltaEtaSeedClusterTrackAtVtx",1);
  oldtree->SetBranchStatus("gsf_deltaPhiSuperClusterTrackAtVtx",1);
  oldtree->SetBranchStatus("gsf_full5x5_e1x5",1);
  oldtree->SetBranchStatus("gsf_full5x5_e5x5",1);
  oldtree->SetBranchStatus("gsf_full5x5_e2x5Max",1);
  oldtree->SetBranchStatus("gsf_full5x5_hcalOverEcal",1);
  oldtree->SetBranchStatus("gsf_full5x5_sigmaIetaIeta",1);
  oldtree->SetBranchStatus("gsf_energy",1);
  oldtree->SetBranchStatus("gsf_VIDmvaEleIDwp90",1);
  oldtree->SetBranchStatus("gsf_VIDmvaEleIDwp80",1);
  oldtree->SetBranchStatus("gsf_VIDHEEP7",1);
  oldtree->SetBranchStatus("gsf_sc_theta",1);
  oldtree->SetBranchStatus("gsf_sc_px",1);
  oldtree->SetBranchStatus("gsf_sc_py",1);
  oldtree->SetBranchStatus("gsf_sc_pz",1);
  oldtree->SetBranchStatus("gsf_theta",1);
  oldtree->SetBranchStatus("gsf_px",1);
  oldtree->SetBranchStatus("gsf_py",1);
  oldtree->SetBranchStatus("gsf_pz",1);
  oldtree->SetBranchStatus("gsf_p",1);
  oldtree->SetBranchStatus("gsf_pt",1);
  oldtree->SetBranchStatus("gsf_deltaEtaSuperClusterTrackAtVtx",1);
  oldtree->SetBranchStatus("gsf_deltaPhiSuperClusterTrackAtVtx",1);
  oldtree->SetBranchStatus("gsf_hcalDepth1OverEcal",1);
  oldtree->SetBranchStatus("gsf_hcalDepth2OverEcal",1);
  oldtree->SetBranchStatus("gsf_dr03TkSumPt",1);
  oldtree->SetBranchStatus("gsf_dr03HcalDepth2TowerSumEt",1);
  oldtree->SetBranchStatus("gsf_charge",1);
  oldtree->SetBranchStatus("gsf_trackerdrivenSeed",1);
  oldtree->SetBranchStatus("gsf_scE1x5",1);
  oldtree->SetBranchStatus("gsf_scE5x5",1);
  oldtree->SetBranchStatus("gsf_scE2x5Max",1);
  oldtree->SetBranchStatus("ele_VIDLoose",1);
  oldtree->SetBranchStatus("ele_VIDMedium",1);
  oldtree->SetBranchStatus("ele_VIDTight",1);
  oldtree->SetBranchStatus("ele_VIDVeto",1);

  // jet id
  oldtree->SetBranchStatus("jet_pt",1);
  oldtree->SetBranchStatus("jet_eta",1);
  oldtree->SetBranchStatus("jet_isJetIDLoose",1);
  oldtree->SetBranchStatus("jet_CSVv2",1);
  oldtree->SetBranchStatus("jet_phi",1);
  oldtree->SetBranchStatus("jet_energy",1);
  oldtree->SetBranchStatus("jet_Area",1);

  // PV info
  oldtree->SetBranchStatus("pv_isValid",1);
  oldtree->SetBranchStatus("pv_x",1);
  oldtree->SetBranchStatus("pv_y",1);
  oldtree->SetBranchStatus("pv_z",1);
  oldtree->SetBranchStatus("pv_ndof",1);
  // MET info
  oldtree->SetBranchStatus("MET_T1Txy_Pt",1);
  oldtree->SetBranchStatus("MET_T1Txy_phi",1);
  oldtree->SetBranchStatus("MET_T1_Pt",1);
  oldtree->SetBranchStatus("MET_T1_phi",1);
  oldtree->SetBranchStatus("MET_T1Txy_significance",1);
  oldtree->SetBranchStatus("MET_FinalCollection_Pt",1);
  oldtree->SetBranchStatus("MET_FinalCollection_Px",1);
  oldtree->SetBranchStatus("MET_FinalCollection_Py",1);
  oldtree->SetBranchStatus("MET_FinalCollection_phi",1);
  oldtree->SetBranchStatus("MET_FinalCollection_significance",1);
  

  // Rho
  oldtree->SetBranchStatus("ev_fixedGridRhoFastjetAll",1);
  oldtree->SetBranchStatus("event_rho",1);
  oldtree->SetBranchStatus("mc_weight",1);
  oldtree->SetBranchStatus("mc_w_sign",1);
  oldtree->SetBranchStatus("pv_n",1);

  if(oldtree->GetListOfBranches()->FindObject("LHE_Pt")) {

    oldtree->SetBranchStatus("LHE_Pt", 1);
    oldtree->SetBranchStatus("LHE_Eta", 1);
    oldtree->SetBranchStatus("LHE_Phi",1);
    oldtree->SetBranchStatus("LHE_E",1);
    oldtree->SetBranchStatus("LHE_pdgid",1);
    oldtree->SetBranchStatus("LHE_status",1);
    oldtree->SetBranchStatus("LHE_weight_nominal",1); 
    oldtree->SetBranchStatus("LHE_weight_sys",1); 
    oldtree->SetBranchStatus("LHE_id_sys",1);
    oldtree->SetBranchStatus("mc_n",1); 
    oldtree->SetBranchStatus("mc_weight", 1);
    oldtree->SetBranchStatus("mc_w_sign",1); 
    oldtree->SetBranchStatus("mc_id_first",1);
    oldtree->SetBranchStatus("mc_id_second",1);
    oldtree->SetBranchStatus("mc_x_first", 1);
    oldtree->SetBranchStatus("mc_x_second", 1);
    oldtree->SetBranchStatus("mc_xPDF_first", 1);
    oldtree->SetBranchStatus("mc_xPDF_second", 1);
    oldtree->SetBranchStatus("mc_scalePDF",1);
    oldtree->SetBranchStatus("mc_index", 1);
    oldtree->SetBranchStatus("mc_pdgId",1);
    oldtree->SetBranchStatus("mc_charge",1);
    oldtree->SetBranchStatus("mc_status",1);
    oldtree->SetBranchStatus("mc_status_flags",1);
    oldtree->SetBranchStatus("mc_mass", 1);
    oldtree->SetBranchStatus("mc_px", 1);
    oldtree->SetBranchStatus("mc_py",1);
    oldtree->SetBranchStatus("mc_pz",1);
    oldtree->SetBranchStatus("mc_pt",1);
    oldtree->SetBranchStatus("mc_eta", 1);

    oldtree->SetBranchStatus("mc_phi",1);
    oldtree->SetBranchStatus("mc_energy",1);
    oldtree->SetBranchStatus("mc_numberOfDaughters",1);
    oldtree->SetBranchStatus("mc_numberOfMothers", 1);
    oldtree->SetBranchStatus("mc_mother_index",1);
    oldtree->SetBranchStatus("mc_mother_pdgId",1);
    oldtree->SetBranchStatus("mc_mother_px",1);
    oldtree->SetBranchStatus("mc_mother_py", 1);
    oldtree->SetBranchStatus("mc_mother_pz",1);
    oldtree->SetBranchStatus("mc_mother_pt", 1);
    oldtree->SetBranchStatus("mc_mother_eta",1);
    oldtree->SetBranchStatus("mc_mother_phi",1);
    oldtree->SetBranchStatus("mc_mother_energy",1);
    oldtree->SetBranchStatus("mc_mother_mass", 1);
    oldtree->SetBranchStatus("mc_trueNumInteractions",1);
    oldtree->SetBranchStatus("mc_PU_NumInteractions", 1);
    oldtree->SetBranchStatus("genjet_pt", 1);
    oldtree->SetBranchStatus("genjet_eta", 1);
    oldtree->SetBranchStatus("genjet_phi", 1);
    oldtree->SetBranchStatus("genjet_energy",1);
  }

  oldtree->SetBranchStatus("trig_Flag_goodVertices_accept",1);
  oldtree->SetBranchStatus("trig_Flag_globalTightHalo2016Filter_accept",1);
  oldtree->SetBranchStatus("trig_Flag_HBHENoiseFilter_accept",1);
  oldtree->SetBranchStatus("trig_Flag_HBHENoiseIsoFilter_accept",1);
  oldtree->SetBranchStatus("trig_Flag_EcalDeadCellTriggerPrimitiveFilter_accept",1);
  oldtree->SetBranchStatus("trig_Flag_BadPFMuonFilter_accept",1);
  oldtree->SetBranchStatus("trig_Flag_BadChargedCandidateFilter_accept",1);
  if(oldtree->GetListOfBranches()->FindObject("trig_Flag_eeBadScFilter_accept")) {oldtree->SetBranchStatus("trig_Flag_eeBadScFilter_accept",1);
  }

  // mu triggers
  oldtree->SetBranchStatus("trig_HLT_IsoTkMu24_accept",1);
  oldtree->SetBranchStatus("trig_HLT_IsoMu24_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Mu50_accept",1);
  oldtree->SetBranchStatus("trig_HLT_TkMu50_accept",1);

  // electron triggers
  oldtree->SetBranchStatus("trig_HLT_Ele115_CaloIdVT_GsfTrkIdT_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Ele27_WPTight_Gsf_accept",1);
  oldtree->SetBranchStatus("trig_HLT_Photon175_accept",1);

  //tau trigger
  oldtree->SetBranchStatus("trig_HLT_VLooseIsoPFTau140_Trk50_eta2p1_accept",1);

  // tau branches
  oldtree->SetBranchStatus("tau_px",1);
  oldtree->SetBranchStatus("tau_py",1);
  oldtree->SetBranchStatus("tau_pz",1);
  oldtree->SetBranchStatus("tau_pt",1);
  oldtree->SetBranchStatus("tau_eta",1);
  oldtree->SetBranchStatus("tau_phi",1);
  oldtree->SetBranchStatus("tau_dz",1);
  oldtree->SetBranchStatus("tau_decayModeFindingNewDMs",1);
  oldtree->SetBranchStatus("tau_decayModeFinding",1);
  oldtree->SetBranchStatus("tau_byVLooseIsolationMVArun2v1DBoldDMwLT",1);
  oldtree->SetBranchStatus("tau_byLooseIsolationMVArun2v1DBoldDMwLT",1);
  oldtree->SetBranchStatus("tau_byMediumIsolationMVArun2v1DBoldDMwLT",1);
  oldtree->SetBranchStatus("tau_byTightIsolationMVArun2v1DBoldDMwLT",1);
  oldtree->SetBranchStatus("tau_byVTightIsolationMVArun2v1DBoldDMwLT",1);
  oldtree->SetBranchStatus("tau_againstMuonLoose3",1);
  oldtree->SetBranchStatus("tau_againstMuonTight3",1);
  oldtree->SetBranchStatus("tau_againstElectronVLooseMVA6",1);
  oldtree->SetBranchStatus("tau_againstElectronLooseMVA6",1);
  oldtree->SetBranchStatus("tau_againstElectronMediumMVA6",1);
  oldtree->SetBranchStatus("tau_againstElectronTightMVA6",1);
  oldtree->SetBranchStatus("tau_againstElectronVTightMVA6",1);
  oldtree->SetBranchStatus("tau_energy",1);
  oldtree->SetBranchStatus("tau_theta",1);
  oldtree->SetBranchStatus("tau_decayMode",1);
  oldtree->SetBranchStatus("tau_charge",1);

  //  muon gt & ibt
  oldtree->SetBranchStatus("mu_gt_pt",1);
  oldtree->SetBranchStatus("mu_gt_eta",1);
  oldtree->SetBranchStatus("mu_gt_phi",1);
  oldtree->SetBranchStatus("mu_gt_charge",1);
  oldtree->SetBranchStatus("mu_gt_dz_firstPVtx",1);
  oldtree->SetBranchStatus("mu_gt_dxy_firstPVtx",1);
  oldtree->SetBranchStatus("mu_gt_normalizedChi2",1);
  
  oldtree->SetBranchStatus("mu_ibt_pt",1);
  oldtree->SetBranchStatus("mu_ibt_px",1);
  oldtree->SetBranchStatus("mu_ibt_py",1);
  oldtree->SetBranchStatus("mu_ibt_pz",1);
  oldtree->SetBranchStatus("mu_ibt_eta",1);
  oldtree->SetBranchStatus("mu_ibt_phi",1);
  oldtree->SetBranchStatus("mu_ibt_charge",1);

  oldtree->SetBranchStatus("mu_isolationR03_sumPt",1);
  oldtree->SetBranchStatus("mu_isLooseMuon",1);
  oldtree->SetBranchStatus("mu_isPFMuon",1);
  oldtree->SetBranchStatus("mu_isMediumMuon",1);
  oldtree->SetBranchStatus("mu_pfIsoDbCorrected04",1);
  oldtree->SetBranchStatus("mu_pfIsoDbCorrected03",1);
  oldtree->SetBranchStatus("mu_isTightMuon",1);
  oldtree->SetBranchStatus("mu_isHighPtMuon",1);
  oldtree->SetBranchStatus("mu_combinedQuality_chi2LocalPosition",1);
  oldtree->SetBranchStatus("mu_combinedQuality_trkKink",1);
  oldtree->SetBranchStatus("mu_innerTrack_validFraction",1);
  oldtree->SetBranchStatus("mu_segmentCompatibility",1);
  oldtree->SetBranchStatus("mu_isoTrackerBased03",1);


  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile(out_name,"recreate");
  TTree *newtree = oldtree->CloneTree();
  file_in->Close();

  newfile->cd();
  newtree->Print();
  newfile->Write();
  newfile->Close();


  return 0;
}
