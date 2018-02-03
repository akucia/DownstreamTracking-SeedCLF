//
// Created by akucia on 17.01.18.
//

/////////////////////////////////////////////////////////////////////////
//
// 'BASIC FUNCTIONALITY' RooFit tutorial macro #110
//
// Examples on normalization of p.d.f.s,
// integration of p.d.fs, construction
// of cumulative distribution functions from p.d.f.s
// in one dimension
//
// 07/2008 - Wouter Verkerke
//
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooAbsReal.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
using namespace RooFit ;
using namespace std;

void rf110_normintegration()
{
  using namespace RooFit;
  RooRealVar x("x","x",-10,10);
  RooRealVar m("m","m",0);
  RooRealVar s("s","s",3) ;
  RooGaussian g("g","g",x,m,s) ;
  
  x.setRange("oneSigma",-s.getVal(),s.getVal()) ;
//  RooAbsReal* fracInt = g.createIntegral(x);
    RooAbsReal* fracInt = g.createIntegral(x,Range("oneSigma"));
  cout << fracInt->getVal() << endl;
  auto frame = x.frame();
  
  g.plotOn(frame);

  // Draw plot on canvas
  auto canvas = new TCanvas("rf110_normintegration","rf110_normintegration",600,600) ;
  gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.6) ;
  frame->Draw() ;
    canvas->SaveAs("roo_fit_example.pdf");
}

int main(){
    rf110_normintegration();
}