#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi():
    lowX=0.58
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.06)
    lumi.SetTextFont (   42 )
    lumi.AddText("2016, 35.9 fb^{-1} (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.21
    lowY=0.70
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(61)
    lumi.SetTextSize(0.08)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("CMS")
    return lumi

def add_Preliminary():
    lowX=0.21
    lowY=0.63
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(52)
    lumi.SetTextSize(0.06)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("Preliminary")
    return lumi

def make_legend():
    output = ROOT.TLegend(0.7, 0.6, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetLineStyle(0)
    output.SetFillStyle(0)
    output.SetBorderSize(0)
    output.SetTextFont(62)
    return output

ROOT.gStyle.SetFrameLineWidth(3)
ROOT.gStyle.SetLineWidth(3)
ROOT.gStyle.SetOptStat(0)

c=ROOT.TCanvas("canvas","",0,0,600,600)
c.cd()

#file=ROOT.TFile("final.root","r")
file=ROOT.TFile("../HighMassLFVMuTau/fakerate.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

var=[]
var.append("hpass_jetpt_pass")          
len_tau = len(var)
var.append("FakeRate_jetpt_pass")


nvar=len(var)
print nvar, len_tau

photogenic_var=[]
photogenic_var.append("jet p_{T} (GeV)")
photogenic_var.append("jet p_{T} (GeV)")


dms = []
dms.append("DM0")
dms.append("DM1")
dms.append("DM10")


ptrange = []
ptrange.append("_tau_pt_30_50")
ptrange.append("_tau_pt_50_100")
ptrange.append("_tau_pt_100")


eta = []
eta.append("barrel")
eta.append("endcap")

ends = "_tauindiff"

for k in range (0,nvar):
    for l in range (0,len(dms)):
        if k<len_tau and l>0: break
        for m in range (0,len(eta)):
            for n in range (0,len(ptrange)):
                var_in_MC = ""
                var_in_data = ""
                var_in = ""
                if k<len_tau:
                    var_in_MC = var[k]+"_MC_total"+ends+ptrange[n]
                    var_in_data = var[k]+"_data_total"+ends+ptrange[n]
                    var_in = var[k]+"_total"+ends+ptrange[n]
                    if m>0: break
                else:
                    var_in_MC = var[k]+"_MC_"+dms[l]+"_"+eta[m]+ptrange[n]
                    var_in_data = var[k]+"_data_"+dms[l]+"_"+eta[m]+ptrange[n]
                    var_in = var[k]+"_"+dms[l]+"_"+eta[m]+ptrange[n]
                print var_in
                Data=file.Get(var_in_data)
                MC=file.Get(var_in_MC)
                
                Data.GetXaxis().SetTitle("")
                Data.GetXaxis().SetTitleSize(0)
                Data.GetXaxis().SetRangeUser(30,400)
                Data.GetXaxis().SetNdivisions(505)
                Data.GetYaxis().SetRangeUser(0,1)
                Data.GetYaxis().SetLabelFont(42)
                Data.GetYaxis().SetLabelOffset(0.01)
                Data.GetYaxis().SetLabelSize(0.06)
                Data.GetYaxis().SetTitleSize(0.075)
                Data.GetYaxis().SetTitleOffset(1.04)
                Data.SetTitle("")
                Data.GetYaxis().SetTitle("Fake Rate")
                Data.SetMarkerStyle(20)
                Data.SetMarkerSize(1)
                Data.SetMarkerColor(1)
                Data.SetLineColor(1)
                
                MC.SetMarkerStyle(22)
                MC.SetMarkerSize(1)
                MC.SetMarkerColor(2)
                MC.SetLineColor(2)
                
                errorBand = MC.Clone()
                errorBand.SetMarkerSize(0)
                errorBand.SetFillColor(new_idx)
                errorBand.SetFillStyle(3001)
                errorBand.SetLineWidth(1)
                
                pad1 = ROOT.TPad("pad1","pad1",0,0.35,1,1)
                pad1.Draw()
                pad1.cd()
                pad1.SetFillColor(0)
                pad1.SetBorderMode(0)
                pad1.SetBorderSize(10)
                pad1.SetTickx(1)
                pad1.SetTicky(1)
                #pad1.SetLogx()
                pad1.SetLeftMargin(0.18)
                pad1.SetRightMargin(0.05)
                pad1.SetTopMargin(0.122)
                pad1.SetBottomMargin(0.026)
                pad1.SetFrameFillStyle(0)
                pad1.SetFrameLineStyle(0)
                pad1.SetFrameLineWidth(3)
                pad1.SetFrameBorderMode(0)
                pad1.SetFrameBorderSize(10)
                
                Data.GetXaxis().SetLabelSize(0)
                Data.SetMaximum(Data.GetMaximum())#2.5)#FIXME
                Data.SetMinimum(0.0)
                Data.Draw("e")
                MC.Draw("esame")
                #errorBand.Draw("e2same")
                Data.Draw("esame")
                
                
                legende=make_legend()
                if k>=len_tau: legende.SetHeader(dms[l]+", "+eta[m])
                legende.AddEntry(Data,"Observed","elp")
                legende.AddEntry(MC,"Simulation","elp")
                legende.Draw()
                
                l1=add_lumi()
                l1.Draw("same")
                l2=add_CMS()
                l2.Draw("same")
                l3=add_Preliminary()
                l3.Draw("same")
                
                pad1.RedrawAxis()
                
                '''
                finalstate  = ROOT.TLegend(0.21, 0.52+0.013, 0.43, 0.70+0.155)
                finalstate.SetBorderSize(   0 )
                finalstate.SetFillStyle(    0 )
                finalstate.SetTextAlign(   12 )
                finalstate.SetTextSize ( 0.06 )
                finalstate.SetTextColor(    1 )
                #finalstate.SetTextFont (   41 )
                finalstate.SetHeader("#mu #tau")
                finalstate.Draw("same")
                
                categ  = ROOT.TPaveText(0.21, 0.45+0.013, 0.43, 0.65+0.155, "NDC")
                categ.SetBorderSize(   0 )
                categ.SetFillStyle(    0 )
                categ.SetTextAlign(   12 )
                categ.SetTextSize ( 0.06 )
                categ.SetTextColor(    1 )
                categ.SetTextFont (   41 )
                categ.AddText("OS iso #mu anti-iso #tau")
                #categ.AddText("Z#rightarrow#mu#mu CR")
                categ.Draw("same")
                '''
                
                c.cd()
                pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.35)
                pad2.SetTopMargin(0.05)
                pad2.SetBottomMargin(0.35)
                pad2.SetLeftMargin(0.18)
                pad2.SetRightMargin(0.05)
                pad2.SetTickx(1)
                pad2.SetTicky(1)
                #pad2.SetLogx()
                pad2.SetFrameLineWidth(3)
                pad2.SetGridx()
                pad2.SetGridy()
                pad2.Draw()
                pad2.cd()
                h1=Data.Clone()
                h1.SetMaximum(1.7)#FIXME(1.5)
                h1.SetMinimum(0.5)#FIXME(0.5)
                h1.SetMarkerStyle(20)
                h3=errorBand.Clone()
                hwoE=errorBand.Clone()
                for iii in range (1,hwoE.GetSize()-2):
                    hwoE.SetBinError(iii,0)
                h3.Sumw2()
                h1.Sumw2()
                h1.SetStats(0)
                h1.Divide(hwoE)
                h3.Divide(hwoE)
                h1.GetXaxis().SetTitle(photogenic_var[k])
                h1.GetXaxis().SetLabelSize(0.08)
                h1.GetYaxis().SetLabelSize(0.08)
                h1.GetYaxis().SetTitle("Obs./Sim.")
                h1.GetXaxis().SetNdivisions(505)
                h1.GetYaxis().SetNdivisions(10)
                
                h1.GetXaxis().SetTitleSize(0.15)
                h1.GetYaxis().SetTitleSize(0.15)
                h1.GetYaxis().SetTitleOffset(0.56)
                h1.GetXaxis().SetTitleOffset(1.04)
                h1.GetXaxis().SetLabelSize(0.11)
                h1.GetYaxis().SetLabelSize(0.11)
                h1.GetXaxis().SetTitleFont(42)
                h1.GetYaxis().SetTitleFont(42)
                
                h1.Draw("ep")
                h3.Draw("e2same")
                
                c.cd()
                pad1.Draw()
                
                ROOT.gPad.RedrawAxis()
                
                c.Modified()
                c.SaveAs(var_in+".png")


