//
// Created by akucia on 22.10.17.
//

#include "PatNeuralNetworkSeedClassifier.h"


//DECLARE_TOOL_FACTORY( PatNeuralNetworkSeedClassifier )

using std::vector;
using std::string;

PatNeuralNetworkSeedClassifier::PatNeuralNetworkSeedClassifier(
        const string& type,
        const string& name,
        const IInterface* parent
)
        :
        tf_classifier(kSavedModelDir, kMyPredictProbabilitySignatureDef),
        GaudiTool (type, name, parent) {declareInterface<IPatMvaClassifier>(this); }

double PatNeuralNetworkSeedClassifier::getMvaValue(const vector<double>& parametersVector) {
	auto X = process_parameters<double>(parametersVector);
	auto predictions = tf_classifier.predict(X);
	return (double) predictions[0];
}

template <typename T>
vector<float> PatNeuralNetworkSeedClassifier::process_parameters(
		const vector<T>& parameters
){
	// TODO magic numbers
	vector<float> X (15);
	for (auto value: parameters)
		// TODO feature pre-processing here
		X.push_back((float) value);

	return X;
}

StatusCode PatNeuralNetworkSeedClassifier::initialize() {
    StatusCode sc =  GaudiTool::initialize();
    if(sc.isFailure()) return Error("Failed to initialize", sc);
	return StatusCode::SUCCESS;

}

