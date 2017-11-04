//
// Created by akucia on 04.11.17.
//

#include "LWTNNClassifier.h"



LWTNNClassifier::LWTNNClassifier(int n) {
    using namespace lwt;
    // get your saved JSON file as an std::istream object

    std::ifstream input(" ");
    // build the graph
    LightweightGraph graph(parse_json_graph(input));

    // fill a map of input nodes
    float value = 1;
    float value_2 = 2;
    float another_value = 3;

    std::map<std::string, std::map<std::string, double> > inputs;
    inputs["input_node"] = {{"value", value}, {"value_2", value_2}};
    inputs["another_input_node"] = {{"another_value", another_value}};
    // compute the output values
    std::map<std::string, double> outputs = graph.compute(inputs);
    std::cout << "LWTNN DONE" << std::endl;
}