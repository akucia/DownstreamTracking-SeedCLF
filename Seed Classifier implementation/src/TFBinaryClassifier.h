//
// Created by akucia on 21.10.17.
//

#ifndef CLASSIFIER_IN_CPP_KERASBINARYCLASSIFIER_H
#define CLASSIFIER_IN_CPP_KERASBINARYCLASSIFIER_H

#include <iostream>
#include <string>
#include <vector>

#include "tensorflow/core/public/session.h"
#include "tensorflow/cc/saved_model/loader.h"
#include "tensorflow/cc/saved_model/tag_constants.h"
#include "tensorflow/cc/client/client_session.h"

#include "myconstants.h"

using std::cout;
using std::endl;
using std::string;
namespace tensorflow {

    class BinaryClassifier {

    public:
        BinaryClassifier(string model_path, string prediction_mode);
        ~BinaryClassifier();
        std::vector<float> predict(std::vector<float>& X);

    private:
        const string saved_model_dir;
        const string prediction_mode;
        int input_features;

        SavedModelBundle bundle;
        string input_name;
        string output_name;
        Tensor input;
        std::vector<Tensor> outputs{};

        template<typename T>
        Tensor make_tensor(DataType dt, TensorShape shape);

        template<typename T>
        void update_tensor(Tensor& tensor, const std::vector<T>& vector);
    };
}
#endif //CLASSIFIER_IN_CPP_KERASBINARYCLASSIFIER_H
