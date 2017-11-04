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

namespace PatNeuralNetworkSeedClassifier_details {
	static constexpr int s_featuresNumber = 10;
	static constexpr int s_binPerFeature = 3;
	static constexpr int s_nBins = 1048576; //  ipow( s_binPerFeature+1, s_featuresNumber );
}


class PatNeuralNetworkSeedClassifier : public extends<GaudiTool, IPatMvaClassifier>
{
public:

	using base_class::base_class;

	StatusCode initialize() override;

	//TODO: even better, pass a vector of LHCb::Track, and return a vector
	//      of MVA values
	//TODO: even better, pass a vector of const LHCb::Track*, and add a property to this
	//      tool which specifies a cut on the BDT (or pass it as argument), and return a
	//      vector of the const LHCb::Track* which survive the cut...
	double getMvaValue(const LHCb::Track& track) const  override;

private:

	std::array<float,PatBBDTSeedClassifier_details::s_nBins> m_tupleClassifier = { {0.f} };

	Gaudi::Property<std::string> m_lookupTableLocation
			{this, "LookupTableLocation" , "PatLLT_BBDT_tuple_DR_v1.csv" };

    //todo magic 'number' hardcoded model location
    Gaudi::Property<std::string> m_saved_model_location
            {this, "SavedModelLocation" , "saved_model" };

    double classify(
            double chi2PerDoF,
            double p,
            double pt,
            double nLHCbIDs,
            double x,
            double y,
            double r,
            double pseudo_rapidity
    );

};

#endif //CLASSIFIER_IN_CPP_PATNEURALNETWORKSEEDCLASSIFIER_H
