#ifndef PATDOWNTRACK_H
#define PATDOWNTRACK_H 1

// Include files
#include <numeric>
#include "GaudiKernel/Point3DTypes.h"

#include "Event/Track.h"
#include "Event/State.h"

#include "PatKernel/PatTTHit.h"
#include "TfKernel/RecoFuncs.h"

#include "vdt/log.h"

/** @class PatDownTrack PatDownTrack.h
 *  Track helper for Downstream track search
 *  Adapted from Pat/PatKShort package
 *
 *  The parameters for the extrapolation are tuned with PatLongLivedParams in Tr/PatFitParams
 *
 *
 *  @author Olivier Callot
 *  @date   2007-10-18
 *
 *  @author Michel De Cian
 *  @date   2017-04-04
 *
 */

class PatDownTrack final {
public:
  /// Standard constructor
  PatDownTrack( LHCb::Track* tr,
                double zTT,
                const std::array<double,7>& magnetParams,
                const std::array<double,3>& momentumParams,
                const std::array<double,2>& yParams,
                const double magnetScale );

  // getters
  const LHCb::Track*  track()        const noexcept{ return m_track;      }
  const LHCb::State*  state()        const noexcept{ return m_state;      }
  PatTTHits&          hits()               noexcept{ return m_hits;       }
  const PatTTHits&    hits()         const noexcept{ return m_hits;       }
  double              xMagnet()      const noexcept{ return m_magnet.x(); }
  double              yMagnet()      const noexcept{ return m_magnet.y(); }
  double              zMagnet()      const noexcept{ return m_magnet.z(); }
  double              slopeX()       const noexcept{ return m_slopeX;     }
  double              slopeY()       const noexcept{ return m_slopeY;     }
  double              errXMag()      const noexcept{ return m_errXMag;    }
  double              errYMag()      const noexcept{ return m_errYMag;    }
  double              displX()       const noexcept{ return m_displX;     }
  double              displY()       const noexcept{ return m_displY;     }
  double              chi2()         const noexcept{ return m_chi2;       }
  bool                ignore()       const noexcept{ return m_ignore;     }
  int                 firedLayers()  const noexcept{ return m_firedLayers;}
  double              mva()          const noexcept{ return m_mva;        }
  
  // setters
  void setFiredLayers( const int nF ) noexcept {
    m_firedLayers = nF;
  }

  void setIgnore( const bool ignore) noexcept {
    m_ignore = ignore;
  }

  void setCurvature( const double curvature) noexcept {
    m_curvature = curvature;
  }

  void setSlopeX( const double slope ) noexcept {
    m_slopeX = slope;
    m_slopeXCand = slope;
  }

  void setDisplX( const double displX ) noexcept { m_displX = displX; }
  void setDisplY( const double displY ) noexcept { m_displY = displY; }
  void setChi2( const double chi2 ) noexcept { m_chi2 = chi2; }
  void setMva( const double mva ) noexcept { m_mva = mva; }

  // functions
  inline double xAtZ( const double z ) const noexcept{
    return m_magnet.x() + (z-m_magnet.z() ) * m_slopeX + m_curvature * ( z-m_zTT) * (z-m_zTT);
  }

  inline double xAtZSave( const double z ) const noexcept{
    return m_magnetSave.x() + (z-m_magnetSave.z() ) * m_slopeXSave + m_curvature * ( z-m_zTT) * (z-m_zTT);
  }

  inline double xAtZ( const double z, const double slopeX ) const noexcept{
    return m_magnet.x() + (z-m_magnet.z() ) * slopeX;
  }

  inline double yAtZ( const double z ) const noexcept{
    return m_magnet.y() + m_displY + (z-m_magnet.z() ) * slopeY();
  }

  inline void updateX( const double dx, const double dsl ) noexcept {
    m_displX += dx;
    m_magnet  = { m_magnet.x() + dx, m_magnet.y(), m_magnet.z() };
    m_slopeX += dsl;
  }

  inline double dxMagnet() const noexcept{ return m_magnetSave.x() - m_magnet.x(); }

  inline double initialChi2() const noexcept{
    auto x = m_displX / m_errXMag;
    auto y = m_displY / m_errYMag;
    return x*x + y*y;
  }

  inline double sagitta( const double z ) const noexcept{ return m_curvature * ( z-m_zTT) * (z-m_zTT); }

  inline double distance( const PatTTHit* hit ) const noexcept{
    return hit->x() - xAtZ( hit->z() );
  }

  inline double distanceSave( const PatTTHit* hit ) const noexcept{
    return hit->x() - xAtZSave( hit->z() );
  }

  /// signed momentum
  inline double momentum() const {
    const auto params = { m_state->tx() * m_state->tx(),
                          m_state->ty() * m_state->ty() };
    return std::inner_product( std::next(m_momPar->begin()), m_momPar->end(),
                               begin(params), m_momPar->front() ) / ( m_state->tx() - m_slopeX ) * m_magnetScale;
  }
  /// unsigned momentum
  inline double p() const noexcept{
    return std::abs( momentum() );
  }
  
  /// transverse momentum
  inline double pt() const noexcept{
    const auto tx2 = slopeX()*slopeX();
    const auto ty2 = slopeY()*slopeY();
    const auto sinTrack = std::sqrt( 1. - 1./(1.+tx2 + ty2) );
    return sinTrack*std::abs(momentum());
  }
  
  /// pseudorapidity
  inline double eta() const noexcept{
    const auto pL = sqrt( p()*p() - pt()*pt() );
    return 0.5*vdt::fast_log( ( p() + pL )/( p() - pL ));
  }

  void startNewCandidate() {
    m_hits.clear();
    m_magnet = m_magnetSave;
    m_slopeX = m_slopeXSave;
    m_displY  = 0.;
    m_displX  = 0.;
  }

  void startNewXCandidate(PatTTHit* firstHit) {
    m_hits.clear();
    m_hits.push_back(firstHit);
    m_magnet = m_magnetSave;
    m_slopeX = m_slopeXCand;
    m_displY  = 0.;
    m_displX  = 0.;
  }

  void startNewXCandidate() {
    m_hits.clear();
    m_magnet = m_magnetSave;
    m_slopeX = m_slopeXCand;
    m_displY  = 0.;
    m_displX  = 0.;
  }


  void startNewXUCandidate(const double slopeX, const double displX, const double magnetX) {
    m_magnet  = { magnetX, m_magnet.y(), m_magnet.z() };
    m_slopeX = slopeX;
    m_displY  = 0.;
    m_displX  = displX;
  }

  void sortFinalHits() {
    std::sort( m_hits.begin(), m_hits.end(), Tf::increasingByZ<>() );
  }

private:
  const std::array<double,3>* m_momPar;
  
  LHCb::Track*        m_track;
  LHCb::State*        m_state;
  Gaudi::XYZPoint     m_magnet;
  Gaudi::XYZPoint     m_magnetSave;

  double      m_magnetScale;
  double      m_zTT;
  double      m_slopeX;
  double      m_slopeXSave;
  double      m_slopeXCand;
  double      m_slopeY;
  double      m_displX;
  double      m_displY;
  double      m_errXMag;
  double      m_errYMag;
  double      m_chi2;
  double      m_curvature;
  bool        m_ignore;
  int         m_firedLayers;
  double      m_mva;
  
  PatTTHits m_hits;      /// working list of hits on this track
};

// -- A typedef for a collection of downstream tracks...
typedef std::vector<PatDownTrack> PatDownTracks;


#endif // PATDOWNTRACK_H
