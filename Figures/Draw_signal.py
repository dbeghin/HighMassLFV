#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi():
    lowX=0.38
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
    output = ROOT.TLegend(0.5, 0.5, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetLineStyle(0)
    output.SetFillStyle(0)
    output.SetBorderSize(0)
    output.SetTextFont(62)
    output.SetTextSize(0.037)
    return output

ROOT.gStyle.SetFrameLineWidth(3)
ROOT.gStyle.SetLineWidth(3)
ROOT.gStyle.SetOptStat(0)

c=ROOT.TCanvas("canvas","",0,0,600,600)
c.cd()

file=ROOT.TFile("histos_signal.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

masspoints=[]
masspoints.append("500")    
masspoints.append("600")  
masspoints.append("700")  
masspoints.append("800")  
masspoints.append("900")  
masspoints.append("1000") 
masspoints.append("1100") 
masspoints.append("1200") 
masspoints.append("1300") 
masspoints.append("1400") 
masspoints.append("1500") 
masspoints.append("1600") 
masspoints.append("1700") 
masspoints.append("1800") 
masspoints.append("1900") 
masspoints.append("2000") 
masspoints.append("2200") 
masspoints.append("2400") 
masspoints.append("2600") 
masspoints.append("2800") 
masspoints.append("3000") 
masspoints.append("3500") 
masspoints.append("4000") 
masspoints.append("4500") 
masspoints.append("5000")

Mvis=[]
Mtot=[]
Mcol=[]
colors=[
"#ffccff",
"#de5a6a",
"#d89a6a",
"#9999cc",
"#ffcc66",
"#c338e2",
]

for mass in masspoints:
    Mvis.append( file.Get(mass+"_ev_Mvis_realtau_MtHigh") )
    Mtot.append( file.Get(mass+"_ev_Mtot_realtau_MtHigh") )
    Mcol.append( file.Get(mass+"_ev_Mcol_realtau_MtHigh") )


variables=[
Mvis,
Mtot,
Mcol
]

ph_var=[
["m_{vis}", "Signal_Mvis"],
["m_{tot}", "Signal_Mtot"],
["m_{col}", "Signal_Mcol"],
]

for iVar in range(0, len(variables)):
    var=variables[iVar]
    for i in range(0, len(var)):
        var[i].GetXaxis().SetTitle("")
        var[i].GetXaxis().SetTitleSize(0.06)
        var[i].GetXaxis().SetRangeUser(0,4000)
        var[i].GetXaxis().SetNdivisions(505)
        var[i].GetYaxis().SetLabelFont(42)
        var[i].GetYaxis().SetLabelOffset(0.01)
        var[i].GetYaxis().SetLabelSize(0.06)
        var[i].GetYaxis().SetTitleSize(0.075)
        var[i].GetYaxis().SetTitleOffset(1.04)
        var[i].SetMaximum(var[i].GetMaximum()*1000)
        var[i].SetTitle("")
        var[i].GetYaxis().SetTitle("Events")
        iCol = i % len(colors)
        var[i].SetLineColor(ROOT.TColor.GetColor(colors[iCol]))
        var[i].SetLineWidth(2)
    

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
    pad1.SetLogy()
    
    var[0].SetMinimum(1)
    var[0].Draw("hist")
    for i in range(0, len(var)):
        var[i].Draw("histsame")

    legende=make_legend()
    for i in range(0, len(var)):
        legende.AddEntry(var[i],"RPV mass "+masspoints[i]+" GeV","l")
    legende.Draw()

    l1=add_lumi()
    l1.Draw("same")
    l2=add_CMS()
    l2.Draw("same")
    l3=add_Preliminary()
    l3.Draw("same")
    
    pad1.RedrawAxis()

    finalstate  = ROOT.TLegend(0.21, 0.52+0.013, 0.43, 0.70+0.155)
    finalstate.SetBorderSize(   0 )
    finalstate.SetFillStyle(    0 )
    finalstate.SetTextAlign(   12 )
    finalstate.SetTextSize ( 0.06 )
    finalstate.SetTextColor(    1 )
    #finalstate.SetTextFont (   41 )
    finalstate.SetHeader(ph_var[iVar][0])
    finalstate.Draw("same")

    '''
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
    c.SaveAs(ph_var[iVar][1]+".png")


