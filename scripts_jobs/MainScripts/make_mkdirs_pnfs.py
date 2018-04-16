#! /usr/bin/python
#Code to make a script to submit jobs to the localgrid
#This code doesn't submit directly jobs to localgrid, it just produces a bunch of *.sh files
#In order to submit the jobs, you'll need to type the command "source Submit_*.sh"
#Note : your code needs to be already compiler (with a .exe extension for this to work)

from datasets import * #imports dataset paths: pnfn[], myname[] and myoption[] arrays 
if __name__ == "__main__":

    outfile = open("mkdirs_pnfs.sh", "w")
    for jj in range(0, len(pnfn)):    
        command1 = "gfal-mkdir srm://maite.iihe.ac.be:8443/pnfs/iihe/cms/store/user/dbeghin/Skims/Out_"+myname[jj] + "\n"      #Make output directories
        outfile.write(command1)

    outfile.close()
