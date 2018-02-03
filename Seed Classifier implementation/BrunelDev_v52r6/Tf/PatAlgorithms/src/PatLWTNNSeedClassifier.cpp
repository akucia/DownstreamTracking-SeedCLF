//
// Created by akucia on 04.11.17.
//

#include "PatLWTNNSeedClassifier.h"


#include "Kernel/STLExtensions.h"
#include "GaudiKernel/IFileAccess.h"
#include <numeric>
#include <utility>
#include <stdexcept>
#include <cstdlib>



DECLARE_TOOL_FACTORY( PatLWTNNSeedClassifier )
namespace {
std::string getEnvVar( const char* key ) {
        const char *val = getenv( key );
        return val ? val : "" ;
    }
}

StatusCode PatLWTNNSeedClassifier::initialize() {
    StatusCode sc =  GaudiTool::initialize();
    if(sc.isFailure())
        return Error("Failed to initialize", sc);
    
    auto file_access = service <IFileAccess> ( "VFSSvc" );
    if (!file_access) {
        error() << "Unable to locate IFileAccess('VFSSvc') service"
                << endmsg;
        return StatusCode::FAILURE;
    }
    
    std::string classifier_path = getEnvVar("LOCALPARAMFILESROOT")
                                + "/data/"
                                + m_PatLWTNNSeedClassifier_network_graph_file;
    
    auto file = file_access->open(classifier_path);
    
    if (!file->good()) {
        error() << "Unable to open file '" << classifier_path << "'"
                << endmsg;
        return StatusCode::FAILURE;
    }
    
    info() << "opened network graph file: " << classifier_path
           << endmsg;
    
    lwt::GraphConfig graph_config;
    try {
        graph_config = lwt::parse_json_graph(*file);
    }
    catch (std::exception& e) {
        error() << "problem parsing graph file: " << e.what()
                << endmsg;
        return StatusCode::FAILURE;
    }
    try {
        lwg = std::make_unique<lwt::LightweightGraph>(graph_config, "");
        return StatusCode::SUCCESS;
    }
    catch (std::exception& e) {
        error() << "problem initializing network graph: " << e.what()
                << endmsg;
        return StatusCode::FAILURE;
    }
}

double PatLWTNNSeedClassifier::getMvaValue(const LHCb::Track& track) const {
    const Gaudi::XYZPoint position = track.position();
    const Gaudi::XYZVector slopes  = track.slopes();
   
    std::map<std::string, std::map<std::string, double> > inputs;

	inputs["input"] = {
		    {"seed_chi2PerDoF", std::sqrt(track.chi2PerDoF())},
		    {"seed_p", std::log10(track.p())},
		    {"seed_pt", std::log10(track.pt())},
		    {"seed_x", position.x()},
		    {"seed_y", position.y()},
		    {"seed_tx", slopes.x()},
		    {"seed_ty", slopes.y()},
		    {"seed_angle", std::atan(position.y()/position.x())},
		    {"seed_pr", std::atanh(track.pt()/track.p())},
		    {"seed_z_magnet", (5376.8 - 3895.12 * slopes.y() * slopes.y() + 309.877 * slopes.x() * slopes.x() + 89.901 / track.p() - 4732.0) / (5870.0 - 4732.0) },
		    {"seed_r", std::sqrt(position.x()*position.x() + position.y()*position.y())},
    };
    
    std::map<std::string, double> outputs = lwg->compute(inputs);
    
    return outputs["seed"];
}
