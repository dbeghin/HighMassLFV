#!/usr/bin/env python
import ROOT
import re
from array import array


file=ROOT.TFile("histos_massresolution.root","r")

masspoints=[
"500",   
"600", 
"700", 
"800", 
"900", 
"1000",
"1100",
"1200",
"1300",
"1400",
"1500",
"1600",
"1700",
"1800",
"1900",
"2000",
"2200",
"2400",
"2600",
"2800",
"3000",
"3500",
"4000",
"4500",
"5000",
]

file_out=open("signaleff.tex", "w")
header="\\documentclass[11pt]{beamer}\n"
header+="\\usetheme{Copenhagen}\n"
header+="\\usepackage[utf8]{inputenc}\n"
header+="\\usepackage[T1]{fontenc}\n"
header+="\\usepackage{amsmath}\n"
header+="\\usepackage{amsfonts}\n"
header+="\\usepackage{amssymb}\n"
header+="\\usepackage{changepage}\n"
header+="\\usepackage{multirow}\n\n"

header+="\\begin{document}\n"
header+="\\begin{frame}\n"
header+="\\begin{adjustwidth}{-2.5em}{-2em}\n"
#header+="\\tiny\n"
header+="\\fontsize{0.15cm}{0.17cm}\\selectfont\n"
header+="\\begin{tabular}{|l|c|c|c|c|c|c|c|c|c|}\n"
header+="\\hline\n"

file_out.write(header)
file_out.write("\\multirow{2}{*}{mass} & No discr. & \\multicolumn{2}{c|}{VLoose anti-ele} & \\multicolumn{2}{c|}{Loose anti-ele} & \\multicolumn{2}{c|}{Medium anti-ele} & \\multicolumn{2}{c|}{Tight anti-ele} \\\\\n")
#file_out.write("\\multirow{2}{*}{mass} & No discr. & \\multicolumn{2}{c|}{VLoose anti-ele} & \\multicolumn{2}{c|}{Loose anti-ele} & \\multicolumn{2}{c|}{Medium anti-ele} & \\multicolumn{2}{c|}{Tight anti-ele} & \\multicolumn{2}{c|}{Loose anti-mu} & \\multicolumn{2}{c|}{Tight anti-mu} \\\\\n")
#file_out.write("\\hline\n")
file_out.write(" & events & events & ratio & events & ratio & events & ratio & events & ratio \\\\\n")
file_out.write("\\hline\n")

for k in range (0,len(masspoints)):
    var_in_nodisc = masspoints[k]+"_Mcol_nodisc"
    var_in_elevloose = masspoints[k]+"_Mcol_elevloose"
    var_in_eleloose = masspoints[k]+"_Mcol_eleloose"
    var_in_elemedium = masspoints[k]+"_Mcol_elemedium"
    var_in_eletight = masspoints[k]+"_Mcol_eletight"
    var_in_muloose = masspoints[k]+"_Mcol_muloose"
    var_in_mutight = masspoints[k]+"_Mcol_mutight"
    var_in_std = masspoints[k]+"_Mcol_final"
    var_in_tightest = masspoints[k]+"_Mcol_tightest"
    Data=file.Get(var_in_nodisc)
    Data_evl=file.Get(var_in_elevloose)
    Data_el=file.Get(var_in_eleloose)
    Data_em=file.Get(var_in_elemedium)
    Data_et=file.Get(var_in_eletight)
    Data_ml=file.Get(var_in_muloose)
    Data_mt=file.Get(var_in_mutight)
    Data_std=file.Get(var_in_std)
    Data_test=file.Get(var_in_tightest)
        
    file_out.write("%s & %s & %s & %s & %s & %s & %s & %s & %s & %s \\\\\n" %(masspoints[k], round(Data.Integral(), 2), round(Data_evl.Integral(), 2), round(Data_evl.Integral()/Data.Integral(), 2), round(Data_el.Integral(), 2), round(Data_el.Integral()/Data.Integral(), 2), round(Data_em.Integral(), 2), round(Data_em.Integral()/Data.Integral(), 2), round(Data_et.Integral(), 2), round(Data_et.Integral()/Data.Integral(), 2) ))
    #file_out.write("%s & %s & %s & %s & %s & %s & %s & %s & %s & %s \\\\\n" %(masspoints[k], round(Data.Integral(), 2), round(Data_el.Integral(), 2), round(Data_el.Integral()/Data.Integral(), 2), round(Data_et.Integral(), 2), round(Data_et.Integral()/Data.Integral(), 2), round(Data_ml.Integral(), 2), round(Data_ml.Integral()/Data.Integral(), 2), round(Data_mt.Integral(), 2), round(Data_mt.Integral()/Data.Integral(), 2), round(Data_std.Integral(), 2), round(Data_std.Integral()/Data.Integral(), 2) ))

file_out.write("\\hline\n")

closer="\\end{tabular}\n"
closer+="\\end{adjustwidth}\n"
closer+="\\end{frame}\n"
closer+="\\end{document}\n"
file_out.write(closer)
file_out.close()
