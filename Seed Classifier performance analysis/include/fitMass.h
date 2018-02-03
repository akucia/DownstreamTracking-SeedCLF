//
// Created by akucia on 13.12.17.
//

#ifndef SEED_CLF_EVALUATION_FITMASS_H_H
#define SEED_CLF_EVALUATION_FITMASS_H_H

#include <TFile.h>
#include <TTree.h>
#include <RooDataSet.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TGaxis.h>

#include <RooGaussian.h>
#include <RooExponential.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooAddPdf.h>
#include <RooPlot.h>

#include <iostream>
#include <string>

#include <TLegend.h>



using std::cout;
using std::endl;
using std::string;

void fitMassKs(std::string data_file, std::string baseline_file);
void fitMassLambda(std::string data_file, std::string baseline_file);

void plot_ks_fit_mass(
        const string& file_1, const string& name_1,
        const string& file_2, const string& name_2,
        const string& output_name
);

void plot_lambda_fit_mass(
        const string& file_1, const string& name_1,
        const string& file_2, const string& name_2,
        const string& output_name
);

#endif //SEED_CLF_EVALUATION_FITMASS_H_H
