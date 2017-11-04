//
// Created by akucia on 22.10.17.
//

#include "PatNeuralNetworkSeedClassifier.h"


#include "Kernel/STLExtensions.h"
#include "GaudiKernel/IFileAccess.h"
#include <numeric>
#include <utility>
#include <stdexcept>
#include <cstdlib>

DECLARE_TOOL_FACTORY( PatNeralNetworkSeedClassifier )



namespace
{
	using namespace std::string_literals;
	using namespace PatNeuralNetworkClassifier_details;

	static const std::array <std::pair <std::string, std::array <double, s_binPerFeature>>, s_featuresNumber> s_binsEdgeMap = LHCb::make_array(
            std::make_pair(
                    "seed_chi2PerDoF"s, LHCb::make_array(
                            0.984236677055,
                            1.50321304937,
                            2.42723090951
                    )
            ),
			std::make_pair(
                    "seed_p"s, LHCb::make_array(
                            3306.52056882,
                            6433.94177793,
                            14679.2279654
                    )
            ),
			std::make_pair(
                    "seed_pt"s, LHCb::make_array(
                            974.757029327,
                            1150.16476634,
                            1398.70883468
                    )
            ),
			std::make_pair(
                    "seed_nLHCbIDs"s, LHCb::make_array(
                            12.0,
                            19.0,
                            22.0
                    )
            ),
			std::make_pair(
                    "abs_seed_x"s, LHCb::make_array(
                            165.914375049,
                            391.313089922,
                            812.675652024
                    )
            ),
			std::make_pair(
                    "abs_seed_y"s, LHCb::make_array(
                            84.4601830606,
                            194.563503148,
                            465.430832506
                    )
            ),
			std::make_pair(
                    "abs_seed_tx"s, LHCb::make_array(
                            0.0719119714649,
                            0.169597614236,
                            0.34911262278
                    )
            ),
			std::make_pair(
                    "abs_seed_ty"s, LHCb::make_array(
                            0.011964398121,
                            0.0283512657104,
                            0.0636668621569
                    )
            ),
			std::make_pair(
                    "seed_r"s, LHCb::make_array(
                            std::pow(267.974713881, 2),
                            std::pow(565.104115682, 2),
                            std::pow(1041.65791307, 2)
                    )
            ),
			std::make_pair(
                    "pseudo_rapidity"s, LHCb::make_array(
                            std::tanh(0.0841431458013),
                            std::tanh(0.183059022997),
                            std::tanh(0.35202555464)
                    )
            )
    );

	static const auto observables = view::keys(s_binsEdgeMap) | view::join( ',') | to_<std::string>();

	std::string getEnvVar( const char* key ) {
		const char *val = getenv( key );
		return val ? val : "" ;
	}

	int combine_indices(int offset, int indx) {
		return offset*(s_binPerFeature+1)+indx;
	}

	std::pair <int,float> toOffsetValue(const std::string& line) {
        auto tokens = view::split(line,',') | view::chunk(s_featuresNumber);
		auto stoi = [](const std::string& s) {
          return std::stoi(s);
        };
		auto index = accumulate(front(tokens), 0, combine_indices, stoi);
		if (index>=s_nBins)
            throw std::invalid_argument("index out of range");
		return {
                index, std::stof(to_<std::string>(front(*next(begin(tokens)))))
        };
	}

	template <typename Range>
	int getOffset(const Range& params) {
		assert(params.size() == s_featuresNumber);
		auto get_index = [](const auto& edges, double value) {
		  return distance(begin(edges), lower_bound(edges, value));
		};
		return inner_product(
                view::values(s_binsEdgeMap) ,
                params,
                0,
                combine_indices,
                get_index
        );
	}
}

StatusCode PatNeuralNetworkSeedClassifier::initialize()
{
	StatusCode sc =  GaudiTool::initialize();
	if(sc.isFailure())
        return Error("Failed to initialize", sc);

    // File access service
	auto fileAccess = service <IFileAccess> ( "VFSSvc" );
	if (!fileAccess) {
		error() << "Unable to locate IFileAccess('VFSSvc') service" << endmsg;
		return StatusCode::FAILURE;
	}
    //
	std::string classifierPath = getEnvVar("PARAMFILESROOT") + "/data/" + m_lookupTableLocation;
	auto file = fileAccess->open (classifierPath);
	if (!file->good()) {
		error() << "Unable to open file '" << classifierPath << "'" << endmsg;
		return StatusCode::FAILURE;
	}

	info() << "open file: " << classifierPath<<endmsg;
	try {
		auto lines = getlines(*file);
		auto features = front(lines);
		info() << "input features list " << features << endmsg;
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

double PatNeuralNetworkSeedClassifier::getMvaValue(const LHCb::Track& track) const
{
	//============================================================================
	// Evaluate the Seed Classifier discriminant for the T-seeds
	//============================================================================

	const Gaudi::XYZPoint position = track.position();
	const Gaudi::XYZVector slopes  = track.slopes();

	const double seed_r2 = position.x()*position.x()+position.y()*position.y();
	const double tanh_pseudo_rapidity = track.pt()/track.p();

	return m_tupleClassifier[ getOffset( LHCb::make_array( track.chi2PerDoF(),track.p(),  track.pt(),
			static_cast<double>(track.nLHCbIDs()),
			std::abs(position.x()), std::abs(position.y()),
			std::abs(slopes.x()), std::abs(slopes.y()),
			seed_r2, tanh_pseudo_rapidity) ) ];
}

double PatNeuralNetworkSeedClassifier::classify(
        double chi2PerDoF,
        double p,
        double pt,
        double nLHCbIDs,
        double x,
        double y,
        double r,
        double pseudo_rapidity
) {

    update_input(input, X);

    TF_CHECK_OK((bundle.session->Run({{input_name, input}}, {output_name}, {},
            &outputs)));

    std::vector<float> results(2);

    if (prediction_mode == tensorflow::kMyClassifySignatureDef) {
        results[0] = (float) outputs[0].vec<int64>()(0);
        results[1] = (float) 1.0 - results[0];

    }
    else if (prediction_mode == kMyPredictProbabilitySignatureDef) {
        results[0] = outputs[0].matrix<float>()(0);
        results[1] = outputs[0].matrix<float>()(1);
    }
    outputs.clear();
    return results;
}

void PatNeuralNetworkSeedClassifier::update_tensor(Tensor& tensor, const std::vector<T>& value) {
      input.flat<T>()(i) = vector[i];
        }
}