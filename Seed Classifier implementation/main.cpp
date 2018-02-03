#include "src/myconstants.h"
#include "src/TFBinaryClassifier.h"
#include "src/LWTNNClassifier.h"

#include <iostream>
#include <chrono>

using std::cout;
using std::endl;

class Timer {
public:
    Timer()
            :beg_(clock_::now()) { }
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
                (clock_::now() - beg_).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

int main(int argc, char* argv[]) {
    
    std::string model_path = argv[2];
    std::vector<double> input_values_d{
            3.82356015e-01, 6.75829624e+03, 7.14254677e+02,
            -7.12043777e+01, 1.44388506e+02, 1.04572226e-01,
            1.89806215e-02
    };
    
    Timer tmr;
    double t;
    
    LWTNNClassifier clf("pat_nn_seed_clf_half_model_v1.0.json", "");
    
    tmr.reset();
    cout << clf.predict(input_values_d) << " " << endl;
    
    double results = 0.0;
    t = tmr.elapsed();
    cout << t << endl;
    
    tmr.reset();
    for (int i = 0; i < 10; i++) {
        clf.predict(input_values_d);
    }
    t = tmr.elapsed();
    
    cout << "lwtnn: " << t / 10.0 << endl;
}
