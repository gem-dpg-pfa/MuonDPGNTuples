import CRABClient
from WMCore.Configuration import Configuration 

config = Configuration()

config.section_("General")
config.General.requestName = 'CRUZET_345084_Prompt'
config.General.workArea = "/afs/cern.ch/user/f/fivone/Documents/NTuplizer/CRUZET/CMSSW_11_3_2/src/MuDPGAnalysis/"
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/user/f/fivone/Documents/NTuplizer/CRUZET/CMSSW_11_3_2/src/MuDPGAnalysis/MuonDPGNtuples/test/muDpgNtuples_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.pyCfgParams = ['isMC=False','nEvents=-1']

config.section_("Data")
config.Data.inputDataset = '/Cosmics/Commissioning2021-PromptReco-v1/AOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 180
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/fivone/GEMMuonNtuplizerZmumu'
config.Data.outputDatasetTag = 'CRUZET_345084_Prompt'
config.Data.runRange = '345084'

config.section_("Site")
config.Site.storageSite = 'T2_DE_RWTH'

config.section_("User")
config.User.voGroup = 'dcms'
