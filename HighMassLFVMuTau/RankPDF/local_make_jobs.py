#! /usr/bin/python
#Code to make a script to submit jobs to the localgrid
#This code doesn't submit directly jobs to localgrid, it just produces a bunch of *.sh files
#In order to submit the jobs, you'll need to type the command "source Submit_*.sh"
#Note : your code needs to be already compiler (with a .exe extension for this to work)

import os
from datasets2016 import * #imports dataset paths: pnfn[], myname[] and myoption[] arrays 
if __name__ == "__main__":
    location=os.getcwd();
    #name of your *compiled* code (omit the .exe extension)
    folder = "/user/dbeghin/Work/MuTauHighMass/HighMassLFVMuTau/SignalRegion_CR100/"
    walltime = "01:00:00"

    submit_File = open("Submit_hadd.sh" , 'w')
    for jj in range(0, len(pnfn)):    

        outFile = open("arrange_histos_local_"+myname[jj]+".sh" , 'w')

        con_name = myname[jj]+".root"
        outFile.write("hadd -f "+con_name+" " + folder + pnfn[jj] + "/Con*.root\n")
        outFile.write("mkdir -p " + folder + myfolder[jj]+"\n")
        outFile.write("mv  "+con_name+" \t" + folder + myfolder[jj]+"\n")
        outFile.close()
        submit_File.write("source  arrange_histos_local_"+myname[jj]+".sh\n")

    submit_File.close()
