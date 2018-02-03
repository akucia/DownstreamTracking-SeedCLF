//
// Created by akucia on 13.12.17.
//



#include "fitMass.h"

void fitMassKs(std::string data_file, std::string baseline_file) {
    
    
    TFile* file_data = TFile::Open(data_file.c_str());
    TFile* file_baseline = TFile::Open(baseline_file.c_str());
    
    
    TFile* dummy = new TFile("dummy.root","OVERWRITE");
    
    
    TTree* tree_baseline = (TTree*)file_baseline->Get("KsPiPiTuple/DecayTree");
    TTree* tree_data = (TTree*)file_data->Get("KsPiPiTuple/DecayTree");
    
    
    TTree* tree_baseline_copy = tree_baseline->CopyTree("piminus_TRACK_Type == 5 && piplus_TRACK_Type == 5");


    // -- Parameters for CB
    RooRealVar KSmass("KS0_M", "K^{0}_{s} Invariant mass [#frac{MeV}{c^{2}}]", 450.0, 550.0);
    RooRealVar meanKS("meanKS", "meanKS", 475.0, 450.0, 530.0);
    RooRealVar sigma("sigma","sigma",10.0, 2.0, 20.0);
    RooGaussian gauss("gauss","gauss", KSmass, meanKS,sigma);

    RooRealVar tau("tau","tau", -0.0001, -0.01, 0.0);
    RooExponential exp("exp","exp", KSmass, tau);

    RooRealVar coeff("coeff","coeff", 0.0, -1.0, 1.0);
    
    RooChebychev bg("bg","bg", KSmass, RooArgList(coeff));


    RooRealVar signalYield("signalYield","signalYield", 100.0, 0.0, 1e6);
    RooRealVar bgYield("bgYield","bgYield", 100.0, 0.0, 1e6);
    RooAddPdf massPdf(
            "massPdf","pdf for mass",
            RooArgList(gauss, bg),
            RooArgList(signalYield, bgYield)
    );
    
    RooRealVar piminusPT("piminus_PT", "pi- pt", 50.0, 10.0, 50000.0);
    RooRealVar piplusPT("piplus_PT", "pi+ pt", 50.0, 10.0, 50000.0);
    RooRealVar piminusP("piminus_P", "pi- p", 50.0, 10.0, 500000.0);
    RooRealVar piplusP("piplus_P", "pi+ p", 50.0, 10.0, 500000.0);
    RooRealVar piminusEta("piminus_eta", "pi- eta", 2.0, 1.7, 5.5);
    RooRealVar piplusEta("piplus_eta", "p+ eta", 2.0, 1.7, 5.5);
    RooRealVar piminusPhi("piminus_phi", "pi- phi", 0.0, -3.15, 3.15);
    RooRealVar piplusPhi("piplus_phi", "p+ phi", 0.0, -3.15, 3.15);

    RooRealVar piminusGP("piminus_TRACK_GhostProb", "ghost prob pi-", 0.1, 0.0, 1.0);
    RooRealVar piplusGP("piplus_TRACK_GhostProb", "ghost prob pi+", 0.1, 0.0, 1.0);

    RooRealVar VtxChi2("KS0_ENDVERTEX_CHI2","Vtx chi2", 1.0, 0.0, 100.0);


    RooDataSet baseline_ds(
            "baseline_ds","baseline_ds",
            RooArgSet(
                    KSmass, piminusPT, piplusPT,
                    piminusP, piplusP, piminusEta,
                    piplusEta, piminusPhi, piplusPhi
//                    piminusGP, piplusGP, VtxChi2
            ),
            RooFit::Import(*tree_baseline_copy),
            RooFit::Cut(
                    "piplus_PT > 50 && piminus_PT > 50 && piminus_P > 2000 && piplus_P > 2000"
            )
    );
    
    RooPlot* mass_plot = KSmass.frame();
    
    baseline_ds.plotOn(
            mass_plot,
//            RooFit::MarkerStyle(kDot),
            RooFit::MarkerSize(0.5),
            RooFit::MarkerColor(kBlue)
    );
    massPdf.fitTo(baseline_ds);
    massPdf.plotOn(
            mass_plot,
            RooFit::LineColor(kBlue),
            RooFit::Name(baseline_file.c_str())
    );
    massPdf.plotOn(
            mass_plot,
            RooFit::Components(bg),
            RooFit::LineStyle(kDashed),
            RooFit::LineColor(kBlue)
    );
//    massPdf.plotOn(
//            mass_plot,
//            RooFit::Components(gauss),
//            RooFit::LineStyle(kDashed),
//            RooFit::LineColor(kYellow)
//    );
//
    
    
    
    // Define signal range in which events counts are to be defined
    KSmass.setRange(
            "signalRange",
            meanKS.getVal()-2*sigma.getVal(),
            meanKS.getVal()+2*sigma.getVal()
            );
    
    RooAbsReal* gauss_integral_baseline = gauss.createIntegral(KSmass,RooFit::Range("signalRange"));
    RooAbsReal* bg_integral_baseline = bg.createIntegral(KSmass,RooFit::Range("signalRange"));
    
    
    TTree* tree_data_copy = tree_data->CopyTree(
            "piminus_TRACK_Type == 5 && piplus_TRACK_Type == 5"
    );
    
    
    // -- Parameters for CB
    RooRealVar KSmass2("KS0_M", "Invariant mass [MeV/c^2]", 450.0, 550.0);
    RooRealVar meanKS2("meanKS", "meanKS", 475.0, 450.0, 530.0);
    RooRealVar sigma2("sigma","sigma",10.0, 2.0, 20.0);
    RooGaussian gauss2("gauss","gauss", KSmass2, meanKS2,sigma2);
    
    RooRealVar tau2("tau","tau", -0.0001, -0.01, 0.0);
    RooExponential exp2("exp","exp", KSmass2, tau2);
    
    RooRealVar coeff2("coeff","coeff", 0.0, -1.0, 1.0);
    
    RooChebychev bg2("bg","bg", KSmass2, RooArgList(coeff2));
    
    
    RooRealVar signalYield2("signalYield","signalYield", 100.0, 0.0, 1e6);
    RooRealVar bgYield2("bgYield","bgYield", 100.0, 0.0, 1e6);
    RooAddPdf massPdf2(
            "massPdf","pdf for mass",
            RooArgList(gauss2, bg2),
            RooArgList(signalYield2, bgYield2)
    );
    
    RooRealVar piminusPT2("piminus_PT", "pi- pt", 50.0, 10.0, 50000.0);
    RooRealVar piplusPT2("piplus_PT", "pi+ pt", 50.0, 10.0, 50000.0);
    RooRealVar piminusP2("piminus_P", "pi- p", 50.0, 10.0, 500000.0);
    RooRealVar piplusP2("piplus_P", "pi+ p", 50.0, 10.0, 500000.0);
    RooRealVar piminusEta2("piminus_eta", "pi- eta", 2.0, 1.7, 5.5);
    RooRealVar piplusEta2("piplus_eta", "p+ eta", 2.0, 1.7, 5.5);
    RooRealVar piminusPhi2("piminus_phi", "pi- phi", 0.0, -3.15, 3.15);
    RooRealVar piplusPhi2("piplus_phi", "p+ phi", 0.0, -3.15, 3.15);
    
    RooRealVar piminusGP2("piminus_TRACK_GhostProb", "ghost prob pi-", 0.1, 0.0, 1.0);
    RooRealVar piplusGP2("piplus_TRACK_GhostProb", "ghost prob pi+", 0.1, 0.0, 1.0);
    
    RooRealVar VtxChi22("KS0_ENDVERTEX_CHI2","Vtx chi2", 1.0, 0.0, 100.0);
    
    
    RooDataSet data_ds(
            "data_ds","data_ds",
            RooArgSet(
                    KSmass2, piminusPT2, piplusPT2,
                    piminusP2, piplusP2, piminusEta2,
                    piplusEta2, piminusPhi2, piplusPhi2
//                    piminusGP2, piplusGP2, VtxChi22
            ),
            RooFit::Import(*tree_data_copy),
            RooFit::Cut(
                    "piplus_PT > 50 && piminus_PT > 50 && piminus_P > 2000 && piplus_P > 2000"
            )
    );
    
    data_ds.plotOn(
            mass_plot,
//            RooFit::MarkerStyle(kDot),
            RooFit::MarkerSize(0.5),
            RooFit::MarkerColor(kRed)
    );
    massPdf2.fitTo(data_ds);
    massPdf2.plotOn(
            mass_plot,
            RooFit::LineColor(kRed),
            RooFit::Name(data_file.c_str())
    );
    massPdf2.plotOn(
            mass_plot,
            RooFit::Components(bg2),
            RooFit::LineStyle(kDashed),
            RooFit::LineColor(kRed)
    );
    
    KSmass2.setRange(
            "signalRange2",
            meanKS2.getVal()-2*sigma2.getVal(),
            meanKS2.getVal()+2*sigma2.getVal()
    );
    
    RooAbsReal* gauss_integral_data = gauss2.createIntegral(KSmass2,RooFit::Range("signalRange2"));
    RooAbsReal* bg_integral_data = bg2.createIntegral(KSmass2,RooFit::Range("signalRange2"));
    
    
    
    file_baseline->Close();
    file_data->Close();
    
    mass_plot->SetTitle("K_{s}^{0} mass fit");
    mass_plot->Draw();
    std::cout << "############################" << std::endl;
    std::cout << baseline_file << std::endl;
    meanKS.Print();
    sigma.Print();
    signalYield.Print();
    bgYield.Print();
    cout << "Ks signal count [-2s, 2s] " << baseline_file.c_str() << " " << gauss_integral_baseline->getVal() << endl;
    cout << "Ks bg count [-2s, 2s] " << baseline_file.c_str() << " " << bg_integral_baseline->getVal() << endl;
    
    std::cout << data_file << std::endl;
    meanKS2.Print();
    sigma2.Print();
    signalYield2.Print();
    bgYield2.Print();
    cout << "Ks signal count [-2s, 2s] " << data_file.c_str() << gauss_integral_data->getVal() << endl;
    cout << "Ks bg count [-2s, 2s] " << data_file.c_str() << bg_integral_data->getVal() << endl;
    
    
    delete dummy;
    
    
}


void fitMassLambda(std::string data_file, std::string baseline_file) {
    
    
    TFile* file_data = TFile::Open(data_file.c_str());
    TFile* file_baseline = TFile::Open(baseline_file.c_str());
    
    
    TFile* dummy = new TFile("dummy.root","OVERWRITE");
    
    
    TTree* tree_baseline = (TTree*)file_baseline->Get("LambdaPPiTuple/DecayTree");
    TTree* tree_data = (TTree*)file_data->Get("LambdaPPiTuple/DecayTree");
    
    
    TTree* tree_baseline_copy = tree_baseline->CopyTree(
            "piminus_TRACK_Type == 5 && pplus_TRACK_Type == 5"
    );
    
    
    // -- Parameters for CB
    
    RooRealVar Lambdamass("Lambda0_M", "#Lambda^{0} Invariant mass [#frac{MeV}{c^{2}}]", 1090.0, 1140.0);
    RooRealVar meanLambda("meanLambda", "meanLambda", 1115.0, 1110.0, 1120.0);
    RooRealVar sigma("sigma","sigma",10.0, 0.5, 20.0);
    RooGaussian gauss("gauss","gauss", Lambdamass, meanLambda,sigma);
    
    RooRealVar tau("tau","tau", -0.0001, -0.01, 0.0);
    RooExponential exp("exp","exp", Lambdamass, tau);
    
    RooRealVar coeff("coeff","coeff", 0.0, -1.0, 1.0);
    
    RooChebychev bg("bg","bg", Lambdamass, RooArgList(coeff));
    
    
    RooRealVar signalYield("signalYield","signalYield", 100.0, 10.0, 1e6);
    RooRealVar bgYield("bgYield","bgYield", 100.0, 10.0, 1e6);
    RooAddPdf massPdf(
            "massPdf","pdf for mass",
            RooArgList(gauss, bg),
            RooArgList(signalYield, bgYield)
    );
    
    RooRealVar piminusPT("piminus_PT", "pi- pt", 50.0, 10.0, 50000.0);
    RooRealVar pplusPT("pplus_PT", "pi+ pt", 50.0, 0.0, 50000.0);
    RooRealVar piminusP("piminus_P", "pi- p", 50.0, 0.0, 500000.0);
    RooRealVar pplusP("pplus_P", "pi+ p", 50.0, 0.0, 500000.0);
    RooRealVar piminusEta("piminus_eta", "pi- eta", 2.0, 1.7, 5.5);
    RooRealVar pplusEta("pplus_eta", "p+ eta", 2.0, 1.7, 5.5);
    RooRealVar pminusPhi("pminus_phi", "pi- phi", 0.0, -3.15, 3.15);
    RooRealVar pplusPhi("pplus_phi", "p+ phi", 0.0, -3.15, 3.15);
    
    RooRealVar piminusGP("piminus_TRACK_GhostProb", "ghost prob", 0.1, 0.0, 1.0);
    RooRealVar piplusGP("pplus_TRACK_GhostProb", "ghost prob", 0.1, 0.0, 1.0);
    
    RooRealVar fd("Lambda0_FD_OWNPV","lambda fd", 0.0, 0.0, 3000.0);
    
    
    
    RooDataSet baseline_ds(
            "baseline_ds","baseline_ds",
            RooArgSet(
                    Lambdamass, piminusPT, pplusPT,
                    piminusP, pplusP, piminusEta,
                    pplusEta, fd, pplusPhi
            ),
            RooFit::Import(*tree_baseline_copy),
            RooFit::Cut("pplus_PT > 50 && piminus_PT > 50 && piminus_P > 2000 && pplus_P > 2000")
    );
    
    RooPlot* mass_plot = Lambdamass.frame();
    
    baseline_ds.plotOn(
            mass_plot,
//            RooFit::MarkerStyle(kDot),
            RooFit::MarkerSize(0.5),
            RooFit::MarkerColor(kBlue)
    );
    massPdf.fitTo(baseline_ds);
    massPdf.plotOn(
            mass_plot,
            RooFit::LineColor(kBlue),
            RooFit::Name(baseline_file.c_str())
    );
    massPdf.plotOn(
            mass_plot,
            RooFit::Components(bg),
            RooFit::LineStyle(kDashed),
            RooFit::LineColor(kBlue)
    );
    
    TTree* tree_data_copy = tree_data->CopyTree(
            "piminus_TRACK_Type == 5 && pplus_TRACK_Type == 5"
    );
    
    // Define signal range in which events counts are to be defined
    Lambdamass.setRange(
            "signalRange",
            meanLambda.getVal()-2*sigma.getVal(),
            meanLambda.getVal()+2*sigma.getVal()
    );
    
    RooAbsReal* gauss_integral_baseline = gauss.createIntegral(Lambdamass,RooFit::Range("signalRange"));
    RooAbsReal* bg_integral_baseline = bg.createIntegral(Lambdamass,RooFit::Range("signalRange"));
    
    
    // -- Parameters for CB
    
    RooRealVar Lambdamass2("Lambda0_M", "mass", 1090.0, 1140.0);
    RooRealVar meanLambda2("meanLambda", "meanLambda", 1115.0, 1110.0, 1120.0);
    RooRealVar sigma2("sigma","sigma",10.0, 0.5, 20.0);
    RooGaussian gauss2("gauss","gauss", Lambdamass2, meanLambda2,sigma2);
    
    RooRealVar tau2("tau","tau", -0.0001, -0.01, 0.0);
    RooExponential exp2("exp","exp", Lambdamass2, tau2);
    
    RooRealVar coeff2("coeff","coeff", 0.0, -1.0, 1.0);
    
    RooChebychev bg2("bg","bg", Lambdamass2, RooArgList(coeff2));
    
    
    RooRealVar signalYield2("signalYield","signalYield", 100.0, 10.0, 1e6);
    RooRealVar bgYield2("bgYield","bgYield", 100.0, 10.0, 1e6);
    RooAddPdf massPdf2(
            "massPdf","pdf for mass",
            RooArgList(gauss2, bg2),
            RooArgList(signalYield2, bgYield2)
    );
    
    RooRealVar piminusPT2("piminus_PT", "pi- pt", 50.0, 10.0, 50000.0);
    RooRealVar pplusPT2("pplus_PT", "pi+ pt", 50.0, 0.0, 50000.0);
    RooRealVar piminusP2("piminus_P", "pi- p", 50.0, 10.0, 500000.0);
    RooRealVar pplusP2("pplus_P", "pi+ p", 50.0, 10.0, 500000.0);
    RooRealVar piminusEta2("piminus_eta", "pi- eta", 2.0, 1.7, 5.5);
    RooRealVar pplusEta2("pplus_eta", "p+ eta", 2.0, 1.7, 5.5);
    RooRealVar pminusPhi2("pminus_phi", "pi- phi", 0.0, -3.15, 3.15);
    RooRealVar pplusPhi2("pplus_phi", "p+ phi", 0.0, -3.15, 3.15);
    
    RooRealVar piminusGP2("piminus_TRACK_GhostProb", "ghost prob", 0.1, 0.0, 1.0);
    RooRealVar piplusGP2("pplus_TRACK_GhostProb", "ghost prob", 0.1, 0.0, 1.0);
    
    RooRealVar fd2("Lambda0_FD_OWNPV","lambda fd", 0.0, 0.0, 3000.0);
    
    
    RooDataSet data_ds(
            "data_ds","data_ds",
            RooArgSet(
                    Lambdamass2, piminusPT2, pplusPT2,
                    piminusP2, pplusP2, piminusEta2,
                    pplusEta2, fd2, pplusPhi2
            ),
            RooFit::Import(*tree_data_copy),
            RooFit::Cut(
                    "pplus_PT > 50 && piminus_PT > 50 && piminus_P > 2000 && pplus_P > 2000"
            )
    );
    
    data_ds.plotOn(
            mass_plot,
//            RooFit::MarkerStyle(kDot),
            RooFit::MarkerSize(0.5),
            RooFit::MarkerColor(kRed)
    );
    massPdf2.fitTo(data_ds);
    massPdf2.plotOn(
            mass_plot,
            RooFit::LineColor(kRed),
            RooFit::Name(data_file.c_str())
    );
    massPdf2.plotOn(
            mass_plot,
            RooFit::Components(bg2),
            RooFit::LineStyle(kDashed),
            RooFit::LineColor(kRed)
    );
    
    Lambdamass2.setRange(
            "signalRange2",
            meanLambda2.getVal()-2*sigma2.getVal(),
            meanLambda2.getVal()+2*sigma2.getVal()
    );
    
    RooAbsReal* gauss_integral_data = gauss2.createIntegral(Lambdamass2,RooFit::Range("signalRange2"));
    RooAbsReal* bg_integral_data = bg2.createIntegral(Lambdamass2,RooFit::Range("signalRange2"));
    
    
    file_baseline->Close();
    
    file_data->Close();
    
    mass_plot->SetTitle("#Lambda^{0} mass fit");
    mass_plot->Draw();
    std::cout << "############################" << std::endl;
    std::cout << baseline_file << std::endl;
    meanLambda.Print();
    sigma.Print();
    signalYield.Print();
    bgYield.Print();
    gauss_integral_baseline->Print();
    bg_integral_baseline->Print();
    
    std::cout << data_file << std::endl;
    meanLambda2.Print();
    sigma2.Print();
    std::cout << sigma2.getVal() << std::endl;
    signalYield2.Print();
    bgYield2.Print();
    gauss_integral_data->Print();
    bg_integral_data->Print();
    
    
    
    delete dummy;
    
    
}

void plot_ks_fit_mass(
        const string& file_1, const string& name_1,
        const string& file_2, const string& name_2,
        const string& output_name
) {
    auto c1 = new TCanvas();
    auto leg1 = new TLegend;
    
    leg1->SetFillColor(kWhite);
    leg1->SetLineColor(kWhite);
    
    fitMassKs(file_1, file_2);
    
    leg1->AddEntry(file_1.c_str(), name_1.c_str());
    leg1->AddEntry(file_2.c_str(), name_2.c_str());
    leg1->Draw();
    
    c1->SaveAs(output_name.c_str());
//    delete c1;
//    delete leg1;
}

void plot_lambda_fit_mass(
        const string& file_1, const string& name_1,
        const string& file_2, const string& name_2,
        const string& output_name
) {
    auto c1 = new TCanvas();
    auto leg1 = new TLegend;
    leg1 = new TLegend;
    leg1->SetFillColor(kWhite);
    leg1->SetLineColor(kWhite);
    
    fitMassLambda(file_1, file_2);
    
    leg1->AddEntry(file_1.c_str(), name_1.c_str());
    leg1->AddEntry(file_2.c_str(), name_2.c_str());
    leg1->Draw();
    
    c1->SaveAs(output_name.c_str());
    
//    delete c1;
//    delete leg1;
}
