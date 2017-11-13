//
// Created by akucia on 04.11.17.
//

#include "PatLWTNNSeedClassifier.h"

DECLARE_TOOL_FACTORY( PatLWTNNSeedClassifier )

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
    
    std::string classifier_path = getEnvVar("PARAMFILESROOT")
                                + "/data/"
                                + m_PatLWTNNSeedClassifier_network_graph_file;
    
    auto file = file_access->open(classifier_path);
    
    if (!file->good()) {
        error() << "Unable to open file '" << classifier_path << "'"
                << endmsg;
        return StatusCode::FAILURE;
    }
    
    info() << "opened PatLWTNNSeedClassifier network graph file: " << classifier_path
           << endmsg;
    
    lwt::GraphConfig graph_config;
    try {
        std::ifstream model_json_stream(file);
        graph_config = lwt::parse_json_graph(model_json_stream);
    }
    catch () {
//        error() << "problem parsing data: " << e.what()
//                << endmsg;
        return StatusCode::FAILURE;
    }
    try {
        lwg = std::make_unique<lwt::LightweightGraph>(graph_config, "");
        return StatusCode::SUCCESS;
    }
    catch () {
//        error() << "problem parsing data: " << e.what()
//                << endmsg;
        return StatusCode::FAILURE;
    }
}

double PatLWTNNSeedClassifier::getMvaValue(const LHCb::Track& track) {
    const Gaudi::XYZPoint position = track.position();
    const Gaudi::XYZVector slopes  = track.slopes();
    
    const double seed_r2 = position.x()*position.x() + position.y()*position.y();
    const double tanh_pseudo_rapidity = track.pt()/track.p();
    
    std::map<std::string, std::map<std::string, double> > inputs;
    inputs["input"] = {
            {"seed_chi2PerDoF", track.chi2PerDoF()},
            {"seed_p", track.p()},
            {"seed_pt", track.pt()},
            {"seed_x", position.x()},
            {"seed_y", position.y()},
            {"seed_tx", slopes.x()},
            {"seed_ty", slopes.y()},
            {"seed_pr", tanh_pseudo_rapidity},
            {"seed_r", seed_r2},
        
    };
    
    std::map<std::string, double> outputs = lwg->compute(inputs);
    
    double results = 0.0;
    for (auto i : outputs) {
        results = i.second;
    }
    return results;
}