//
// Created by akucia on 22.10.17.
//

#include "PatNeuralNetworkTupledSeedClassifier.h"

#include "Kernel/STLExtensions.h"
#include "GaudiKernel/IFileAccess.h"
#include <numeric>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <cmath>

#if __GNUC_ < 6  && !defined(__clang__)
// disable warnings from range-v3 view with gcc 4.9
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#include "range/v3/view.hpp"
#if __GNUC_ < 6  && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include "range/v3/front.hpp"
#include "range/v3/numeric.hpp"
#include "range/v3/algorithm.hpp"
#include "range/v3/getlines.hpp"


DECLARE_TOOL_FACTORY( PatNeuralNetworkTupledSeedClassifier )

namespace {
	using namespace std::string_literals;
	using namespace PatNeuralNetworkTupledSeedClassifier_details;

//    using feature_bins = std::array <double, s_binEdgesPerFeature>;
//
//    using named_feature_bins = std::pair <std::string, feature_bins>;
//
//    using named_feature_bins_array = std::array <named_feature_bins, s_featuresNumber>;

//    const named_feature_bins_array s_binsEdgeMap = LHCb::make_array(
    static const std::array<
            std::pair<
                    std::string,
                    std::array <double,s_binEdgesPerFeature>
            >,
            s_featuresNumber
    >
            s_binsEdgeMap = LHCb::make_array(
    
            std::make_pair(
                    "seed_chi2PerDoF"s, LHCb::make_array(
                            0.83062251768, 0.966449975967, 1.07437366956, 1.1806936264,
                            1.29992735386, 1.45262792889, 1.6921743315
                    )
            ),
			std::make_pair(
                    "seed_p"s, LHCb::make_array(
                            3.40419179023, 3.54774620985, 3.6885446049, 3.83795881814,
                            4.00216394749,4.19318873788,4.44552460337
                    )
            ),
			std::make_pair(
                    "seed_pt"s, LHCb::make_array(
                            2.93287723089, 3.00056036336, 3.03630805016, 3.06373023987,
                            3.09747967679, 3.14478530602, 3.23500652822
                    )
            ),
			std::make_pair(
                    "seed_nLHCbIDs"s, LHCb::make_array(
                            12.0, 12.0, 18.0, 19.0,
                            21.0, 22.0, 23.0
                    )
            ),
			std::make_pair(
                    "abs_seed_x"s, LHCb::make_array(
                            0.13584864002, 0.227734415294, 0.358202199672, 0.535721838474,
                            0.78708477677, 1.13540373566, 1.62112611253
                    )
            ),
			std::make_pair(
                    "abs_seed_y"s, LHCb::make_array(
                            0.0786098579155, 0.153074478715, 0.226096953958, 0.343206497893,
                            0.530559645248,0.83622646678,1.43651447563
                    )
            ),
			std::make_pair(
                    "abs_seed_tx"s, LHCb::make_array(
                            0.130490017939, 0.230097115693, 0.360599216109, 0.536835267813,
                            0.780595404923, 1.11747034859, 1.62756533932
                    )
            ),
			std::make_pair(
                    "abs_seed_ty"s, LHCb::make_array(
                            0.0838901448342, 0.167362714627, 0.256956820805, 0.393944647835,
                            0.599766724659, 0.912113262057, 1.47878086304
                    )
            )
    );

	static const auto observables = view::keys(s_binsEdgeMap) | view::join(',') | to_<std::string>();

	std::string getEnvVar(const char* key) {
        const char *val = getenv(key);
        return val ? val : "" ;
	}

	int combine_indices(int offset, int indx) {
		return offset * (s_binEdgesPerFeature + 1) + indx;
	}

	std::pair <int, float> toOffsetValue(const std::string& line) {
        auto tokens = view::split(line,',') | view::chunk(s_featuresNumber);
        
		auto stoi = [] (const std::string& s) { return std::stoi(s); };
  
		auto index = accumulate(front(tokens), 0, combine_indices, stoi);
  
		if (index >= s_nBins)
            throw std::invalid_argument("index out of range");
        
        return {
                index,
                std::stof(to_<std::string>(front(*next(begin(tokens)))))
        };
	}

	template <typename Range>
	int getOffset(const Range& params) {
        assert(params.size() == s_featuresNumber);
        
		auto get_index = [] (const auto& edges, double value) {
          return distance(begin(edges), lower_bound(edges, value));
        };
        
		return inner_product(
                view::values(s_binsEdgeMap),
                params,
                0,
                combine_indices,
                get_index
        );
	}
}

StatusCode PatNeuralNetworkTupledSeedClassifier::initialize() {
	StatusCode sc =  GaudiTool::initialize();
	if(sc.isFailure())
        return Error("Failed to initialize", sc);

    // File access service
	auto fileAccess = service <IFileAccess> ( "VFSSvc" );
	if (!fileAccess) {
		error() << "Unable to locate IFileAccess('VFSSvc') service"
                << endmsg;
        
		return StatusCode::FAILURE;
	}
    //
	std::string classifierPath = getEnvVar("PARAMFILESROOT") + "/data/" + m_lookupTableLocation;
	auto file = fileAccess->open (classifierPath);
 
	if (!file->good()) {
		error() << "Unable to open file '" << classifierPath << "'"
                << endmsg;
        
		return StatusCode::FAILURE;
	}

	info() << "open file: " << classifierPath
           << endmsg;
    
	try {
		auto lines = getlines(*file);
		auto features = front(lines);
		info() << "input features list " << features
               << endmsg;
        
		if (!equal(features,observables + ", pred")) {
			warning() << "expected \"" << observables << ",pred\" but got \""
                      << features << "\" instead"
                      << endmsg;
		}

		int n = 0;
		for_each(
                view::tail(lines) | view::transform(toOffsetValue),
                [&](const std::pair<int,float>& offsetValue) {
                  m_tupleClassifier[offsetValue.first] = offsetValue.second;
                  ++n;
                }
        );

		if (n!=s_nBins) {
			warning() << "expected " << s_nBins << " entries, got "  << n << "instead"
                      << endmsg;
		}

		return StatusCode::SUCCESS;

	}
    catch (const std::invalid_argument& e) {
		error() << "problem parsing data: " << e.what()
                << endmsg;

		return StatusCode::FAILURE;
	}
}

double PatNeuralNetworkTupledSeedClassifier::getMvaValue(const LHCb::Track& track) const {
	
	const Gaudi::XYZPoint position = track.position();
	const Gaudi::XYZVector slopes  = track.slopes();
    std::ipow();
	return m_tupleClassifier[
            getOffset(
                    LHCb::make_array(
                            std::sqrt(track.chi2PerDoF()),
                            std::log10(track.p()),
                            std::log10(track.pt()),
                            static_cast<double>(track.nLHCbIDs()),
                            (std::abs(position.x()) + 3.58761813) / 718.45885408,
                            (std::abs(position.y()) - 1.0987292 ) / 504.799883,
                            (std::abs(slopes.x())   + 0.00422249) / 0.29576847,
                            (std::abs(slopes.y())   - 0.00013073) / 0.06486862
                    )
            )
    ];
}
