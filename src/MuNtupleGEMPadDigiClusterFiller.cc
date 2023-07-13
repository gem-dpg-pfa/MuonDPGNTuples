#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMPadDigiClusterFiller.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"


MuNtupleGEMPadDigiClusterFiller::MuNtupleGEMPadDigiClusterFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<MuNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label) : MuNtupleBaseFiller(config, tree, label)
{
  
  edm::InputTag &iTag = m_config->m_inputTags["GEMPadDigiCluster"];
  if (iTag.label() != "none") m_GEMPadDigiClusterCollection_token_ = collector.consumes<GEMPadDigiClusterCollection>(iTag);
}

MuNtupleGEMPadDigiClusterFiller::~MuNtupleGEMPadDigiClusterFiller()
{
  
};

void MuNtupleGEMPadDigiClusterFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nClusters").c_str(), &nClusterCollections, (m_label + "_nClusterCollectionsx/i").c_str());
  m_tree->Branch((m_label + "_ClustersCollectionID").c_str(), &m_collectionID);
  m_tree->Branch((m_label + "_station").c_str(), &m_station);
  m_tree->Branch((m_label + "_region").c_str(), &m_endcap);
  m_tree->Branch((m_label + "_chamber").c_str(), &m_chamber);
  m_tree->Branch((m_label + "_layer").c_str(), &m_layer);
  m_tree->Branch((m_label + "_etaPartition").c_str(), &m_etaP);
  m_tree->Branch((m_label + "_PadBX").c_str(), &m_bx);
  m_tree->Branch((m_label + "_PadClusterSize").c_str(), &m_clusterSize);
  m_tree->Branch((m_label + "_ClusterFirstPad").c_str(), &m_ClusterFirstPad);
  m_tree->Branch((m_label + "_ClusterALCTMatchTime").c_str(), &m_ClusterALCTMatchTime);

}

void MuNtupleGEMPadDigiClusterFiller::clear()
{
    nClusterCollections=0;
  m_collectionID.clear();
  m_station.clear();
  m_endcap.clear();
  m_chamber.clear();
  m_layer.clear();
  m_etaP.clear();
  m_bx.clear();
  m_clusterSize.clear();
  m_ClusterALCTMatchTime.clear();
  m_ClusterFirstPad.clear();
}

void MuNtupleGEMPadDigiClusterFiller::fill(const edm::Event & ev)
{
  
  clear();

  edm::Handle<GEMPadDigiClusterCollection> dataClusters;
  ev.getByToken(m_GEMPadDigiClusterCollection_token_,dataClusters);
    
  
  //LOOP ON THE GEMDigiPadClusterCollection
  for (auto it = dataClusters->begin(); it != dataClusters->end(); it++) {

        const GEMDetId  &gemid = (*it).first;
        //std::cout<<"##########\t\tnClusterCollections "<<nClusterCollections<<std::endl;
        short int station = gemid.station();
        short int endcap = gemid.region();
        short int chamber = gemid.chamber();
        short int layer = gemid.layer();
        short int etaP = gemid.roll();

        GEMPadDigiClusterCollection::Range range = (*it).second;
        //LOOP ON THE GEM CLUSTERS
        for (auto cluster = range.first; cluster != range.second; cluster++) {
            // CLUSTER IS VALID
            //std::cout<<"-------------------------- "<<std::endl;
            short int bx = cluster->bx();
            short int ClusterSize = cluster->pads().size();
            short int ALCTMatchTime = cluster->alctMatchTime();
            short int ClusterFirstPad = cluster->pads().front();

            m_collectionID.push_back(nClusterCollections);
            m_station.push_back(station);
            m_endcap.push_back(endcap);
            m_chamber.push_back(chamber);
            m_layer.push_back(layer);
            m_etaP.push_back(etaP);
            m_bx.push_back(bx);
            m_clusterSize.push_back(ClusterSize);
            m_ClusterALCTMatchTime.push_back(ALCTMatchTime);
            m_ClusterFirstPad.push_back(ClusterFirstPad);

        }
        nClusterCollections ++;
        //std::cout <<  "No more clusters for this GEMID\n\n\n";
        //END LOOP ON THE GEM CLUSTERS
  }
  //END LOOP ON THE GEMDigiPadClusterCollection
  //std::cout<<"\n";
  return;
}
