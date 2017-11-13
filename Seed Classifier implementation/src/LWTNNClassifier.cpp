//
// Created by akucia on 04.11.17.
//

#include "LWTNNClassifier.h"

LWTNNClassifier::LWTNNClassifier(std::string model_path, std::string default_output=""){
    std::ifstream model_json_stream (model_path);
    lwt::GraphConfig gc(lwt::parse_json_graph(model_json_stream));
    lwg = std::make_unique<lwt::LightweightGraph>(gc, default_output);
}

double LWTNNClassifier::predict(std::vector<double>& input){
    // fill a map of input nodes
    using namespace std;
    
    std::map<std::string, std::map<std::string, double> > inputs;
    inputs["input"] = {
            {"seed_chi2PerDoF", input[0]},
            {"seed_p", input[1]},
            {"seed_pt", input[2]},
            {"seed_x", input[3]},
            {"seed_y", input[4]},
            {"seed_tx", input[5]},
            {"seed_ty", input[6]},
            {"seed_pr", input[7]},
            {"seed_r", input[8]},
            
    };
    // compute the output values
    std::map<std::string, double> outputs = lwg->compute(inputs);
    double results = 0.0;
    for (auto i : outputs) {
        results = i.second;
    }
    return results;
}