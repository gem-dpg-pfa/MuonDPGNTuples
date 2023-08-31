#ifndef MuNtuple_MuNtupleCSCCorrelatedLCTFiller_h
#define MuNtuple_MuNtupleCSCCorrelatedLCTFiller_h


#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class MuNtupleCSCCorrelatedLCTFiller : public MuNtupleBaseFiller
{
 public:

  //Constructor
  MuNtupleCSCCorrelatedLCTFiller(edm::ConsumesCollector && collector,
			  const std::shared_ptr<MuNtupleConfig> config,
			  std::shared_ptr<TTree> tree, const std::string & label);


  //Destructor
  virtual ~MuNtupleCSCCorrelatedLCTFiller();

  virtual void initialize() final; 
  
  virtual void clear() final;

  virtual void fill(const edm::Event & ev) final;

 
 private:

  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> m_CSCcorrLCT_token_;
  
  unsigned int m_nCSCDigis;

  // DET ID
  std::vector<int> m_station;
  std::vector<int> m_endcap;
  std::vector<int> m_chamber;
  std::vector<int> m_layer;
  std::vector<int> m_ring;
  std::vector<unsigned short> m_chamberType;
  std::vector<int> m_triggerID;
  std::vector<int> m_triggerSector;
  
  // LCT quality
  std::vector<uint16_t> m_LCTquality;
  std::vector<uint16_t> m_KeyWG;
  std::vector<uint16_t> m_Strip;
  std::vector<uint16_t> m_CLCT_getQuality;
  std::vector<uint16_t> m_ALCT_Quality;
  std::vector<uint16_t> m_bend;
  std::vector<uint16_t> m_slope;
  
  // GEM Pad DIGI
  std::vector<int16_t>  m_GEM1_bx;
  std::vector<int16_t>  m_GEM2_bx;
  std::vector<uint16_t> m_GEM1_pad;
  std::vector<uint16_t> m_GEM2_pad;
  std::vector<int> m_GEM1_nPartitions;  
  std::vector<int> m_GEM2_nPartitions;
  
  
  

//   std::vector<int> m_rechit_region;
//   std::vector<int> m_rechit_station;
//   std::vector<int> m_rechit_chamber;
//   std::vector<int> m_rechit_layer;
//   std::vector<int> m_rechit_etaPartition;

//   std::vector<float>  m_rechit_loc_r;
//   std::vector<float>  m_rechit_loc_phi;
//   std::vector<float>  m_rechit_loc_x;
//   std::vector<float>  m_rechit_loc_y;
//   std::vector<float>  m_rechit_loc_z;

//   std::vector<float>  m_rechit_loc_errX;
//   std::vector<float>  m_rechit_loc_errY;

//   std::vector<float>  m_rechit_g_r;
//   std::vector<float>  m_rechit_g_phi;
//   std::vector<float>  m_rechit_g_x;
//   std::vector<float>  m_rechit_g_y;
//   std::vector<float>  m_rechit_g_z;
  
};

#endif
