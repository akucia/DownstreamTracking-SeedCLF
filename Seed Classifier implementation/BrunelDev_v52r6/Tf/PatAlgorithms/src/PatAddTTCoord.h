#ifndef PATADDTTCOORD_H
#define PATADDTTCOORD_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

#include "Event/Track.h"
#include "Event/State.h"
#include "TrackInterfaces/IAddTTClusterTool.h"            // Interface
#include "PatKernel/PatTTHit.h"
#include "Kernel/ILHCbMagnetSvc.h"
#include "TfKernel/TTStationHitManager.h"
#include "GaudiKernel/IIncidentListener.h"


  /** @class PatAddTTCoord PatAddTTCoord.h
   *
   * \brief  Adds TT clusters to tracks, see note LHCb-INT-2010-20 for the basic principle
   *
   *
   * Parameters:
   * - ZTTField: Z-Position of the kink for the state extrapolation
   * - ZMSPoint: Z-Position of the multiple scattering point
   * - TTParam: Parameter of the slope of the state extrapolation
   * - MaxChi2Tol: Offset of the chi2 cut
   * - MaxChi2Slope: Slope of the chi2 cut
   * - MaxChi2POffset: Momentum offest of the chi2 cut
   * - YTolSlope: Offest of the y-tolerance cut
   * - XTol: Offest of the x-window cut
   * - XTolSlope: Slope of the x-window cut
   * - MajAxProj: Major axis of the ellipse for the cut on the projection
   * - MinAxProj: Minor axis of the ellipse for the cut on the projection
   * - ZTTProj: Z-Position which state has to be closest to
   * The parameters were tuned on MC11a and differ from the ones stated in LHCb-INT-2010-20
   *
   *  @author Olivier Callot
   *  @author Michel De Cian (added method and code restructured)
   *  @date   2014-11-13
   *
   */

class PatAddTTCoord : public extends<GaudiTool, IAddTTClusterTool, IIncidentListener>
{
public:
    /// Standard constructor
  using base_class::base_class;

  StatusCode initialize() override;

  /** @brief Add TT clusters to matched tracks. This calls returnTTClusters internally
      @param track Track to add the TT hits to
  */
  StatusCode addTTClusters( LHCb::Track& track) override;

  /** Return TT clusters without adding them.
      @param state State closest to TT for extrapolation (normally Velo state)
      @param ttHits Container to fill TT hits in
      @param finalChi2 internal chi2 of the TT hit adding
      @param p momentum estimate. If none given, the one from the state will be taken
  */
  StatusCode returnTTClusters( LHCb::State& state, PatTTHits& ttHits, double& finalChi2, double p = 0 ) override;

  void handle ( const Incident& incident ) override;

 private:

  void selectHits(const LHCb::State& state, const double p);
  void calculateChi2(double& chi2, const double& bestChi2, double& finalDist, const double& p);
  void printInfo(double dist, double chi2, const LHCb::State& state);
  void initEvent();

  bool m_newEvent = true;
  double m_invMajAxProj2 = 0;

  Tf::TTStationHitManager<PatTTHit> * m_ttHitManager = nullptr;
  Gaudi::Property<double> m_ttParam { this, "TTParam", 29. };
  Gaudi::Property<double> m_zTTField { this, "ZTTField", 1740. * Gaudi::Units::mm };
  Gaudi::Property<double> m_zTTProj { this, "ZTTProj", 2500. * Gaudi::Units::mm };
  Gaudi::Property<double> m_zMSPoint { this, "ZMSPoint", 400. * Gaudi::Units::mm  };

  Gaudi::Property<double> m_maxChi2Tol { this, "MaxChi2Tol", 2.0 };
  Gaudi::Property<double> m_maxChi2Slope { this, "MaxChi2Slope", 25000 };
  Gaudi::Property<double> m_maxChi2POffset { this, "MaxChi2POffset", 100 };
  Gaudi::Property<double> m_yTolSlope { this, "YTolSlope", 20000. };
  Gaudi::Property<double> m_xTol { this, "XTol", 1.0 };
  Gaudi::Property<double> m_xTolSlope { this, "XTolSlope", 30000.0 };
  Gaudi::Property<double> m_majAxProj { this, "MajAxProj", 20.0  * Gaudi::Units::mm };
  Gaudi::Property<double> m_minAxProj { this, "MinAxProj", 2.0   * Gaudi::Units::mm };

  ILHCbMagnetSvc*     m_magFieldSvc = nullptr;
  double m_bendProtoParam = 0;

  PatTTHits m_selected; // -- The container for all the hits compatible with the track
  PatTTHits m_goodTT; // -- The container with one "group" of hits


};

#endif // PATADDTTCOORD_H
