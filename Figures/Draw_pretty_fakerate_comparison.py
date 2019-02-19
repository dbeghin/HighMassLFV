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
    output = ROOT.TLegend(0.6, 0.6, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetLineStyle(0)
    output.SetTextSize(0.045)
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
file_OS=ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLowOS.root","r")
file_SS=ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLowSS.root","r")
file_DY=ROOT.TFile("../HighMassLFVMuTau/fakerate_DY.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

var=[]
var.append("FakeRateByTauPt_")
var.append("RatioCorrectionFactor_")
nvar=len(var)
print nvar

photogenic_var=[]
photogenic_var.append("tau p_{T} (GeV)")
photogenic_var.append("tau p_{T}/jet p_{T}")

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

ends_map = {
"total":         "Inclusive",
"eta_barrel":    "Barrel - All DMs",
"eta_endcap":    "Endcap - All DMs",
"DM0_barrel":    "Barrel - DM0",
"DM0_endcap":    "Endcap - DM0",
"DM1_barrel":    "Barrel - DM1",
"DM1_endcap":    "Endcap - DM1",
"DM10_barrel":   "Barrel - DM10",
"DM10_endcap":   "Endcap - DM10",
}


for k in range (0,nvar):
    for l in range (0,len(ends)):
        var_in = var[k]+ends[l]
        print var_in

        OS=file_OS.Get(var_in)
        SS=file_SS.Get(var_in)
        DY=file_DY.Get(var_in)
                
        OS.GetXaxis().SetTitle("")
        OS.GetXaxis().SetTitleSize(0)
        OS.GetXaxis().SetRangeUser(30,400)
        OS.GetXaxis().SetNdivisions(505)
        OS.GetYaxis().SetRangeUser(0,1)
        OS.GetYaxis().SetLabelFont(42)
        OS.GetYaxis().SetLabelOffset(0.01)
        OS.GetYaxis().SetLabelSize(0.06)
        OS.GetYaxis().SetTitleSize(0.075)
        OS.GetYaxis().SetTitleOffset(1.04)
        OS.SetTitle("")
        OS.GetYaxis().SetTitle("Fake Rate")
        OS.SetMarkerStyle(20)
        OS.SetMarkerSize(1)
        OS.SetMarkerColor(1)
        OS.SetLineColor(1)
                
        SS.GetXaxis().SetTitle("")
        SS.GetXaxis().SetTitleSize(0)
        SS.GetXaxis().SetRangeUser(30,400)
        SS.GetXaxis().SetNdivisions(505)
        SS.GetYaxis().SetRangeUser(0,1)
        SS.GetYaxis().SetLabelFont(42)
        SS.GetYaxis().SetLabelOffset(0.01)
        SS.GetYaxis().SetLabelSize(0.06)
        SS.GetYaxis().SetTitleSize(0.075)
        SS.GetYaxis().SetTitleOffset(1.04)
        SS.SetTitle("")
        SS.GetYaxis().SetTitle("Fake Rate")
        SS.SetMarkerStyle(20)
        SS.SetMarkerSize(1)
        SS.SetMarkerColor(2)
        SS.SetLineColor(2)
                
        DY.GetXaxis().SetTitle("")
        DY.GetXaxis().SetTitleSize(0)
        DY.GetXaxis().SetRangeUser(30,400)
        DY.GetXaxis().SetNdivisions(505)
        DY.GetYaxis().SetRangeUser(0,1)
        DY.GetYaxis().SetLabelFont(42)
        DY.GetYaxis().SetLabelOffset(0.01)
        DY.GetYaxis().SetLabelSize(0.06)
        DY.GetYaxis().SetTitleSize(0.075)
        DY.GetYaxis().SetTitleOffset(1.04)
        DY.SetTitle("")
        DY.GetYaxis().SetTitle("Fake Rate")
        DY.SetMarkerStyle(20)
        DY.SetMarkerSize(1)
        DY.SetMarkerColor(6)
        DY.SetLineColor(6)
                
                
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
                
        OS.GetXaxis().SetLabelSize(0)
        OS.SetMaximum(1)#FIXME
        if k == 1:
            OS.SetMaximum(3)
        OS.SetMinimum(0.0)
        OS.Draw("e")
        SS.Draw("esame")
        DY.Draw("esame")
                
                
        legende=make_legend()
        legende.SetHeader(ends_map[ends[l]])
        legende.AddEntry(OS,"m_{T} < 120 GeV - OS","p")
        legende.AddEntry(SS,"m_{T} < 120 GeV - SS","p")
        legende.AddEntry(DY,"DY control region","p")
        legende.Draw()
                
        l1=add_lumi()
        l1.Draw("same")
        l2=add_CMS()
        l2.Draw("same")
        l3=add_Preliminary()
        l3.Draw("same")
                
        pad1.RedrawAxis()
                
                        
        c.cd()
        pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.35)
        pad2.SetTopMargin(0.05)
        pad2.SetBottomMargin(0.35)
        pad2.SetLeftMargin(0.18)
        pad2.SetRightMargin(0.05)
        pad2.SetTickx(1)
        pad2.SetTicky(1)
        pad2.SetFrameLineWidth(3)
        pad2.SetGridx()
        pad2.SetGridy()
        pad2.Draw()
        pad2.cd()

        h0=OS.Clone()
        h1=SS.Clone()
        h0.SetMaximum(1.8)#FIXME(1.5)
        h0.SetMinimum(0.2)#FIXME(0.5)

        h0.SetMarkerStyle(20)
        h1.SetMarkerStyle(20)
        h3=DY.Clone()

        h3.Sumw2()
        h0.Sumw2()
        h0.SetStats(0)
        h1.Sumw2()

        h0.Divide(h3)
        h1.Divide(h3)

        h0.GetXaxis().SetTitle(photogenic_var[k])
        h0.GetYaxis().SetTitle("Ratio to DY")
        h0.GetXaxis().SetNdivisions(505)
        h0.GetYaxis().SetNdivisions(8)
                
        h0.GetXaxis().SetTitleSize(0.15)
        h0.GetYaxis().SetTitleSize(0.15)
        h0.GetYaxis().SetTitleOffset(0.56)
        h0.GetXaxis().SetTitleOffset(1.04)
        h0.GetXaxis().SetLabelSize(0.11)
        h0.GetYaxis().SetLabelSize(0.1)
        h0.GetXaxis().SetTitleFont(42)
        h0.GetYaxis().SetTitleFont(42)
                
        h0.Draw("ep")
        h1.Draw("esame")
                
        c.cd()
        pad1.Draw()
                
        ROOT.gPad.RedrawAxis()
        
        c.Modified()
        c.SaveAs(var_in+".png")


