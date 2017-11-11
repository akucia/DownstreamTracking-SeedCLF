#include "TFBinaryClassifier.h"


using std::cout;
using std::endl;
namespace tensorflow {

    BinaryClassifier::BinaryClassifier(const string model_path,
            const string prediction_mode)
            :
            saved_model_dir(model_path),
            prediction_mode(prediction_mode),
            input_features(15) // TODO magic number
    {
        cout << "READING MODEL: " << saved_model_dir << endl;
        GraphDef graph_def;

        if (!MaybeSavedModelDirectory(saved_model_dir)) {
            cout << "Provided directory does not contain TF Model" << "\n";
            exit(1);
        }

        auto status = LoadSavedModel(SessionOptions(), RunOptions(), saved_model_dir,
                {kSavedModelTagServe}, &bundle);

        if (!status.ok()) {
            cout << status.ToString() << "\n";
            exit(1);
        }

        cout << "DONE reading model.\n";
        const auto& signature_def_map = bundle.meta_graph_def.signature_def();
        const auto& signature_def = signature_def_map.at(prediction_mode);

        input_name = signature_def.inputs().at(kMyPredictInputs).name();
        if (prediction_mode == kMyClassifySignatureDef)
            output_name = signature_def.outputs().at(kMyPredictOutputClasses).name();
        else if (prediction_mode == kMyPredictProbabilitySignatureDef)
            output_name = signature_def.outputs().at(kMyPredictOutputScores).name();

        input = make_tensor<float>(DT_FLOAT, TensorShape({1, input_features}));

    }

    std::vector<float> BinaryClassifier::predict(std::vector<float>& X) {
        update_tensor(input, X);

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

    template<typename T>
    Tensor BinaryClassifier::make_tensor(DataType dt, TensorShape shape) {
        Tensor tensor(dt, shape);
        for (int i = 0; i<tensor.NumElements(); ++i)
            tensor.flat<T>()(i) = 0.0;

        return tensor;
    }

    template<typename T>
    void BinaryClassifier::update_tensor(Tensor& tensor, const std::vector<T>& vector) {
        if (vector.size() != input.NumElements())
            cout << "Wrong input" << endl;
        else
            for (int i = 0; i<input.NumElements(); ++i) {
                input.flat<T>()(i) = vector[i];
            }
    }

    BinaryClassifier::~BinaryClassifier() {
        TF_CHECK_OK(bundle.session->Close());
    }
}