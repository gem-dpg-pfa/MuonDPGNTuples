#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleCSCCorrelatedLCTFiller.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"


MuNtupleCSCCorrelatedLCTFiller::MuNtupleCSCCorrelatedLCTFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<MuNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label) : MuNtupleBaseFiller(config, tree, label)
{
  
  edm::InputTag &iTag = m_config->m_inputTags["cscLCTDigi"];
  if (iTag.label() != "none") m_CSCcorrLCT_token_ = collector.consumes<CSCCorrelatedLCTDigiCollection>(iTag);
  
}

MuNtupleCSCCorrelatedLCTFiller::~MuNtupleCSCCorrelatedLCTFiller()
{
  
};

void MuNtupleCSCCorrelatedLCTFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nLCT").c_str(), &m_nCSCDigis, (m_label + "_nLCT/i").c_str());
  m_tree->Branch((m_label + "_LCTstation").c_str(), &m_station);
  m_tree->Branch((m_label + "_LCTendcap").c_str(), &m_endcap);
  m_tree->Branch((m_label + "_LCTchamber").c_str(), &m_chamber);
  m_tree->Branch((m_label + "_LCTlayer").c_str(), &m_layer);
  m_tree->Branch((m_label + "_LCTring").c_str(), &m_ring);
  m_tree->Branch((m_label + "_LCTchamberType").c_str(), &m_chamberType);
  m_tree->Branch((m_label + "_LCTtriggerID").c_str(), &m_triggerID);
  m_tree->Branch((m_label + "_LCTtriggerSector").c_str(), &m_triggerSector);
  m_tree->Branch((m_label + "_LCTquality").c_str(), &m_LCTquality); 
  m_tree->Branch((m_label + "_KeyWG").c_str(), &m_KeyWG);
  m_tree->Branch((m_label + "_Strip").c_str(), &m_Strip);
  m_tree->Branch((m_label + "_CLCTquality").c_str(), &m_CLCT_getQuality);
  m_tree->Branch((m_label + "_ALCTquality").c_str(), &m_ALCT_Quality);
  m_tree->Branch((m_label + "_bend").c_str(), &m_bend);
  m_tree->Branch((m_label + "_GEM1_bx").c_str(), &m_GEM1_bx);
  m_tree->Branch((m_label + "_GEM2_bx").c_str(), &m_GEM2_bx);
  m_tree->Branch((m_label + "_GEM1_pad").c_str(), &m_GEM1_pad);
  m_tree->Branch((m_label + "_GEM2_pad").c_str(), &m_GEM2_pad);
  m_tree->Branch((m_label + "_GEM1_nPartitions").c_str(), &m_GEM1_nPartitions);
  m_tree->Branch((m_label + "_GEM2_nPartitions").c_str(), &m_GEM2_nPartitions);

  // m_tree->Branch((m_label + "_GEM1_BX").c_str(), &m_LCTquality); 
  // m_tree->Branch((m_label + "_GEM1_etaPart").c_str(), &m_LCTquality); 
  // m_tree->Branch((m_label + "_GEM1_digiPad").c_str(), &m_LCTquality); 
}

void MuNtupleCSCCorrelatedLCTFiller::clear()
{

  m_nCSCDigis = 0;

  m_station.clear();
  m_endcap.clear();
  m_chamber.clear();
  m_layer.clear();
  m_ring.clear();
  m_chamberType.clear();
  m_triggerID.clear();
  m_triggerSector.clear();
  m_LCTquality.clear();
  m_KeyWG.clear();
  m_Strip.clear();
  m_CLCT_getQuality.clear();
  m_ALCT_Quality.clear();
  m_bend.clear();
  m_GEM1_bx.clear();
  m_GEM2_bx.clear();
  m_GEM1_pad.clear();
  m_GEM2_pad.clear();
  m_GEM1_nPartitions.clear();
  m_GEM2_nPartitions.clear();
}

void MuNtupleCSCCorrelatedLCTFiller::fill(const edm::Event & ev)
{
  
  clear();

  edm::Handle<CSCCorrelatedLCTDigiCollection> CSCcorrLCT_collection;
  ev.getByToken(m_CSCcorrLCT_token_,CSCcorrLCT_collection);
    
    if (CSCcorrLCT_collection.isValid())
      {
	
	for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator cscItr1 = CSCcorrLCT_collection->begin();cscItr1 != CSCcorrLCT_collection->end();cscItr1++) 
	  {
        m_nCSCDigis++;
        
        const CSCDetId  &cscid = (*cscItr1).first;
        int station = cscid.station();
        int endcap = cscid.endcap();
        int chamber = cscid.chamber();
        int layer = cscid.layer();
        int ring = cscid.ring();
        unsigned short chType = cscid.iChamberType();
        std::string chName = cscid.chamberName(); 
        
        
        
        int triggerID = cscid.triggerCscId();
        int triggerSector = cscid.triggerSector();
        CSCCorrelatedLCTDigiCollection::Range range1 = (*cscItr1).second;
        for (CSCCorrelatedLCTDigiCollection::const_iterator lctItr1 = range1.first; lctItr1 != range1.second; lctItr1++) {
            
            // IF VERBOSE
            // std::cout << "Chamber " << chName<< std::endl;
            // std::cout << "CSC QUALITY " << lctItr1->getQuality()<< std::endl;
            int quality = lctItr1->getQuality();
            // std::cout<<"lctItr1->getGEM1().bx() "<<lctItr1->getGEM1().bx()<<std::endl;
            // std::cout<<"lctItr1->getGEM1().pad() "<<lctItr1->getGEM1().pad()<<std::endl;
            // std::cout<<"lctItr1->getGEM1().nPartitions() "<<lctItr1->getGEM1().nPartitions()<<std::endl;
            // std::cout<<"lctItr1->getGEM2().bx() "<<lctItr1->getGEM2().bx()<<std::endl;                          int16_t
            // std::cout<<"lctItr1->getGEM2().pad() "<<lctItr1->getGEM2().pad()<<std::endl;                        uint16_t 	
            // std::cout<<"lctItr1->getGEM2().nPartitions() "<<lctItr1->getGEM2().nPartitions()<<std::endl;        unsigned 
            // std::cout<<"lctItr1->getKeyWG() "<<lctItr1->getKeyWG()<<std::endl;                                  uint16_t
            // std::cout<<"lctItr1->getStrip(8) "<<lctItr1->getStrip(8)<<std::endl;                                uint16_t 	
            // std::cout<<"lctItr1->getCLCT.getQuality() "<<lctItr1->getCLCT().getQuality()<<std::endl;            uint16_t 
            // std::cout<<"lctItr1->getALCT.getQuality() "<<lctItr1->getALCT().getQuality()<<std::endl;            uint16_t 	
                     
            // DET ID
            m_station.push_back(station);
            m_endcap.push_back(endcap);
            m_chamber.push_back(chamber);
            m_layer.push_back(layer);
            m_ring.push_back(ring);
            m_chamberType.push_back(chType);
            m_triggerID.push_back(triggerID);
            m_triggerSector.push_back(triggerSector);
            
            // LCT quality
            m_LCTquality.push_back(quality);
            m_KeyWG.push_back(lctItr1->getKeyWG());
            m_Strip.push_back(lctItr1->getStrip(8));
            m_CLCT_getQuality.push_back(lctItr1->getCLCT().getQuality());
            m_ALCT_Quality.push_back(lctItr1->getALCT().getQuality());
            m_bend.push_back(lctItr1->getBend());
            // GEM 
            m_GEM1_bx.push_back(lctItr1->getGEM1().bx());
            m_GEM2_bx.push_back(lctItr1->getGEM2().bx());
            m_GEM1_pad.push_back(lctItr1->getGEM1().pad());
            m_GEM2_pad.push_back(lctItr1->getGEM2().pad());
            m_GEM1_nPartitions.push_back(lctItr1->getGEM1().bx());
            m_GEM2_nPartitions.push_back(lctItr1->getGEM2().bx());
  
            }

	  }

      }
    else
    {std::cout<< "CSCcorrLCT_collection is invalid" << std::endl;}

  return;

}
