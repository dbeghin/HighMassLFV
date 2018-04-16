#! /usr/bin/python
#Code to find all jobs to submit and make a script which will easily submit them
#Jobs must already have been produce by make_submit_script.py

import os
if __name__ == "__main__":
    location=os.getcwd();

    outFile = open("Sub_all.sh", 'w')
    f=os.popen("ls -t " + "../Jobs_to_submit/*.sh | sort")

    for i in f.readlines():   #f contains the root files in the /pnfs directory
        outFile.write("source " + i[0:-1] + "\n")

    outFile.write("qstat -u dbeghin")
    outFile.close()
