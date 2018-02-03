#ifndef INCLUDE_PATSEEDFIT_H
#define INCLUDE_PATSEEDFIT_H 1

#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IIncidentListener.h"
#include "TrackInterfaces/IPatSeedFit.h"
#include "Kernel/ILHCbMagnetSvc.h"
#include "Event/STLiteCluster.h"

#include "PatSeedTool.h"


// forward declarations
namespace Tf {
  struct IOTHitCreator;
}
class PatForwardHit;
typedef PatForwardHit PatFwdHit;

/** @class PatSeedFit PatSeedFit.h
 *
 * provide a convenient interface to the internal fit used in the PatSeeding
 * algorithm in the pattern reco
 *
 * @author Manuel Tobias Schiller <schiller@physi.uni-heidelberg.de>
 * @date   2009-01-28
 */
class PatSeedFit : public extends<GaudiTool, IPatSeedFit, IIncidentListener> {
public:
  using base_class::base_class;

  StatusCode initialize() override; ///< Tool initialization
  StatusCode   finalize() override; ///< Tool finalize

  StatusCode fitSeed( const std::vector<LHCb::LHCbID> lhcbIDs,
                      std::vector<LHCb::State> *states) const override;

  /// fit a PatSeedTrack
  StatusCode fitTrack( PatSeedTrack& track, double maxChi2,
                       unsigned minPlanes, bool xOnly, bool forceDebug ) const override;

private:
  /// patseed tool (handles the actual track fit)
  PatSeedTool* m_patSeedTool = nullptr;
  /// IT detector
  DeSTDetector* m_itDet  = nullptr;
  /// OT hit creator
  const Tf::IOTHitCreator* m_othitcreator = nullptr;
  /// momentum estimator
  const ITrackMomentumEstimate *m_momentumTool  = nullptr;
  /// magnetic field service
  ILHCbMagnetSvc*  m_magFieldSvc = nullptr;
  /// cache for STLiteClusters (so we do not have to reget them in every event)
  mutable LHCb::STLiteCluster::FastContainer* m_stLiteContainer = nullptr;

  Gaudi::Property<double> m_stateErrorX2 { this,  "StateErrorX2", 4. };
  Gaudi::Property<double> m_stateErrorY2 { this,  "StateErrorY2", 400. };
  Gaudi::Property<double> m_stateErrorTX2 { this,  "StateErrorTX2", 6e-5 };
  Gaudi::Property<double> m_stateErrorTY2 { this,  "StateErrorTY2", 1e-4 };
  Gaudi::Property<double> m_dRatio { this,  "PatSeedDRatio", -3.2265e-4 };
  Gaudi::Property<double> m_initialArrow { this,  "PatSeedInitialArrow", 4.25307e-9 };
  Gaudi::Property<double> m_maxChi2 { this,  "TrackMaxChi2", -1.  };
  Gaudi::Property<double> m_zReference { this,  "zReference",StateParameters::ZMidT };
  Gaudi::Property<double> m_momentumScale { this,  "MomentumScale",	35.31328};

  /// return a track constructed from an ITOT stub
  /** construct a track from an ITOT stub
   * @param hits   list of hits from which to construct the track
   * @param staIT  station in IT which has enough hits to form a stub
   *               (if -1, such a station is unknown or unavailable, and
   *               the method falls back on using whatever hits are available)
   * @returns a PatSeedTrack
   */
  template<PatSeedTool::TrackType tracktype>
  PatSeedTrack getTrackITOT(std::vector<PatFwdHit>& hits, int staIT = -1) const;
  /// return a track constructed by fitting the x only projection first then y
  /** construct a track from x hits in three different stations, then adding
   * stereo hits
   * @param hits   list of hits from which to construct the track
   * @returns a PatSeedTrack
   */
  template<PatSeedTool::TrackType tracktype>
  PatSeedTrack getTrackXY(std::vector<PatFwdHit>& hits) const;

  /// make an OT cluster from ihit and a suitable one in hits (if possible)
  void makeCluster(std::vector<PatFwdHit>& hits, PatFwdHit& ihit,
      const PatFwdHit*& in, double& x, double& z, bool& isCluster) const;

  /// handle incidents (specifically clear m_stLiteContainer at EndEvent)
  void handle(const Incident& incident) override;
};
#endif // INCLUDE_PATSEEDFIT_H
