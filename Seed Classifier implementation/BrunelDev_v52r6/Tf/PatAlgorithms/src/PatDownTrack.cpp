// Include files

// local
#include "PatDownTrack.h"
#include <iterator>

//-----------------------------------------------------------------------------
// Implementation file for class : PatDownTrack, from Pat/PatKShort package
//
// 2007-10-18 : Olivier Callot
// 2017-01-25 : Michel De Cian, for PatLongLivedTracking
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PatDownTrack::PatDownTrack( LHCb::Track* tr,
                            double zTT,
                            const std::array<double,7>& magnetParams,
                            const std::array<double,3>& momentumParams,
                            const std::array<double,2>& yParams,
                            const double magnetScale) :
  m_momPar(&momentumParams),
  m_track(tr),
  m_magnetScale(magnetScale),
  m_zTT(zTT),
  m_ignore(false),
  m_firedLayers(0)
{
  m_hits.reserve(6);

  // -- Number of IT hits
  const unsigned int nbIT = std::count_if( tr->lhcbIDs().begin(), tr->lhcbIDs().end(),
                                           [](const LHCb::LHCbID id){ return id.isIT();});

  m_state = &tr->closestState( 10000. );

  // -- See PatLongLivedParams to see how these coefficients are derived.
  const auto params = { m_state->ty() * m_state->ty(),
                        m_state->tx() * m_state->tx(),
                        1/m_state->p(),
                        std::abs( m_state->x() ),
                        std::abs( m_state->y() ),
                        std::abs( m_state->ty() ) };

  const auto zMagnet = std::inner_product( std::next(begin(magnetParams)), end(magnetParams),
                                           begin(params), magnetParams[0] );

  const auto dz = zMagnet - m_state->z();
  const auto xMagnet = m_state->x() + dz * m_state->tx();
  m_slopeX       = xMagnet / zMagnet;

  const auto dSlope = std::abs( m_slopeX - m_state->tx() );
  const auto dSlope2 = dSlope*dSlope;
  
  // -- This is how it is done for OT tracks
  // -- see LHCb-PUB-2017-001 for an explanation
  // -- yParams are derived in PatLongLivedParams
  const auto  by = ( nbIT > 4
                     ? m_state->ty()
                     : ( m_state->y() / ( m_state->z() +
                                          ( yParams[0] * fabs(m_state->ty()) * zMagnet + yParams[1] )* dSlope2  ) ) );
  
  
  m_slopeY = by * ( 1. + yParams[0] * std::abs(by) * dSlope2 );
  
  const auto yMagnet = m_state->y() + dz * by - yParams[1] * by * dSlope2;
  
  // -- These resolutions are semi-empirical and are obtained by fitting residuals
  // -- with MCHits and reconstructed tracks
  // -- See LHCb-PUB-2017-001 for details
  if(nbIT > 4 ){
    m_errXMag = dSlope * 16.0 + 1.0;
    m_errYMag = dSlope * 15.0 + 2.0;
  }else{
    m_errXMag = dSlope * 18.0 + 2.0;
    m_errYMag = dSlope2 * 50.0 + dSlope * 20.0 + 5.0;
  }

  m_magnet = { xMagnet, yMagnet, zMagnet };

  //=== Save for reference
  m_magnetSave = m_magnet;
  m_slopeXSave = m_slopeX;
  m_displX     = 0.;
  m_displY     = 0.;

  //=== Initialize all other data members
  m_chi2       = 0.;
  m_slopeXCand = m_slopeX;

  m_curvature  = 1.48e-5 * ( m_state->tx() - m_slopeX );
}
//=============================================================================
