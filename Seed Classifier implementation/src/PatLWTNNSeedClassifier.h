//
// Created by akucia on 04.11.17.
//


#ifndef CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H
#define CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "PatKernel/IPatMvaClassifier.h"
#include "GaudiAlg/GaudiTool.h"
#include "Event/Track.h"

#include <iostream>
#include <array>
#include <string>

class PatLWTNNSeedClassifier : public extends<GaudiTool, IPatMvaClassifier>
{
public:
    
    using base_class::base_class;
    StatusCode initialize() override;
    double getMvaValue(const LHCb::Track& track) override;

private:
    std::unique_ptr<lwt::LightweightGraph> lwg;
};

#endif //CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H
