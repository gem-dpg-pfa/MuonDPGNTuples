#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMPadDigiClusterFiller.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"


MuNtupleGEMPadDigiClusterFiller::MuNtupleGEMPadDigiClusterFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<MuNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label) : MuNtupleBaseFiller(config, tree, label)
{
  
  edm::InputTag &iTag = m_config->m_inputTags["GEMPadDigiCluster"];
  std::cout<<iTag<<std::endl;
  if (iTag.label() != "none") m_GEMPadDigiClusterCollection_token_ = collector.consumes<GEMPadDigiClusterCollection>(iTag);
}

MuNtupleGEMPadDigiClusterFiller::~MuNtupleGEMPadDigiClusterFiller()
{
  
};

void MuNtupleGEMPadDigiClusterFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nLCT").c_str(), &m_nGEMDigis, (m_label + "_nGEMDigis/i").c_str());
  // m_tree->Branch((m_label + "_LCTstation").c_str(), &m_station);
  // m_tree->Branch((m_label + "_LCTendcap").c_str(), &m_endcap);
  // m_tree->Branch((m_label + "_LCTchamber").c_str(), &m_chamber);
  // m_tree->Branch((m_label + "_LCTlayer").c_str(), &m_layer);
  // m_tree->Branch((m_label + "_LCTring").c_str(), &m_ring);
  // m_tree->Branch((m_label + "_LCTchamberType").c_str(), &m_chamberType);
  // m_tree->Branch((m_label + "_LCTtriggerID").c_str(), &m_triggerID);
  // m_tree->Branch((m_label + "_LCTtriggerSector").c_str(), &m_triggerSector);
  // m_tree->Branch((m_label + "_LCTquality").c_str(), &m_LCTquality); 
  // m_tree->Branch((m_label + "_KeyWG").c_str(), &m_KeyWG);
  // m_tree->Branch((m_label + "_Strip").c_str(), &m_Strip);
  // m_tree->Branch((m_label + "_CLCTquality").c_str(), &m_CLCT_getQuality);
  // m_tree->Branch((m_label + "_ALCTquality").c_str(), &m_ALCT_Quality);
  // m_tree->Branch((m_label + "_bend").c_str(), &m_bend);
  // m_tree->Branch((m_label + "_GEM1_bx").c_str(), &m_GEM1_bx);
  // m_tree->Branch((m_label + "_GEM2_bx").c_str(), &m_GEM2_bx);
  // m_tree->Branch((m_label + "_GEM1_pad").c_str(), &m_GEM1_pad);
  // m_tree->Branch((m_label + "_GEM2_pad").c_str(), &m_GEM2_pad);
  // m_tree->Branch((m_label + "_GEM1_nPartitions").c_str(), &m_GEM1_nPartitions);
  // m_tree->Branch((m_label + "_GEM2_nPartitions").c_str(), &m_GEM2_nPartitions);

}

void MuNtupleGEMPadDigiClusterFiller::clear()
{

  m_nGEMDigis = 0;

  // m_station.clear();
  // m_endcap.clear();
  // m_chamber.clear();
  // m_layer.clear();
  // m_ring.clear();
  // m_chamberType.clear();
  // m_triggerID.clear();
  // m_triggerSector.clear();
  // m_LCTquality.clear();
  // m_KeyWG.clear();
  // m_Strip.clear();
  // m_CLCT_getQuality.clear();
  // m_ALCT_Quality.clear();
  // m_bend.clear();
  // m_GEM1_bx.clear();
  // m_GEM2_bx.clear();
  // m_GEM1_pad.clear();
  // m_GEM2_pad.clear();
  // m_GEM1_nPartitions.clear();
  // m_GEM2_nPartitions.clear();
}

void MuNtupleGEMPadDigiClusterFiller::fill(const edm::Event & ev)
{
  
  clear();

  edm::Handle<GEMPadDigiClusterCollection> dataClusters;
  ev.getByToken(m_GEMPadDigiClusterCollection_token_,dataClusters);
    
  unsigned int nClusterCollections = 0;
  //LOOP ON THE GEMDigiPadClusterCollection
  for (auto it = dataClusters->begin(); it != dataClusters->end(); it++) {
        nClusterCollections ++;
        const GEMDetId  &gemid = (*it).first;
        std::cout<<"##########\t\tnClusterCollections "<<nClusterCollections<<std::endl;
        int station = gemid.station();
        int endcap = gemid.region();
        int chamber = gemid.chamber();
        int layer = gemid.layer();
        int etaP = gemid.roll();

        std::cout<<"GEMID\tStation " << station << "\tRegion" << endcap <<"\tChamber "<<chamber<<"\tLayer "<< layer<<"\tEtaP "<<etaP<< std::endl;
        GEMPadDigiClusterCollection::Range range = (*it).second;
        //LOOP ON THE GEM CLUSTERS
        for (auto cluster = range.first; cluster != range.second; cluster++) {
            // CLUSTER IS VALID
            std::cout<<"-------------------------- "<<std::endl;
            std::cout<<"Cluster Pads BX "<<cluster->bx()<<std::endl;
            std::cout<<"Cluster Pads Size "<<cluster->pads().size()<<std::endl;
            std::cout<<"Cluster ALCT MatchTime "<<cluster->alctMatchTime()<<std::endl;
            std::cout<<"Cluster Pads Front "<<cluster->pads().front()<<std::endl;
            // std::cout <<"GEMDigiPads values " ;
            // if (cluster->isValid()) {
            //     for (const auto& element : cluster->pads()) {
            //         std::cout << element << " ";
            //     }
            //     std::cout <<  "\n";
            // }
            // END CLUSTER IS VALID
            // else{
            //     std::cout<<"GEMPadDigiCluster is invalid"<<std::endl;
            // }    
        }
        std::cout <<  "No more clusters for this GEMID\n\n\n";
        //END LOOP ON THE GEM CLUSTERS
  }
  //END LOOP ON THE GEMDigiPadClusterCollection
  std::cout<<"\n";
  return;

}
