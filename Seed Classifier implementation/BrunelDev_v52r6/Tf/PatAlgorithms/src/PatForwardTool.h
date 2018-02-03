#ifndef PATFORWARDTOOL_H
#define PATFORWARDTOOL_H 1

// Include files
#include "boost/range/iterator_range.hpp"
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/extends.h"
#include "GaudiKernel/IIncidentListener.h"

#include "TrackInterfaces/IPatForwardTool.h"
#include "Event/STCluster.h"
#include "TrackInterfaces/IAddTTClusterTool.h"
#include "TrackInterfaces/ITracksFromTrack.h"
#include "TrackInterfaces/ITrackSelector.h"


#include "Kernel/IUsedLHCbID.h"

#include "STDet/DeSTDetector.h"

#include "Event/Track.h"

#include "PatFwdTool.h"
#include "PatRange.h"
#include "PatFwdTrackCandidate.h"

#include "PatKernel/PatTStationHitManager.h"
#include "PatKernel/PatForwardHit.h"

//For convenient trailing-return-types in C++11:
#define AUTO_RETURN(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) {return (__VA_ARGS__);}

/** @class PatForwardTool PatForwardTool.h
 *  Tool to extend one Velo (VeloTT) track through the magnet
 *
 *  @author Olivier Callot
 *  @date   2005-10-04 Initial version
 *  @date   2007-08-20 Update for A-Team framework
 */

class IClassifierReader;

class PatForwardTool : public extends<GaudiTool,IPatForwardTool,ITracksFromTrack,IIncidentListener> {
public:

  /// Standard constructor
  PatForwardTool( const std::string& type,
                  const std::string& name,
                  const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  void forwardTrack( const LHCb::Track& track, LHCb::Tracks& output ) const override;

  StatusCode tracksFromTrack( const LHCb::Track& seed, std::vector<LHCb::Track*>& output ) const override;


  // added for NNTools
  void setNNSwitch( bool nnSwitch) override { m_nnSwitch = nnSwitch;}

  void handle ( const Incident& incident ) override;

private:
  bool acceptTrack(const LHCb::Track& track) const;
  void prepareHits() const;

  std::vector<PatFwdTrackCandidate> buildXCandidatesList(PatFwdTrackCandidate& track , boost::iterator_range<typename PatFwdHits::const_iterator> &rng) const;

  template <typename Storage = double>
  class XInterval {
    Storage m_zMagnet,m_xMagnet,m_txMin,m_txMax,m_xmin,m_xmax;
  public:
    XInterval(Storage zMagnet, Storage xMagnet, Storage txMin, Storage txMax, Storage xMinRef, Storage xMaxRef)
      : m_zMagnet{zMagnet}, m_xMagnet{xMagnet}, m_txMin{txMin}, m_txMax{txMax}, m_xmin {xMinRef},m_xmax{xMaxRef} {}
    template <typename T> auto xMinAtZ(T z) const AUTO_RETURN( m_txMin*(z-m_zMagnet)+m_xMagnet )
    template <typename T> auto xMaxAtZ(T z) const AUTO_RETURN( m_txMax*(z-m_zMagnet)+m_xMagnet )
    //== This is the range at the reference plane
    auto xMin() const AUTO_RETURN( m_xmin )
    auto xMax() const AUTO_RETURN( m_xmax )
    template <typename T> auto inside(T x) const AUTO_RETURN( m_xmin <= x && x < m_xmax )
    template <typename T> auto outside(T x) const AUTO_RETURN( x < m_xmin || m_xmax <= x )
    template <typename Range, typename Projection>
    Range inside(const Range& r, Projection p) const {
      // TODO: linear search from the edges is probably faster given the typical input...
      auto f = std::partition_point( std::begin(r), std::end(r),
                                     [&]( const typename Range::value_type& i ) { return p(i)<m_xmin; } );
      auto l = std::partition_point( f, std::end(r),
                                     [&]( const typename Range::value_type& i ) { return p(i)<m_xmax; } );
      return { f, l };
    }
  };


  template <typename T> T dSlope_kick( T pt, T sinTrack ) const {
    return sinTrack * m_magnetKickParams.value().first / ( pt - sinTrack * m_magnetKickParams.value().second );
  }

  template <typename T = double>
  XInterval<T> make_XInterval(const PatFwdTrackCandidate& track) const {
    double xExtrap = track.xStraight( m_fwdTool->zReference() );
    //== calculate center of magnet from Velo track
    const double zMagnet =  m_fwdTool->zMagnet( track );
    //== calculate if minPt or minMomentum sets the window size
    const double dSlope =  dSlope_kick( m_minPt.value(), track.sinTrack() );
    const double dz     = m_fwdTool->zReference() - zMagnet;
    const double maxRange = dSlope*dz;
    double xMin = xExtrap - maxRange;
    double dSlopeMin = -dSlope;
    double dSlopeMax =  dSlope;
    double xMax = xExtrap + maxRange;

    //== based on momentum a wrong-charge sign window size is defined
    if (m_useMomentumEstimate && !m_withoutBField && track.qOverP() != 0 ) {
      bool useKick { m_UseWrongSignWindow && track.track()->pt()>m_WrongSignPT };
      double kickRange = useKick ? dSlope_kick(m_WrongSignPT.value(), track.sinTrack())*dz : 0;
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) {
        debug() << "   xExtrap = " << xExtrap
                << " q/p " << track.qOverP()
                << " predict " << xExtrap + kickRange << endmsg;
      }
      //== In the case of a given charge estimate, the search window is not symmetric
      //== around the velo track extrapolation.
      if ( std::signbit( track.qOverP() ) != std::signbit( m_fwdTool->magscalefactor() ) ) {
        xMin = xExtrap - kickRange;
        dSlopeMin = -kickRange/dz;
      } else {
        xMax = xExtrap + kickRange;
        dSlopeMax = kickRange/dz;
      }
    }else if ( m_useProperMomentumEstimate && !m_withoutBField && track.qOverP() != 0 ) {
      auto q = track.qOverP() > 0. ? 1. : -1.;
      auto kick =  -q * m_fwdTool->magscalefactor() * dSlope_kick(std::abs( track.sinTrack()/track.qOverP()), track.sinTrack() );
      auto kickError =  m_minRange/dz + m_momentumEstimateError * std::abs(kick);
      dSlopeMin = kick - kickError;
      dSlopeMax = kick + kickError;
      xMin = xExtrap + dSlopeMin * dz;
      xMax = xExtrap + dSlopeMax * dz;;
    }
    // compute parameters of deltaX as a function of z
    return { zMagnet, track.xStraight( zMagnet ),
          track.slX()+dSlopeMin,
          track.slX()+dSlopeMax,
          xMin, xMax };
  }


  boost::iterator_range<typename PatFwdHits::const_iterator>
  fillXList( PatFwdTrackCandidate& track ) const;

  bool fillStereoList( PatFwdTrackCandidate& track, double tol ) const;

  void debugFwdHits( const PatFwdTrackCandidate& track ) const;

  void debugFwdHits( const PatFwdTrackCandidate& track, MsgStream& msg ) const;


  bool driftInRange( const PatFwdHit& hit )  const {
    auto drift = hit.driftDistance();
    return m_minOTDrift < drift && drift < m_maxOTDrift ;
  }

  double allowedXSpread(const PatFwdHit *hit, double xExtrap ) const {
    auto spreadSl = ( hit->projection() - xExtrap ) * m_maxSpreadSlopeX;
    return m_maxSpreadX +
        fabs( spreadSl ) +
        int(hit->isOT()) * 1.5;  // OT drift ambiguities...
  }

  double allowedStereoSpread(const PatFwdHit *hit) const {
    // in case of OT, add 1.5 to account for OT drift ambiguities...
    return  3. + int(hit->isOT())*1.5;
  }

  bool inCenter(const PatFwdTrackCandidate& cand) const {
    return  m_centerOTYSize > fabs( cand.y( 0. ) );
  }

  double computeQOverP( const PatFwdTrackCandidate& c) const {
    double qOverP = 1000. * m_fwdTool->qOverP( c );  // in 1/GeV
    if (m_withoutBField) {
      if (m_minMomentum !=0)  qOverP = 1/m_minMomentum;
      else                    qOverP = 1;
    }
    return qOverP;
  }

  double computeQuality(const PatFwdTrackCandidate& c, double qOverP) const {
    double quality  = 5. * fabs(  m_fwdTool->changeInY( c ) ) / ( m_maxDeltaY + qOverP * qOverP * m_maxDeltaYSlope );
    quality += c.chi2PerDoF() / 10.;
    quality += 10 * fabs(qOverP);  // low momentum are worse
    return quality;
  }

  template <typename T> T computeStereoTol( T qOverP) const {
    return m_maxSpreadY + m_maxSpreadSlopeY * qOverP *  qOverP;
  }

  bool hasEnoughStereo( const PatFwdTrackCandidate& c) const {
    // Enough stereo planes
    PatFwdPlaneCounter fullCount( std::begin(c), std::end(c) );
    return fullCount.nbStereo() > 3 ;
  }

  bool passMomentum(const PatFwdTrackCandidate& c, double sinTrack) const {
    const double momentum=1.0/fabs(m_fwdTool->qOverP( c ));
    const double pt = sinTrack*momentum;
    //== reject if below threshold
    return  m_withoutBField || ( momentum>m_minMomentum && pt>m_minPt) ;
  }


  PatFwdTool*                            m_fwdTool = nullptr;        ///< Tool to compute extrapolations of tracks
  Tf::TStationHitManager<PatForwardHit>* m_tHitManager = nullptr;    ///< Tool to provide hits
  IAddTTClusterTool*                     m_addTTClusterTool = nullptr;
  ITrackSelector*                        m_trackSelector = nullptr;
  IUsedLHCbID*                           m_usedLHCbIDTool = nullptr; ///< Tool to check if hits are already being used
  IClassifierReader* m_NNBeforeXFitReader = nullptr;
  IClassifierReader* m_NNAfterStereoFitReader  = nullptr;

  mutable bool m_newEvent;

  bool  m_nnSwitch = false;           // switch on or off NN var. writing
  bool  m_skipUsedHits = false;              // skip hits which are flagged as "used"

  mutable PatFwdHits  m_xHitsAtReference; // workspace

  Gaudi::Property<std::string> m_addTtToolName{ this, "AddTTClusterName", "PatAddTTCoord" };
  Gaudi::Property<std::string> m_trackSelectorName{ this, "TrackSelectorName", "None"};

  //== Parameters of the algorithm
  Gaudi::Property<bool>   m_secondLoop { this, "SecondLoop",  true };
  Gaudi::Property<bool>   m_useMomentumEstimate { this, "UseMomentumEstimate", false };// Uses charge estimate only.
  Gaudi::Property<bool>   m_useProperMomentumEstimate { this, "UseProperMomentumEstimate", false }; // Uses momentum and charge estimate.
  Gaudi::Property<double> m_momentumEstimateError { this, "MomentumEstimateError" , 0.3 };
  Gaudi::Property<double> m_zAfterVelo { this, "ZAfterVelo", 1640. * Gaudi::Units::mm };
  Gaudi::Property<double> m_yCompatibleTol { this, "YCompatibleTol", 10. * Gaudi::Units::mm };
  Gaudi::Property<double> m_yCompatibleTolFinal { this, "YCompatibleTolFinal", 1. * Gaudi::Units::mm };
  Gaudi::Property<double> m_minOTDrift { this, "MinOTDrift", -0.3 * Gaudi::Units::mm  };
  Gaudi::Property<double> m_maxOTDrift { this, "MaxOTDrift",  2.5 * Gaudi::Units::mm  };
  Gaudi::Property<double> m_maxSpreadX { this, "MaxSpreadX",  0.6  };
  Gaudi::Property<double> m_maxSpreadY { this, "MaxSpreadY",  1.5  };
  Gaudi::Property<double> m_maxSpreadSlopeX { this, "MaxSpreadSlopeX", .011  };
  Gaudi::Property<double> m_maxSpreadSlopeY { this, "MaxSpreadSlopeY", 70.   };
  mutable int    m_minXPlanes;
  mutable int    m_minPlanes;
  Gaudi::Property<double> m_minPt { this, "MinPt", 80. * Gaudi::Units::MeV };
  Gaudi::Property<double> m_minMomentum { this, "MinMomentum",  1. * Gaudi::Units::GeV };
  Gaudi::Property<double> m_maxChi2X { this, "MaxChi2X",  40 };
  Gaudi::Property<double> m_maxChi2Y { this, "MaxChi2Y",  40 };
  Gaudi::Property<int>    m_minHits { this, "MinHits",  12 };
  Gaudi::Property<int>    m_minOTHits { this, "MinOTHits",  14 };
  Gaudi::Property<double> m_centerOTYSize { this, "CenterOTYSize", 100 * Gaudi::Units::mm };
  Gaudi::Property<double> m_maxDeltaY { this, "MaxDeltaY",  30. };
  Gaudi::Property<double> m_maxDeltaYSlope { this, "MaxDeltaYSlope", 300. };
  Gaudi::Property<std::vector<int>> m_maxXCandidateSize { this, "MaxXCandidateSize", {70,36,20}};

  Gaudi::Property<std::pair<double,double>>  m_magnetKickParams { this, "MagnetKickParams", { 1255 * Gaudi::Units::MeV, 175 * Gaudi::Units::MeV } };
  Gaudi::Property<double> m_minRange{ this, "MinRange", 100. * Gaudi::Units::mm };//Only used when using momentum estimate.

  // setting the cov matrix of the state
  Gaudi::Property<double> m_stateErrorX2 { this,"StateErrorX2", 4.0};
  Gaudi::Property<double> m_stateErrorY2 { this,"StateErrorY2", 400.};
  Gaudi::Property<double> m_stateErrorTX2 { this,"StateErrorTX2", 6.e-5};
  Gaudi::Property<double> m_stateErrorTY2 { this,"StateErrorTY2", 1.e-4};
  Gaudi::Property<double> m_stateErrorP { this,"StateErrorP", 0.15};

  Gaudi::Property<bool>  m_withoutBField{this, "WithoutBField", false};
  Gaudi::Property<bool>  m_Preselection{this, "Preselection", false};
  Gaudi::Property<double> m_PreselectionPT{this,"PreselectionPT",400.*Gaudi::Units::MeV};
  Gaudi::Property<bool>  m_UseWrongSignWindow{this,"UseWrongSignWindow",false};
  Gaudi::Property<double> m_WrongSignPT{this,"WrongSignPT",2000.*Gaudi::Units::MeV};
  //Gaudi::Property<bool>  m_FlagUsedSeeds{this,"FlagUsedSeeds", false};// flag velo seeds as used if a track is upgraded
  Gaudi::Property<std::vector<std::string>> m_veloVetoTracksNames{ this, "VeloVetoTracksNames", {}};
  Gaudi::Property<std::string> m_LHCbIDToolName { this, "UsedLHCbIDToolName",""};


  Gaudi::Property<bool>   m_NNBeforeXFit { this,  "NNBeforeXFit", true };
  Gaudi::Property<bool>   m_NNAfterStereoFit { this,  "NNAfterStereoFit", true };
  Gaudi::Property<double> m_NNBeforeXFitCut { this,  "NNBeforeXFitCut", 0.12  };
  Gaudi::Property<double> m_NNAfterStereoFitCut { this,  "NNAfterStereoFitCut", 0.1 };
  Gaudi::Property<double> m_NextBestResponse { this,  "NextBestResponse", 0.7 };
};

#endif // PATFORWARDTOOL_H
