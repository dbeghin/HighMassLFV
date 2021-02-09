#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TLegend.h"
#include "THStack.h"
#include "TStyle.h"
#include "aux.h"
#include "merge_aux.h"

using namespace std;




TH1F* MC_histo_PDF(TString var, TString weightID, TFile* file_in, double xs, int rebin) {
  cout << var << endl;
  cout << file_in->GetName() << endl;

  TH1F* h_SOWbase = (TH1F*) file_in->Get("sumOfPDFWeights_base");
  double SOWbase = h_SOWbase->Integral();
  delete h_SOWbase;

  TH1F* h_before = (TH1F*) file_in->Get("mcWeight_before");
  double Nbefore = h_before->Integral();
  delete h_before;

  TH1F* h_after = (TH1F*) file_in->Get("mcWeight_after");
  double Nafter = h_after->Integral();
  delete h_after;

  TH1F* h_events = (TH1F*) file_in->Get("weighted_events");
  double Nevents = h_events->Integral();
  delete h_events;

  double lumi = 41.529 * pow(10,3);

  double w = 0;
  TH1F* h_SOWsys = (TH1F*) file_in->Get("sumOfPDFWeights_"+weightID);
  double SOWsys = h_SOWsys->Integral();
  delete h_SOWsys;

  //Weight
  if (SOWsys > 0) w = xs*lumi/Nevents * Nbefore/Nafter * SOWbase/SOWsys;
  else w = 0;

  cout << "Events in data/MC " << w << endl;

  TH1F* hh;
  hh = (TH1F*) file_in -> Get(var);

  hh -> Scale(w);
  hh -> Rebin(rebin);

  return hh;
}



int main(int argc, char** argv) {


  int rebin = 1;
  //string CR = *(argv + 1);

  TString folder_in = "HighMassLFVMuTau/RankPDF";
  //TString folder_in = "HighMassLFVMuTau/SignalPDFTest";
  TString name_out = "histos_signal";

  TFile* file_out = new TFile("Figures/"+name_out+".root", "RECREATE");


  ifstream weights_file("Reweighting/PDFweightIDs.txt", ifstream::in);
  string temp = "";
  vector<TString> PDFweights;
  vector<string> PDFnumbers;
  int iCounter = 0;
  while (getline(weights_file, temp) && iCounter < 101) {
    cout << temp << endl;
    if (temp == "") continue;
    TString weightPDF = "PDF_"+temp;
    PDFweights.push_back(weightPDF);
    PDFnumbers.push_back(to_string(iCounter));
    ++iCounter;
  }
  weights_file.close();



                                       //cross-sections in pb           
  vector<TString> signal;              vector<double> xs;	      
  signal.push_back("ZPrime_500");      xs.push_back(9.56     );       
  signal.push_back("ZPrime_600");      xs.push_back(5.03     );      
  signal.push_back("ZPrime_700");      xs.push_back(2.83     ); 
  signal.push_back("ZPrime_800");      xs.push_back(1.704    ); 
  signal.push_back("ZPrime_900");      xs.push_back(1.075    ); 
  signal.push_back("ZPrime_1000");     xs.push_back(0.7141   );  
  signal.push_back("ZPrime_1100");     xs.push_back(0.4775   );  
  signal.push_back("ZPrime_1200");     xs.push_back(0.329    ); 
  signal.push_back("ZPrime_1300");     xs.push_back(0.234    ); 
  signal.push_back("ZPrime_1400");     xs.push_back(0.1675   );  
  signal.push_back("ZPrime_1500");     xs.push_back(0.1226   );  
  signal.push_back("ZPrime_1600");     xs.push_back(0.09071  );  
  signal.push_back("ZPrime_1700");     xs.push_back(0.06808  );  
  signal.push_back("ZPrime_1800");     xs.push_back(0.05166  );  
  signal.push_back("ZPrime_1900");     xs.push_back(0.03912  );  
  signal.push_back("ZPrime_2000");     xs.push_back(0.03027  );  
  signal.push_back("ZPrime_2200");     xs.push_back(0.01847  );  
  signal.push_back("ZPrime_2400");     xs.push_back(0.01147  );  
  signal.push_back("ZPrime_2600");     xs.push_back(0.007258 );  
  signal.push_back("ZPrime_2800");     xs.push_back(0.004695 );  
  signal.push_back("ZPrime_3000");     xs.push_back(0.003079 );  
  signal.push_back("ZPrime_3500");     xs.push_back(0.001163 );  
  signal.push_back("ZPrime_4000");     xs.push_back(0.0004841);  
  signal.push_back("ZPrime_4500");     xs.push_back(0.0002196);  
  signal.push_back("ZPrime_5000");     xs.push_back(0.0001113);  
  signal.push_back("ZPrime_5500");     xs.push_back(6.238e-05); 	 
  signal.push_back("ZPrime_6000");     xs.push_back(3.896e-05); 	 
  				       
  //signal.push_back("QBH_200");         xs.push_back(24304.548    );
  //signal.push_back("QBH_400");         xs.push_back(1794.1462    );
  //signal.push_back("QBH_600");         xs.push_back(334.19232    );
  //signal.push_back("QBH_800");         xs.push_back(94.06898     );
  //signal.push_back("QBH_1000");        xs.push_back(33.19272     );
  //signal.push_back("QBH_1200");        xs.push_back(13.331938    );
  //signal.push_back("QBH_1400");        xs.push_back(5.958376     );
  //signal.push_back("QBH_1600");        xs.push_back(2.8127452    );
  //signal.push_back("QBH_1800");        xs.push_back(1.4207024    );
  //signal.push_back("QBH_2000");        xs.push_back(0.74266324   );
  //signal.push_back("QBH_2500");        xs.push_back(0.16715634   );
  //signal.push_back("QBH_3000");        xs.push_back(0.0423081    );
  //signal.push_back("QBH_3500");        xs.push_back(0.011538935  );
  //signal.push_back("QBH_4000");        xs.push_back(0.003249312  );
  //signal.push_back("QBH_4500");        xs.push_back(0.000944568  );
  //signal.push_back("QBH_5000");        xs.push_back(0.00027294006);
  ////signal.push_back("QBH_5500");        xs.push_back(7.84E-05     );
  //signal.push_back("QBH_6000");        xs.push_back(2.20E-05     );
  //signal.push_back("QBH_7000");        xs.push_back(1.59E-06     );
  ////signal.push_back("QBH_8000");        xs.push_back(0     );
  //signal.push_back("QBH_9000");        xs.push_back(4.49E-09     );
  //signal.push_back("QBH_10000");       xs.push_back(1.60E-10     );
  //
  //signal.push_back("RPV_l001_200");    xs.push_back(773.72);
  //signal.push_back("RPV_l001_300");    xs.push_back(212.55);
  //signal.push_back("RPV_l001_400");    xs.push_back(81.57);
  ////signal.push_back("RPV_l001_500");    xs.push_back(37.73);
  //signal.push_back("RPV_l001_600");    xs.push_back(19.66);
  //signal.push_back("RPV_l001_700");    xs.push_back(11.130);
  //signal.push_back("RPV_l001_800");    xs.push_back(6.6938);
  //signal.push_back("RPV_l001_900");    xs.push_back(4.2154);
  //signal.push_back("RPV_l001_1000");   xs.push_back(2.7521);
  //signal.push_back("RPV_l001_1200");   xs.push_back(1.27272);
  //signal.push_back("RPV_l001_1400");   xs.push_back(0.63690);
  //signal.push_back("RPV_l001_1600");   xs.push_back(0.3369363);
  //signal.push_back("RPV_l001_1800");   xs.push_back(0.185585);
  //signal.push_back("RPV_l001_2000");   xs.push_back(0.105317);
  //signal.push_back("RPV_l001_2500");   xs.push_back(2.7834e-02);
  //signal.push_back("RPV_l001_3000");   xs.push_back(7.879e-03);
  //signal.push_back("RPV_l001_3500");   xs.push_back(2.2935e-03);
  //signal.push_back("RPV_l01_3500");    xs.push_back(2.2935e-01);
  //signal.push_back("RPV_l001_4000");   xs.push_back(6.7061e-04);
  //signal.push_back("RPV_l01_4000");    xs.push_back(0.06704);
  //signal.push_back("RPV_l02_4000");    xs.push_back(0.2682463);
  //signal.push_back("RPV_l05_4000");    xs.push_back(1.);
  //signal.push_back("RPV_l001_4500");   xs.push_back(1.9445e-04);
  //signal.push_back("RPV_l02_4500");    xs.push_back(1.);
  //signal.push_back("RPV_l001_5000");   xs.push_back(5.5668e-05);
  //signal.push_back("RPV_l02_5000");    xs.push_back(1.);
  //signal.push_back("RPV_l001_5500");   xs.push_back(1.);
  //signal.push_back("RPV_l02_5500");    xs.push_back(1.);
  //signal.push_back("RPV_l02_6000");    xs.push_back(1.);
  //signal.push_back("RPV_l05_6000");    xs.push_back(1.);
  //signal.push_back("RPV_l001_6500");   xs.push_back(1.);
  //signal.push_back("RPV_l02_6500");    xs.push_back(1.);
  //signal.push_back("RPV_l05_6500");    xs.push_back(1.);


  vector<TFile*> files_in;
  for (unsigned int i=0; i<signal.size(); ++i) {
    files_in.push_back( new TFile(folder_in+"/Arranged_signal/"+signal[i]+".root", "R") );
  }

  
  vector<TString> PDFs;
  map<TString,TString> PDFnumbers_map;
  for (unsigned int iPDF=0; iPDF<PDFnumbers.size(); ++iPDF) {
    PDFs.push_back("PDF_"+PDFnumbers[iPDF]);
    PDFnumbers_map[PDFs[iPDF]] = PDFnumbers[iPDF];
  }
  

  TString var_in, var_out;

  vector<TString> orderedPDFs[signal.size()];
  for (unsigned int j = 0; j<signal.size(); ++j) {
    cout << signal[j] << endl;
    vector<TString> PDFs_temp;
    PDFs_temp = PDFs;
    while (PDFs_temp.size()) {
      double maxWeights = -1;
      int iMax = -1;
      vector<TString> rest;
      rest.clear();
      for (unsigned int i = 0; i<PDFs_temp.size(); ++i) {
	if (PDFs_temp[i].Contains("base")) continue;
	TH1F* h_SOWsys = (TH1F*) files_in[j]->Get("sumOfPDFWeights_"+PDFnumbers_map[PDFs_temp[i]]);
	double w_SOWsys = h_SOWsys->Integral();
	if (w_SOWsys < 0) w_SOWsys = 0;


	if (w_SOWsys >= maxWeights) {
	  maxWeights = w_SOWsys;
	  if (iMax >= 0) rest.push_back(PDFs_temp[iMax]);
	  iMax = i;
	}
	else {
	  rest.push_back(PDFs_temp[i]);
	}
      }
      if (maxWeights <= 0) {
	rest.clear();
      }
      else {
	orderedPDFs[j].push_back(PDFs_temp[iMax]);
      }
      PDFs_temp = rest;
    }
  }

  float xs_dummy = 1;



  fstream median_file;
  median_file.open("HighMassLFVMuTau/median_file.cc",ios::out);
  median_file << "using namespace std;" << endl << endl << endl;
  median_file << "map<TString, TString> MakeMedianMap() {" << endl << endl;
  median_file << "map<TString, TString> medianMap;" << endl;
  

  for (unsigned int j = 0; j<signal.size(); ++j) {
    int iMedian = floor(orderedPDFs[j].size()*0.5);
    cout << signal[j] << endl;
    cout << "iMedian: " << iMedian << endl;
    median_file << "medianMap[\"" << signal[j] << "\"] = \"" << orderedPDFs[j][iMedian] << "\";" << endl;
  }
  
  median_file << endl << "return medianMap;" << endl;
  median_file << "}" << endl << endl;


  median_file << "map<TString, TString> MakeUpMap() {" << endl << endl;
  median_file << "map<TString, TString> upMap;" << endl;
  

  for (unsigned int j = 0; j<signal.size(); ++j) {
    int iUp = floor(orderedPDFs[j].size()*0.16);  //var up == 16p% percentile
    cout << signal[j] << endl;
    cout << "iUp: " << iUp << endl;
    median_file << "upMap[\"" << signal[j] << "\"] = \"" << orderedPDFs[j][iUp] <<"\";" << endl;
  }
  
  median_file << endl << "return upMap;" << endl;
  median_file << "}" << endl << endl;


  median_file << "map<TString, TString> MakeDownMap() {" << endl << endl;
  median_file << "map<TString, TString> downMap;" << endl;
  

  for (unsigned int j = 0; j<signal.size(); ++j) {
    int iDown = floor(orderedPDFs[j].size()*0.84); //var down == 84% percentile
    cout << signal[j] << endl;
    cout << "iDown: " << iDown << endl;
    median_file << "downMap[\"" << signal[j] << "\"] = \"" << orderedPDFs[j][iDown] <<"\";" << endl;
  }
  
  median_file << endl << "return downMap;" << endl;
  median_file << "}" << endl;
  median_file.close();
  file_out->Close();



  return 0;
}
