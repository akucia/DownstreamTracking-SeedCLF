#ifndef CLASSIFIER_IN_CPP_PATBDDSEEDCLASSIFIER_H
#define CLASSIFIER_IN_CPP_PATBDDSEEDCLASSIFIER_H

#include "PatKernel/IPatMvaClassifier.h"
#include "GaudiAlg/GaudiTool.h"
#include "Event/Track.h"

#include <iostream>
#include <array>
#include <string>

namespace PatBBDTSeedClassifier_details {
    static constexpr int s_featuresNumber = 10;
    static constexpr int s_binPerFeature = 3;
    static constexpr int s_nBins = 1048576; //  ipow( s_binEdgesPerFeature+1, s_featuresNumber );
}


class PatBBDTSeedClassifier : public extends<GaudiTool, IPatMvaClassifier>
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
};


#endif //CLASSIFIER_IN_CPP_PATBDDSEEDCLASSIFIER_H
