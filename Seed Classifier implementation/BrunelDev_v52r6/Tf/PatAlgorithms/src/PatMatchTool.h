#ifndef PATMATCHTOOL_H
#define PATMATCHTOOL_H 1

// Include files
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

#include "Event/Track.h"
#include "GaudiKernel/IRegistry.h"

#include "TrackInterfaces/IMatchTool.h"            // Interface
#include "TrackInterfaces/ITrackMatch.h"            // Interface for Hlt



// forward decls
struct ITrackMomentumEstimate;
struct IAddTTClusterTool;


/** @class PatMatchTool PatMatchTool.h
 *
 * Match Velo and Seed tracks
 * The parameters used to calculate the chi2 can be derived using the "MatchFitParams" algorithm.
 *
 * Parameters:
 *
 * - maxMatchChi2: Cut value for matching chi2. This is the main classifier that influences efficiency / ghost rate.
 * - zMagnet, zMagnetTx2, zMagnetDsl, zMagnetDsl2, zMagnetSeedX: Parameters (coefficients) to calculate the z-position for the matching in the x-coordinate
 * - zMatchY, MagnetBendDsl, MagnetBendDslY: Parameters (coefficients) for the matching in the y-coordinate
 * - dxTol, dxTolSlope, dyTol, dyTolSlope: Tolerances for the chi2 calculation.
 * - MinPt: Minimum pT for a track to be accepted
 * - MinMomentum: Mimimum momentum for a track to be accepted.
 * - FastMomentumToolName: Name of the tool that calculates the momentum.
 * - AddTTClusters: Add TT hits to this track (when possible)?
 * - AddTTClusterName: Name of the tool that adds TT hits.
 * - writeNNVariables: Write out LHCb::Track::NCandCommonHits?
 * - RejectVeloUsed: Use a velo track only once?
 * - RejectSeedUsed: Use a seed track only once?
 *
 * @author Michel De Cian
 * @date 2015-01-09
 *  change parametrisation for z(x) and bending in y.
 *
 * @author Manuel Schiller
 * @date 2012-01-31
 *	code cleanup, make sure all states are there for fit in HLT
 *
 * @author Johannes Albrecht
 * @date 2008-04-25
 * 	reorganize match as tool
 *
 * @author Olivier Callot
 * @date   2007-02-07
 *	initial implementation
 */

class PatMatchTool : public extends<GaudiTool, ITrackMatch, IMatchTool>
{
public:
    using base_class::base_class;
	/// Algorithm initialization
	StatusCode initialize() override;

	/// create match tracks from containers of velo and seed tracks
	StatusCode match(const LHCb::Tracks& velos,
                   const LHCb::Tracks& seeds,
                   LHCb::Tracks& matchs) override;

	/// create a match track from a velo and a seed track
	StatusCode matchSingle(const LHCb::Track& velo,
                         const LHCb::Track& seed,
                         LHCb::Track& output ,
                         double& chi2) override;

	/// Function for matching in Hlt
	StatusCode match(const LHCb::Track& veloTrack,
                   const LHCb::Track& tTrack,
                   LHCb::Track& matchedTrack,
                   double& quality, double& quality2) override;

private:
	/// calculate matching chi^2
	double getChi2Match(const LHCb::State& veloState,
                      const LHCb::State& seedState) const;

  /// merge velo and seed segment to output track
	void makeTrack(const LHCb::Track& velo,
                 const LHCb::Track& seed, LHCb::Track& output,
                 const double chi2) const;

  /// Check if Velo or T-Seed should be processed.
  bool acceptTrack(const LHCb::Track& track) const;


  ITrackMomentumEstimate* m_fastMomentumTool = nullptr;
  IAddTTClusterTool*   m_addTTClusterTool = nullptr;

  // -- Skip Velo tracks if used before
  Gaudi::Property<std::vector<std::string>> m_veloVetoTracksNames { this, "VeloVetoTracksName", {} };

	// use tool for momentum parametrisation
  Gaudi::Property<std::string> m_fastMomentumToolName { this, "FastMomentumToolName", "FastMomentumEstimate" };
  Gaudi::Property<std::string> m_addTTClusterName { this, "AddTTClusterName", "PatAddTTCoord" };

  Gaudi::Property<bool> m_addTT { this, "AddTTClusters", true };
  Gaudi::Property<bool> m_writeNNVariables { this, "writeNNVariables", true };

  // -- Use the Velo and Seed tracks in more than one match candidate?
  Gaudi::Property<bool> m_rejectVeloUsed { this, "RejectVeloUsed", true };
  Gaudi::Property<bool> m_rejectSeedUsed { this, "RejectSeedUsed", true };

  // -- Parameters for z position of matching in x-coordinate
  Gaudi::Property<double> m_zMagnet { this, "zMagnet", 5197.83 * Gaudi::Units::mm };
  Gaudi::Property<double> m_zMagnetTx2 { this, "zMagnetTx2", -1372.63 * Gaudi::Units::mm };
  Gaudi::Property<double> m_zMagnetDsl { this, "zMagnetDsl", 13.5453      };
  Gaudi::Property<double> m_zMagnetDsl2 { this, "zMagnetDsl2", 297.853 * Gaudi::Units::mm };
  Gaudi::Property<double> m_zMagnetSeedX { this, "zMagnetSeedX", 0.0251294  };

  // -- Parameters for matching in y-coordinate
  Gaudi::Property<double> m_zMatchY { this, "zMatchY", 8420. * Gaudi::Units::mm };
  Gaudi::Property<double> m_magnetBendDsl { this, "MagnetBendDsl", -646.047 };
  Gaudi::Property<double> m_magnetBendDslY { this, "MagnetBendDslY", -44118.5 };

  // -- Tolerances
  Gaudi::Property<double> m_dxTol { this, "dxTol", 8. * Gaudi::Units::mm };
  Gaudi::Property<double> m_dxTolSlope { this, "dxTolSlope", 60. * Gaudi::Units::mm };
  Gaudi::Property<double> m_dyTol { this, "dyTol", 6. * Gaudi::Units::mm };
  Gaudi::Property<double> m_dyTolSlope { this, "dyTolSlope", 300. * Gaudi::Units::mm };

  // -- The main cut value
  Gaudi::Property<double> m_maxChi2 { this, "maxMatchChi2", 4.8 };

  Gaudi::Property<double> m_minMomentum { this, "MinMomentum", 0. * Gaudi::Units::GeV  };
  Gaudi::Property<double> m_minPt { this, "MinPt", 0. * Gaudi::Units::MeV  };

};

#endif // PATMATCHTOOL_H

// vim: sw=4:tw=78:ft=cpp
