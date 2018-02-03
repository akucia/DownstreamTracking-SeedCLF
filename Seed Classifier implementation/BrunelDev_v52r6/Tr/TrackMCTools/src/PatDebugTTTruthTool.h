// $Id: PatDebugTTTruthTool.h,v 1.4 2008-12-04 09:05:07 cattanem Exp $
#ifndef PATDEBUGTTTRUTHTOOL_H
#define PATDEBUGTTTRUTHTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTupleTool.h"
#include "PatKernel/IPatDebugTTTool.h"            // Interface
#include <TfKernel/TTStationHitManager.h>
class DeSTDetector;

/** @class PatDebugTTTruthTool PatDebugTTTruthTool.h
 *
 *
 *  @author Olivier Callot
 *  @date   2007-10-22
 */
class PatDebugTTTruthTool : public GaudiTupleTool, virtual public IPatDebugTTTool {
public:
  /// Standard constructor
  PatDebugTTTruthTool( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  virtual ~PatDebugTTTruthTool( ); ///< Destructor

  StatusCode initialize() override; /// initialize

  void debugTTClusterOnTrack( const LHCb::Track* track,
                              const PatTTHits::const_iterator beginCoord,
                              const PatTTHits::const_iterator endCoord   ) override;

  void debugTTCluster( MsgStream& msg, const PatTTHit* hit ) override;

  bool isTrueHit( const LHCb::Track* track, const PatTTHit* hit) override;

  double fracGoodHits( const LHCb::Track* track, const PatTTHits& hits) override;

  bool isDownRecoble( const LHCb::Track* track, const Tf::TTStationHitManager<PatTTHit>::HitRange& hits, const unsigned int minHits ) override;
  
  bool isTrueTrack( const LHCb::Track* track, const PatTTHits& hits) override;

  bool isTrueTrack( const LHCb::Track* track, const Tf::TTStationHitManager<PatTTHit>::HitRange& hits) override;//AD, overload
  void chi2Tuple( const double p, const double chi2, const unsigned int nHits) override;



  std::map<std::string, bool> getMCTrackInfo(const LHCb::Track* seed) const;
  bool hasMCParticle(const LHCb::Track* seed) const;                                                                                                                                                                             
  bool isDownstreamReconstructible(const LHCb::Track* seed)const ;                                                                                                                                                                                        
  bool hasMCParticleNotElectron(const LHCb::Track* seed)const ;                                                                                                                                                                                                
  bool isDownstreamReconstructibleNotElectron(const LHCb::Track* seed) const;                                                                                                                                                                                     
  bool isTrueSeed(const LHCb::Track* seed) const;                                                                                                                                                                                                                 

  //added by AD 2/1/16 for efficiency vs step

  void initializeSteps(std::vector<std::string> steps) override;//initialize all steps in the process

  void recordStepInProcess(std::string step, bool result) override;//record the result of a step in the process

  void resetflags() override;//reset all flags

  void ForceMCHits(PatTTHits& hits, LHCb::Track* track) override;//Special. Force only MC matched hits in the track.
  
  void seedTuple(const LHCb::Track* trackSeed, double seedClassifierValue) override;
  
  void finalTuple( const bool goodTrack, const LHCb::Track* track, const std::vector<double>& vals ) override;
  
  
  
private:

  DeSTDetector* m_tracker;
  std::map<std::string,bool> m_flags;

};
#endif // PATDEBUGTTTRUTHTOOL_H
