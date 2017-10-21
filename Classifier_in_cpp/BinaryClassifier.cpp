#include "BinaryClassifier.h"


using std::cout;
using std::endl;
using namespace tensorflow;


BinaryClassifier::BinaryClassifier(string model_path, string prediction_mode)
:
        prediction_mode(prediction_mode),
        input_features(15)
{
    cout << "READING MODEL: " << model_path << endl;
    GraphDef graph_def;

    const string export_dir = model_path;

    if (!MaybeSavedModelDirectory(export_dir)){
        cout << "Provided directory does not contain TF Model" << "\n";
        exit(1);
    }

    auto status = LoadSavedModel(SessionOptions(), RunOptions(), export_dir,  {kSavedModelTagServe}, &bundle);

    if (!status.ok()) {
        cout << status.ToString() << "\n";
        exit(1);
    }

    cout << "DONE reading model.\n";
    const auto signature_def_map = bundle.meta_graph_def.signature_def();
    const auto signature_def = signature_def_map.at(prediction_mode);

    input_name = signature_def.inputs().at(kMyPredictInputs).name();
    if (prediction_mode == kMyClassifySignatureDef)
        output_name = signature_def.outputs().at(kMyPredictOutputClasses).name();
    else if(prediction_mode == kMyPredictProbabilitySignatureDef)
        output_name = signature_def.outputs().at(kMyPredictOutputScores).name();

    input = make_tensor<float>(DT_FLOAT, TensorShape({1, input_features}));


}

std::vector<float> BinaryClassifier::predict(std::vector<float>& X) {
    update_tensor(input, X);
    outputs.clear();

    TF_CHECK_OK((bundle.session->Run({{input_name, input}}, {output_name}, {}, &outputs)));
    if (prediction_mode == kMyClassifySignatureDef)
        outputs[0].matrix<int64>();
    else if (prediction_mode == kMyPredictProbabilitySignatureDef)
        outputs[0].matrix<float>();
    return X;
}

template <typename T>
Tensor BinaryClassifier::make_tensor(DataType dt, TensorShape shape) {
    Tensor tensor(dt, shape);
    for (int i = 0; i < tensor.NumElements(); ++i)
        tensor.flat<T>()(i) = 0.0;

    return tensor;
}

template <typename T>
void BinaryClassifier::update_tensor(Tensor& tensor, const std::vector<T> &vector) {
    if (vector.size() != input.NumElements())
        cout << "Wrong input" << endl;
    else
        for (int i = 0; i < input.NumElements(); ++i) {
            input.flat<T>()(i) = vector[i];
    }
}

BinaryClassifier::~BinaryClassifier() {
    TF_CHECK_OK(bundle.session->Close());
}