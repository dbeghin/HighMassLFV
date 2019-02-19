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
    output = ROOT.TLegend(0.6, 0.55, 0.92, 0.82, "", "brNDC")
    output.SetTextSize(0.045)
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
file_total=ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow.root","r")
file_other=[
ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow_30_50.root","r"),
ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow_50_70.root","r"),
ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow_70_100.root","r"),
ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow_100_150.root","r"),
ROOT.TFile("../HighMassLFVMuTau/fakerate_MtLow_150_1000.root","r"),
]

ptrange=[
"Pt 30-50 GeV",
"Pt 50-70 GeV",
"Pt 70-100 GeV",
"Pt 100-150 GeV",
"Pt 150-1000 GeV",
]

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

var=[]
var.append("RatioCorrectionFactor_")
nvar=len(var)
print nvar

photogenic_var=[]
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
        var_in = var[k]+ends[l]
        print var_in

        total=file_total.Get(var_in)
        total.GetXaxis().SetTitle("")
        total.GetXaxis().SetTitleSize(0)
        total.GetXaxis().SetRangeUser(30,400)
        total.GetXaxis().SetNdivisions(505)
        total.GetYaxis().SetRangeUser(0,1)
        total.GetYaxis().SetLabelFont(42)
        total.GetYaxis().SetLabelOffset(0.01)
        total.GetYaxis().SetLabelSize(0.06)
        total.GetYaxis().SetTitleSize(0.075)
        total.GetYaxis().SetTitleOffset(1.04)
        total.SetTitle("")
        total.GetYaxis().SetTitle("Correction factor")
        total.SetMarkerStyle(20)
        total.SetMarkerSize(1)
        total.SetMarkerColor(1)
        total.SetLineColor(1)
                
        histo_other=[]
        for m in range(0,len(file_other)):
            histo_other.append( file_other[m].Get(var_in) );
            histo_other[m].GetXaxis().SetTitle("")
            histo_other[m].GetXaxis().SetTitleSize(0)
            histo_other[m].GetXaxis().SetRangeUser(30,400)
            histo_other[m].GetXaxis().SetNdivisions(505)
            histo_other[m].GetYaxis().SetRangeUser(0,1)
            histo_other[m].GetYaxis().SetLabelFont(42)
            histo_other[m].GetYaxis().SetLabelOffset(0.01)
            histo_other[m].GetYaxis().SetLabelSize(0.06)
            histo_other[m].GetYaxis().SetTitleSize(0.075)
            histo_other[m].GetYaxis().SetTitleOffset(1.04)
            histo_other[m].SetTitle("")
            histo_other[m].GetYaxis().SetTitle("Correction factor")
            histo_other[m].SetMarkerStyle(20)
            histo_other[m].SetMarkerSize(1)
            
            ccolor = m+2
            if ccolor == 5: ccolor = 7
            histo_other[m].SetMarkerColor(ccolor)
            histo_other[m].SetLineColor(ccolor)
                

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
                
        total.GetXaxis().SetLabelSize(0)
        total.SetMaximum(4.0)#FIXME
        total.SetMinimum(0.0)
        total.Draw("e")
        for m in range(0, len(histo_other)):
            histo_other[m].Draw("esame")
                
                
        legende=make_legend()
        legende.SetHeader(ends_map[ends[l]])
        legende.AddEntry(total,"Full pt range","elp")
        for m in range(0, len(histo_other)):
            legende.AddEntry(histo_other[m], ptrange[m], "elp")
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

        h0=histo_other[0].Clone()
        h0.SetMaximum(1.8)#FIXME(1.5)
        h0.SetMinimum(0.2)#FIXME(0.5)
        h0.SetMarkerStyle(20)
        h0.SetStats(0)

        h3=total.Clone()

        h0.GetXaxis().SetTitle(photogenic_var[k])
        h0.GetYaxis().SetTitle("Ratio to full p_{T} range")
        h0.GetXaxis().SetNdivisions(505)
        h0.GetYaxis().SetNdivisions(8)
                
        h0.GetXaxis().SetTitleSize(0.15)
        h0.GetYaxis().SetTitleSize(0.1)
        h0.GetYaxis().SetTitleOffset(0.7)
        h0.GetXaxis().SetTitleOffset(1.04)
        h0.GetXaxis().SetLabelSize(0.11)
        h0.GetYaxis().SetLabelSize(0.1)
        h0.GetXaxis().SetTitleFont(42)
        h0.GetYaxis().SetTitleFont(42)
                
        h0.Divide(h3)

        h0.Draw("ep")
        histo_div=[]
        histo_div.append(h0)
        for m in range(1, len(histo_other)):
            histo_div.append(histo_other[m].Clone())
            histo_div[m].Divide(h3)
            histo_div[m].Draw("epsame")
                
        c.cd()
        pad1.Draw()
                
        ROOT.gPad.RedrawAxis()
        
        c.Modified()
        c.SaveAs(var_in+".png")


