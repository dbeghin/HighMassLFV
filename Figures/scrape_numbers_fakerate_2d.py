#!/usr/bin/env python
import ROOT
import re
from array import array


file=ROOT.TFile("../HighMassLFVMuTau/fakerate_unfactorised_MtLow.root","r")


var=[]
var.append("FakeRateByTauPtAndRatio")


ends = []
ends.append("total")     
ends.append("eta_barrel")
ends.append("eta_endcap")
#ends.append("DM0_barrel")
#ends.append("DM0_endcap")
#ends.append("DM1_barrel")
#ends.append("DM1_endcap")
#ends.append("DM10_barrel")
#ends.append("DM10_endcap")

ph_ends = {
"total"       : "Inclusive",
"eta_barrel"  : "Barrel",
"eta_endcap"  : "Endcap",
"DM0_barrel"  : "Barrel - DM0",
"DM0_endcap"  : "Endcap - DM0",
"DM1_barrel"  : "Barrel - DM1",
"DM1_endcap"  : "Endcap - DM1",
"DM10_barrel" : "Barrel - DM10",
"DM10_endcap" : "Endcap - DM10",
}


ptrange = []
ptrange.append("taupt_0_150")
ptrange.append("taupt_150_1000")

file_out=open("table.tex", "w")
file_out.write("\\tauh \\pt & \\tauh over jet \\pt ratio & fake factor (barrel) & fake factor (endcap) \\\\\n")

for k in range (0,len(var)):
    for p in range (0,len(ptrange)):
        var_in_barrel = var[k]+"_"+ends[1]+"_"+ptrange[p]
        var_in_endcap = var[k]+"_"+ends[2]+"_"+ptrange[p]
        Data=file.Get(var_in_barrel)
        Data_e=file.Get(var_in_endcap)
            
        for iBinX in range(1, Data.GetNbinsX()+1):
            for iBinY in range(1, Data.GetNbinsY()+1):
                fakeRate_b=round(Data.GetBinContent(iBinX, iBinY),2)
                if (fakeRate_b == 0): continue
                error_b=round(Data.GetBinError(iBinX, iBinY),2)
                fakeRate_e=round(Data_e.GetBinContent(iBinX, iBinY),2)
                error_e=round(Data_e.GetBinError(iBinX, iBinY),2)

                pt_lowEdge=int(Data.GetXaxis().GetBinLowEdge(iBinX))
                pt_upEdge=int(Data.GetXaxis().GetBinUpEdge(iBinX))
                ratio_lowEdge=round(Data.GetYaxis().GetBinLowEdge(iBinY),1)
                ratio_upEdge=round(Data.GetYaxis().GetBinUpEdge(iBinY),1)

                file_out.write("%s-%s & %s-%s & %s $\\pm$ %s & %s $\\pm$ %s \\\\\n" %(pt_lowEdge, pt_upEdge, ratio_lowEdge, ratio_upEdge, fakeRate_b, error_b, fakeRate_e, error_e ) )
                


file_out.close()
