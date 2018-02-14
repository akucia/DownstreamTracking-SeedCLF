#ifndef CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H
#define CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "PatKernel/IPatMvaClassifier.h"
#include "GaudiAlg/GaudiTool.h"
#include "Event/Track.h"

#include <exception>
#include <cmath>
#include <iostream>
#include <array>
#include <string>

#include <TRandom3.h>

class PatLWTNNSeedClassifier : public extends<GaudiTool, IPatMvaClassifier>
{
public:
    
    using base_class::base_class;
    StatusCode initialize() override;
    double getMvaValue(const LHCb::Track& track) const override;

private:
    std::unique_ptr<TRandom3> rand;
    std::unique_ptr<lwt::LightweightGraph> lwg;
    Gaudi::Property<std::string> m_PatLWTNNSeedClassifier_network_graph_file
            {this, "NetworkGraphLocation" , "lwtnn_v1.13.json" };
};

#endif //CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H
