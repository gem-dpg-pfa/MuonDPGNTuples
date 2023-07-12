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
  
  unsigned int m_nGEMDigis;

  // DET ID
  std::vector<int> m_station;
  std::vector<int> m_endcap;
  std::vector<int> m_chamber;
  std::vector<int> m_layer;
  std::vector<int> m_ring;  
  // GEM Pad DIGI
  std::vector<int16_t>  m_GEM1_bx;
  std::vector<int16_t>  m_GEM2_bx;
  std::vector<uint16_t> m_GEM1_pad;
  std::vector<uint16_t> m_GEM2_pad;
  std::vector<int> m_GEM1_nPartitions;  
  std::vector<int> m_GEM2_nPartitions;
  
};

#endif
