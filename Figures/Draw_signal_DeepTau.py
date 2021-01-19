#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi(text):
    lowX=0.38
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.06)
    lumi.SetTextFont (   42 )
    lumi.AddText(text)
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
    output = ROOT.TLegend(0.7, 0.55, 0.92, 0.85, "", "brNDC")
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

file=ROOT.TFile("histos_massresolution.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

masspoints={}
masspoints["ZPrime"]=[]
masspoints["ZPrime"].append("ZPrime_600")    
masspoints["ZPrime"].append("ZPrime_1000") 
masspoints["ZPrime"].append("ZPrime_2000") 
masspoints["ZPrime"].append("ZPrime_3000") 
masspoints["ZPrime"].append("ZPrime_4000") 
masspoints["ZPrime"].append("ZPrime_5000") 

masspoints["RPV"]=[]
masspoints["RPV"].append("RPV_l001_600")    
masspoints["RPV"].append("RPV_l001_1000") 
masspoints["RPV"].append("RPV_l001_2000") 
masspoints["RPV"].append("RPV_l001_3000") 
masspoints["RPV"].append("RPV_l001_4000") 
masspoints["RPV"].append("RPV_l001_5000") 

masspoints["QBH"]=[]
masspoints["QBH"].append("QBH_600")    
masspoints["QBH"].append("QBH_1000") 
masspoints["QBH"].append("QBH_2000") 
masspoints["QBH"].append("QBH_3000") 
masspoints["QBH"].append("QBH_4000") 
masspoints["QBH"].append("QBH_5000") 

massvalues=[
"600",
"1000",
"2000",
"3000",
"4000",
"5000",
]


colors=[
"#ffccff",
"#de5a6a",
"#d89a6a",
"#9999cc",
"#ffcc66",
"#c338e2",
]

Mvis={}
Mtot={}
Mcol={}
oldMcol={}
Mvis_res={}
Mtot_res={}
Mcol_res={}
oldMcol_res={}
for key in masspoints.keys():
    Mvis[key]=[]
    Mtot[key]=[]
    Mcol[key]=[]
    oldMcol[key]=[]
    Mvis_res[key]=[]
    Mtot_res[key]=[]
    Mcol_res[key]=[]
    oldMcol_res[key]=[]
    for mass in masspoints[key]:
        Mvis[key].append( file.Get(mass+"_Mvis_wdisc") )
        Mtot[key].append( file.Get(mass+"_Mtot_wdisc") )
        Mcol[key].append( file.Get(mass+"_Mcol_wdisc") )
        oldMcol[key].append( file.Get(mass+"_oldMcol_wdisc") )
        Mvis_res[key].append( file.Get(mass+"_Mvis_res") )
        Mtot_res[key].append( file.Get(mass+"_Mtot_res") )
        Mcol_res[key].append( file.Get(mass+"_Mcol_res") )
        oldMcol_res[key].append( file.Get(mass+"_oldMcol_res") )


variables=[
Mvis,
Mtot,
Mcol,
oldMcol,
Mvis_res,
Mtot_res,
Mcol_res, 
oldMcol_res, 
]

ph_var=[
["m_{vis}", "Signal_Mvis"],
["m_{tot}", "Signal_Mtot"],
["m_{col}", "Signal_Mcol"],
["old m_{col}", "Signal_oldMcol"],
["#sigma (m_{vis})", "Signal_Mvis_resolution"],
["#sigma (m_{tot})", "Signal_Mtot_resolution"],
["#sigma (m_{col})", "Signal_Mcol_resolution"],
["#sigma (old m_{col})", "Signal_oldMcol_resolution"],
]

for iVar in range(0, len(variables)):
    var=variables[iVar]
    
    for key in var.keys():
        maxi=0
        for j in range(0,len(var[key])):
            var[key][j].Rebin(10)
            if var[key][j].GetMaximum()*1.4 > maxi: maxi = var[key][j].GetMaximum()*1.4
        for i in range(0,len(var[key])):
            var[key][i].GetXaxis().SetTitle("")
            var[key][i].GetXaxis().SetTitleSize(0.04)
            var[key][i].GetXaxis().SetTitleOffset(1.04)
            var[key][i].GetXaxis().SetRangeUser(0,6000)
            var[key][i].SetMaximum(maxi)
            print var[key][i].GetTitle(), var[key][i].GetMaximum()
            if iVar >= 4:
                var[key][i].GetXaxis().SetRangeUser(-1,1)
            else:
                var[key][i].GetXaxis().SetTitle("GeV")
            var[key][i].GetXaxis().SetNdivisions(505)
            var[key][i].GetYaxis().SetLabelFont(42)
            var[key][i].GetYaxis().SetLabelOffset(0.01)
            var[key][i].GetYaxis().SetLabelSize(0.04)
            var[key][i].GetYaxis().SetTitleSize(0.075)
            var[key][i].GetYaxis().SetTitleOffset(1.04)
            var[key][i].SetTitle("")
            #var[key][i].GetYaxis().SetTitle("Events")
            iCol = i % len(colors)
            var[key][i].SetLineColor(ROOT.TColor.GetColor(colors[iCol]))
            var[key][i].SetLineWidth(2)
    

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
        #pad1.SetLogy()
    
        var[key][0].SetMinimum(1)
        var[key][0].Draw("hist")
        for i in range(0, len(var[key])):
            var[key][i].Draw("histsame")

        legende=make_legend()
        legende.SetHeader(key)
        for i in range(0, len(var[key])):
            legende.AddEntry(var[key][i],massvalues[i]+" GeV","l")
        legende.Draw()

        if "ZPrime" in key:
            l1=add_lumi("2017 simulation")
        else:
            l1=add_lumi("2016 simulation")
        l1.Draw("same")
        l2=add_CMS()
        l2.Draw("same")
        l3=add_Preliminary()
        l3.Draw("same")
    
        pad1.RedrawAxis()

        finalstate  = ROOT.TLegend(0.21, 0.5, 0.43, 0.70+0.155)
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
        c.SaveAs(ph_var[iVar][1]+"_"+key+".pdf")


