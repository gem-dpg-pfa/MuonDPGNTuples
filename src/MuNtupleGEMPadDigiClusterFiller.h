#ifndef MuNtuple_MuNtupleGEMPadDigiClusterFiller_h
#define MuNtuple_MuNtupleGEMPadDigiClusterFiller_h


#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiCluster.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiClusterCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class MuNtupleGEMPadDigiClusterFiller : public MuNtupleBaseFiller
{
 public:

  //Constructor
  MuNtupleGEMPadDigiClusterFiller(edm::ConsumesCollector && collector,
			  const std::shared_ptr<MuNtupleConfig> config,
			  std::shared_ptr<TTree> tree, const std::string & label);


  //Destructor
  virtual ~MuNtupleGEMPadDigiClusterFiller();

  virtual void initialize() final; 
  
  virtual void clear() final;

  virtual void fill(const edm::Event & ev) final;

 
 private:

  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;
  edm::EDGetTokenT<GEMPadDigiClusterCollection> m_GEMPadDigiClusterCollection_token_;
  
  unsigned int nClusterCollections;

  // DET ID
    std::vector<unsigned int> m_collectionID;
  std::vector<short int> m_station;
  std::vector<short int> m_endcap;
  std::vector<short int> m_chamber;
  std::vector<short int> m_layer;
  std::vector<short int> m_etaP;  
  // GEM Pad DIGI
  std::vector<int16_t>  m_GEM1_bx;
  std::vector<short int> m_bx;
  std::vector<short int> m_clusterSize;
  std::vector<short int> m_ClusterALCTMatchTime;
  std::vector<short int> m_ClusterFirstPad;
};

#endif
