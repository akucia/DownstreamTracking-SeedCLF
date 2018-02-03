#ifndef PATTSTATIONHITMANAGER_H
#define PATTSTATIONHITMANAGER_H 1
// Include files
#include "PatKernel/PatForwardHit.h"
#include "TfKernel/TStationHitManager.h"

/** @class PatTStationHitManager PatTStationHitManager.h
 *
 *  T station hit manager for Pat.
 *
 *  @author S. Hansmann-Menzemer, W. Hulsbergen, C. Jones, K. Rinnert
 *  @date   2007-06-01
 */

struct PatTStationHitManager : Tf::TStationHitManager<PatForwardHit>
{
  using Tf::TStationHitManager<PatForwardHit>::TStationHitManager;
  void prepareHits() const override;
};

#endif // PATTSTATIONHITMANAGER_H
