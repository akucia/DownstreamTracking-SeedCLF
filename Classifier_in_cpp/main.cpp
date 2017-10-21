#include "myconstants.h"
#include "BinaryClassifier.h"

#include <iostream>
#include <chrono>

using std::cout;
using std::endl;

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
                (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};


int main(int argc, char* argv[]) {

    std::string model_path = argv[2];

    std::vector<float> input_values {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    std::vector<float> input_values_2 {2.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    Timer tmr;
    double t = tmr.elapsed();
    tmr.reset();
    BinaryClassifier classifier(model_path, kMyPredictProbabilitySignatureDef);


    t = tmr.elapsed();
    std::cout << t << std::endl;

    tmr.reset();
    for (int i=0; i<100000; i++) {
        classifier.predict(input_values);
    }
    t = tmr.elapsed();
    std::cout << t << std::endl;

}
