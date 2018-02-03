//
// Created by akucia on 04.11.17.
//

#include "LWTNNClassifier.h"
#include <cmath>

LWTNNClassifier2::LWTNNClassifier2(std::string model_path, std::string default_output=""){
    std::ifstream model_json_stream (model_path);
    lwt::GraphConfig gc(lwt::parse_json_graph(model_json_stream));
    lwg = std::make_unique<lwt::LightweightGraph>(gc, default_output);
    srand (static_cast <unsigned> (time(0)));
}

double LWTNNClassifier2::predict(std::vector<double>& input){
    // fill a map of input nodes
    using namespace std;
    std::map<std::string, std::map<std::string, double> > inputs;
    inputs["input"] = {
            {"seed_chi2PerDoF", std::sqrt(input[0])},
            {"seed_p", std::log10(input[1])},
            {"seed_pt", std::log10(input[2])},
            {"seed_x", input[3]},
            {"seed_y", input[4]},
            {"seed_tx", input[5]},
            {"seed_ty", input[6]},
            {"seed_angle", std::atan(input[4]/input[3])},
            {"seed_pr", std::atanh(input[2]/input[1])},
            {"seed_r", std::sqrt(input[3]*input[3] + input[4]*input[4])},
    };
    // compute the output values
    std::map<std::string, double> outputs = lwg->compute(inputs);
    for (auto i : outputs){
        std::cout << i.first << " " << i.second << std::endl;
    }
    return outputs["seed"];
}