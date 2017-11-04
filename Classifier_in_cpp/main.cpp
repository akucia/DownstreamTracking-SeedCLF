#include "src/myconstants.h"
#include "src/TFBinaryClassifier.h"
#include "src/LWTNNClassifier.h"

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

    using namespace tensorflow;
    std::string model_path = argv[2];
    std::vector<float> input_values {1.0, 9.0, 4.0, 1.0, 1.0, 1.0, 1.0, 50.0, 1.0, 1.0, 1.0, 3.0, 1.0, 1.0, 1.0};

    Timer tmr;
    double t;

    tmr.reset();
    BinaryClassifier classifier(model_path, kMyPredictProbabilitySignatureDef);

    t = tmr.elapsed();
    cout << t << endl;

    tmr.reset();
    for (int i=0; i<10; i++) {
        cout << classifier.predict(input_values)[0] << " " << classifier.predict(input_values)[1]  << " " << endl;
    }
    cout << endl;
    t = tmr.elapsed();
    cout << t << endl;

    LWTNNClassifier clf (2);

}
