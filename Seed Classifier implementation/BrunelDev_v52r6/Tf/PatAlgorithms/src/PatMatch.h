#ifndef PATMATCH_H
#define PATMATCH_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/Track.h"
#include "GaudiKernel/IRegistry.h"
#include "TrackInterfaces/IMatchTool.h"

  /** @class PatMatch PatMatch.h
   *  Match Velo and Seed tracks. This is just a wrapper,
   *  the acutal work is done in the tool, per default in PatMatchTool.
   *
   *  - VeloInput: Input location of Velo tracks
   *  - SeedInput: Input location of Seed tracks
   *  - MatchOutput: Output location of match tracks
   *  - ToolName: Name of the tool that does the matching
   *
   *
   *  @author Olivier Callot
   *  @date   2007-02-07
   */

class PatMatch : public GaudiAlgorithm
{

  public:

    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode execute() override;    ///< Algorithm execution


  private:
    Gaudi::Property<std::string> m_veloLocation { this, "VeloInput", LHCb::TrackLocation::Velo };
    Gaudi::Property<std::string> m_seedLocation { this, "SeedInput", LHCb::TrackLocation::Seed };
    Gaudi::Property<std::string> m_matchLocation { this, "MatchOutput", LHCb::TrackLocation::Match };
    Gaudi::Property<std::string> m_matchToolName { this, "ToolName", "PatMatchTool" };

    IMatchTool* m_matchTool = nullptr;

};

#endif // PATMATCH_H

