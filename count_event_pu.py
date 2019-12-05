import os
import ROOT

sample_path = {}

############################ ZToEE #########################
#path_ZToEE_50_120   =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_50_120/crab_ZToEE_NNPDF31_13TeV-powheg_M_50_120/180218_161630/0000/"]
#path_ZToEE_120_200  =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_120_200/crab_ZToEE_NNPDF31_13TeV-powheg_M_120_200/180218_161650/0000/"]
#path_ZToEE_200_400  =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_200_400/crab_ZToEE_NNPDF31_13TeV-powheg_M_200_400/180218_161707/0000/"]
#path_ZToEE_400_800  =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_400_800/crab_ZToEE_NNPDF31_13TeV-powheg_M_400_800/180218_161727/0000/"]
#path_ZToEE_800_1400 =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_800_1400/crab_ZToEE_NNPDF31_13TeV-powheg_M_800_1400/180218_161744/0000/"]
#path_ZToEE_1400_2300=["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_1400_2300/crab_ZToEE_NNPDF31_13TeV-powheg_M_1400_2300/180218_161810/0000/"]
#path_ZToEE_2300_3500=["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_2300_3500/crab_ZToEE_NNPDF31_13TeV-powheg_M_2300_3500/180218_161830/0000/"]
#path_ZToEE_3500_4500=["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_3500_4500/crab_ZToEE_NNPDF31_13TeV-powheg_M_3500_4500/180218_161848/0000/"]
#path_ZToEE_4500_6000=["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_4500_6000/crab_ZToEE_NNPDF31_13TeV-powheg_M_4500_6000/180218_161906/0000/"]
#path_ZToEE_6000_Inf =["/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/ZToEE_NNPDF31_13TeV-powheg_M_6000_Inf/crab_ZToEE_NNPDF31_13TeV-powheg_M_6000_Inf/180218_161928/0000/"]
########################### DYToEE powheg #####################
#path_DYToEE_pow   =['/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYToEE_M-50_NNPDF31_13TeV-powheg-pythia8/crab_DYToEE_M-50_NNPDF31_13TeV-powheg-pythia8/180218_162058/0000/']

########################### DYJetToLL amc #####################

'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190527_135948/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/crab_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/190620_184305/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/crab_WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/190612_162747/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WZ_TuneCUETP8M1_13TeV-pythia8/crab_WZ_TuneCUETP8M1_13TeV-pythia8/190527_135321/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WZ_TuneCUETP8M1_13TeV-pythia8/crab_WZ_TuneCUETP8M1_13TeV-pythia8_ext/190527_134853/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ZZTo2L2Nu_13TeV_powheg_pythia8/crab_ZZTo2L2Nu_13TeV_powheg_pythia8/190612_162605/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ZZTo2L2Q_13TeV_powheg_pythia8/crab_ZZTo2L2Q_13TeV_powheg_pythia8/190612_162821/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ZZTo4L_13TeV_powheg_pythia8/crab_ZZTo4L_13TeV_powheg_pythia8/190612_162640/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_ZZ_TuneCUETP8M1_13TeV-pythia8/190527_134519/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_ZZ_TuneCUETP8M1_13TeV-pythia8_ext/190527_134619/0000/',


path_DYToLL_amc=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190718_163619/0000/',
]


path_DYToLL_200=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190527_135216/0000/',
]


path_DYToLL_400=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190527_135709/0000/',
]


path_DYToLL_500=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190528_085039/0000/',
]


path_DYToLL_700=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190528_084946/0000/',
]


path_DYToLL_800=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-800to1000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-800to1000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190611_103001/0000/',
]


path_DYToLL_1000=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190528_085011/0000/',
]


path_DYToLL_1500=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190527_135601/0000/',
]


path_DYToLL_2000=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/190528_084920/0000/',
]


#path_DY1J=[
#'/pnfs/iihe/cms/store/user/dbeghin/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY1JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8/180618_134813/0000/',
#'/pnfs/iihe/cms/store/user/dbeghin/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY1JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8/180618_134813/0001/',
#'/pnfs/iihe/cms/store/user/dbeghin/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY1JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8ext/180618_134618/0000/',
#'/pnfs/iihe/cms/store/user/dbeghin/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY1JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8ext/180618_134618/0001/',
#]
#
#path_DY2J=[
#'/pnfs/iihe/cms/store/user/dbeghin/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY2JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8/180618_134650/0000/',
#'/pnfs/iihe/cms/store/user/dbeghin/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY2JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8ext/180618_134704/0000/',
#]
#
#path_DY3J=[
#'/pnfs/iihe/cms/store/user/dbeghin/DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY3JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8/180618_134635/0000/',
#]
#
#path_DY4J=[
#'/pnfs/iihe/cms/store/user/dbeghin/DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DY4JetsToLL_TuneCP5_13TeV-madgraphMLM-pythia8/180618_134848/0000/',
#]


########################### DYJetToLL mad V2 #####################
#path_DYToLL_mad=['/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/180218_162022/0000/','/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/180218_162022/0001/','/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_v1/180218_162040/0000/','/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_v1/180218_162040/0001/','/pnfs/iihe/cms/store/user/amkalsi/Moriond2018/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_v1/180218_162040/0002/']

########################### WJets ############################

#path_WJet_mad  =[
#'/pnfs/iihe/cms/store/user/amkalsi/2017_MET_Corrected_10Dec2018/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_WJetsToLNu_TuneCP5_13TeV/181214_095754/0000/',
#'/pnfs/iihe/cms/store/user/amkalsi/2017_MET_Corrected_10Dec2018/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_WJetsToLNu_TuneCP5_13TeV/181214_095754/0001/',
#]

#path_W1J = [
#'/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_Final/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/180619_135442/0000/',
#'/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_Final/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/180619_135442/0001/',
#]
#
#path_W2J = [
#'/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_Final/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/180619_135552/0000/',
#]
#
#path_W3J = [
#'/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_Final/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/180619_135630/0000/',
#]
#
#path_W4J = [
#'/pnfs/iihe/cms/store/user/amkalsi/Moriond2018_Final/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/crab_W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/180619_135652/0000/',
#]

############################# WW ############################


path_WW=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WWTo2L2Nu_13TeV-powheg/crab_WWTo2L2Nu_13TeV-powheg/190527_134446/0000/',
]

path_WW_200=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WWTo2L2Nu_Mll_200To600_13TeV-powheg/crab_WWTo2L2Nu_Mll_200To600_13TeV-powheg/190527_135250/0000/',
]

path_WW_600=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WWTo2L2Nu_Mll_600To1200_13TeV-powheg/crab_WWTo2L2Nu_Mll_600To1200_13TeV-powheg/190527_135914/0000/',
]

path_WW_1200=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WWTo2L2Nu_Mll_1200To2500_13TeV-powheg/crab_WWTo2L2Nu_Mll_1200To2500_13TeV-powheg/190527_134548/0000/',
]

path_WW_2500=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/WWTo2L2Nu_Mll_2500ToInf_13TeV-powheg/crab_WWTo2L2Nu_Mll_2500ToInf_13TeV-powheg/190527_135757/0000/',
]

############################# WWTo2L2Nv ############################
#
############################## WZ ###########################
path_WZ=[
]

################################ ZZ #########################
path_ZZ=[
]

################################## ST ###################
path_ST=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/190527_134822/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_ext/190527_134320/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/190527_134958/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_ext/190527_135505/0000/',
]

################################# DY mad #####################


##################################### Gamma+Jet ############
#path_GJ  =['/pnfs/iihe/cms/store/user/wenxing/RunIISummer17MiniAOD-92X/GJet_DoubleEMEnriched_13TeV_pythia8/crab_GJet_DoubleEMEnriched_13TeV_pythia8/171019_161408/0000/','/pnfs/iihe/cms/store/user/wenxing/RunIISummer17MiniAOD-92X/GJet_DoubleEMEnriched_13TeV_pythia8/crab_GJet_DoubleEMEnriched_13TeV_pythia8/171019_161408/0001/','/pnfs/iihe/cms/store/user/wenxing/RunIISummer17MiniAOD-92X/GJet_DoubleEMEnriched_13TeV_pythia8/crab_GJet_DoubleEMEnriched_13TeV_pythia8/171019_161408/0002/']


####################################### TTbar ####################
path_TT=[
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/190527_134925/0000/',
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/crab_TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/190527_134925/0001/',
]

path_TT_500 = [
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTToLL_MLL_500To800_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TTToLL_MLL_500To800_TuneCUETP8M1_13TeV-powheg-pythia8/190527_135431/0000/',
]

path_TT_800 = [
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTToLL_MLL_800To1200_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TTToLL_MLL_800To1200_TuneCUETP8M1_13TeV-powheg-pythia8/190527_134747/0000/',
]

path_TT_1200 = [
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTToLL_MLL_1200To1800_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TTToLL_MLL_1200To1800_TuneCUETP8M1_13TeV-powheg-pythia8/190527_135635/0000/',
]

path_TT_1800 = [
'/pnfs/iihe/cms/store/user/dbeghin/Legacy/2016/TTToLL_MLL_1800ToInf_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TTToLL_MLL_1800ToInf_TuneCUETP8M1_13TeV-powheg-pythia8/190527_134249/0000/',
]



sample_path['DYToLL_amc']     =path_DYToLL_amc
sample_path['DYToLL_200']     =path_DYToLL_200
sample_path['DYToLL_400']     =path_DYToLL_400
sample_path['DYToLL_500']     =path_DYToLL_500
sample_path['DYToLL_700']     =path_DYToLL_700
sample_path['DYToLL_800']     =path_DYToLL_800
sample_path['DYToLL_1000']    =path_DYToLL_1000
sample_path['DYToLL_1500']    =path_DYToLL_1500
sample_path['DYToLL_2000']    =path_DYToLL_2000
sample_path['WW']             =path_WW
sample_path['WW_200']         =path_WW_200
sample_path['WW_600']         =path_WW_600
sample_path['WW_1200']        =path_WW_1200
sample_path['WW_2500']        =path_WW_2500
sample_path['WZ']             =path_WZ
sample_path['ZZ']             =path_ZZ
sample_path['ST']             =path_ST
#sample_path['WJetsToLNu']     =path_WJet_mad
#sample_path['DYToLL_mad']     =path_DYToLL_mad
sample_path['TT']            = path_TT
sample_path['TT_500']        = path_TT_500
sample_path['TT_800']        = path_TT_800
sample_path['TT_1200']       = path_TT_1200
sample_path['TT_1800']       = path_TT_1800


for sample_name in sample_path:
    H_pu=ROOT.TH1F("pileup_%s"%(sample_name),"",120,0,120)
    H_pu.Scale(0)
    nEventsraw = 0
    neventsweight = 0
    nEventsStored = 0
    nEventsiihe = 0
    for path in sample_path[sample_name]:
        print path
        filenames = os.listdir(path)
        for fname in filenames:
            filename = path +  fname
            if 'root' not in fname: continue
            f = ROOT.TFile.Open(filename)
            if not f: print 'not exist'+fname
            tree_in = f.Get('IIHEAnalysis')
            tree_meta = f.Get('meta')
            nEventsiihe += tree_in.GetEntries()
            tree_meta.GetEntry(0)
            nEventsraw += tree_meta.nEventsRaw
            nEventsStored += tree_meta.nEventsStored
            neventsweight += tree_meta.mc_nEventsWeighted
            h_tmp=f.Get("pileupDist")
            H_pu.Add(h_tmp,1)
            f.Close()
    fout=ROOT.TFile("./mc_pu/%s.root"%(sample_name),"RECREATE")
    fout.cd()
    H_pu.Write("pileup")
    fout.Close()
    print '#####################'
    print '%s'%(sample_name) 
    print 'nEventsraw %d   '%(nEventsraw)
    print 'neventsweight %d   '%(neventsweight)
    print 'nEventsStored %d   '%(nEventsStored)
    print 'nEventsiihe %d   '%(nEventsiihe)
    print '#####################' 
