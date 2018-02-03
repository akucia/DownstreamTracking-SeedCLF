#ifndef PATKERNEL_IPATMVACLASSIFIER_H
#define PATKERNEL_IPATMVACLASSIFIER_H 1

#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
     class Track;
}

/** @class IPatMvaClassifier PatKernel/IPatMvaClassifier.h
 *
 *  @author Adam Dendek
 *  @date   2016-03-17
 */
struct IPatMvaClassifier : extend_interfaces<IAlgTool>
{
  DeclareInterfaceID( IPatMvaClassifier, 2, 0 );
  virtual double getMvaValue(const LHCb::Track& track) const  = 0;
};
#endif //PATKERNEL_IPATMVACLASSIFIER_H
