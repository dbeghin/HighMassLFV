
#!/usr/bin/env python
import ROOT
import re
from array import array

def add_lumi():
    lowX=0.58
    #lowX=0.38
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
    output = ROOT.TLegend(0.65, 0.4, 0.92, 0.82, "", "brNDC")
    output.SetLineWidth(0)
    output.SetLineStyle(0)
    output.SetFillStyle(0)
    output.SetBorderSize(0)
    output.SetTextFont(62)
    return output


def make_plot(var_out, Data, DY, TT, VV, ST, Faketau, sq_combined_error):
    Data.GetXaxis().SetTitle("")
    Data.GetXaxis().SetTitleSize(0)
    #if var_in == "ev_Mcol": Data.GetXaxis().SetRangeUser(0,500) 
    Data.GetXaxis().SetNdivisions(505)
    Data.GetYaxis().SetLabelFont(42)
    Data.GetYaxis().SetLabelOffset(0.01)
    Data.GetYaxis().SetLabelSize(0.06)
    Data.GetYaxis().SetTitleSize(0.075)
    Data.GetYaxis().SetTitleOffset(1.04)
    Data.SetTitle("")
    if var_log_dic[var[k]]:
        Data.GetYaxis().SetTitle("Events/GeV")
    else:
        Data.GetYaxis().SetTitle("Events/bin")

    print Data.GetBinContent(5)
    
    #ST.GetXaxis().SetTitle("")
    ST.GetXaxis().SetTitleSize(0.06)
    ST.GetXaxis().SetTitle(photogenic_var[var[k]])
    ST.GetXaxis().SetNdivisions(505)
    ST.GetYaxis().SetLabelFont(42)
    ST.GetYaxis().SetLabelOffset(0.01)
    ST.GetYaxis().SetLabelSize(0.06)
    ST.GetYaxis().SetTitleSize(0.075)
    ST.GetYaxis().SetTitleOffset(1.04)
    ST.SetTitle("")
    ST.GetYaxis().SetTitle("Events/bin")
    
    
    
    #QCD.SetFillColor(ROOT.TColor.GetColor("#ffccff"))
    #W.SetFillColor(ROOT.TColor.GetColor("#de5a6a"))
    VV.SetFillColor(ROOT.TColor.GetColor("#d89a6a"))
    TT.SetFillColor(ROOT.TColor.GetColor("#9999cc"))
    DY.SetFillColor(ROOT.TColor.GetColor("#ffcc66"))
    ST.SetFillColor(ROOT.TColor.GetColor("#c338e2"))
    Faketau.SetFillColor(ROOT.TColor.GetColor("#de5a6a"))
    
    Data.SetMarkerStyle(20)
    Data.SetMarkerSize(1)
    #QCD.SetLineColor(1)
    #W.SetLineColor(1)
    VV.SetLineColor(1)
    TT.SetLineColor(1)
    DY.SetLineColor(1)
    ST.SetLineColor(1)
    Faketau.SetLineColor(1)
    Data.SetLineColor(1)
    Data.SetLineWidth(2)
    
    

    stack=ROOT.THStack("stack","stack")
    stack.Add(DY)
    stack.Add(ST)
    #stack.Add(QCD)
    #stack.Add(W)
    stack.Add(VV)
    stack.Add(TT)
    stack.Add(Faketau)
    
    errorBand = TT.Clone()
    #errorBand.Add(QCD)
    errorBand.Add(ST)
    errorBand.Add(DY)
    errorBand.Add(VV)
    errorBand.Add(Faketau)
    errorBand.SetMarkerSize(0)
    errorBand.SetFillColor(new_idx)
    errorBand.SetFillStyle(3001)
    errorBand.SetLineWidth(1)
    
    errorBand_ersatz = errorBand.Clone()

    for iii in range(1, errorBand.GetNbinsX()+1):
        bin_error_up = pow(sq_combined_error["up"].GetBinContent(iii) + pow(errorBand.GetBinError(iii),2), 0.5)
        bin_error_down = pow(sq_combined_error["down"].GetBinContent(iii) + pow(errorBand.GetBinError(iii),2), 0.5)
        bin_content=errorBand.GetBinContent(iii)
        midpoint = (bin_error_up-bin_error_down)/2
        bin_content += midpoint
        sym_error = bin_error_up-midpoint

        errorBand_ersatz.SetBinContent(iii, bin_content)
        errorBand_ersatz.SetBinError(iii, sym_error)

    
    pad1 = ROOT.TPad("pad1","pad1",0,0.35,1,1)
    #pad1 = ROOT.TPad("pad1","pad1",0,0,1,1)
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
    #pad1.SetBottomMargin(0.026)
    pad1.SetBottomMargin(0.16)
    pad1.SetFrameFillStyle(0)
    pad1.SetFrameLineStyle(0)
    pad1.SetFrameLineWidth(3)
    pad1.SetFrameBorderMode(0)
    pad1.SetFrameBorderSize(10)
    if (var_log_dic[var[k]]): pad1.SetLogy()
    
    Data.GetXaxis().SetLabelSize(0)
    if (var_log_dic[var[k]]):
        Data.SetMaximum(Data.GetMaximum()*1000)#1.5)#FIXME
        ST.SetMaximum(Data.GetMaximum()*200)#1.5)#FIXME
    else:
        Data.SetMaximum(Data.GetMaximum()*2)#2.5)#FIXME
        ST.SetMaximum(Data.GetMaximum()*2)#2.5)#FIXME
    Data.SetMinimum(0.001)
    Data.Draw("e")
    #ST.Draw("hist")
    stack.Draw("histsame")
    errorBand_ersatz.Draw("e2same")
    Data.Draw("esame")
    
    
    #Signal.SetLineColor(2)
    #Signal.SetLineWidth(2)
    #Signal.Draw("histsame")
    
    
    legende=make_legend()
    legende.AddEntry(Data,"Observed","elp")
    #legende.AddEntry(Signal,"1 TeV RPV#rightarrow #mu #tau","f")
    legende.AddEntry(DY,"Z#rightarrow#tau #tau","f")
    legende.AddEntry(Faketau,"Fake #tau bg","f")
    legende.AddEntry(TT,"t#bar{t}+jets","f")
    #legende.AddEntry(W,"W+jets","f")
    legende.AddEntry(VV,"Diboson","f")
    #legende.AddEntry(QCD,"QCD multijet","f")
    legende.AddEntry(ST,"Single Top","f")
    legende.AddEntry(errorBand,"Uncertainty","f")
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
    finalstate.SetHeader("#mu #tau")
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
    pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.35)
    pad2.SetTopMargin(0.05)
    pad2.SetBottomMargin(0.35)
    pad2.SetLeftMargin(0.18)
    pad2.SetRightMargin(0.05)
    pad2.SetTickx(1)
    #pad2.SetTicky(1)
    pad2.SetFrameLineWidth(3)
    pad2.SetGridx()
    pad2.SetGridy()
    pad2.Draw()
    pad2.cd()
    h1=Data.Clone()
    h1.SetMaximum(2.0)#FIXME(1.6)
    h1.SetMinimum(0.0)#FIXME(0.4)
    h1.SetMarkerStyle(20)
    h3=errorBand_ersatz.Clone()
    hwoE=errorBand.Clone()
    for iii in range (1,hwoE.GetSize()-2):
        hwoE.SetBinError(iii,0)
    h3.Sumw2()

    h1.Sumw2()
    h1.SetStats(0)
    h1.Divide(hwoE)
    h3.Divide(hwoE)

    h1.GetXaxis().SetTitle(photogenic_var[var[k]])
    h1.GetXaxis().SetLabelSize(0.08)
    h1.GetYaxis().SetLabelSize(0.08)
    h1.GetYaxis().SetTitle("Obs./Exp.")
    h1.GetXaxis().SetNdivisions(505)
    h1.GetYaxis().SetNdivisions(6)
    
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
    c.SaveAs(var_out+".png")



ROOT.gStyle.SetFrameLineWidth(3)
ROOT.gStyle.SetLineWidth(3)
ROOT.gStyle.SetOptStat(0)

c=ROOT.TCanvas("canvas","",0,0,600,600)
c.cd()

#file=ROOT.TFile("final.root","r")
file=ROOT.TFile("histos_highmassmutau.root","r")

adapt=ROOT.gROOT.GetColor(12)
new_idx=ROOT.gROOT.GetListOfColors().GetSize() + 1
trans=ROOT.TColor(new_idx, adapt.GetRed(), adapt.GetGreen(),adapt.GetBlue(), "",0.5)


var=[]
#var.append("ev_Mvis")          
#var.append("ev_Mtot")          
#var.append("tau_pt")           
#var.append("tau_eta")          
#var.append("tau_phi")          
#var.append("mu_pt")            
#var.append("mu_eta")           
#var.append("mu_phi")           
#var.append("mu_isolation")           
#var.append("ev_DRmutau")       
##var.append("ev_DeltaPhimutau") 
##var.append("ev_DeltaPhiMETtau")
#var.append("ev_MET")           
var.append("ev_Mcol")          
#var.append("ev_Mt")            
#var.append("sign")

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

Mth=[
"_MtHigh", 
"_MtHigh_TT", 
"_MtLow_TT", 
"_MtLow_OS",
"_MtLow_SS"
]

systs_aux=[
"TES",
"MES",
"mres",
"minbias",
"muonID",
"muonIso",
"tracking",
"trigger",
"tauID",
"eletauFR",
"mutauFR",
"FRstat",
"FRsys",
"topPt",
]

systs_up=[]
systs_down=[]

variation=[
"up",
"down",
]

for i in range(0,len(systs_aux)):
    systs_up.append(systs_aux[i]+"_up")
    systs_down.append(systs_aux[i]+"_down")

systs={}
systs["up"]=systs_up
systs["down"]=systs_down


for k in range (0,nvar):
    TT_MtLow={}
    VV_MtLow={}
    DY_MtLow={}
    ST_MtLow={}
    Faketau_MtLow={}
    MC_MtLow={}
    sq_combined_error_MtLow={}

    TT_OS={}
    VV_OS={}
    DY_OS={}
    ST_OS={}
    Faketau_OS={}
    MC_OS={}
    
    Data_OS=file.Get("nominal/data_nominal_"+var[k]+"_MtLow_OS")

    for iVar in range(0,len(variation)):
        vary=variation[iVar]
        TT_MtLow[vary]=[]
        VV_MtLow[vary]=[]
        DY_MtLow[vary]=[]
        ST_MtLow[vary]=[]
        Faketau_MtLow[vary]=[]
        MC_MtLow[vary]=[]

        TT_OS[vary]=[]
        VV_OS[vary]=[]
        DY_OS[vary]=[]
        ST_OS[vary]=[]
        Faketau_OS[vary]=[]
        MC_OS[vary]=[]


    for l in range (0,len(Mth)):
        TT={}
        VV={}
        DY={}
        ST={}
        Faketau={}
        MC={}
        sq_combined_error={}

        ###########################################
        ###########################################
        #nominal plots
        var_in = "nominal_"+var[k]+Mth[l]
        print var_in
        if "MtLow_OS" in var_in:
            TT_OS["nominal"]=file.Get("nominal/TT_"+var_in)
            VV_OS["nominal"]=file.Get("nominal/VV_"+var_in)
            DY_OS["nominal"]=file.Get("nominal/DY_"+var_in)
            ST_OS["nominal"]=file.Get("nominal/ST_"+var_in)
            Faketau_OS["nominal"]=file.Get("nominal/faketau_"+var_in)
            MC_OS["nominal"]=Faketau_OS["nominal"].Clone()
            MC_OS["nominal"].Add(TT_OS["nominal"])
            MC_OS["nominal"].Add(VV_OS["nominal"])
            MC_OS["nominal"].Add(DY_OS["nominal"])
            MC_OS["nominal"].Add(ST_OS["nominal"])
            
            

        TT["nominal"]=file.Get("nominal/TT_"+var_in)
        VV["nominal"]=file.Get("nominal/VV_"+var_in)
        DY["nominal"]=file.Get("nominal/DY_"+var_in)
        ST["nominal"]=file.Get("nominal/ST_"+var_in)
        Faketau["nominal"]=file.Get("nominal/faketau_"+var_in)
        MC["nominal"]=Faketau["nominal"].Clone()
        MC["nominal"].Add(TT["nominal"])
        MC["nominal"].Add(VV["nominal"])
        MC["nominal"].Add(DY["nominal"])
        MC["nominal"].Add(ST["nominal"])

        Data=file.Get("nominal/data_"+var_in)
    
        if "MtLow_SS" in var_in:
            DY_MtLow["nominal"]=DY["nominal"].Clone()
            TT_MtLow["nominal"]=TT["nominal"].Clone()
            ST_MtLow["nominal"]=ST["nominal"].Clone()
            VV_MtLow["nominal"]=VV["nominal"].Clone()
            Faketau_MtLow["nominal"]=Faketau["nominal"].Clone()
            MC_MtLow["nominal"]=MC["nominal"].Clone()
            Data_MtLow=Data.Clone()

            DY_MtLow["nominal"].Add(DY_OS["nominal"])
            TT_MtLow["nominal"].Add(TT_OS["nominal"])
            ST_MtLow["nominal"].Add(ST_OS["nominal"])
            VV_MtLow["nominal"].Add(VV_OS["nominal"])
            Faketau_MtLow["nominal"].Add(Faketau_OS["nominal"])
            MC_MtLow["nominal"].Add(Faketau_OS["nominal"])
            Data_MtLow.Add(Data_OS)
    
        #end nominal
        ###########################################
        ###########################################

        #systematics
        for iVar in range(0,len(variation)):
            vary=variation[iVar]
            TT[vary]=[]
            VV[vary]=[]
            DY[vary]=[]
            ST[vary]=[]
            Faketau[vary]=[]
            MC[vary]=[]

            sq_combined_error[vary]=MC["nominal"].Clone()
            for iBin in range(1,sq_combined_error[vary].GetNbinsX()+1):
                sq_combined_error[vary].SetBinContent(iBin, 0)
                sq_combined_error[vary].SetBinError(iBin, 0)

            if "MtLow_SS" in var_in:
                sq_combined_error_MtLow[vary]=MC["nominal"].Clone()
                for iBin in range(1,sq_combined_error_MtLow[vary].GetNbinsX()+1):
                    sq_combined_error_MtLow[vary].SetBinContent(iBin, 0)
                    sq_combined_error_MtLow[vary].SetBinError(iBin, 0)

        for j in range(0,len(systs_up)):
            for iVar in range(0,len(variation)):
                vary=variation[iVar]
                var_in = systs[vary][j]+"_"+var[k]+Mth[l]
                print var_in
                if "MtLow_OS" in var_in:
                    TT_OS[vary].append(file.Get(systs[vary][j]+"/TT_"+var_in))
                    VV_OS[vary].append(file.Get(systs[vary][j]+"/VV_"+var_in))
                    DY_OS[vary].append(file.Get(systs[vary][j]+"/DY_"+var_in))
                    ST_OS[vary].append(file.Get(systs[vary][j]+"/ST_"+var_in))
                    Faketau_OS[vary].append(file.Get(systs[vary][j]+"/faketau_"+var_in))
                    MC_OS[vary].append(Faketau_OS[vary][j].Clone())
                    MC_OS[vary][j].Add(TT_OS[vary][j])
                    MC_OS[vary][j].Add(VV_OS[vary][j])
                    MC_OS[vary][j].Add(DY_OS[vary][j])
                    MC_OS[vary][j].Add(ST_OS[vary][j])


                TT[vary].append(file.Get(systs[vary][j]+"/TT_"+var_in))
                VV[vary].append(file.Get(systs[vary][j]+"/VV_"+var_in))
                DY[vary].append(file.Get(systs[vary][j]+"/DY_"+var_in))
                ST[vary].append(file.Get(systs[vary][j]+"/ST_"+var_in))
                Faketau[vary].append(file.Get(systs[vary][j]+"/faketau_"+var_in))
                MC[vary].append(Faketau[vary][j].Clone())
                MC[vary][j].Add(TT[vary][j])
                MC[vary][j].Add(VV[vary][j])
                MC[vary][j].Add(DY[vary][j])
                MC[vary][j].Add(ST[vary][j])

                htemp_up = sq_combined_error["up"].Clone()
                htemp_down = sq_combined_error["down"].Clone()
                for iBin in range(1, htemp_up.GetNbinsX()+1):
                    bin_content = MC[vary][j].GetBinContent(iBin)-MC["nominal"].GetBinContent(iBin)
                    if bin_content < 0:
                        htemp_down.SetBinContent(iBin, pow(bin_content,2))
                    else:
                        htemp_up.SetBinContent(iBin, pow(bin_content,2))
                sq_combined_error["up"].Add(htemp_up)
                sq_combined_error["down"].Add(htemp_down)


                if "MtLow_SS" in var_in:
                    DY_MtLow[vary].append(DY[vary][j].Clone())
                    TT_MtLow[vary].append(TT[vary][j].Clone())
                    ST_MtLow[vary].append(ST[vary][j].Clone())
                    VV_MtLow[vary].append(VV[vary][j].Clone())
                    Faketau_MtLow[vary].append(Faketau[vary][j].Clone())
                    MC_MtLow[vary].append(MC[vary][j].Clone())
        
                    DY_MtLow[vary][j].Add(DY_OS[vary][j])
                    TT_MtLow[vary][j].Add(TT_OS[vary][j])
                    ST_MtLow[vary][j].Add(ST_OS[vary][j])
                    VV_MtLow[vary][j].Add(VV_OS[vary][j])
                    Faketau_MtLow[vary][j].Add(Faketau_OS[vary][j])
                    MC_MtLow[vary][j].Add(MC_OS[vary][j])

                    htemp_up = sq_combined_error_MtLow["up"].Clone()
                    htemp_down = sq_combined_error_MtLow["down"].Clone()
                    for iBin in range(1, htemp_up.GetNbinsX()+1):
                        bin_content = MC_MtLow[vary][j].GetBinContent(iBin)-MC_MtLow["nominal"].GetBinContent(iBin)
                        if bin_content < 0:
                            htemp_down.SetBinContent(iBin, pow(bin_content,2))
                        else:
                            htemp_up.SetBinContent(iBin, pow(bin_content,2))
                    sq_combined_error_MtLow["up"].Add(htemp_up)
                    sq_combined_error_MtLow["down"].Add(htemp_down)
    
        
        make_plot(var[k]+Mth[l], Data, DY["nominal"], TT["nominal"], VV["nominal"], ST["nominal"], Faketau["nominal"], sq_combined_error)

        if "MtLow_SS" in var[k]:
            make_plot(var[k]+"_MtLow", Data_MtLow, DY_MtLow["nominal"], TT_MtLow["nominal"], VV_MtLow["nominal"], ST_MtLow["nominal"], Faketau_MtLow["nominal"], sq_combined_error_MtLow)

