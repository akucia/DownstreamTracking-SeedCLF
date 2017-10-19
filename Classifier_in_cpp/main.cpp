#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "loader.h"
#include "tensorflow/cc/saved_model/tag_constants.h"

using namespace tensorflow;
using std::cout;
using std::vector;
using std::pair;

int main(int argc, char* argv[]) {
    // Initialize a tensorflow session
    Session* session;
    Status status = NewSession(SessionOptions(), &session);
    if (!status.ok()) {
        cout << status.ToString() << "\n";
        return 1;
    }
    char* model_path = argv[2];
    cout << "READING MODEL: " << model_path << std::endl;
    GraphDef graph_def;

    const string export_dir = "saved_model";

    if (!MaybeSavedModelDirectory("saved_model")){
        cout << "Provided directory does not contain TF Model" << "\n";
        return 1;
    }


    SavedModelBundle bundle;
    status = LoadSavedModel(SessionOptions(), RunOptions(), export_dir,  {kSavedModelTagServe}, &bundle);

    if (!status.ok()) {
        cout << status.ToString() << "\n";
        return 1;
    }
    cout << "DONE reading model.\n";
}