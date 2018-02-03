#ifndef PATFORWARD_H
#define PATFORWARD_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "TrackInterfaces/IPatForwardTool.h"
#include "TfKernel/TStationHitManager.h"
#include "PatKernel/PatForwardHit.h"


class IOTHitCreator;

  /** @class PatForward PatForward.h
   *  Forward pattern recognition. Connect a Velo track to the T stations.
   *
   *  @author Olivier Callot
   *  @date   2005-04-01 Initial version
   *  @date   2007-08-20 Update for A-Team framework
   */

class PatForward : public GaudiAlgorithm {
  public:
    /// Standard constructor
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override;    ///< Algorithm initialization
    StatusCode execute   () override;    ///< Algorithm execution

  private:

    int overlaps(const LHCb::Track* lhs, const LHCb::Track* rhs ) const;

    int              m_fwdTime = 0;

    Gaudi::Property<std::string> m_inputTracksName { this, "InputTracksName", LHCb::TrackLocation::Velo };
    Gaudi::Property<std::string> m_outputTracksName { this, "OutputTracksName", LHCb::TrackLocation::Forward};
    Gaudi::Property<std::string> m_forwardToolName { this, "ForwardToolName", "PatForwardTool" };

    Gaudi::Property<int> m_deltaNumberInTT { this, "DeltaNumberInTT", 1 };
    Gaudi::Property<int> m_deltaNumberInT { this, "DeltaNumberInT", 3 };

    Gaudi::Property<bool> m_doClean { this, "DoCleanUp", true};
    Gaudi::Property<bool> m_doTiming { this, "TimingMeasurement", false};

    Gaudi::Property<unsigned int> m_maxNVelo { this, "MaxNVelo", 1000 };
    Gaudi::Property<unsigned int> m_maxNumberOTHits { this, "maxOTHits", 10000 };
    Gaudi::Property<unsigned int> m_maxNumberITHits { this, "maxITHits",  3000 };

    const Tf::IOTHitCreator* m_otHitCreator = nullptr;


    IPatForwardTool*     m_forwardTool = nullptr;
    ISequencerTimerTool* m_timerTool = nullptr;
    // switch on or off NN var. writing
    Gaudi::Property<bool> m_writeNNVariables { this,"writeNNVariables", true }; // switch on or off NN var. writing
  };
#endif // PATFORWARD_H
