#ifndef PATLONGLIVEDTRACKING_H
#define PATLONGLIVEDTRACKING_H 1

// Include files
#include <string>
#include <vector>
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/ILHCbMagnetSvc.h"
#include "GaudiAlg/ISequencerTimerTool.h"

#include "TfKernel/TTStationHitManager.h"
#include "PatKernel/PatTTHit.h"

#include "vdt/vdtMath.h"
#include "TMVA/PatLLT/PatLLT_MLPBNN.C"

// forward declarations
class IPatDebugTTTool;
class PatDownTrack;

namespace LHCb {
  class Track;
}

/** @class PatLongLivedTracking PatLongLiveTracking.h
 *  Algorithm to reconstruct tracks with seed and TT, for daughters of long lived particles like Kshort, Lambda, ...
 *  Partial reimplementation of PatDownstream
 *
 * Parameters:
 * - InputLocation: Input location of seed tracks
 * - OutputLocation: Output location of downstream tracks.
 * - ForwardLocation: Location of forward tracks, for hit rejection
 * - MatchLocation: Location of match tracks, for seed rejection
 * - XPredTolConst: x-window for preselection is XPredTolConst/p + XPredTolOffset
 * - XPredTolOffset: x-window for preselection is XPredTolConst/p + XPredTolOffset
 * - TolMatchConst: x-window for matching x hits is TolMatchConst/p + TolMatchOffset
 * - TolMatchOffset: x-window for matching x hits is TolMatchConst/p + TolMatchOffset
 * - MaxWindowSize: maximum window size for matching x hits
 * - TolUConst: window for u hits is TolUConst/p + TolUOffset
 * - TolUOffset: window for u hits is TolUConst/p + TolUOffset
 * - TolVConst: window for v hits is TolVConst/p + TolVOffset
 * - TolVOffset: window for v hits is TolVConst/p + TolVOffset
 * - MaxChi2: Maximum chi2 for tracks with at least 4 hits
 * - MaxChi2ThreeHits: Maximum chi2 for tracks with 3 hits
 * - MinPt: Minimum pT of the track
 * - MinMomentum: Minimum momentum of the track
 * - MinTTx: half-width  of beampipe rectangle
 * - MinTTy: half-height of of beampipe rectangle
 * - ZMagnetParams: Parameters to determine the z-position of the magnet point. Tune with PatLongLivedParams.
 * - MomentumParams: Parameters to determine the momentum. Tune with PatLongLivedParams.
 * - YParams: Parameters to determine the bending in y.  Tune with PatLongLivedParams.
 * - ZTT: z-position of middle of TT.
 * - ZTTa: z-position of first TT station
 * - MaxDeltaPConst: Window for deltaP is: MaxDeltaPConst*p + MaxDeltaPOffset
 * - MaxDeltaPOffset: Window for deltaP is: MaxDeltaPConst*p + MaxDeltaPOffset
 * - XCorrectionConst: Correction for x-position of search window in preselection is XCorrectionConst/p + XCorrestionOffset
 * - XCorrectionOffset: Correction for x-position of search window in preselection is XCorrectionConst/p + XCorrestionOffset
 * - MaxXTracks: Maximum number of x-tracklets to process further
 * - MaxChi2DistXTracks: Maximum chi2 difference to x-tracklet with best chi2
 * - MaxXUTracks:  Maximum number of xu-tracklets to process further
 * - MinChi2XProjection: Maximum chi2 of x-tracklet
 * - OverlapTol: Tolerance for adding overlap hits
 * - YTol: YTolerance for adding / removing hits.
 * - NMinHighThresHits: Minimum number of hits on track which have the high-threshold bit set in TT
 * - SeedCut: Cut on seed-track classifier to reject bad seed tracks.
 * - FinalMvaCut: Cut on the final classifier
 * - FinalMvaSpread: Spread of allowed MVA values of the final classifier to accept the candidate (only makes sense when MaxOutputCands > 1).
 * - MaxOutputCands: Maximum number of candidates that are stored.
 * - StateErrorX2: Error^2 on x-position (for making LHCb::Track)
 * - StateErrorY2: Error^2 on y-position (for making LHCb::Track)
 * - StateErrorTX2: Error^2 on tx-slope (for making LHCb::Track)
 * - StateErrorTY2: Error^2 on ty-slope (for making LHCb::Track)
 * - StateErrorP:  Error^2 on momentum (for making LHCb::Track)
 * - RemoveUsed: Remove seed tracks and used TT hits (with chi2-cut on long track)?
 * - RemoveAll: Remove seed tracks and used TT hits (withoug chi2-cut on long track)?
 * - LongChi2: Chi2-cut for the removal
 * - SeedKey: Key of MCParticle for debugging
 * - WithDebugTool: Use the debug tool?
 * - DebugTool: Name of the debug tool
 * - PrintTracks: Print the tracks?
 * - TimingMeasurement: Do timing measurements?
 * - ForceMCTrack: Do you want to force the downstream track to only have true hits?
 *
 *  @author Michel De Cian. Sascha Stahl and Olivier Callot for the original PatDownstream
 *  @date   2017-04-04
 *
 *
 */

class PatLongLivedTracking : public GaudiAlgorithm {

public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;


private:

  void ttCoordCleanup();  ///< Tag already used coordinates

  void prepareSeeds(LHCb::Tracks* inTracks, std::vector<LHCb::Track*>& myInTracks); ///< Tag already used T-Seeds

  void getPreSelection( PatDownTrack& track ); ///< Get a preselection of hits around a first track estimate

  template<bool onlyFit>
  void fitAndRemove( PatDownTrack& track ); ///< Perform a chi2 fit to the track and remove outliers

  void findMatchingHits( const PatDownTrack& track, const int plane ); ///< Find x hits matching the first track estimate

  void storeTrack( PatDownTrack& track, LHCb::Tracks* finalTracks, const LHCb::Track* tr ); ///< Store the track as LHCb::Track

  void addUHits( const PatDownTrack& track, const unsigned int maxNumTracks ); ///< Add hits in u layer

  void addVHits( PatDownTrack& track ); ///< Add hits in v layer. The hit with the best chi2 is taken.

  void tagUsedTT( const LHCb::Track* tr ); ///< Tag hits that were already used elsewhere

  void fitXProjection( PatDownTrack& track, PatTTHit* firstHit ); ///< Fit the different x candidates and take the best ones (according to chi2)

  bool acceptCandidate( PatDownTrack& track, int& maxPoints ); ///< Is this candidate good enough?

  void addOverlapRegions( PatDownTrack& track ); ///< Add hits in the overlap regions

  double evaluateFisher( const LHCb::Track* track );///< Evaluate the Fisher discriminant for a preselection of seed tracks

  void xFit( PatDownTrack& track, const PatTTHit* hit1, const PatTTHit* hit2 ); ///< Fit tracklet with hits in x layers only.

  double evalFinalMVA( const PatDownTrack& track ); ///< Evaluate the final discriminant


  // -- TT hits don't need the correction for the differnce in the reference frame, this should be a bit faster
  // -- than the original implementation
  inline void updateTTHitForTrackFast(PatTTHit* hit, const double y0, const double dyDz){
    const auto y  = ( y0 + dyDz * hit->z() );
    hit->setX( hit->hit()->x(y) );
  }

  /// Does this track point inside the beampipe?
  inline bool insideBeampipe(const PatDownTrack& track){
    return (m_minTTx > fabs( track.xAtZ( m_zTTa ) )) && (m_minTTy > fabs( track.yAtZ( m_zTTa ) ));
  }

  /// Add to nStereo if hit is in a stereo layer
  inline bool addIsStereo(const PatTTHit* hit) const{
    return m_addIsStereoHelper[hit->planeCode()];
  }

  /// Helper to evaluate the maximum discrepancy between momentum from kink and curvature in T-stations
  inline double maxDeltaP( const PatDownTrack& track ){
    return track.p()*m_maxDeltaPConst + m_maxDeltaPOffset;
  }

  /// Helper to evaluate the correction to the x position in the TT
  inline double xPosCorrection( const PatDownTrack& track ){
    const auto magScaleFactor = m_magFieldSvc->signedRelativeCurrent() ;
    return std::copysign( m_xCorrectionOffset + m_xCorrectionConst/track.p(), track.momentum()*magScaleFactor );
  }

  // -- timing information
  int           m_downTime = 0;
  int           m_preselTime = 0;
  int           m_findMatchingHitsTime = 0;
  int           m_fitXProjectionTime = 0;
  int           m_fitAndRemoveTime = 0;
  int           m_xFitTime = 0;
  int           m_addUHitsTime = 0;
  int           m_addVHitsTime = 0;
  int           m_acceptCandidateTime = 0;
  int           m_storeTrackTime = 0;
  int           m_overlapTime = 0;

  // -- other member variables which are not settable
  bool          m_printing = false;
  bool          m_magnetOff = false;


  // -- pointers to tools
  ILHCbMagnetSvc*                     m_magFieldSvc = nullptr;
  Tf::TTStationHitManager<PatTTHit>*  m_ttHitManager = nullptr;
  IPatDebugTTTool*                    m_debugTool = nullptr;
  ISequencerTimerTool*                m_timerTool = nullptr;
  std::unique_ptr<IClassifierReader>  m_finalMVA = nullptr;
  IPatMvaClassifier*                  m_seedSelectionMVATool = nullptr;

  // -- settable quantities
  Gaudi::Property<std::string> m_inputLocation {          this, "InputLocation" , "" };
  Gaudi::Property<std::string> m_outputLocation {         this, "OutputLocation", LHCb::TrackLocation::Downstream };
  Gaudi::Property<std::string> m_forwardLocation {        this, "ForwardLocation", LHCb::TrackLocation::Forward };
  Gaudi::Property<std::string> m_matchLocation {          this, "MatchLocation", LHCb::TrackLocation::Match };

  Gaudi::Property<double> m_xPredTolConst {               this,  "XPredTolConst", 200. * Gaudi::Units::mm * Gaudi::Units::GeV };
  Gaudi::Property<double> m_xPredTolOffset {              this,  "XPredTolOffset",  7. * Gaudi::Units::mm };
  Gaudi::Property<double> m_tolMatchConst {               this,  "TolMatchConst", 20000. };
  Gaudi::Property<double> m_tolMatchOffset {              this,  "TolMatchOffset", 1.5 *  Gaudi::Units::mm };
  Gaudi::Property<double> m_tolUConst {                   this,  "TolUConst", 20000.0 };
  Gaudi::Property<double> m_tolUOffset {                  this,  "TolUOffset",  2.5  };
  Gaudi::Property<double> m_tolVConst {                   this,  "TolVConst", 2000.0  };
  Gaudi::Property<double> m_tolVOffset {                  this,  "TolVOffset",  0.5  };

  Gaudi::Property<double> m_maxWindow {                   this,  "MaxWindowSize", 10.0 * Gaudi::Units::mm };
  Gaudi::Property<double> m_maxChi2 {                     this,  "MaxChi2", 30. };
  Gaudi::Property<double> m_maxChi2ThreeHits {            this,  "MaxChi2ThreeHits", 50.0 };

  Gaudi::Property<double> m_minPt {                       this, "MinPt", 40. * Gaudi::Units::MeV };
  Gaudi::Property<double> m_minMomentum {                 this,  "MinMomentum", 1400. * Gaudi::Units::MeV };

  Gaudi::Property<double> m_minTTx{                       this, "MinTTx", 33. * Gaudi::Units::mm }; ///< x distance of beampipe hole
  Gaudi::Property<double> m_minTTy{                       this, "MinTTy", 33. * Gaudi::Units::mm }; ///< y distance of beampipe hole

  // -- parameters for making the track
  // Define parameters for Sim09b field, zState = 9410
  Gaudi::Property<std::array<double,7> > m_zMagnetParams { this, "ZMagnetParams", { 5363.5, -3615.15, 399.87, 97205, -0.01081, -0.15562, 1624.3 }};
  Gaudi::Property<std::array<double,3> > m_momentumParams {this, "MomentumParams", {1217, 487.5, 2864.7 } };
  Gaudi::Property<std::array<double,2> > m_yParams {       this, "YParams", { 5.588, 1260.0 } };

  // -- more settable quantities
  Gaudi::Property<double> m_zTTa {                        this, "ZTTa", 2350.* Gaudi::Units::mm };                  ///< Position of first station
  Gaudi::Property<double> m_zTT {                         this,"ZTT", 2485.* Gaudi::Units::mm };                   ///< Average position for track estimate

  // -- Parameters for the cut on deltaP (momentum estimate from Seeding and Downstream kink)
  Gaudi::Property<double> m_maxDeltaPConst {              this, "MaxDeltaPConst", 4e-6 };
  Gaudi::Property<double> m_maxDeltaPOffset {             this, "MaxDeltaPOffset", 0.075 };

  // -- Parameters for correcting the predicted position
  Gaudi::Property<double> m_xCorrectionConst {            this,  "XCorrectionConst",  23605.0 };
  Gaudi::Property<double> m_xCorrectionOffset {           this,  "XCorrestionOffset",  0.4 };

  Gaudi::Property<unsigned int>  m_maxXTracks {           this, "MaxXTracks", 2 };
  Gaudi::Property<double>        m_maxChi2DistXTracks {   this, "MaxChi2DistXTracks", 0.2 };
  Gaudi::Property<unsigned int>  m_maxXUTracks {          this, "MaxXUTracks", 2 };
  Gaudi::Property<double>        m_fitXProjChi2Offset {   this, "FitXProjChi2Offset", 7.0 };
  Gaudi::Property<double>        m_fitXProjChi2Const {    this, "FitXProjChi2Const", 35000.0 };

  // -- Minimum number of hits on the track which have the high threshold bit (to fight spillover) set
  // -- In Sim09b, setting this to more than 0 decreases the efficiency with no positive effect on ghost rate.
  // -- However, on data the distribution is different on cutting on 2 does not have a negative effect
  Gaudi::Property<unsigned int> m_nMinHighThresHits {     this, "NMinHighThresHits", 2 };
  // -- Parameter to reject seed track which are likely ghosts
  Gaudi::Property<std::string> m_mvaPreselectionToolName {this, "MVAPreselectionTool", "PatLWTNNSeedClassifier"};
  Gaudi::Property<double> m_seedCut {                     this, "SeedCut", 0.145 };
  Gaudi::Property<double> m_finalMvaCut {                 this, "FinalMvaCut", 0.1 };
  Gaudi::Property<double> m_finalMvaSpread {              this, "FinalMvaSpread", 0.1 };
  Gaudi::Property<unsigned int> m_maxOutputCands {        this, "MaxOutputCands", 1 };

  Gaudi::Property<double> m_stateErrorX2 {                this, "StateErrorX2", 4.0 };
  Gaudi::Property<double> m_stateErrorY2 {                this, "StateErrorY2", 400.};
  Gaudi::Property<double> m_stateErrorTX2 {               this, "StateErrorTX2", 6.e-5 };
  Gaudi::Property<double> m_stateErrorTY2 {               this, "StateErrorTY2", 1.e-4 };
  Gaudi::Property<double> m_stateErrorP {                 this, "StateErrorP", 0.15 };

  // -- Tolerance for adding overlap hits
  Gaudi::Property<double> m_overlapTol{                   this, "OverlapTol", 2.0*Gaudi::Units::mm };
  Gaudi::Property<double> m_yTol{                         this, "YTol", 2.0*Gaudi::Units::mm };

  // Change this in order to remove hits and T-tracks used for longtracks.
  // RemoveAll configures that everything is removed.
  // If false only hits and T-tracks from good longtracks are removed.
  // The criterion for this is the Chi2 of the longtracks from the fit.
  Gaudi::Property<bool>        m_removeUsed {             this, "RemoveUsed", false };
  Gaudi::Property<bool>        m_removeAll {              this, "RemoveAll", false };
  Gaudi::Property<double>      m_longChi2 {               this, "LongChi2", 1.5 };

  //== debugging options
  Gaudi::Property<int>         m_seedKey {                this,  "SeedKey", -1 };
  Gaudi::Property<bool>        m_withDebugTool {          this,  "WithDebugTool", false };
  Gaudi::Property<std::string> m_debugToolName {          this,  "DebugTool", "PatDebugTTTruthTool" };
  Gaudi::Property<bool>        m_printTracks {            this,  "PrintTracks", false };
  Gaudi::Property<bool>        m_doTiming {               this,  "TimingMeasurement", false };
  Gaudi::Property<bool>        m_forceMCTrack {           this,  "ForceMCTrack", true };



  // -- hit collections
  std::array<PatTTHits,4> m_preSelHits;
  PatTTHits               m_xHits;
  PatTTHits               m_uvHits;
  std::array<PatTTHits,4> m_hitsLayers;
  PatTTHits               m_matchingXHits;
  PatTTHits               m_uHitsTemp;

  // -- track collections
  PatDownTracks           m_goodXTracks;
  PatDownTracks           m_goodXUTracks;
  PatDownTracks           m_trackCandidates;

};

//=========================================================================
//  Fit and remove the worst hit, as long as over tolerance
//=========================================================================
template<bool onlyFit>
void PatLongLivedTracking::fitAndRemove ( PatDownTrack& track ) {

  if ( UNLIKELY(m_doTiming) ) m_timerTool->start( m_fitAndRemoveTime );

  if ( UNLIKELY(2 > track.hits().size()) ){
    if ( UNLIKELY(m_doTiming) ) m_timerTool->stop( m_fitAndRemoveTime );
    return;  // no fit if single point only !
  }

  bool again = false;
  do {

    again = false;

    //== Fit, using the magnet point as constraint.
    double mat[6], rhs[3];
    mat[0] = 1./( track.errXMag() * track.errXMag() );
    mat[1] = 0.;
    mat[2] = 0.;
    mat[3] = 0.;
    mat[4] = 0.;
    mat[5] = 0.;
    rhs[0] = track.dxMagnet() /( track.errXMag() * track.errXMag() );//( m_magnetSave.x() - m_magnet.x() );
    rhs[1] = 0.;
    rhs[2] = 0.;

    int nbUV = 0;

    std::array<int,4> differentPlanes = {0, 0, 0, 0 };
    unsigned int nDoF = 0;

    double yTrack = track.yAtZ( 0. );
    double tyTr   = track.slopeY();

    PatTTHits::const_iterator itEnd = track.hits().end();
    for(PatTTHits::iterator iHit = track.hits().begin() ; iHit != itEnd; ++iHit) {

      PatTTHit* hit = *iHit;
      if( !onlyFit ) updateTTHitForTrackFast( hit, yTrack, tyTr);
      const double dz   = 0.001*(hit->z() - track.zMagnet());
      const double dist = track.distance( hit );
      const double w    = hit->hit()->weight();
      const double t    = hit->hit()->sinT();

      mat[0] += w;
      mat[1] += w * dz;
      mat[2] += w * dz * dz;
      mat[3] += w * t;
      mat[4] += w * dz * t ;
      mat[5] += w * t  * t ;
      rhs[0] += w * dist;
      rhs[1] += w * dist * dz;
      rhs[2] += w * dist * t ;

      // -- check how many different layers have fired
      differentPlanes[hit->planeCode()]++;
      nbUV += addIsStereo( hit );

      if ( UNLIKELY( m_printing )) {
        info() << format( "   Plane %2d x %7.2f dist %6.3f ",
                          hit->planeCode(), hit->x(), dist );
        if ( m_debugTool ) m_debugTool->debugTTCluster( info(), hit );
        info() << endmsg;
      }

    }

    nDoF = std::count_if(differentPlanes.begin(), differentPlanes.end(), [](const int a){ return a > 0; });
    track.setFiredLayers( nDoF );

    // -- solve the equation and update the parameters of the track
    CholeskyDecomp<double, 3> decomp(mat);
    if (UNLIKELY(!decomp)) {
      track.setChi2(1e42);
      if ( m_doTiming ) m_timerTool->stop( m_fitAndRemoveTime );
      return;
    } else {
      decomp.Solve(rhs);
    }

    const double dx  = rhs[0];
    const double dsl = 0.001*rhs[1];
    const double dy  = rhs[2];

    if ( UNLIKELY( m_printing )) {
      info() << format( "  dx %7.3f dsl %7.6f dy %7.3f, displY %7.2f",
                        dx, dsl, dy, track.displY() ) << endmsg;
    }

    if ( 4 > nbUV ) track.updateX( dx, dsl );
    track.setDisplY( track.displY() + dy );

    //== Remove worst hit and retry, if too far.
    double chi2 = track.initialChi2();

    double maxDist = -1.;
    PatTTHits::iterator worst;

    yTrack = track.yAtZ( 0. );
    tyTr   = track.slopeY();

    itEnd = track.hits().end();
    for (PatTTHits::iterator itH = track.hits().begin();
         itH != itEnd; ++itH){

      PatTTHit* hit = *itH;
      updateTTHitForTrackFast( hit, yTrack, tyTr);

      if( !onlyFit ){
        const double yTrackAtZ = track.yAtZ( hit->z() );
        if( !hit->hit()->isYCompatible(yTrackAtZ, m_yTol ) ){
          // --
          if ( UNLIKELY( m_printing )) {
            info() << "   remove Y incompatible hit measure = " << hit->x()
                   << " : y " << yTrackAtZ << " min " << hit->hit()->yMin()
                   << " max " << hit->hit()->yMax() << endmsg;
          }
          // --
          track.hits().erase( itH );
          if ( 2 < track.hits().size() ) again = true;
          break;
        }
      }

      const double dist = track.distance( hit );

      // --
      if ( UNLIKELY( m_printing )) {
        info() << format( "   Plane %2d x %7.2f dist %6.3f ",
                          hit->planeCode(), hit->x(), dist );
        if ( m_debugTool ) m_debugTool->debugTTCluster( info(), hit );
        info() << endmsg;
      }
      // --
      chi2 += dist * dist * hit->hit()->weight();
      // -- only flag this hit as removable if it is not alone in a plane or there are 4 planes that fired
      if ( !onlyFit && maxDist < std::abs(dist) &&  (1 < differentPlanes[hit->planeCode()] || nDoF == track.hits().size() ) ) {
        maxDist = std::abs( dist );
        worst = itH;
      }
    }

    if(again) continue;


    if ( 2 < track.hits().size() ) chi2 /= (track.hits().size() - 2 );
    track.setChi2( chi2 );

    if(onlyFit){
      if ( m_doTiming ) m_timerTool->stop( m_fitAndRemoveTime );
      return;
    }

    if ( m_maxChi2 < chi2 && track.hits().size() > 3 && maxDist > 0) {

      track.hits().erase( worst );

      again = true;
      if( UNLIKELY( m_printing )) info() << "   remove worst and retry" << endmsg;
    }

    if ( UNLIKELY( m_printing )) {
      info() << format( "  ---> chi2 %7.2f maxDist %7.3f", chi2, maxDist) << endmsg;
    }
  } while (again);

  if ( m_doTiming ) m_timerTool->stop( m_fitAndRemoveTime );

}
#endif // PATKSHORT_H
