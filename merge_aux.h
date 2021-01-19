#include "TH1.h"
#include "TH2.h"
#include <vector>
#include <iostream>
#include "TString.h"

using namespace std;


TH1F* adjust_binning(TH1F* h_in, bool bSimple, vector<float> points, int rebin, bool normed) {
  TString name_in = h_in->GetName();

  if (bSimple) {
    TH1F* h_out = (TH1F*) h_in->Clone("rebinned_"+name_in);
    h_out->SetTitle("rebinned_"+name_in);
    h_out->Rebin(rebin);
    return h_out;
  }
  else {
    int array_size = points.size();
    float rebin_array[array_size];
    for (unsigned int ii=0; ii<array_size; ++ii) rebin_array[ii] = points[ii];
    TH1F* h_out = new TH1F("rebinned_"+name_in, "rebinned_"+name_in, array_size-1, rebin_array);

    int jBin = 1;
    float bin_content = 0, bin_error=0;
    for (unsigned int iBin=1; iBin < h_in->GetNbinsX()+1; ++iBin) {
      if (h_in->GetBinCenter(iBin) < rebin_array[jBin]) {
	if (h_in->GetBinCenter(iBin) < rebin_array[jBin-1]) continue;
	bin_content += h_in->GetBinContent(iBin);
	bin_error += pow(h_in->GetBinError(iBin), 2);
      }
      else {
	bin_error = sqrt(bin_error);
	if (normed) {
	  bin_content = bin_content/(rebin_array[jBin]-rebin_array[jBin-1]);
	  bin_error = bin_error/(rebin_array[jBin]-rebin_array[jBin-1]);
	}
	h_out->SetBinContent(jBin, bin_content);
	h_out->SetBinError(jBin, bin_error);
	bin_content = h_in->GetBinContent(iBin);
	bin_error = pow(h_in->GetBinError(iBin), 2);
	
	++jBin;
      }
    }
    bin_error = sqrt(bin_error);
    if (normed) {
      bin_content = bin_content/(rebin_array[jBin]-rebin_array[jBin-1]);
      bin_error = bin_error/(rebin_array[jBin]-rebin_array[jBin-1]);
    }
    h_out->SetBinContent(jBin, bin_content);
    h_out->SetBinError(jBin, bin_error);
    return h_out;
  }
}



TH1F* MC_histo(TString var, TFile* file_in, TFile* file_in_d, double xs, int rebin) {

  cout << file_in->GetName() << endl;

  TH1F* h_events_data = (TH1F*) file_in_d->Get("weighted_events");
  double full_data = 7.86454e+08;
  double succ_data_ratio = h_events_data->Integral()/full_data;
  cout << "succesfull data ratio " << succ_data_ratio << endl;
  double lumi = 35.9 * pow(10,3) * succ_data_ratio; //luminosity in pb^-1                                                                                                                

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();

  double e_Nevents = pow(Nevents,0.5);
  double e_xs = 0.01*xs;

  //Weight                                                                                                                                                                               
  double w = 0;
  if (Nevents != 0) w = xs*lumi/Nevents;
  cout << "xs: " << xs << "  Nevents: " << Nevents << endl;
  cout << "Events in data/events in MC " << w << endl;

  TH1F* h;
  h = (TH1F*) file_in -> Get(var);

  h -> Scale(w);
  h -> Rebin(rebin);

  return h;

}
