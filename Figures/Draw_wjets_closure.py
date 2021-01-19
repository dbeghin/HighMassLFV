#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi():
    lowX=0.5
    lowY=0.835
    lumi  = ROOT.TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC")
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.SetTextSize(0.045)
    lumi.SetTextFont (   42 )
    lumi.AddText("2016 simulation (13 TeV)")
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
    lumi.SetTextSize(0.045)
    lumi.SetBorderSize(   0 )
    lumi.SetFillStyle(    0 )
    lumi.SetTextAlign(   12 )
    lumi.SetTextColor(    1 )
    lumi.AddText("Preliminary")
    return lumi

def make_legend():
    output = ROOT.TLegend(0.45, 0.65, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetLineStyle(0)
    output.SetTextSize(0.037)
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
file_wjets=ROOT.TFile("histos_wjets.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)

#categories=["passOS","failOS"] 
#ncat=2

var=[]
var.append("ev_Mvis")      
var.append("ev_Mtot")      
var.append("tau_pt")       
var.append("tau_eta")      
var.append("tau_phi")      
var.append("mu_pt")        
var.append("mu_eta")       
var.append("mu_phi")       
var.append("mu_isolation") 
var.append("ev_DRmutau")   
var.append("ev_MET")       
var.append("ev_Mcol")
var.append("ev_Mt")
var.append("sign") 

var_log_dic = {
"ev_Mvis"          : True,
"ev_Mtot"          : True,
"tau_pt"           : True,
"tau_eta"          : False,
"tau_phi"          : False,
"mu_pt"            : True,
"mu_eta"           : False,
"mu_phi"           : False,
"mu_isolation"     : True,
"ev_DRmutau"       : False,
"ev_DeltaPhimutau" : False,
"ev_DeltaPhiMETtau": False,
"ev_MET"           : True,
"ev_Mcol"          : True,
"ev_Mt"            : True,
"sign"             : False,
}

nvar=len(var)
print nvar


photogenic_var={
"ev_Mvis":              "m_{vis} (GeV)",
"ev_Mtot":              "m_{tot} (GeV)",
"tau_pt":               "#tau p_{T} (GeV)",
"tau_eta":              "#tau #eta",
"tau_phi":              "#tau #phi",
"mu_pt":                "#mu p_{T} (GeV)",
"mu_eta":               "#mu #eta",
"mu_phi":               "#mu #phi",
"mu_isolation":         "#mu iso",
"ev_DRmutau":           "#DeltaR (#mu #tau)",
"ev_DeltaPhimutau":     "#Delta#Phi (#mu #tau)",
"ev_DeltaPhiMETtau":    "#Delta#Phi (E_{T}^{miss} #tau)",
"ev_MET":               "E_{T}^{miss} (GeV)",
"ev_Mcol":              "m_{col}",
"ev_Mt":                "m_{T}",
"sign":                 "OS (left) versus SS (right)",
}



for k in range (0,nvar):
    var_in_CR100 = "nominal/WJets_signalregion_"+var[k]
    var_in_CR101 = "nominal/WJets_fakerate_"+var[k]
    print var_in_CR100

    h_CR100=file_wjets.Get(var_in_CR100)
    h_CR101=file_wjets.Get(var_in_CR101)
            
    h_CR100.GetXaxis().SetTitle(photogenic_var[var[k]])
    h_CR100.GetXaxis().SetTitleSize(0.06)
    h_CR100.GetXaxis().SetLabelSize(0.05)
    if var_log_dic[var[k]]:
        h_CR100.GetXaxis().SetRangeUser(0,500)
        if "Mcol" in var[k]: h_CR100.GetXaxis().SetRangeUser(0,500)
    h_CR100.GetXaxis().SetNdivisions(505)
    h_CR100.GetYaxis().SetLabelFont(42)
    h_CR100.GetYaxis().SetLabelOffset(0.01)
    h_CR100.GetYaxis().SetLabelSize(0.05)
    h_CR100.GetYaxis().SetTitleSize(0.06)
    h_CR100.GetYaxis().SetTitleOffset(1.64)
    h_CR100.SetTitle("")
    h_CR100.GetYaxis().SetTitle("Events/bin")
    h_CR100.SetMarkerStyle(20)
    h_CR100.SetMarkerSize(1)
    h_CR100.SetMarkerColor(2)
    h_CR100.SetLineColor(2)
            
    h_CR101.GetXaxis().SetTitle("")
    h_CR101.GetXaxis().SetTitleSize(0)
    if var_log_dic[var[k]]:
        h_CR101.GetXaxis().SetRangeUser(0,500)
    h_CR101.GetXaxis().SetNdivisions(505)
    h_CR101.GetYaxis().SetLabelFont(42)
    h_CR101.GetYaxis().SetLabelOffset(0.01)
    h_CR101.GetYaxis().SetLabelSize(0.06)
    h_CR101.GetYaxis().SetTitleSize(0.075)
    h_CR101.GetYaxis().SetTitleOffset(1.64)
    h_CR101.SetTitle("")
    h_CR101.GetYaxis().SetTitle("Events/bin")
    h_CR101.SetMarkerStyle(20)
    h_CR101.SetMarkerSize(1)
    h_CR101.SetMarkerColor(8)
    h_CR101.SetLineColor(8)
            
            
    pad1 = ROOT.TPad("pad1","pad1",0,0,1,1)
    pad1.Draw()
    pad1.cd()
    pad1.SetFillColor(0)
    pad1.SetBorderMode(0)
    pad1.SetBorderSize(10)
    pad1.SetTickx(1)
    pad1.SetTicky(1)
    if var_log_dic[var[k]]:
        pad1.SetLogy()
        h_CR100.SetMaximum(h_CR100.GetMaximum()*10**2)
    else:
        h_CR100.SetMaximum(h_CR100.GetMaximum()*2)
    pad1.SetLeftMargin(0.18)
    pad1.SetRightMargin(0.05)
    pad1.SetTopMargin(0.122)
    pad1.SetBottomMargin(0.15)
    pad1.SetFrameFillStyle(0)
    pad1.SetFrameLineStyle(0)
    pad1.SetFrameLineWidth(3)
    pad1.SetFrameBorderMode(0)
    pad1.SetFrameBorderSize(10)
            
    h_CR100.Draw("e")
    h_CR101.Draw("esame")
            
            
    legende=make_legend()
    legende.SetHeader("W+jets simulation")
    legende.AddEntry(h_CR101,"V.V.Loose WP*FF","ep")
    legende.AddEntry(h_CR100,"Tight WP","ep")
    legende.Draw()
            
    l1=add_lumi()
    l1.Draw("same")
    l2=add_CMS()
    l2.Draw("same")
    l3=add_Preliminary()
    l3.Draw("same")
            
    pad1.RedrawAxis()
            
                    
    c.cd()
    pad1.Draw()
            
    ROOT.gPad.RedrawAxis()
    
    c.Modified()
    c.SaveAs("WJetsClosure_"+var[k]+".pdf")


