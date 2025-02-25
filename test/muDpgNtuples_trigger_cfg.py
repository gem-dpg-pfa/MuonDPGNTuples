import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
from Configuration.AlCa.GlobalTag import GlobalTag

import os
import subprocess
import sys


"""
OPTIONS SUMMARY
* globalTag       --> GT of the run 
* nEvents         --> Number of events to be processed
* displacement    --> Track propagation displacement: propagate the muon tracks not on GE11 ROB but on the virtual surface of ROB moved closed to the IP by <displacement>
* STA             --> Process CSC hltDigis and build standalong tracks. Only works for RPC Monitor
* isMC            --> MC dataset
* reUnpack        --> gem digis not stored in prompt reco. In case you need to access the GEM digis on a RAW-RECO dataset (e.g. reading OH or AMC status) the reunpakc has to be triggered. The RPCMonitor already contains the digis so it's not needed there.
* storeOHStatus   --> Enables the filler for the OHStatus
* storeAMCStatus  --> Enables the filler for the AMCStatus
* GE21            --> Forces the runpacking of gemDigi and then the construction of GE21 rechits which is turned off by default

"""


options = VarParsing.VarParsing()

options.register('globalTag',
                 #'130X_dataRun3_Prompt_v3', #'124X_dataRun3_Prompt_v4',
                 '140X_dataRun3_Prompt_v4',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #to run on a sub-sample
                 #-1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('displacement',
                 0, ## propagate the track at GEM RO board + displacement
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.float,
                 "Maximum number of processed events")

options.register('STA',
                 True, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Process STA from RPCMonitor")

options.register('isMC',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Dataset is MC")

options.register('reUnpack',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "enables reprocessing of digis: i.e OHStatus is not stored in RECO datesets, but can be extracted by re-unpacking data from a RAW dataset.")

options.register('storeOHStatus',
                 False, #default value,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save OH status info from unpacker")

options.register('storeAMCStatus',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save AMC status info from unpacker")

options.register('GE21',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "enables storing of GE21 rechits, disabled by default in CMSSW: i.e when running on a RAW dataset it's possible to reprocess digi and build GE21 rechits and save them in the ntuples")
options.register('CSClct',
                 True, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "enables unpacking of CSC digi to extract CSC diti LCT")

options.register('inputFolder',
                 #/eos/cms/store/
                 #"/eos/cms/store/data/Run2022D/Muon/RAW-RECO/ZMu-PromptReco-v2/000/357/734/00000/",
                 #"/eos/cms/store/group/dpg_gem/comm_gem/reRECO/Muon/crab_gemReReco_hv_re3/230213_023322/0000/",
                 #"/eos/cms/store/data/Run2024F/RPCMonitor/RAW/v1/000/383/323/00000/",
                 #"/store/data/Run2023F/RPCMonitor/RAW/v1/000/383/323/00000/",
                 #"/eos/cms/tier0/store/data/Run2024F/RPCMonitor/RAW/v1/000/383/468/00000",
                 #"/eos/cms/tier0/store/data/Run2024H/RPCMonitor/RAW/v1/000/385/841/00000",
                 "/eos/cms/tier0/store/data/Run2024H/RPCMonitor/RAW/v1/000/385/934/00000",
                 #"davs://cmsdcache-kit-tape.gridka.de:2880/pnfs/gridka.de/cms/tape/store/data/Run2024F/RPCMonitor/RAW/v1/000/383/323/00000/",
                 #"/eos/cms/store/group/dpg_gem/comm_gem/reRECO/SingleMuon/GEM-reRECO-GEM-only__Run2022B-ZMu-PromptReco-v1__RAW-RECO/220721_151149/0000/",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('secondaryInputFolder',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files for secondary files")

options.register('ntupleName',
                 'MuDPGNtuple_trigger', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Name for output ntuple")

options.parseArguments()

print("Input folder:", options.inputFolder)

process = cms.Process("MUNTUPLES",eras.Run3)#Run2_2018)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#SkipEvent = cms.untracked.vstring('ProductNotFound')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))


#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag, '')

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(),
        secondaryFileNames = cms.untracked.vstring()
)

if "eos/cms" in options.inputFolder:
    #files = subprocess.check_output(['xrdfs', 'root://eoscms.cern.ch/', 'ls', options.inputFolder]) ## Did work with CMSSW 11XX, not anymore w CMSSW 12
    files = os.listdir(options.inputFolder)
    #process.source.fileNames = ["file:"+options.inputFolder + f for f in files if "07a64f0e-25eb-40b6-b2a6-e8971a4e0ce8.root" in f]
    #process.source.fileNames = ["root://cms-xrd-global.cern.ch//store/data/Run2023C/Muon0/RAW-RECO/ZMu-PromptReco-v4/000/368/567/00000/23a959f6-169b-4ec6-aaec-23c1f9683cb2.root"]
    #process.source.fileNames = ["file:/eos/cms/store/group/dpg_gem/comm_gem/P5_Commissioning/2023/RPCMonitor/RAW/v2/000/367/758/00000/01e92558-6268-4891-a6bc-22859c27bde2.root"]
    #process.source.fileNames = ["file:/eos/cms/tier0/store/data/Run2024H/RPCMonitor/RAW/v1/000/385/934/00000/00bb1881-cb74-4c9e-84c8-221f9e2db0ad.root"]
    #process.source.fileNames = ["file:/eos/cms/tier0/store/data/Run2024I/RPCMonitor/RAW/v1/000/386/951/00000/9fe45216-b009-41a5-9bdf-191ffb554adb.root"]
    process.source.fileNames = ["root://xrootd-cms.infn.it//store/data/Run2024I/RPCMonitor/RAW/v1/000/386/924/00000/381ebcc1-6cb7-4f1d-b804-40cb25f6ded5.root"]
    #process.source.fileNames = ["file:/eos/cms/tier0/store/data/Run2024H/Muon0/RAW/v1/000/385/841/00000/00a80358-0924-45b8-b965-24e39acdf2af.root"]
    #process.source.fileNames = ["file:/eos/home-i/iawatson/cscfeds/outputRPCMON.root"]

elif "/xrd/" in options.inputFolder:
    files = subprocess.check_output(['xrdfs', 'root://cms-xrdr.sdfarm.kr/', 'ls', options.inputFolder])
    process.source.fileNames = ["root://cms-xrdr.sdfarm.kr//" +f for f in files.split()]

else:
    files = subprocess.check_output(['ls', options.inputFolder])
    process.source.fileNames = ["file://" + options.inputFolder + "/" + f for f in files.split()]

if options.secondaryInputFolder != "" :
    files = subprocess.check_output(["ls", options.secondaryInputFolder])
    process.source.secondaryFileNames = ["file://" + options.secondaryInputFolder + "/" + f for f in files.split()]


if options.STA == False:
    process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(options.ntupleName+".root")
    )
else:
    process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(options.ntupleName+"_STA.root")
    )

from RecoMuon.Configuration.RecoMuon_cff import *
#from RecoVertex.BeamSpotProducer.BeamSpot_cff import offlineBeamSpot
process.source.inputCommands = cms.untracked.vstring(
        'keep *',
        'drop *_hltGtStage2Digis_*_HLT',
        )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('RecoLocalMuon.GEMRecHit.gemRecHits_cfi')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('RecoMuon.StandAloneMuonProducer.standAloneMuons_cfi')
process.load('RecoMuon.Configuration.RecoMuonPPonly_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load('MuDPGAnalysis.MuonDPGNtuples.muNtupleProducer_cfi')

process.standAloneMuons = process.standAloneMuons.clone()
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.EnableGEMMeasurement = cms.bool(False)
process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.EnableGEMMeasurement = cms.bool(False)

#process.standAloneMuons.STATrajBuilderParameters.FilterParameters.CSCRecSegmentLabel = cms.InputTag("hltCscSegments")
#process.standAloneMuons.STATrajBuilderParameters.FilterParameters.RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
#process.standAloneMuons.STATrajBuilderParameters.FilterParameters.DTRecSegmentLabel = cms.InputTag("hltDt4DSegments")
#process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
#process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.CSCRecSegmentLabel = cms.InputTag("hltCscSegments")
#process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.DTRecSegmentLabel = cms.InputTag("hltDt4DSegments")
#ancientMuonSeed.EnableDTMeasurement = False
#ancientMuonSeed.CSCRecSegmentLabel = "hltCscSegments"
muonstandalonereco = cms.Sequence(process.offlineBeamSpot + standAloneMuonSeeds * process.standAloneMuons)


process.muNtupleProducer.isMC = cms.bool(options.isMC)
process.muNtupleProducer.storeOHStatus = cms.bool(options.storeOHStatus)
process.muNtupleProducer.storeAMCStatus = cms.bool(options.storeAMCStatus)
process.muNtupleProducer.STA = cms.bool(options.STA)
process.muNtupleProducer.CSClct = cms.bool(options.CSClct)
process.muNtupleProducer.displacement = cms.double(options.displacement)

if options.reUnpack and options.GE21:
    process.gemRecHits.ge21Off = cms.bool(not options.GE21) ## user selection GE21 = True means "store GE21 rechits"
    process.p = cms.Path(
        process.muonGEMDigis *
        process.gemRecHits *
        process.muNtupleProducer)
elif options.reUnpack:
    process.p = cms.Path(
        process.muonGEMDigis *
        process.muNtupleProducer)
    #elif options.CSClct:
    # TO STORE OUTPUT FILE
    #process.gino = cms.OutputModule("PoolOutputModule", outputCommands = cms.untracked.vstring("keep *_*_*_*"), fileName=cms.untracked.string("out.root"))
    #process.p = cms.Path(process.muonCSCDigis * process.muNtupleProducer)
    # TO STORE OUTPUT FILE
    #process.this_is_the_end = cms.EndPath(process.gino)
elif options.STA:
    #process.muNtupleProducer.muonTag = cms.untracked.InputTag("TestSTA")

    ## TCDS Unpacker
    process.tcdsDigis = cms.EDProducer("TcdsRawToDigi",
        InputLabel=cms.InputTag("hltFEDSelectorTCDS")
    )
    process.muNtupleProducer.tcdsTag = cms.untracked.InputTag("tcdsDigis", "tcdsRecord")

    """ Run unpacker and local reconstruction for the four stations,
    then run standalone muon fitting. Needed because the RPCMonitor stream only contains FEDRaw data now.
    Copied from RPC DPG:
    https://gitlab.cern.ch/CMSRPCDPG/OfflineRPCEfficiency/-/blob/run3_offlineAna_12_5_3/DQM/RPCMonitorModule/test/run3_2024RPCMon_RAW.py?ref_type=heads
    and also partially explained here:
    https://gitlab.cern.ch/CMSRPCDPG/OfflineRPCEfficiency/-/blob/7555926c050dbf0542c3977cd9a02ecf3ac861f0/README.md
    """

    
    # RPC unpacker
    ### RPC RawToDigi - from TwinMux
    process.muonRPCDigisTwinMux = cms.EDProducer("RPCTwinMuxRawToDigi",
            bxMax = cms.int32(2),
            bxMin = cms.int32(-2),
            calculateCRC = cms.bool(True),
            fillCounters = cms.bool(True),
            inputTag = cms.InputTag("hltFEDSelectorTwinMux"),
            )
    # alternative way? to be checked
    #process.load("EventFilter.RPCRawToDigi.RPCTwinMuxRawToDigi_cff")
    process.load("EventFilter.RPCRawToDigi.RPCDigiMerger_cff")
    process.rpcDigiMerger.inputTagTwinMuxDigis = 'muonRPCDigisTwinMux'	#'rpcTwinMuxRawToDigi'

    # RPC local reco - digi to rechits
    process.load("RecoLocalMuon.Configuration.RecoLocalMuon_cff")
    process.rpcRecHits.rpcDigiLabel = cms.InputTag('rpcDigiMerger')

    ## GEM Unpacker
    process.muonGEMDigis.InputLabel = cms.InputTag("hltFEDSelectorGEM")
    process.muonGEMDigis.fedIdEnd = cms.uint32(1478)
    process.muonGEMDigis.fedIdStart = cms.uint32(1467)
    process.muonGEMDigis.ge21Off = cms.bool(False)
    process.muonGEMDigis.keepDAQStatus = cms.bool(True)
    process.muonGEMDigis.readMultiBX = cms.bool(False)
    process.muonGEMDigis.useDBEMap = cms.bool(True)
    process.muNtupleProducer.gemDigiTag = cms.untracked.InputTag("muonGEMDigis")

    ## GEM Local Reco
    process.gemRecHits.gemDigiLabel = cms.InputTag("muonGEMDigis")
    process.gemRecHits.ge21Off = cms.bool(False)
    process.muNtupleProducer.gemRecHitTag = cms.untracked.InputTag("gemRecHits")

    process.load("EventFilter.CSCRawToDigi.muonCSCDCCUnpacker_cfi")
    process.muonCSCDigis.useGEMs = True
    process.muonCSCDigis.useCSCShowers = True
    process.muonCSCDigis.InputObjects = cms.InputTag("hltFEDSelectorCSC")
    process.load("RecoLocalMuon.CSCRecHitD.cscRecHitD_cfi")
    process.load("RecoLocalMuon.CSCSegment.cscSegments_cfi")
    
    process.load("CalibMuon.CSCCalibration.CSCChannelMapper_cfi")
    process.load("CalibMuon.CSCCalibration.CSCIndexer_cfi")

    process.csc2DRecHits.CSCStripPeakThreshold = cms.double( 10.0 )
    process.csc2DRecHits.CSCStripClusterChargeCut = cms.double( 25.0 )
    process.csc2DRecHits.CSCStripxtalksOffset = cms.double( 0.03 )
    process.csc2DRecHits.UseAverageTime = cms.bool( False )
    process.csc2DRecHits.UseParabolaFit = cms.bool( False )
    process.csc2DRecHits.UseFivePoleFit = cms.bool( True )
    process.csc2DRecHits.CSCWireClusterDeltaT = cms.int32( 1 )
    process.csc2DRecHits.CSCUseCalibrations = cms.bool( True )
    process.csc2DRecHits.CSCUseStaticPedestals = cms.bool( False )
    process.csc2DRecHits.CSCNoOfTimeBinsForDynamicPedestal = cms.int32( 2 )
    process.csc2DRecHits.readBadChannels = cms.bool( False )
    process.csc2DRecHits.readBadChambers = cms.bool( True )
    process.csc2DRecHits.CSCUseTimingCorrections = cms.bool( True )
    process.csc2DRecHits.CSCUseGasGainCorrections = cms.bool( False )
    process.csc2DRecHits.CSCDebug = cms.untracked.bool( False )
    process.csc2DRecHits.CSCstripWireDeltaTime = cms.int32( 8 )
    process.csc2DRecHits.XTasymmetry_ME1a = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME1b = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME12 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME13 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME21 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME22 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME31 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME32 = cms.double( 0.0 )
    process.csc2DRecHits.XTasymmetry_ME41 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME1a = cms.double( 0.022 )
    process.csc2DRecHits.ConstSyst_ME1b = cms.double( 0.007 )
    process.csc2DRecHits.ConstSyst_ME12 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME13 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME21 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME22 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME31 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME32 = cms.double( 0.0 )
    process.csc2DRecHits.ConstSyst_ME41 = cms.double( 0.0 )
    process.csc2DRecHits.NoiseLevel_ME1a = cms.double( 7.0 )
    process.csc2DRecHits.NoiseLevel_ME1b = cms.double( 8.0 )
    process.csc2DRecHits.NoiseLevel_ME12 = cms.double( 9.0 )
    process.csc2DRecHits.NoiseLevel_ME13 = cms.double( 8.0 )
    process.csc2DRecHits.NoiseLevel_ME21 = cms.double( 9.0 )
    process.csc2DRecHits.NoiseLevel_ME22 = cms.double( 9.0 )
    process.csc2DRecHits.NoiseLevel_ME31 = cms.double( 9.0 )
    process.csc2DRecHits.NoiseLevel_ME32 = cms.double( 9.0 )
    process.csc2DRecHits.NoiseLevel_ME41 = cms.double( 9.0 )
    process.csc2DRecHits.CSCUseReducedWireTimeWindow = cms.bool( False )
    process.csc2DRecHits.CSCWireTimeWindowLow = cms.int32( 0 )
    process.csc2DRecHits.CSCWireTimeWindowHigh = cms.int32( 15 )

    # Unpack DT
    process.muonDTDigis = cms.EDProducer( "DTuROSRawToDigi",
            inputLabel = cms.InputTag( "hltFEDSelectorDT" ),
            debug = cms.untracked.bool( False )
            )
    # DT rechits
    process.dt1DRecHits.dtDigiLabel = cms.InputTag('muonDTDigis')
    process.dt1DCosmicRecHits.dtDigiLabel = cms.InputTag('muonDTDigis')
    # DT segments
    process.dt4DSegments.dtDigiLabel = cms.InputTag('muonDTDigis')
    process.dt4DCosmicSegments.dtDigiLabel = cms.InputTag('muonDTDigis')

    # TO STORE OUTPUT FILE
    #process.gino = cms.OutputModule("PoolOutputModule", outputCommands = cms.untracked.vstring("keep *_*_*_*"), fileName=cms.untracked.string("out.root"))
    process.p = cms.Path(
        process.tcdsDigis *
        process.muonGEMDigis * process.gemRecHits *
        process.muonRPCDigisTwinMux * process.rpcDigiMerger * process.rpcRecHits *
        process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *
        process.muonDTDigis * process.dt1DRecHits * process.dt4DSegments *
        muonstandalonereco *
        process.muNtupleProducer
        )
    # TO STORE OUTPUT FILE
    #process.this_is_the_end = cms.EndPath(process.gino)

    # TO STORE OUTPUT FILE
    #process.this_is_the_end = cms.EndPath(process.gino)
else:
    process.p = cms.Path(
        process.muNtupleProducer)
