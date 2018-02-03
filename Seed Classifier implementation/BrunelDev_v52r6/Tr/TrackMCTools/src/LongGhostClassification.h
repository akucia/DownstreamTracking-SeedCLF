#ifndef _LongGhostClassification_H
#define _LongGhostClassification_H

#include "TrackGhostClassificationBase.h"


namespace LHCb{
  class Track;
}

class LongGhostClassification: public TrackGhostClassificationBase {

public:

  /// constructer
  LongGhostClassification(const std::string& type,
                               const std::string& name,
                               const IInterface* parent);

  /** destructer */
  virtual ~LongGhostClassification();

  /**
   *  Check this is a ghost .
   *  specialize for long tracks [check velo and T separately]
   *  @param aTrack to link
   *  @return bool true if a ghost
  */
  bool isGhost(LHCbIDs::const_iterator& start,
               LHCbIDs::const_iterator& stop) const override;

  bool isGhost(const LHCb::Track& aTrack) const override;

 private:

 StatusCode specific(LHCbIDs::const_iterator& start,
               LHCbIDs::const_iterator& stop, LHCb::GhostTrackInfo& tinfo) const override;

};

// Dummy override to avoid "partially overridden" warnings from icc
inline bool LongGhostClassification::isGhost(const LHCb::Track& aTrack) const
{
  return TrackGhostClassificationBase::isGhost(aTrack);
}
#endif
