#! /usr/bin/python
#Code to make a script to submit jobs to the localgrid
#This code doesn't submit directly jobs to localgrid, it just produces a bunch of *.sh files
#In order to submit the jobs, you'll need to type the command "source Submit_*.sh"
#Note : your code needs to be already compiler (with a .exe extension for this to work)

import os
from skimmed_datasets import * #imports dataset paths: pnfn[], myname[] and myoption[] arrays 
if __name__ == "__main__":
    location=os.getcwd();
    #name of your *compiled* code (omit the .exe extension)
    code_name = "HighMassLFVMuTau"
    region = "CR3"
    folder = "HighMassLFVMuTau/SSisomuantitau_CR3"

    for jj in range(0, len(pnfn)):    
        #Main file, which you'll use to submit the jobs
        #To submit the jobs, you'll need to type "source Submit_myname.sh" in your terminal
        submit_File = open("../Submit/Submit_"+myname[jj]+".sh" , 'w')
        f=os.popen("ls -t " + pnfn[jj] + "Out* | sort")   #you may want to replace "outfile" with the name of the root files found in the /pnfs folder
        dir = "dcap://maite.iihe.ac.be" +  pnfn[jj]  + "/"
        name_out =  myname[jj] + ".sh"
        outFile = open("../Jobs_to_submit/"+name_out , 'w')
        command1 = "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"
        command1 = command1 + "mkdir /user/dbeghin/Work/Taus/"+folder+"/Out_"+myname[jj] + "\n"
        command1 = command1 + "cd /user/dbeghin/CMSSW_8_0_26_patch1/src " + "\n" #go to the top of your CMSSW release
        command1 = command1 + "eval `scram runtime -sh` " + "\n"   #load the cms environment
        command1 = command1 + "export X509_USER_PROXY=/user/dbeghin/x509up_u$(id -u dbeghin)" + "\n"
        command1 = command1 + "export scratchdir=$TMPDIR " + "\n"
        command1 = command1 + "cd $scratchdir" + "\n"      
        command1 = command1 + "cp /user/dbeghin/Work/Taus/" + code_name + ".exe $scratchdir/" + "\n"
        command1 = command1 + "mkdir Reweighting" + "\n"
        command1 = command1 + "cp /user/dbeghin/Work/Taus/Reweighting/*.root Reweighting/" + "\n"
        #command1 = command1 + "export workdir=/user/dbeghin/Work/Taus/ " + "\n"  #Set your working directory, where the code is located
        #command1 = command1 + "cd $workdir"
        outFile.write(command1)
        command3 = "qsub -q localgrid@cream02 -o " + "../out_err/"+myname[jj] +".stdout -e " +"../out_err/"+myname[jj] +".stderr -l walltime=18:00:00    " + "../Jobs_to_submit/"+name_out + "\n"  #Command to submit one job to the localgrid
        submit_File.write(command3)
        ligne=0

        files_per_job = int(filesperjob[jj])  #Set the number of root files per job
        for i in f.readlines():   #f contains the root files in the /pnfs directory
            #print ligne
            ligne=ligne+1
            if ligne%files_per_job==0:   
                outFile.write("hadd -f Con"+str(ligne/files_per_job)+myname[jj]+".root Outout*.root\n")
                outFile.write("cp  Con" +str(ligne/files_per_job)+myname[jj]+ ".root \t" + "/user/dbeghin/Work/Taus/"+folder+"/Out_"+myname[jj]+"/Con" +str(ligne/files_per_job)+myname[jj]+ ".root\n")
                outFile.write("rm -f *.root\n")
                outFile.close()
                outFile = open("../Jobs_to_submit/test"+str(ligne/files_per_job)+name_out , 'w')
                command1 = "mkdir /user/dbeghin/Work/Taus/"+folder+"/Out_"+myname[jj] + "\n"
                command1 = command1 + "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"
                command1 = command1 + "cd /user/dbeghin/CMSSW_8_0_26_patch1/src " + "\n"
                command1 = command1 + "eval `scram runtime -sh` " + "\n"
                command1 = command1 + "export X509_USER_PROXY=/user/dbeghin/x509up_u$(id -u dbeghin)" + "\n"
                command1 = command1 + "export scratchdir=$TMPDIR " + "\n"
                #command1 = command1 + "export workdir=/user/dbeghin/Work/Taus/ " + "\n"
                command1 = command1 + "cd $scratchdir" + "\n"      
                command1 = command1 + "cp /user/dbeghin/Work/Taus/" + code_name + ".exe $scratchdir/" + "\n"
                command1 = command1 + "mkdir Reweighting" + "\n"
                command1 = command1 + "cp /user/dbeghin/Work/Taus/Reweighting/*.root Reweighting/" + "\n"
                outFile.write(command1)
                command3 = "qsub -q localgrid@cream02 -o "+ "../out_err/test"+str(ligne/files_per_job)+myname[jj] +".stdout -e " +"../out_err/test"+str(ligne/files_per_job)+myname[jj] +".stderr -l walltime=18:00:00    " + "../Jobs_to_submit/test"+str(ligne/files_per_job)+name_out + "\n"
                submit_File.write(command3)
            #Below, the command to execute your code, use the correct syntax for your own code, with all arguments (file in, file out, etc.) in the proper order
            #Note that i[0:-1] is just the name of the root file in /pnfs
            command2 = "dccp dcap://maite.iihe.ac.be"+   i[0:-1] + " $scratchdir/" + "\n"
            bare_fname = i[len(pnfn[jj]):-1]
            command2 = command2 + "\n" + "./" + code_name + ".exe " + "Outout" +str(ligne)+myname[jj]+ ".root " +   bare_fname + " " + region + " " + myoption[jj]
            #command2 = command2 + " \n"+ "cp  Out" +str(ligne)+myname[jj]+ ".root \t" + "/user/dbeghin/Work/Taus/"+folder+"/Out_"+myname[jj]+"/Out" +str(ligne)+myname[jj]+ ".root"
            command2 = command2 + "\n rm " + bare_fname
            command2 = command2 + " \n\n\n"
            outFile.write(command2)
            
        outFile.write("hadd -f Con"+str(ligne/files_per_job)+myname[jj]+".root Outout*.root\n")
        outFile.write("cp  Con" +str(ligne/files_per_job)+myname[jj]+ ".root \t" + "/user/dbeghin/Work/Taus/"+folder+"/Out_"+myname[jj]+"/Con" +str(ligne/files_per_job)+myname[jj]+ ".root\n")
        outFile.write("rm -f *.root\n")
        outFile.close()
        submit_File.close()
