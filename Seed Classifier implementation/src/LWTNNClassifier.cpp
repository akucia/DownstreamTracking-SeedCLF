//
// Created by akucia on 04.11.17.
//

#include "LWTNNClassifier.h"

#include "lwtnn/parse_json.hh"
#include "lwtnn/LightweightGraph.hh"

LWTNNClassifier::LWTNNClassifier(int n) {
    using namespace lwt;
    // get your saved JSON file as an std::istream object
    using namespace std;
    std::ifstream input("neural_net.json");
    
//    cout << input. << endl;
    // build the graph
    LightweightGraph graph(parse_json_graph(input));

    // fill a map of input nodes
    float value = 1;
    
    std::map<std::string, std::map<std::string, double> > inputs;
    inputs["node_0"] = {
            {"variable_0", value},
            {"variable_1", value},
            {"variable_2", value},
            {"variable_3", value},
            {"variable_4", value},
            {"variable_5", value},
            {"variable_6", value},
            {"variable_7", value},
            
    };
    // compute the output values
    std::map<std::string, double> outputs = graph.compute(inputs);
    std::cout << "LWTNN DONE" << std::endl;
}