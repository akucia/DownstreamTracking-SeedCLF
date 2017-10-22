//
// Created by akucia on 22.10.17.
//

#ifndef CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H
#define CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H

//#include "PatKernel/IPatMvaClassifier.h"
//#include "PatKernel/IPatMvaClassifier.h"
//#include "GaudiAlg/GaudiTool.h"

#include "BinaryClassifier.h"
#include "myconstants.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>


using std::vector;
using std::string;

class PatNeuralNetworkSeedClassifier //: public GaudiTool, virtual public IPatMvaClassifier{
{
public:
    PatNeuralNetworkSeedClassifier(
            const string& type,
            const string& name,
            const IInterface* parent
    );

    double getMvaValue(const vector<double>& parametersVector);
    virtual StatusCode initialize();

private:
    BinaryClassifier tf_classifier;

	template <typename T>
	vector<float> process_parameters(const vector<T>& parameters);
};

#endif //CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H
