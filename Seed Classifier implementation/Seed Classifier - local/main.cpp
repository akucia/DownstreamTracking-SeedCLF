#include "src/LWTNNClassifier.h"

#include <iostream>
#include <chrono>

using std::cout;
using std::endl;

class Timer {
public:
    Timer()
            :beg_(clock_::now()) { }
    void reset() {
        beg_ = clock_::now();
    }
    double elapsed() const {
        return std::chrono::duration_cast<second_>(
                clock_::now() - beg_).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

const std::string MODELS_DIR = "../../../models/keras2lwtnn/";

int main(int argc, char* argv[]) {
    
    std::string MODEL_VERSION =
            "lwtnn_v1_13/lwtnn_model_v1_13.json";
    std::string model_path = MODELS_DIR + MODEL_VERSION;
    cout << "Using model: " << model_path << endl;
    Timer tmr;
    double t;
    tmr.reset();
    
    LWTNNClassifier clf(model_path, "");
    
    t = tmr.elapsed();
    cout << "Model loading [s]: " << t << endl;
    
    std::vector<double> input_values{
            3.82356015e-01, 6.75829624e+03, 7.14254677e+02,
            -7.12043777e+01, 1.44388506e+02, 1.04572226e-01,
            1.89806215e-02
    };
    cout << "Example prediction: " << clf.predict(input_values) << endl;
    
    tmr.reset();
    int N = 1000;
    for (int i = 0; i < N; i++) {
        clf.predict(input_values);
    }
    t = tmr.elapsed();
    cout << "Model prediction [s]: " << t / N << endl;
}
