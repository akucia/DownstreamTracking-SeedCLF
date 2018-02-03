//
// Created by akucia on 04.11.17.
//


#ifndef CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H
#define CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class LWTNNClassifier2 {
public:
    LWTNNClassifier2(std::string model_path, std::string default_output);
    double predict(std::vector<double>& input);

private:
    std::unique_ptr<lwt::LightweightGraph> lwg;
};

#endif //CLASSIFIER_IN_CPP_LWTNNCLASSIFIER2_H
