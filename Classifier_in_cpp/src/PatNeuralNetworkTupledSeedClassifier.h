//
// Created by akucia on 22.10.17.
//

#ifndef CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H
#define CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H

#include "PatKernel/IPatMvaClassifier.h"
#include "GaudiAlg/GaudiTool.h"
#include "Event/Track.h"

#include <iostream>
#include <array>
#include <string>

namespace PatNeuralNetworkTupledSeedClassifier_details {
	static constexpr int s_featuresNumber = 8;
	static constexpr int s_binEdgesPerFeature = 7;
	static constexpr int s_nBins = ipow(s_binEdgesPerFeature+1, s_featuresNumber);
}


class PatNeuralNetworkTupledSeedClassifier:
        public extends <GaudiTool, IPatMvaClassifier> {
public:

	using base_class::base_class;
 
	StatusCode initialize() override;
	double getMvaValue(const LHCb::Track& track) const override;

private:

	std::array< float,
                PatNeuralNetworkTupledSeedClassifier_details::s_nBins >
            m_tupleClassifier = { {0.f} };

	Gaudi::Property<std::string> m_lookupTableLocation
            {this, "LookupTableLocation" , "PatLLT_BBDT_tuple_DR_v1.csv"};
};

#endif //CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H
