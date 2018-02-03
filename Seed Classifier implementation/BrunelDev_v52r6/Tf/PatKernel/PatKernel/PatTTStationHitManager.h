#ifndef PATTTSTATIONHITMANAGER_H 
#define PATTTSTATIONHITMANAGER_H 1
// Include files
#include "PatKernel/PatTTHit.h"
#include "TfKernel/TTStationHitManager.h"

/** @class PatTTStationHitManager PatTTStationHitManager.h
 *
 *  TT station hit manager for Pat.
 *
 *  @author S. Hansmann-Menzemer, W. Hulsbergen, C. Jones, K. Rinnert
 *  @date   2007-06-01 Initial version
 */

struct PatTTStationHitManager : Tf::TTStationHitManager<PatTTHit>
{
  using Tf::TTStationHitManager<PatTTHit>::TTStationHitManager;
};


#endif // PATTSTATIONHITMANAGER_H
