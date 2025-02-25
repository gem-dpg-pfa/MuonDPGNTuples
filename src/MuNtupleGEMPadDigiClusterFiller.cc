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
  m_tree->Branch((m_label + "_ClusterCenter").c_str(), &m_ClusterCenter);
  m_tree->Branch((m_label + "_ClusterALCTMatchTime").c_str(), &m_ClusterALCTMatchTime);

  m_tree->Branch((m_label + "_ClusterLocalX").c_str(), &m_ClusterLocalX);
  m_tree->Branch((m_label + "_ClusterLocalY").c_str(), &m_ClusterLocalY);
  m_tree->Branch((m_label + "_ClusterLocalZ").c_str(), &m_ClusterLocalZ);
  m_tree->Branch((m_label + "_ClusterLocalR").c_str(), &m_ClusterLocalR);
  m_tree->Branch((m_label + "_ClusterLocalPhi").c_str(), &m_ClusterLocalPhi);

  m_tree->Branch((m_label + "_ClusterGlobalX").c_str(), &m_ClusterGlobalX);
  m_tree->Branch((m_label + "_ClusterGlobalY").c_str(), &m_ClusterGlobalY);
  m_tree->Branch((m_label + "_ClusterGlobalZ").c_str(), &m_ClusterGlobalZ);
  m_tree->Branch((m_label + "_ClusterGlobalR").c_str(), &m_ClusterGlobalR);
  m_tree->Branch((m_label + "_ClusterGlobalPhi").c_str(), &m_ClusterGlobalPhi);

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
  m_ClusterCenter.clear();
  m_ClusterLocalX.clear();
  m_ClusterLocalY.clear();
  m_ClusterLocalZ.clear();
  m_ClusterLocalR.clear();
  m_ClusterLocalPhi.clear();
  m_ClusterGlobalX.clear();
  m_ClusterGlobalY.clear();
  m_ClusterGlobalZ.clear();
  m_ClusterGlobalR.clear();
  m_ClusterGlobalPhi.clear();

}

void MuNtupleGEMPadDigiClusterFiller::fill(const edm::Event & ev)
{
  
  clear();

  edm::Handle<GEMPadDigiClusterCollection> dataClusters;
  ev.getByToken(m_GEMPadDigiClusterCollection_token_,dataClusters);

  edm::ESHandle<GEMGeometry> gem = m_config->m_gemGeometry;
  if (not gem.isValid()) {
    std::cout << "GEMGeometry is invalid" << std::endl;
    return;
  }
  
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
            m_ClusterCenter.push_back(ClusterFirstPad + ClusterSize * 0.5 - 0.5);

            /**
             * Calculate pad local coordinates from eta partition geometry
             */
            auto etaPartition = gem->etaPartition(gemid);
            LocalPoint padClusterLocalPoint = etaPartition->centreOfPad(m_ClusterCenter.back());
            auto etaPartitionSurface = gem->idToDet(gemid)->surface();
            auto padClusterGlobalPoint = etaPartitionSurface.toGlobal(padClusterLocalPoint);

            m_ClusterLocalX.push_back(padClusterLocalPoint.x());
            m_ClusterLocalY.push_back(padClusterLocalPoint.y());
            m_ClusterLocalZ.push_back(padClusterLocalPoint.z());
            m_ClusterLocalR.push_back(padClusterLocalPoint.perp());
            m_ClusterLocalPhi.push_back(padClusterLocalPoint.phi());

            m_ClusterGlobalX.push_back(padClusterGlobalPoint.x());
            m_ClusterGlobalY.push_back(padClusterGlobalPoint.y());
            m_ClusterGlobalZ.push_back(padClusterGlobalPoint.z());
            m_ClusterGlobalR.push_back(padClusterGlobalPoint.perp());
            m_ClusterGlobalPhi.push_back(padClusterGlobalPoint.phi());
 
        }
        nClusterCollections ++;
        //std::cout <<  "No more clusters for this GEMID\n\n\n";
        //END LOOP ON THE GEM CLUSTERS
  }
  //END LOOP ON THE GEMDigiPadClusterCollection
  //std::cout<<"\n";
  return;
}
