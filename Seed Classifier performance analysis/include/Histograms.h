#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <TCanvas.h>
#include <TH1F.h>
#include <string>
#include <TGaxis.h>
#include <iostream>
#include <string>
#include <Counters.h>
#include "InputData_K.h"
#include "InputData_L.h"
#include <fitMass.h>
#include <RooRealVar.h>
#include <TLegend.h>

using std::cout;
using std::endl;
using std::string;


class Histograms{
	
	public:
		inline Histograms(
                std::string name_first,
                std::string name_second,
                int xmin,
                int xmax,
                int bins=100
        );
		inline void drawPlots(std::string figure_name);
        
        inline ~Histograms(){
            delete h_first;
            delete h_second;
        }
	
	TCanvas canvas;
    std::string name_1;
    std::string name_2;
    
	TH1F* h_first;
	TH1F* h_second;
	
};


Histograms::Histograms(
        std::string name_1,
        std::string name_2,
        int xmin,
        int xmax,
        int bins
):
        name_1(name_1),
        name_2(name_2)
{
	
	h_first = new TH1F(name_1.c_str(), name_1.c_str(), bins, xmin, xmax);
	h_second = new TH1F(name_2.c_str(), name_2.c_str(), bins, xmin, xmax);
	
}


void Histograms::drawPlots(std::string figure_name){
	
    TCanvas c("c", "canvas", 800, 800);
//    TCanvas c;
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();
    // pad1 becomes the current pad
    
    
    h_first->SetTitle(figure_name.c_str());
    
    h_first->SetLineColor(kRed);
    h_first->SetLineWidth(2);
    h_first->SetStats(1);
    h_first->Sumw2();
    h_first->Scale(1.0/h_first->Integral());
    h_first->Draw("hist");
    
    // h_second settings
    h_second->SetLineColor(kBlue);
    h_second->SetLineWidth(2);
    
    h_second->SetStats(0);
    h_second->Sumw2();
    h_second->Scale(1.0/h_second->Integral());
    
    
    h_second->Draw("hist same"); // Draw h_second on top of h_first
    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h_second->GetYaxis()->SetLabelSize(0.);
    
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20, 220, 510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    axis->Draw();
    
    // lower plot will be in pad
    c.cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *ratio_h = (TH1F*)h_first->Clone("ratio");
    ratio_h->SetLineColor(kBlack);
    ratio_h->SetMinimum(0.5);  // Define Y ..
    ratio_h->SetMaximum(1.5); // .. range
    ratio_h->Sumw2();
    ratio_h->SetStats(0);      // No statistics on lower plot
    ratio_h->Divide(h_second);
    ratio_h->SetMarkerStyle(21);
    ratio_h->Draw("hist");       // Draw the ratio plot
    
    // h_first settings
   
    
    
    
    // Ratio plot (ratio_h) settings
    ratio_h->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    std::string ratio_name = "ratio " + name_1 + " / " + name_2;
    ratio_h->GetYaxis()->SetTitle(ratio_name.c_str());
    ratio_h->GetYaxis()->SetNdivisions(505);
    ratio_h->GetYaxis()->SetTitleSize(17);
    ratio_h->GetYaxis()->SetTitleFont(43);
    ratio_h->GetYaxis()->SetTitleOffset(2.2);
    ratio_h->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_h->GetYaxis()->SetLabelSize(15);
    
    // X axis ratio plot settings
    ratio_h->GetXaxis()->SetTitleSize(20);
    ratio_h->GetXaxis()->SetTitleFont(43);
    ratio_h->GetXaxis()->SetTitleOffset(4.);
    ratio_h->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio_h->GetXaxis()->SetLabelSize(15);
    
    c.SaveAs((figure_name + ".pdf").c_str());
    
    delete pad2;
    delete axis;
    delete pad1;
	
}

void compare_normalized_histograms_KS0(
        InputData_K& data_1,
        const string& name_1,
        InputData_K& data_2,
        const string& name_2,
        const string& output_path
) {
    Histograms histo_m(name_1 + "_M", name_2 + "M", 400, 600);
//    Histograms histo_MM(name_1 + "_MM", name_2 + "MM", 400, 600);
    
    Counters counters_1;
    Counters counters_2;
    
    while(data_1.nextEvent()) {
        histo_m.h_first->Fill(data_1.KS0_M);
//        histo_MM.h_first->Fill(data_1.KS0_MM);
        ++counters_1.totalNumOfEvents;
    }
    
    while(data_2.nextEvent()) {
        histo_m.h_second->Fill(data_2.KS0_M);
//        histo_MM.h_second->Fill(data_2.KS0_MM);
        ++counters_2.totalNumOfEvents;
    }
    
    cout << "First:" << endl;
    counters_1.print();
    
    cout << "Second:" << endl;
    counters_2.print();
    
    cout << endl;
    
    histo_m.drawPlots(output_path + "_M_");
//    histo_MM.drawPlots(output_path + "_MM_");
}

void compare_normalized_histograms_L(
        InputData_L& data_1,
        const string& name_1,
        InputData_L& data_2,
        const string& name_2,
        const string& output_path
) {
    Histograms histo_m(name_1 + "_M", name_2 + "M", 1070, 1220);
//    Histograms histo_MM(name_1 + "_MM", name_2 + "MM", 1070, 1220);
    
    Counters counters_1;
    Counters counters_2;
    
    while(data_1.nextEvent()) {
        histo_m.h_first->Fill(data_1.Lambda0_M);
//        histo_MM.h_first->Fill(data_1.Lambda0_MM);
        ++counters_1.totalNumOfEvents;
    }
    
    while(data_2.nextEvent()) {
        histo_m.h_second->Fill(data_2.Lambda0_M);
//        histo_MM.h_second->Fill(data_2.Lambda0_MM);
        ++counters_2.totalNumOfEvents;
    }
    
    cout << "First:" << endl;
    counters_1.print();
    
    cout << "Second:" << endl;
    counters_2.print();
    
    cout << endl;
    
    histo_m.drawPlots(output_path + "_M_");
//    histo_MM.drawPlots(output_path + "_MM_");
}

#endif