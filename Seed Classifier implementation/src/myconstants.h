//
// Created by akucia on 21.10.17.
//

#ifndef CLASSIFIER_IN_CPP_CONSTANTS_H
#define CLASSIFIER_IN_CPP_CONSTANTS_H


////////////////////////////////////////////////////////////////////////////////
/// tensorflow model directory
    static constexpr char kSavedModelDir[] = "saved_models";

namespace tensorflow {

////////////////////////////////////////////////////////////////////////////////
/// My version of tensorflow/cc/saved_model/signature_constants.h
/// combines Classify API and Predict API

/// Predict inputs (not changed)
    static constexpr char kMyPredictInputs[] = "inputs";

/// Predict method name used in a SignatureDef (not changed)
    static constexpr char kMyPredictMethodName[] = "tensorflow/serving/predict";

/// Predict classes output.
    static constexpr char kMyPredictOutputClasses[] = "classes";

/// Predict scores output.
    static constexpr char kMyPredictOutputScores[] = "scores";


////////////////////////////////////////////////////////////////////////////////
/// Default signature definitions used in project:

    static constexpr char kMyClassifySignatureDef[] = "classify";

    static constexpr char kMyPredictProbabilitySignatureDef[] = "predict_probability";
}


#endif //CLASSIFIER_IN_CPP_CONSTANTS_H
