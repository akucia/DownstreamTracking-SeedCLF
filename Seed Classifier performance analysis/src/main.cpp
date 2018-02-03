//
// Created by akucia on 08.12.17.
//

#include <iostream>
#include <Histograms.h>
#include <fitMass.h>

using std::cout;
using std::endl;
using std::string;


int main(int argc, char* argv[]){
    
    
//    std::string baseline_file = "../data/KsPiPi2016Down_baseline_.root";
    std::string bbdt_file = "../data/KsPiPi2016Down_bbdt_big_data.root";
//    std::string random_file = "../data/KsPiPi2016Down_rand_.root";
    std::string dnn_file = "../data/KsPiPi2016Down_lwtnn_big_data.root";
    
    string ks_tree_name = "KsPiPiTuple/DecayTree";
    
//    InputData_K baseline_data(baseline_file, ks_tree_name);
    
    InputData_K bbdt_data(bbdt_file, ks_tree_name);
    InputData_K dnn_data(dnn_file, ks_tree_name);
//    InputData_K random_data(random_file, ks_tree_name);

//    compare_normalized_histograms_KS0(
//            bbdt_data,
//            "BBDT",
//            baseline_data,
//            "baseline",
//            "BBDT_vs_baseline_KS0"
//    );
//    baseline_data.reset();
//
//    compare_normalized_histograms_KS0(
//            dnn_data,
//            "DNN",
//            baseline_data,
//            "baseline",
//            "DNN_vs_baseline_KS0"
//    );
//    baseline_data.reset();
//
//    compare_normalized_histograms_KS0(
//            random_data,
//            "random",
//            baseline_data,
//            "baseline",
//            "random_vs_baseline_KS0"
//    );
//    baseline_data.reset();


    string lambda_tree_name = "LambdaPPiTuple/DecayTree";
//    InputData_L baseline_data_2(baseline_file, lambda_tree_name);
    InputData_L bbdt_data_2(bbdt_file, lambda_tree_name);
    InputData_L dnn_data_2(dnn_file, lambda_tree_name);
//    InputData_L random_data_2(random_file, lambda_tree_name);

//    compare_normalized_histograms_L(
//            bbdt_data_2, "BBDT",
//            baseline_data_2, "baseline",
//            "BBDT_vs_baseline_L"
//    );
//    baseline_data_2.reset();
//
//    compare_normalized_histograms_L(
//            dnn_data_2, "DNN",
//            baseline_data_2, "baseline",
//            "DNN_vs_baseline_L"
//    );
//    baseline_data_2.reset();
//
//    compare_normalized_histograms_L(
//            random_data_2, "random",
//            baseline_data_2,"baseline",
//            "random_vs_baseline_L"
//    );
//    baseline_data_2.reset();
    
    
    
    plot_ks_fit_mass(
            dnn_file, "DNN",
            bbdt_file, "BBDT",
            "fitKs_dnn_vs_bbdt.pdf"
    );
    
//    plot_ks_fit_mass(
//            dnn_file, "DNN",
//            baseline_file, "Baseline",
//            "fitKs_dnn_vs_baseline.pdf"
//    );
    
//    plot_ks_fit_mass(
//            bbdt_file, "BBDT",
//            baseline_file, "Baseline",
//            "fitKs_bbdt_vs_baseline.pdf"
//    );
    
//    plot_ks_fit_mass(
//            dnn_file, "DNN",
//            baseline_file, "Baseline",
//            "fitKs_dnn_vs_baseline.pdf"
//    );
//
//    plot_lambda_fit_mass(
//            dnn_file, "DNN",
//            baseline_file, "Baseline",
//            "fitLambda_dnn_vs_baseline.pdf"
//    );
    
    plot_lambda_fit_mass(
            dnn_file, "DNN",
            bbdt_file, "BBDT",
            "fitLambda_dnn_vs_bbdt.pdf"
    );
    
//    plot_lambda_fit_mass(
//            bbdt_file, "BBDT",
//            baseline_file, "Baseline",
//            "fitLambda_bbdt_vs_baseline.pdf"
//    );
    
    return 0;
}