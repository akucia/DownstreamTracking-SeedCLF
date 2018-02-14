#ifndef CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H
#define CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class LWTNNClassifier {
public:
    LWTNNClassifier(std::string model_path, std::string default_output);
    double predict(std::vector<double>& input);

private:
    std::unique_ptr<lwt::LightweightGraph> lwg;
};

#endif //CLASSIFIER_IN_CPP_LWTNNCLASSIFIER_H
