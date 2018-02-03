#ifndef _TTrackGhostClassification_H
#define _TTrackGhostClassification_H

#include "TrackGhostClassificationBase.h"


namespace LHCb{
  class Track;
  class OTChannelID;
  class STChannelID;
}

class DeOTDetector;
class DeSTDetector;

class TTrackGhostClassification: public TrackGhostClassificationBase {

public:

  /// constructer
  TTrackGhostClassification(const std::string& type,
                               const std::string& name,
                               const IInterface* parent);

  /** destructer */
  virtual ~TTrackGhostClassification();


  /** initialize */
  StatusCode initialize() override;

 private:

  DeOTDetector* m_oTracker;
  DeSTDetector* m_iTracker;

  bool stereoOT(const LHCb::OTChannelID& chan) const;
  bool stereoIT(const LHCb::STChannelID& chan) const;


  StatusCode specific(LHCbIDs::const_iterator& start,
                LHCbIDs::const_iterator& stop, LHCb::GhostTrackInfo& tinfo) const override;


};



#endif
