// Include files

// local
#include "PatMatch.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PatMatch
//
// 2007-02-07 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( PatMatch )

//=============================================================================
// Initialization
//=============================================================================
StatusCode PatMatch::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  m_matchTool = tool<IMatchTool>( m_matchToolName, this);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PatMatch::execute()
{
  LHCb::Tracks* matchs = new LHCb::Tracks;
  put(matchs, m_matchLocation);

  LHCb::Tracks* velos  = getIfExists<LHCb::Tracks>( m_veloLocation );
  if( !velos ){
    error() << "Could not find Velo tracks at: " << m_veloLocation << endmsg;
    return StatusCode::SUCCESS;
  }

  LHCb::Tracks* seeds  = getIfExists<LHCb::Tracks>( m_seedLocation );
  if( !seeds ){
    error() << "Could not find Seed tracks at: " << m_seedLocation << endmsg;
    return StatusCode::SUCCESS;
  }

  matchs->reserve(200);

  StatusCode sc = m_matchTool->match( *velos , *seeds , *matchs);

  if(sc.isFailure()) Warning("PatMatchTool failed",sc).ignore();

  return sc;
}

//=============================================================================
