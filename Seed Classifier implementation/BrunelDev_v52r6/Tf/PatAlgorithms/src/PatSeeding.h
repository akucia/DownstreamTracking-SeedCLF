#ifndef PATSEEDING_H
#define PATSEEDING_H 1

// Include files
// from Gaudi
#include "GaudiAlg/ISequencerTimerTool.h"
#include "GaudiAlg/GaudiAlgorithm.h"

#include "TrackInterfaces/IPatSeedingTool.h"

/** @class PatSeeding PatSeeding.h
 *
 *  Pat Seeding algorithm.
 *
 *  @author Olivier Callot
 *  @date   2006-10-13 Initial version
 *  @date   2007-08-20 Update for a-team framework
 *  @author Manuel Schiller
 *  @date   2008-04-16 obtain a private version of PatSeedingTool
 */

class PatSeeding : public GaudiAlgorithm
{
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_outputTracksName { this, "OutputTracksName", LHCb::TrackLocation::Seed };
  IPatSeedingTool* m_seedingTool=nullptr;
  ISequencerTimerTool* m_timerTool=nullptr;
  int  m_seedTime = 0;
  Gaudi::Property<bool> m_doTiming { this, "TimingMeasurement", false };

};

#endif // PATSEEDING_H
