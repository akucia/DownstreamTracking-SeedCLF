// Include files
// -- Note: These files need to be included here (i.e. before the others)
#include "GaudiKernel/ParsersFactory.h"
#include "GaudiKernel/StdArrayAsProperty.h"

#include <algorithm>
#include <cmath>

// from ROOT
#include <Math/CholeskyDecomp.h>
using ROOT::Math::CholeskyDecomp;

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"
#include "Event/Track.h"
#include "Kernel/STLExtensions.h"

#include "PatKernel/IPatDebugTTTool.h"
#include "PatKernel/IPatMvaClassifier.h"
#include "TfKernel/RecoFuncs.h"
#include "Event/StateParameters.h"
// local
#include "PatDownTrack.h"


// this is kind of a generic projection which sorts according to "lower than"
namespace{
  auto genericProjLT = [](auto&& proj) {
    return [proj=std::forward<decltype(proj)>(proj)](const auto& lhs, const auto& rhs)
    { return LHCb::invoke(proj,lhs) < LHCb::invoke(proj,rhs); };
  };

  // -- internal helper variable
  static constexpr std::array<int,4> m_addIsStereoHelper = {{ 0, 1, 1, 0 }};

}

#include "PatLongLivedTracking.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PatLongLivedTracking
//
// 2015-06-09: Michel De Cian
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( PatLongLivedTracking )

//=============================================================================
// Initialization
//=============================================================================
StatusCode PatLongLivedTracking::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  m_ttHitManager = tool<Tf::TTStationHitManager <PatTTHit> >("PatTTStationHitManager");

  if ( m_withDebugTool ) {
    m_debugTool = tool<IPatDebugTTTool>( m_debugToolName );
    //AD add efficiency vs step. This is recorded for every seed track, as this is the starting point.
    m_debugTool->initializeSteps( { "initEvent", "fisher", "BeampipeCut",
                                    "Preselection", "fitXProjection", "AddUHits",
                                    "AddVHits", "FitAndRemove", "AcceptCandidate",
                                    "AddOverlapRegions" } );
  }

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) 
    debug() << "========================================================= " << endmsg
            << " XPredTol           = " << m_xPredTolConst.value()       << "/p + " << m_xPredTolOffset.value()    << endmsg
            << " TolMatch           = " << m_tolMatchConst.value()       << "/p + " << m_tolMatchOffset.value()    << endmsg
            << " TolU               = " << m_tolUConst.value()           << "/p + " << m_tolUOffset.value()        << endmsg
            << " TolV               = " << m_tolVConst.value()           << "/p + " << m_tolVOffset.value()        << endmsg
            << " MaxDeltaP          = " << m_maxDeltaPConst.value()      << "*p + " << m_maxDeltaPOffset.value()   << endmsg
            << " XCorrection        = " << m_xCorrectionConst.value()    << "/p + " << m_xCorrectionOffset.value() << endmsg
            << " MaxXTracks         = " << m_maxXTracks.value()          << endmsg
            << " MaxChi2DistXTracks = " << m_maxChi2DistXTracks.value()  << endmsg
            << " MaxXUTracks        = " << m_maxXUTracks.value()         << endmsg
            << " MaxChi2XProjection = " << m_fitXProjChi2Const.value()   << "/p + " << m_fitXProjChi2Offset.value()<< endmsg
            << " OverlapTol         = " << m_overlapTol.value()          << endmsg
            << " SeedCut            = " << m_seedCut.value()             << endmsg
            << " FinalMvaCut        = " << m_finalMvaCut.value()         << endmsg
            << " MaxChi2            = " << m_maxChi2.value()             << endmsg
            << " MaxChi2ThreeHits   = " << m_maxChi2ThreeHits.value()    << endmsg
            << " MaxWindowSize      = " << m_maxWindow.value()           << endmsg
            << " RemoveUsed         = " << m_removeUsed.value()          << endmsg
            << " RemoveAll          = " << m_removeAll.value()           << endmsg
            << " LongChi2           = " << m_longChi2.value()            << endmsg
            << " TimingMeasurements = " << m_doTiming.value()            << endmsg
            <<  "========================================================= " << endmsg;

  info() << "zMagnetParams ";
  for ( unsigned int kk = 0; m_zMagnetParams.size() > kk ; kk++) {
    info() << m_zMagnetParams[kk] << " ";
  }
  info() << endmsg << "momentumParams ";
  for ( unsigned int kk = 0; m_momentumParams.size() > kk ; kk++) {
    info() << m_momentumParams[kk] << " ";
  }
  info() << endmsg ;
  if ( 3 > m_zMagnetParams.size() ) {
    return Warning( "Not enough zMagnetParams" );
  }
  if ( 3 > m_momentumParams.size() ) {
    return Warning( "Not enough momentumParams" );
  }

  m_magFieldSvc = svc<ILHCbMagnetSvc>( "MagneticFieldSvc", true );


  if ( m_doTiming) {
    m_timerTool = tool<ISequencerTimerTool>( "SequencerTimerTool/Timer", this );
    m_timerTool->increaseIndent();
    m_downTime = m_timerTool->addTimer( "Execute" );
    m_preselTime = m_timerTool->addTimer( "getPreSelection" );
    m_findMatchingHitsTime = m_timerTool->addTimer( "FindMatchingHits" );
    m_fitXProjectionTime = m_timerTool->addTimer( "FitXProjection" );
    m_fitAndRemoveTime = m_timerTool->addTimer( "FitAndRemove" );
    m_xFitTime = m_timerTool->addTimer( "xFit" );
    m_addUHitsTime = m_timerTool->addTimer( "AddUHits" );
    m_addVHitsTime = m_timerTool->addTimer( "AddVHits" );
    m_acceptCandidateTime = m_timerTool->addTimer( "acceptCandidate" );
    m_overlapTime = m_timerTool->addTimer( "addOverlap" );
    m_storeTrackTime = m_timerTool->addTimer( "storeTrack" );
    m_timerTool->decreaseIndent();
  }

  m_matchingXHits.reserve(64);
  m_trackCandidates.reserve(16);
  m_goodXTracks.reserve(8);
  m_goodXUTracks.reserve(8);
  m_uHitsTemp.reserve(64);

  // -- Seed classifier
  m_seedSelectionMVATool = tool<IPatMvaClassifier>(m_mvaPreselectionToolName);

  // -- Final classifier
  const std::vector<std::string> vars = { "logChi2", "logP", "logPt", "deltaP", "logDxMagnet", "logDistance", "logYAt0", "logXAt0", "firedLayers"};
  m_finalMVA = std::make_unique< ReadMLPBNN>(vars);

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PatLongLivedTracking::execute() {

  if( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_downTime );

  m_printing = msgLevel( MSG::DEBUG );
  if( UNLIKELY( m_printing ) )debug() << "==> Execute" << endmsg;

  // -- If no specified input location, get seed if it exists, else Tsa.
  if ( m_inputLocation.empty() ) {
    if ( exist<LHCb::Tracks>( LHCb::TrackLocation::Seed ) ) {
      m_inputLocation = LHCb::TrackLocation::Seed;
    } else {
      if ( exist<LHCb::Tracks>( LHCb::TrackLocation::Tsa ) ) {
        m_inputLocation = LHCb::TrackLocation::Tsa;
      } else {
        error() << "Could not find input location: " << LHCb::TrackLocation::Seed
                << " or " << LHCb::TrackLocation::Tsa << endmsg;
      }
    }
  }
  if ( UNLIKELY( m_withDebugTool ) ){ m_debugTool->resetflags();}//AD

  //==========================================================================
  // Prepare hits in TT, optional: remove used in PatForward.
  //==========================================================================
  ttCoordCleanup();

  //==========================================================================
  // Prepare the tracks, optional: removing the seends already used in Match.
  //==========================================================================
  LHCb::Tracks* inTracks   = get<LHCb::Tracks>( m_inputLocation );

  //== Local container of good track.
  std::vector<LHCb::Track*> myInTracks;
  myInTracks.reserve(inTracks->size());

  //=== Optional: Remove the seeds already used in Match, check by ancestor of Match tracks
  //=== Prepare T-Seeds
  prepareSeeds( inTracks, myInTracks );

  //==========================================================================
  // Get the output container
  //==========================================================================
  LHCb::Tracks* finalTracks = new LHCb::Tracks();
  finalTracks->reserve(myInTracks.size());
  put( finalTracks, m_outputLocation);

  const double magScaleFactor = m_magFieldSvc->signedRelativeCurrent() ;

  m_magnetOff =  (std::abs(magScaleFactor) > 1e-6) ? false : true;

  //==========================================================================
  // Main loop on tracks
  //==========================================================================
  Tf::TTStationHitManager<PatTTHit>::HitRange ttCoords;
  if ( UNLIKELY( m_printing ) ){
    ttCoords = m_ttHitManager->hits();
    info() << "Will process: " << myInTracks.size() << " seed tracks" << endmsg;
  }


  for ( LHCb::Track* tr : myInTracks ) {
    if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("initEvent",m_debugTool->isTrueTrack(tr,ttCoords));}//AD, add debug step here to see if seed is reco'ble ad downstream

    //if( UNLIKELY(m_withDebugTool) ) m_debugTool->seedTuple( tr );
    // -- LWTNN discriminant to reject bad seed tracks
    const double seedClassifierValue = m_seedSelectionMVATool->getMvaValue(*tr);
    if( UNLIKELY(m_withDebugTool) ){m_debugTool->seedTuple(tr,seedClassifierValue);}
    if( seedClassifierValue >  m_seedCut ) continue;

    if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("fisher",m_debugTool->isTrueTrack(tr,ttCoords));}//AD check fisher
    if ( 0 <= m_seedKey && m_seedKey == tr->key() ) m_printing = true;

    PatDownTrack track( tr, m_zTT.value(), m_zMagnetParams, m_momentumParams, m_yParams, magScaleFactor*(-1) );

    if( track.p() < m_minMomentum ) continue;
    if( track.pt() < m_minPt ) continue;

    // -- Veto particles coming from the beam pipe.
    if( insideBeampipe( track ) ) continue;
    if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("BeampipeCut",m_debugTool->isTrueTrack(tr,ttCoords));}//AD
    const double deltaP = track.momentum() * track.state()->qOverP() - 1.;

    // --
    if ( UNLIKELY( m_printing ) ) {
      for ( PatTTHit* hit : ttCoords ){
        if (hit->hit()->ignore()) continue;
        const double yTrack = track.yAtZ( 0. );
        const double tyTr   = track.slopeY();
        updateTTHitForTrackFast( hit, yTrack, tyTr );
      }

      info() << "Track " << tr->key()
             << format( " [%7.2f %7.2f GeV] x,y(TTa)%7.1f%7.1f dp/p%7.3f errXMag%6.1f YTT%6.1f",
                        .001/track.state()->qOverP(), .001*track.momentum(),
                        track.xAtZ( m_zTTa ), track.yAtZ( m_zTTa ), deltaP,
                        track.errXMag(), track.errYMag() )
             << endmsg;
      info() << format( " Y slope %8.6f computed %8.6f", track.state()->ty(), track.slopeY() )
             << endmsg;

      if ( m_debugTool ) m_debugTool->debugTTClusterOnTrack( tr, ttCoords.begin(), ttCoords.end() );
    }
    // --

    // -- check for compatible momentum
    if ( maxDeltaP( track ) < fabs(deltaP) ) {
      if ( UNLIKELY( m_printing )) info() << "   --- deltaP " << deltaP << " -- rejected" << endmsg;
      if ( !m_magnetOff ) continue;
    }

    // -- Get hits in TT around a first track estimate
    getPreSelection( track );
    if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("Preselection",m_debugTool->isTrueTrack(tr,ttCoords));}//AD
    // -- Need at least 3 hits and at least 1 stereo and 1 x hit
    if( 3 > m_preSelHits[0].size() +  m_preSelHits[1].size() + m_preSelHits[2].size() + m_preSelHits[3].size() ) continue;
    if( 1 > m_preSelHits[0].size() +  m_preSelHits[3].size() ) continue;
    if( 1 > m_preSelHits[1].size() +  m_preSelHits[2].size() ) continue;

    int nbOK = 0;
    int maxPoints = 0;
    m_trackCandidates.clear();
    m_matchingXHits.clear();

    //==============================================================
    // Try to find a candidate: X first, then U, then V.
    //==============================================================
    for( PatTTHit* myHit : m_xHits ){

      // -- Note: So far we only flag at the end, that means as we are iterating over all x hits
      // -- most tracks get made twice. But as you only know in the end which one to keep,
      // -- there might be small differences if you start in TT1X or TT2X, only one track gets chosen in the end
      // -- (i.e. it produces no clones) and the timing is not problematic, let's leave it like this for now
      // -- Flagging hits beforehand would solve the problem, but normally costs a couple of permille efficiency
      if ( myHit->hit()->testStatus( Tf::HitBase::EStatus::UsedByPatDownstream ) ) continue;

      const double meanZ = myHit->z();
      const double posX  = myHit->x( );
      const int myPlane  = myHit->planeCode();

      track.startNewCandidate();


      // -- Create track estimate with one x hit
      const double slopeX = (track.xMagnet() - posX + track.sagitta( meanZ)) / (track.zMagnet() - meanZ);
      track.setSlopeX( slopeX );
      // -- now we know the slope better and can adapt the curvature
      // -- this effect is very small, but seems to be beneficial
      const double curvature =  1.48e-5 * ( track.state()->tx() - slopeX );
      track.setCurvature( curvature );

      // -----------------
      if ( UNLIKELY( m_printing )) {
        const double tolMatch = (std::abs(track.state()->p() * (m_maxWindow - m_tolMatchOffset)) < m_tolMatchConst ) ?
          m_maxWindow.value() : (m_tolMatchOffset + m_tolMatchConst / track.state()->p());
        info() << endmsg
               << format( "... start plane %1d x%8.2f z%8.1f slope%8.2f tolMatch%7.3f",
                          myPlane, posX, meanZ, 1000. * slopeX, tolMatch )
               << endmsg;
      }
      // -----------------

      // -- Fit x projection
      findMatchingHits( track, myPlane);

      if( !m_matchingXHits.empty() ){
        fitXProjection( track, myHit );
      }else{
        m_goodXTracks.clear();
        track.hits().push_back( myHit );
        m_goodXTracks.push_back( track );
      }
      if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("fitXprojection",m_debugTool->isTrueTrack(tr,track.hits()));}//AD

      // -- Take all xTracks into account whose chi2 is close to the best
      // -- until MaxXTracks is reached
      unsigned int maxI = 1;

      if( m_goodXTracks.size() > 1){
        for(unsigned int i = 1; i < m_goodXTracks.size() && i < m_maxXTracks; ++i){
          if(m_goodXTracks[i].chi2() - m_maxChi2DistXTracks < m_goodXTracks[0].chi2()) maxI = i;
        }
      }

      // -- Loop over good x tracks
      for( unsigned int i = 0; i < maxI; ++i){

        PatDownTrack& xTrack = m_goodXTracks[i];

        addUHits( xTrack, m_maxXUTracks );
        unsigned int maxJ = std::min( (unsigned int)m_maxXUTracks, (unsigned int)m_goodXUTracks.size() );
        if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("addUHits",m_debugTool->isTrueTrack(tr,xTrack.hits()));}//AD
        // -- Loop over good xu tracks
        for(unsigned int j = 0; j < maxJ; j++){

          PatDownTrack& xuTrack = m_goodXUTracks[j];
          fitAndRemove<true>( xuTrack );
          addVHits( xuTrack );
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("AddVHits",m_debugTool->isTrueTrack(tr,xuTrack.hits()));}//AD
          fitAndRemove<false>( xuTrack );
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("FitAndRemove",m_debugTool->isTrueTrack(tr,xuTrack.hits()));}//AD
          // -- Check if candidate is better than the old one
          if ( !acceptCandidate( xuTrack, maxPoints ) ) continue;
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("AcceptCandidate",m_debugTool->isTrueTrack(tr,xuTrack.hits()));}//AD
          m_trackCandidates.push_back( std::move(xuTrack) );
          ++nbOK;
        }

        // -- In case no U hits is found, search for V hit
        // -- but with larger search window
        if( xTrack.hits().size() > 1 && maxPoints < 4 && m_goodXUTracks.empty() ){
          addVHits( xTrack );
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("AddVHits",m_debugTool->isTrueTrack(tr,xTrack.hits()));}//AD
          fitAndRemove<false>( xTrack );
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("FitAndRemove",m_debugTool->isTrueTrack(tr,xTrack.hits()));}//AD
          // -- Check if candidate is better than the old one
          if ( !acceptCandidate( xTrack, maxPoints ) ) continue;
          if( UNLIKELY(m_withDebugTool) ){m_debugTool->recordStepInProcess("AcceptCandidate",m_debugTool->isTrueTrack(tr,xTrack.hits()));}//AD
          m_trackCandidates.push_back( std::move(xTrack) );
          ++nbOK;
        }
      }
    }

    // -- The 'nbOK' is just paranoia....
    if( m_trackCandidates.empty() || nbOK == 0) continue;

    // -- Now we have all possible candidates
    // -- Add overlap regions, fit again and flag
    // -- bad candidates
    for( PatDownTrack& track : m_trackCandidates ){

      addOverlapRegions( track );
      if( !acceptCandidate( track, maxPoints ) ) track.setIgnore( true );

      if( UNLIKELY(m_withDebugTool) ){
        m_debugTool->recordStepInProcess("AddOverlapRegions",m_debugTool->isTrueTrack(tr,track.hits()));//AD
        debug() << "This track is: " << m_debugTool->isTrueTrack( track.track(), track.hits() ) << endmsg;
        m_debugTool->fracGoodHits( track.track(), track.hits() );
      }
    }

    // -- The logic looks a bit messy, but the idea is the following:
    // -- First we remove all tracks that are flagged 'ignore'
    // -- In a second step the MVA is evaluated for the rest of the tracks.
    // -- All tracks are removed that don't pass the lower cut on the MVA value
    // -- Then we sort according to the best MVA value and take the m_maxOutputCands
    // -- as long as their MVA value is not too far from the best one.
    // -- Clones are not stored.
    auto itEnd = std::remove_if( m_trackCandidates.begin(), m_trackCandidates.end(),
                                 [](const PatDownTrack& t){ return t.ignore(); });

    for( auto it = m_trackCandidates.begin(); it != itEnd; ++it){
      PatDownTrack& track = *it;
      track.setMva( evalFinalMVA( track ) );
      if( track.mva() < m_finalMvaCut ) track.setIgnore(true);
    }

    itEnd = std::remove_if( m_trackCandidates.begin(), itEnd,
                            [](const PatDownTrack& t){ return t.ignore(); });

    // -- Make sure there's something left to look at
    if( std::distance( m_trackCandidates.begin(), itEnd) == 0 ) continue;

    std::sort( m_trackCandidates.begin(), itEnd,
               [](const PatDownTrack& a, const PatDownTrack& b){ return a.mva() > b.mva();});


    const double bestMVAVal = m_trackCandidates.front().mva();
    unsigned int nTracks = 0;
    for( auto it = m_trackCandidates.begin(); it != itEnd; ++it ){

      PatDownTrack& track = *it;

      if( track.mva() < bestMVAVal - m_finalMvaSpread*bestMVAVal) break;

      unsigned int nUsed = std::count_if(track.hits().begin(), track.hits().end(),
                                         [](const PatTTHit* hit){ return  hit->hit()->testStatus( Tf::HitBase::EStatus::UsedByPatDownstream );});

      if(nUsed > 2 ) continue;


      // -- Flag hits as used
      for( PatTTHit* hit: track.hits() ) {
        hit->hit()->setStatus( Tf::HitBase::EStatus::UsedByPatDownstream, true );
      }

      if( UNLIKELY( m_withDebugTool ) ) m_debugTool->chi2Tuple( track.p(), track.chi2(), track.hits().size());
      storeTrack( track, finalTracks, track.track() );
      ++nTracks;
      if(nTracks >= m_maxOutputCands) break;

    }


  }


  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "Found " << finalTracks->size() << " tracks." << endmsg;

  if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_downTime );

  return StatusCode::SUCCESS;

}
//=========================================================================
//  Cleanup already used T-Seeds
//=========================================================================
void PatLongLivedTracking::prepareSeeds(LHCb::Tracks* inTracks, std::vector<LHCb::Track*>& myInTracks){

  if( !m_removeUsed ){
    myInTracks.insert( myInTracks.end(), inTracks->begin(), inTracks->end());
    return;
  }

  LHCb::Tracks* match = getIfExists<LHCb::Tracks>( m_matchLocation );

  if ( nullptr != match ) {
    if( UNLIKELY( m_printing ) ) debug()<<"Remove seeds and tt hits from Match tracks"<<endmsg;
    for ( LHCb::Track* tr : *inTracks){
      bool store =true;
      if ( UNLIKELY( m_printing ) )debug() << "Seed " << tr->key();
      for( LHCb::Track* matchTr: *match ) {
        const SmartRefVector<LHCb::Track>& ancestors = matchTr->ancestors();
        for ( auto pt : ancestors ){
          if ( tr == pt ) {
            if ( UNLIKELY( m_printing ) )debug() << " is used in match " << matchTr->key();
            if ( m_removeAll || matchTr->chi2PerDoF() < m_longChi2 ) {
              if ( UNLIKELY( m_printing ) )debug() << " good longtrack " << matchTr->key()<<endmsg;
              store = false;
              tagUsedTT( matchTr );
              break;
            }
            break;
          }
        }
        if ( !store ) break;
      }
      if (store) {
        myInTracks.push_back( tr );
        if ( UNLIKELY( m_printing ) )debug() << " will be processed";
      }
      if ( UNLIKELY( m_printing ) )debug() << endmsg;
    }
  }else{
    for(LHCb::Track* tr: *inTracks) {
      //== Ignore tracks with ancestor = forward...
      if ( m_removeUsed && 0 < tr->ancestors().size() ) continue;
      myInTracks.push_back( tr );
    }
  }
}
//=========================================================================
//  Cleanup already used TT coord
//=========================================================================
void PatLongLivedTracking::ttCoordCleanup ( ) {

  Tf::TTStationHitManager<PatTTHit>::HitRange ttCoords = m_ttHitManager->hits();
  for(PatTTHit* hit: ttCoords) hit->hit()->setStatus( Tf::HitBase::EStatus::UsedByPatDownstream, false );

  //== Tag hit used in forward
  if ( m_removeUsed ) {
    if ( exist<LHCb::Tracks>( m_forwardLocation ) ) {
      if( UNLIKELY( m_printing ) )debug()<<"Remove TT hits from Forward tracks from location "
                                         <<m_forwardLocation <<endmsg;
      LHCb::Tracks* tracks = get<LHCb::Tracks>( m_forwardLocation );
      for(const LHCb::Track* tr: *tracks) {
        if (m_removeAll || tr->chi2PerDoF()<m_longChi2) tagUsedTT( tr );
      }
    }
  }
}
//=========================================================================
//  Tag used TT hits on this track.
//=========================================================================
void PatLongLivedTracking::tagUsedTT( const LHCb::Track* tr ) {

  auto ttCoords = m_ttHitManager->hits();

  for(LHCb::LHCbID id: tr->lhcbIDs()) {
    if ( !id.isTT() ) continue;
    for(PatTTHit* hit: ttCoords) {
      if ( hit->hit()->lhcbID() == id ) {
        if( UNLIKELY( m_printing ) )debug()<<"tag hit as used "<<hit->hit()->lhcbID()<<endmsg;
        hit->hit()->setStatus( Tf::HitBase::EStatus::UsedByPatMatch, true );
        break;
      }
    }
  }
}
//=========================================================================
//  Get the PreSelection
//=========================================================================
void PatLongLivedTracking::getPreSelection( PatDownTrack& track ) {

  if( UNLIKELY( m_doTiming ) )m_timerTool->start( m_preselTime );

  // - Max Pt around 100 MeV for strange particle decay -> maximum displacement is in 1/p.
  const double xPredTol =  (track.p() >  1e-6) ?  m_xPredTolConst / track.p() + m_xPredTolOffset : m_xPredTolOffset.value();

  const double yTol = xPredTol;

  // -- a correction turns out to be beneficial
  // -- maybe to compensate tracks not coming from 0/0 (?)
  const double correction = xPosCorrection( track );

  for(int i = 0; i < 4; ++i) m_preSelHits[i].clear();

  m_xHits.clear();

  //== Collect all hits compatible with the extrapolation, region by region.
  if ( UNLIKELY( m_printing )) info() << "-- collect hits with tolerance " << xPredTol << endmsg;

  const double yTrack = track.yAtZ( 0. );
  const double tyTr   = track.slopeY();

  for(int iStation = 0; iStation < 2; ++iStation){
    for(int iLayer = 0; iLayer < 2; ++iLayer){

      Tf::TTStationHitManager<PatTTHit>::HitRange range =  m_ttHitManager->sortedLayerHits(iStation,iLayer);

      if( UNLIKELY( range.empty() ) )continue;

      const double zLayer   = range.front()->z();
      const double yPredLay = track.yAtZ( zLayer );
      const double xPredLay = track.xAtZ( zLayer );
      const double dxDy     = range.front()->hit()->dxDy();

      // -- this should sort of take the stereo angle and some tolerance into account.
      const double lowerBoundX = xPredLay - xPredTol - dxDy*yPredLay - 2.0;

      PatTTHits::const_iterator itHit = std::lower_bound( range.begin(),
                                                          range.end(),
                                                          lowerBoundX,
                                                          Tf::compByX() );

      for( ; itHit != range.end(); ++itHit){

        PatTTHit* hit = *itHit;

        if( UNLIKELY( m_debugTool && m_forceMCTrack && !m_debugTool->isTrueHit( track.track(), hit ) ) ) continue;

        if( UNLIKELY( hit->hit()->ignore()) ) continue;

        const double yPos   = track.yAtZ( hit->z() );
        if ( !hit->hit()->isYCompatible( yPos, yTol ) ) continue;

        updateTTHitForTrackFast( hit, yTrack, tyTr);
        const double pos    = track.xAtZ( hit->z() ) + correction;

        // -- go from -x to +x
        // -- can break if we go out of the right bound
        if( xPredTol < pos - hit->x() ) continue;
        if( xPredTol < hit->x() - pos ) break;

        hit->hit()->setStatus( Tf::HitBase::EStatus::UsedByPatDownstream, false );
        hit->setProjection( std::abs( hit->x()-pos ) );

        m_preSelHits[2*iStation + iLayer].push_back( hit );

        if ( UNLIKELY( m_printing )) {
          info() << format( "  plane%2d z %8.2f x %8.2f pos %8.2f High%2d dist %8.2f",
                            hit->planeCode(), hit->z(), hit->x(), pos,
                            hit->hit()->sthit()->cluster().highThreshold(), hit->x() - pos);
          if ( m_debugTool ) m_debugTool->debugTTCluster( info(), hit );
          info() << endmsg;
        }
      }

      if( iStation == 1 && m_preSelHits[0].empty() && m_preSelHits[1].empty() ) break;

    }
  }


  std::sort(m_preSelHits[1].begin(), m_preSelHits[1].end(), Tf::increasingByProjection<>() );
  std::sort(m_preSelHits[2].begin(), m_preSelHits[2].end(), Tf::increasingByProjection<>() );

  // -- this is a little weird, but having a common vector of x-hits is better
  m_xHits = m_preSelHits[0];
  m_xHits.insert( m_xHits.end(), m_preSelHits[3].begin(), m_preSelHits[3].end() );
  std::sort( m_xHits.begin(),  m_xHits.end(),  Tf::increasingByProjection<>() );

  if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_preselTime );
}

//=========================================================================
//  Fit hits in x layers
//=========================================================================
void PatLongLivedTracking::xFit( PatDownTrack& track, const PatTTHit* hit1, const PatTTHit* hit2 ) {

  if ( UNLIKELY( m_doTiming ) )m_timerTool->start( m_xFitTime );

  //== Fit, using the magnet point as constraint.
  const double w1 = hit1->hit()->weight();
  const double w2 = hit2->hit()->weight();
  const double zMag = track.zMagnet();

  // -- order is: sum w_i, sum w_i*dz_i, sum w_i*dz_i*dz_i
  // -- order is: sum w_i*dx_i, sum w_i*dx_i*dz_i
  double mat[3] = { 1./( track.errXMag() * track.errXMag())  + w1 + w2,
                    w1 * (hit1->z() - zMag) +  w2 * (hit2->z() - zMag),
                    w1 * (hit1->z() - zMag)*(hit1->z() - zMag) + w2 * (hit2->z() - zMag)*(hit2->z() - zMag) };
  double rhs[2] = {  mat[0] * track.dxMagnet() +  w1 *  track.distance( hit1 ) + w2 *  track.distance( hit2 ),
                     w1 * track.distance( hit1 ) * (hit1->z() - zMag) +  w2 * track.distance( hit2 ) * (hit2->z() - zMag)};

  CholeskyDecomp<double, 2> decomp(mat);
  if (UNLIKELY(!decomp)) {
    track.setChi2(1e42);
    if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_xFitTime );
    return;
  } else {
    decomp.Solve(rhs);
  }

  const double dx  = rhs[0];
  const double dsl = rhs[1];

  track.updateX( dx, dsl );

  const double chi2 = track.initialChi2() + w1* track.distance( hit1 )* track.distance( hit1 )
    +  w2* track.distance( hit2 )* track.distance( hit2 );

  track.setChi2( chi2 );

  if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_xFitTime );

}
//=========================================================================
//  Collect the hits in the other x layer
//=========================================================================
void PatLongLivedTracking::findMatchingHits( const PatDownTrack& track, const int plane) {

  if ( UNLIKELY( m_doTiming ) )m_timerTool->start( m_findMatchingHitsTime );

  const int planeToConsider = ( plane == 0 ) ? 3 : 0;

  if( m_preSelHits[planeToConsider].empty() ){
    if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_findMatchingHitsTime );
    return;
  }

  m_matchingXHits.clear();
  // -- If m_tolMatchOffset + m_tolMatchConst/track.p() becomes too large, take m_maxWindow instead to avoid
  // -- unreasonably large values
  const double tol = std::min(m_tolMatchOffset + m_tolMatchConst/track.p(), m_maxWindow.value());

  const double xPred = track.xAtZ( m_preSelHits[planeToConsider].front()->z() );

  PatTTHits::iterator it = std::lower_bound( m_preSelHits[planeToConsider].begin(),
                                             m_preSelHits[planeToConsider].end(), xPred - tol,
                                             [](const PatTTHit* hit, const double uVal){ return hit->x() < uVal; });

  for( ; it != m_preSelHits[planeToConsider].end(); ++it ){
    PatTTHit* hit = *it;
    const double adist = std::abs( hit->x() - xPred );
    if ( adist > tol ) break;
    m_matchingXHits.push_back( hit );
  }

  std::sort( m_matchingXHits.begin(), m_matchingXHits.end(),
             genericProjLT( [xPred](const PatTTHit* hit) { return std::abs(hit->x()-xPred); } ));

  if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_findMatchingHitsTime );

}
//=========================================================================
//  Add the U hits.
//=========================================================================
void PatLongLivedTracking::addUHits ( const PatDownTrack& track, const unsigned int maxNumTracks ) {

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_addUHitsTime );

  m_goodXUTracks.clear();

  if( m_preSelHits[1].empty() ){
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_addUHitsTime );
    m_goodXUTracks.push_back( track );
    return;
  }

  const double tol = m_tolUOffset + m_tolUConst / track.p();
  const double yTrack = track.yAtZ( 0. );
  const double tyTr   = track.slopeY();

  m_uHitsTemp.clear();


  // -- first select all hits, and then
  // -- accept until over a tolerance
  for(PatTTHit* hit: m_preSelHits[1]) {
    updateTTHitForTrackFast( hit, yTrack, tyTr);

    const double dist = std::abs( track.distance( hit ) );
    if ( dist > tol ) continue;
    m_uHitsTemp.push_back( hit );
  }

  if( m_uHitsTemp.empty() ){
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_addUHitsTime );
    return;
  }

  std::sort( m_uHitsTemp.begin(), m_uHitsTemp.end(),
             genericProjLT( [&track](const PatTTHit* hit){ return std::abs( track.distance( hit )); } ));

  const double slopeX = track.slopeX();
  const double displX = track.displX();
  const double magnetX = track.xMagnet();

  PatDownTrack greatTrack( track );

  for( PatTTHit* hit : m_uHitsTemp){

    greatTrack.startNewXUCandidate(slopeX, displX, magnetX);

    greatTrack.hits().push_back( hit );

    // -- it's sorted
    if ( m_goodXUTracks.size() < maxNumTracks-1){
      m_goodXUTracks.push_back( greatTrack );
      greatTrack.hits().pop_back();
    }else{
      m_goodXUTracks.push_back( std::move(greatTrack) );
      break;
    }

  }

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_addUHitsTime );

}
//=========================================================================
//  Add the V hits. Take the one which has the best chi2
//=========================================================================
void PatLongLivedTracking::addVHits ( PatDownTrack& track ) {

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_addVHitsTime );

  if( m_preSelHits[2].empty() ){
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_addVHitsTime );
    return;
  }

  const double tol = (track.hits().size() == 2) ?
    m_tolUOffset + m_tolUConst / track.p() :
    m_tolVOffset + m_tolVConst / track.p() ;

  const double yTrack = track.yAtZ( 0. );
  const double tyTr   = track.slopeY();

  double minChi2 = 10000;

  PatTTHit* bestHit = nullptr;
  for(PatTTHit* hit: m_preSelHits[2]) {

    updateTTHitForTrackFast( hit, yTrack, tyTr);
    const double adist = std::abs( track.distance( hit ) );

    if( adist < tol ){
      track.hits().push_back( hit );
      fitAndRemove<true>( track );
      track.hits().pop_back();

      if( track.chi2() < minChi2){
        bestHit = hit;
        minChi2 = track.chi2();
      }
    }
  }

  if( bestHit != nullptr) track.hits().push_back( bestHit );

  track.sortFinalHits();

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_addVHitsTime );

}
//=========================================================================
//  Check if the new candidate is better than the old one
//=========================================================================
bool PatLongLivedTracking::acceptCandidate( PatDownTrack& track, int& maxPoints ){

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_acceptCandidateTime );

  const int nbMeasureOK = track.hits().size();

  // -- use a tighter chi2 for 3 hit tracks
  // -- as they are more likely to be ghosts
  const double maxChi2 = ( track.hits().size() == 3 ) ? m_maxChi2ThreeHits : m_maxChi2;


  //== Enough mesures to have Chi2/ndof.
  if ( 3 > nbMeasureOK ) {
    if ( UNLIKELY( m_printing ) ) info() << " === not enough points: " << nbMeasureOK << " but at least 3 required" << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  if( 3 > track.firedLayers() ){
    if ( UNLIKELY( m_printing ) ) info() << " === not enough firedLayers: " << track.firedLayers() << " but at least 3 needed" << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  //== Good enough Chi2/ndof
  if ( maxChi2 < track.chi2() ) {
    if ( UNLIKELY( m_printing ) ) info() << " === Chisq too big: " << track.chi2() << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  const double deltaP = track.momentum() * track.state()->qOverP() - 1.;
  //== Compatible momentum
  if ( maxDeltaP( track ) < fabs(deltaP) ) {
    if ( UNLIKELY( m_printing ) ) info() << " === Deltap too big: " << deltaP << endmsg;
    if ( !m_magnetOff ){
      if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
      return false;
    }
  }

  //== Longest -> Keeep it
  if ( maxPoints > nbMeasureOK ) {
    if ( UNLIKELY( m_printing ) ) info() << " === less points than previous: " << nbMeasureOK << " previous: " << maxPoints << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }


  //== Count if enough with high threshold
  unsigned int nbHigh = 0;
  int nbUsed = 0;

  for(const PatTTHit* hit: track.hits()) {
    if ( hit->hit()->sthit()->cluster().highThreshold() ) ++nbHigh;
    if ( hit->hit()->testStatus( Tf::HitBase::EStatus::UsedByPatDownstream )  ) ++nbUsed;
  }

  if (m_nMinHighThresHits.value() > nbHigh ) {
    if ( UNLIKELY( m_printing ) ) info() << " === not enough high threshold points: " << nbHigh << " min required: " << m_nMinHighThresHits.value() << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }
  if ( nbMeasureOK == nbUsed ) {
    if ( UNLIKELY( m_printing ) ) info() << " === is a clone" << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  if( insideBeampipe( track )  ){
    if ( UNLIKELY( m_printing ) ) info() << " === track pointing inside beampipe" << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  if( (track.eta() < 1.7) || (track.eta() > 5.3 )){
    if ( UNLIKELY( m_printing ) ) info() << " === track outside acceptance. eta: " << track.eta() << endmsg;
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  if (track.p() < m_minMomentum){
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  if (track.pt() < m_minPt){
    if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );
    return false;
  }

  //== Better candidate.
  maxPoints = std::min(4, nbMeasureOK);

  if ( UNLIKELY( m_printing ) ) {
    info() << format( "  *** Good candidate ***  slope%8.2f displX%8.2f Y%8.2f Chi2%8.2f",
                      1000.*track.slopeX(), track.displX(), track.displY(), track.chi2() );
  }


  track.sortFinalHits();

  if ( UNLIKELY( m_printing )) {
    for(PatTTHit* hit: track.hits()) {
      LHCb::STChannelID icID = hit->hit()->lhcbID().stID();
      double xCoord = hit->x() ;
      info() << "      TT Clus "
             << format( "(S%1d,L%1d,R%2d,S%2d,S%3d) x%7.1f  dist%7.3f High %1d",
                        icID.station(), icID.layer(), icID.detRegion(),
                        icID.sector(), icID.strip(), xCoord,
                        track.distance( hit ), hit->hit()->sthit()->cluster().highThreshold() ) ;
      if ( m_debugTool ) m_debugTool->debugTTCluster( info(), hit );
      info() << endmsg;
    }
  }

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_acceptCandidateTime );

  return true;
}

//=========================================================================
//  Store Track
//=========================================================================
void PatLongLivedTracking::storeTrack( PatDownTrack& track, LHCb::Tracks* finalTracks, const LHCb::Track* tr ){

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_storeTrackTime );

  counter("# Downstream tracks made")++;

  //=== Store the tracks
  //== new LHCb::Track
  LHCb::Track* dTr = new LHCb::Track();
  //== add ancestor
  dTr->addToAncestors( tr );
  //== Adjust flags
  dTr->setType(         LHCb::Track::Types::Downstream  );
  dTr->setHistory(      LHCb::Track::History::PatDownstream   );
  dTr->setPatRecStatus( LHCb::Track::PatRecStatus::PatRecIDs   );
  //== add Seed LHCbIDs
  dTr->addSortedToLhcbIDs(  tr->lhcbIDs()      );
  //== add new LHCbIDs
  for ( const PatTTHit* hit : track.hits() ){
    dTr->addToLhcbIDs( hit->hit()->lhcbID() );
    counter("#TT hits added")++;
  }


  //== add states
  // S. Stahl added 3 T-States

  // check for FPE and magnet off
  double QOverP = 1e-5;
  if ( track.p() >  1e-6){
      QOverP = 1.0 / track.momentum();
  }

  //== create a state at zTTa
  LHCb::State ttState;
  ttState.setState( track.xAtZ( m_zTTa ),
                    track.yAtZ( m_zTTa ),
                    m_zTTa,
                    track.slopeX(),
                    track.slopeY(),
                    QOverP );
  Gaudi::TrackSymMatrix cov;
  cov(0,0) = m_stateErrorX2;
  cov(1,1) = m_stateErrorY2;
  cov(2,2) = m_stateErrorTX2;
  cov(3,3) = m_stateErrorTY2;
  double errQOverP = m_stateErrorP * QOverP;
  cov(4,4) = errQOverP * errQOverP;

  ttState.setCovariance( cov );
  dTr->addToStates( ttState );

  //== add seed states
  std::vector<LHCb::State*> newstates;
  newstates.reserve(3);
  newstates.push_back(tr->closestState(StateParameters::ZBegT).clone());
  newstates.push_back(tr->closestState(StateParameters::ZMidT).clone());
  // make sure we don't include same state twice
  if (std::abs(newstates[newstates.size() - 2]->z() -
               newstates.back()->z()) < 300.) {
    delete newstates.back();
    newstates.pop_back();
  }
  newstates.push_back(tr->closestState(StateParameters::ZEndT).clone());
  // make sure we don't include same state twice
  if (std::abs(newstates[newstates.size() - 2]->z() -
               newstates.back()->z()) < 300.) {
    delete newstates.back();
    newstates.pop_back();
  }

  // adjust q/p and its uncertainty
  for(LHCb::State* st: newstates) {
    st->covariance()(4,4) = errQOverP * errQOverP;
    st->setQOverP(QOverP);
  }
  dTr->addToStates(newstates);

  //== Save track
  finalTracks->insert( dTr );

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_storeTrackTime );

}
//=============================================================================
// Fit the projection in the zx plane, one hit in each x layer
//=============================================================================
void PatLongLivedTracking::fitXProjection( PatDownTrack& track, PatTTHit* firstHit){

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_fitXProjectionTime );

  m_goodXTracks.clear();

  const double maxChi2 = m_fitXProjChi2Offset + m_fitXProjChi2Const/track.p();

  // Catch if there is no second hit in other station
  PatTTHits::const_iterator itEnd = m_matchingXHits.end();
  for( PatTTHits::const_iterator iHit = m_matchingXHits.begin(); iHit != itEnd; ++iHit) {
    PatTTHit* hit = *iHit;

    track.startNewXCandidate();
    xFit( track, firstHit, hit );

    // -- It is sorted according to the projection
    // -- the chi2 will therefore only increase
    if( track.chi2() > maxChi2) break;
    track.hits().push_back( firstHit );
    track.hits().push_back( hit );

    // -- We can only move the last one
    // -- as the tracks before are 'recycled'
    if( m_goodXTracks.size() < m_maxXTracks-1){
      m_goodXTracks.push_back( track );
    }else{
      m_goodXTracks.push_back( std::move(track) );
      break;
    }

  }

  // -- If no suitable hit has been found
  // -- we just add the first one and make
  // -- it a track.
  if( m_goodXTracks.empty() ){
    track.hits().push_back( firstHit );
    m_goodXTracks.push_back( track );
    if ( m_doTiming ) m_timerTool->stop( m_fitXProjectionTime );
    return;
  }

  std::sort( m_goodXTracks.begin(), m_goodXTracks.end(), genericProjLT( &PatDownTrack::chi2 ) );

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->stop( m_fitXProjectionTime );

}
//=============================================================================
// This is needed for tracks which have more than one x hit in one layer
// Maybe we could make this smarter and do it for every track and add the 'second best'
// this, such that we do not need to loop over them again
//=============================================================================
void PatLongLivedTracking::addOverlapRegions( PatDownTrack& track ){

  if ( UNLIKELY( m_doTiming ) ) m_timerTool->start( m_overlapTime );

  bool hitAdded = false;

  const double yTrack = track.yAtZ( 0. );
  const double tyTr   = track.slopeY();

  for(int i = 0; i < 4; ++i){
    for( PatTTHit* hit : m_preSelHits[i] ){

      updateTTHitForTrackFast( hit, yTrack, tyTr );

      if ( hit->hit()->testStatus( Tf::HitBase::EStatus::UsedByPatDownstream )) continue;
      if ( m_overlapTol > std::abs( track.distance( hit ) ) ) {
        double yTrack = track.yAtZ( hit->z() );

        if ( !hit->hit()->isYCompatible( yTrack, m_yTol ) ) continue;

        // -- check that z-position is different
        bool addHit = true;
        for(const PatTTHit* trackHit : track.hits() ){
          if( trackHit->planeCode() != hit->planeCode() ) continue;
          // -- the displacement in z between overlap modules is larger than 1mm
          if( std::abs( hit->z() - trackHit->z() ) < 1.0 ) addHit = false;
        }

        // -------------------------------------
        if(addHit){
          track.hits().push_back( hit );
          hitAdded = true;
        }

      }
    }
  }

  if ( hitAdded ) {
    track.sortFinalHits();
    fitAndRemove<false>( track );
  }

  if ( UNLIKELY( m_doTiming ) )m_timerTool->stop( m_overlapTime );

}
//=============================================================================
// Evaluate the final classifier for the input tracks
//=============================================================================
double PatLongLivedTracking::evalFinalMVA( const PatDownTrack& track ){

  double distance = 0;
  const double yTrack = track.yAtZ( 0. );
  const double tyTr   = track.slopeY();
  for( auto hit : track.hits()){
    updateTTHitForTrackFast( hit, yTrack, tyTr);
    distance += track.distanceSave( hit );
  }
  distance /= track.hits().size();

  const double deltaP = track.momentum() * track.state()->qOverP() - 1.;

  std::array<double,9> vals ={ vdt::fast_log(track.chi2()),
                               vdt::fast_log(track.p()),
                               vdt::fast_log(track.pt()),
                               std::abs(deltaP),
                               vdt::fast_log(std::abs(track.dxMagnet())),
                               vdt::fast_log(std::abs(distance)),
                               vdt::fast_log(std::abs(track.yAtZ(0.0))),
                               vdt::fast_log(std::abs(track.xAtZ(0.0))),
                               static_cast<double>(track.firedLayers())
  };

  if( UNLIKELY(m_withDebugTool) ){
    const bool trueTrack = m_debugTool->isTrueTrack( track.track(), track.hits() );

    m_debugTool->finalTuple( trueTrack, track.track(), { std::begin(vals), std::end(vals)} );
  }

  return m_finalMVA->GetMvaValue( vals );

}
//=============================================================================
// Finalize
//=============================================================================
StatusCode PatLongLivedTracking::finalize(){

  if(m_finalMVA){
    m_finalMVA.reset();
  }

  return GaudiAlgorithm::finalize();
}
