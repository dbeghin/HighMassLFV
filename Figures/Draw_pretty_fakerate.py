#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi():
    lowX=0.48
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.055)
    lumi.SetTextFont (   42 )
    lumi.AddText("2016, 35.9 fb^{-1} (13 TeV)")
    return lumi

def add_CMS():
    lowX=0.21
    lowY=0.70
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.15, lowY+0.16, "NDC")
    lumi.SetTextFont(61)
    lumi.SetTextSize(0.06)
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
    lumi.SetTextSize(0.05)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("Preliminary")
    return lumi

def make_legend():
    output = ROOT.TLegend(0.65, 0.7, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetTextSize(0.03)
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
#file=ROOT.TFile("../HighMassLFVMuTau/fakerate.root","r")
file=ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

var=[]
var.append("FakeRateByTauPt")
var.append("RatioCorrectionFactor")


nvar=len(var)
print nvar

photogenic_var=[]
photogenic_var.append("tau p_{T} (GeV)")
photogenic_var.append("tau p_{T} / jet p_{T} ratio")

photogenic_var2=[]
photogenic_var2.append("Fake Rate")
photogenic_var2.append("Correction Factor")


ends = []
ends.append("total")     
ends.append("eta_barrel")
ends.append("eta_endcap")
ends.append("DM0_barrel")
ends.append("DM0_endcap")
ends.append("DM1_barrel")
ends.append("DM1_endcap")
ends.append("DM10_barrel")
ends.append("DM10_endcap")

ph_ends = {
"total"       : "Inclusive",
"eta_barrel"  : "Barrel - All DMs",
"eta_endcap"  : "Endcap - All DMs",
"DM0_barrel"  : "Barrel - DM0",
"DM0_endcap"  : "Endcap - DM0",
"DM1_barrel"  : "Barrel - DM1",
"DM1_endcap"  : "Endcap - DM1",
"DM10_barrel" : "Barrel - DM10",
"DM10_endcap" : "Endcap - DM10",
}



for k in range (0,nvar):
    for l in range (0,len(ends)):
        var_in = var[k]+"_"+ends[l]
        print var_in
        Data=file.Get(var_in)
                
        Data.GetXaxis().SetRangeUser(30,400)
        Data.GetXaxis().SetNdivisions(505)
        if (k==0):
            Data.GetYaxis().SetRangeUser(0,1)
        else:
            Data.GetYaxis().SetRangeUser(0,3)
        Data.GetYaxis().SetLabelFont(42)
        Data.GetYaxis().SetLabelOffset(0.01)
        Data.GetYaxis().SetLabelSize(0.05)
        Data.GetXaxis().SetLabelSize(0.05)
        Data.GetYaxis().SetTitleSize(0.05)
        Data.GetXaxis().SetTitleSize(0.05)
        Data.GetYaxis().SetTitleOffset(1.32)
        #Data.GetXaxis().SetTitleOffset(1.04)
        Data.SetTitle("")
        Data.GetYaxis().SetTitle(photogenic_var2[k])
        Data.GetXaxis().SetTitle(photogenic_var[k])
        Data.SetMarkerStyle(20)
        Data.SetMarkerSize(1)
        Data.SetMarkerColor(1)
        Data.SetLineColor(1)
                
                
        pad1 = ROOT.TPad("pad1","pad1",0,0,1,1)
        pad1.Draw()
        pad1.cd()
        pad1.SetFillColor(0)
        pad1.SetBorderMode(0)
        pad1.SetBorderSize(10)
        pad1.SetTickx(1)
        pad1.SetTicky(1)
        pad1.SetLeftMargin(0.18)
        pad1.SetRightMargin(0.05)
        pad1.SetTopMargin(0.122)
        pad1.SetBottomMargin(0.126)
        pad1.SetFrameFillStyle(0)
        pad1.SetFrameLineStyle(0)
        pad1.SetFrameLineWidth(3)
        pad1.SetFrameBorderMode(0)
        pad1.SetFrameBorderSize(10)
                
        Data.SetMaximum(Data.GetMaximum())#2.5)#FIXME
        Data.SetMinimum(0.0)
        Data.Draw("e")
                
                
        legende=make_legend()
        legende.SetHeader(ph_ends[ends[l]])
        legende.AddEntry(Data,"Observed","p")
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
        pad1.Draw()
                
        ROOT.gPad.RedrawAxis()
        
        c.Modified()
        c.SaveAs(var_in+".png")


