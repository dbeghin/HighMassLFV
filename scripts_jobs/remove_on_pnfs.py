import math
import shlex
import os
import subprocess

def runCommand(commandLine):
    #run a command line command                                                                                                                                                                             
    #sys.stdout.write("%s\n" % commandLine)                                                                                                                                                                \
                                                                                                                                                                                                            
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    return retVal


folders = []
'''
ls_command = runCommand('ls -l /pnfs/iihe/cms/store/user/dbeghin/RPV*')
stdout, stderr = ls_command.communicate()
status = ls_command.returncode


if status != 0:
    raise IOError("Folder does not exist !!")
else:
    for line in stdout.splitline():
        fields = line.split()
        if len(fields)<8: continue
	folders.append('/pnfs/iihe/cms/store/user/dbeghin/' + fields[8])
'''

folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-1400_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-1000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-1400_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-1800_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-2000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-2500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-3000_LLE_LQD-01_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-300_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-3500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-3500_LLE_LQD-01_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-400_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-4500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-5000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-5500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-6000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-6000_LLE_LQD-05_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-600_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-6500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-6500_LLE_LQD-02_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-700_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToETau_M-900_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-1200_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-1600_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-1800_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
#folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-2000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-200_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-2500_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-3000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-3000_LLE_LQD-01_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-3500_LLE_LQD-01_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-4000_LLE_LQD-001_TuneCUETP8M1_13TeV-calchep-pythia8')
folders.append('/pnfs/iihe/cms/store/user/dbeghin/RPVresonantToMuTau_M-4000_LLE_LQD-01_TuneCUETP8M1_13TeV-calchep-pythia8')


removefile = open('remove.sh', 'w')

for folder in folders:
    stop = False
    walking_folder = folder
    while not stop:
        ls_command = runCommand('ls -l %s' % walking_folder)
	stdout, stderr = ls_command.communicate()
	status = ls_command.returncode

	if status != 0:
            raise IOError("Folder = %s does not exist !!" % walking_folder)
	    break
	else:
            for line in stdout.splitlines(): #split ls output into lines                                                                                                                                
                fields = line.split()        #split lines into information blocks, block number 8 corresponds to the file or directory name
                #print "Hi yet again"
		if len(fields) < 8: continue
		file_or_dir_name = fields[8]

		if '.root' in file_or_dir_name[-5:]:  #check whether it's a root file (will be a point 5 caracters from the end of the file name                                                        
                    removefile.write("gfal-rm srm://maite.iihe.ac.be:8443" + walking_folder + "/" + file_or_dir_name + '\n')
		    stop = True
		else:
                    if 'log' in file_or_dir_name:
                        stop = True
			continue
		    walking_folder = walking_folder + "/" + file_or_dir_name
                    print walking_folder
                    if '//////' in walking_folder: stop = True
		    break


removefile.close()

