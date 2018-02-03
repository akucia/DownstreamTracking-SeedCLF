// $Id: $
#ifndef PATTSTATIONDEBUGTOOL_H
#define PATTSTATIONDEBUGTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "PatKernel/IPatDebugTool.h"            // Interface


/** @class PatTStationDebugTool PatTStationDebugTool.h
 *
 *
 *  @author Olivier Callot
 *  @date   2010-10-20
 */
class PatTStationDebugTool : public GaudiTool, virtual public IPatDebugTool {
public:
  /// Standard constructor
  PatTStationDebugTool( const std::string& type,
                        const std::string& name,
                        const IInterface* parent);

  virtual ~PatTStationDebugTool( ); ///< Destructor

  bool matchKey( LHCb::LHCbID& id, int key ) override;

  void printKey( MsgStream& msg, LHCb::LHCbID& id ) override;

  double xTrue( int key, double z ) override;

  double yTrue( int key, double z ) override;

protected:

private:

};
#endif // PATTSTATIONDEBUGTOOL_H
