#include <iostream>
#include <cstdlib>
#include <string>
#include "setTDRStyle.C"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TColor.h"
#include "TAxis.h"
#include "TROOT.h"
#include "TPDF.h"
#include "TH2.h"

using namespace std;

void plotresults_3(string inputFile){

  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  setTDRStyle();

  std::string processName[]={"#splitline{W^{+} W^{-}}{#splitline{M_{34}>0}{M_{56}>0}}", 
			     "#splitline{W^{+} Z^{0}}{#splitline{M_{34}>0}{M_{56}>12}}",
			     "#splitline{W^{+} Z^{0}}{#splitline{M_{34}>0}{M_{56}>40}}",
			     "#splitline{W^{-} Z^{0}}{#splitline{M_{34}>0}{M_{56}>12}}",
			     "#splitline{W^{-} Z^{0}}{#splitline{M_{34}>0}{M_{56}>40}}",
			     "#splitline{Z^{0}/#gamma^{*} Z^{0}/#gamma^{*}}{#splitline{M_{34}>12}{M_{56}>12}}",
			     "#splitline{Z^{0}/#gamma^{*} Z^{0}/#gamma^{*}}{#splitline{M_{34}>40}{M_{56}>40}}",
			     "#splitline{Z^{0}/#gamma^{*} Z^{0}}{#splitline{M_{34}>12}{M_{56}>0}}",
			     "#splitline{Z^{0} Z^{0}}{#splitline{M_{34}>1}{M_{56}>1}}",
			     "#splitline{t #bar{t}}{#mu_{1}}",
			     "#splitline{t #bar{t}}{#mu_{2}}"};

  const int font = 43;
  
  const int temp = sizeof(processName)/sizeof(processName[0]);
  const int nProcess = temp;

  Double_t cProcess[nProcess] = {0};
  Double_t eProcess[nProcess] = {0};

  for(int i=0; i<nProcess; i++){
    cProcess[i] = i+1.5; 
    eProcess[i] = 0.5;

  }

  Double_t AveCen[nProcess] = {0};
  Double_t ScaleErr[nProcess] = {0};
  Double_t PDFErrUp[nProcess] = {0};
  Double_t PDFErrDown[nProcess] = {0};
  Double_t TotalErrUp[nProcess] = {0};
  Double_t TotalErrDown[nProcess] = {0};

  ifstream fin;
  fin.open(inputFile.data());

  for(int i=0; i<11; i++)
    fin >> AveCen[i] >> PDFErrUp[i] >> PDFErrDown[i] >> ScaleErr[i];

  fin.close();

  for(int ip=0; ip<nProcess; ip++){
    TotalErrUp[ip]   = sqrt(pow(PDFErrUp[ip],2)   + pow(ScaleErr[ip],2));
    TotalErrDown[ip] = sqrt(pow(PDFErrDown[ip],2) + pow(ScaleErr[ip],2));

  }

  TGraphAsymmErrors *gr_PDF = new TGraphAsymmErrors(nProcess, cProcess, 
						    AveCen, eProcess, eProcess,
						    PDFErrDown, PDFErrUp);  

  TGraphAsymmErrors *gr_Total = new TGraphAsymmErrors(nProcess, cProcess, 
						      AveCen, eProcess, eProcess, 
						      TotalErrDown, TotalErrUp);
    
  TCanvas *gph = new TCanvas("gph", "gph", 1020, 670);
  gph->cd();
  gPad->SetLogy(1);
  gPad->SetGridx();
  gPad->SetTopMargin(0.05);                                                   
  gPad->SetBottomMargin(0.125);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.05);   

  int PDFCOLOR = kMagenta -6;
  gr_PDF->SetTitle("");
  gr_PDF->SetMarkerStyle(25);
  gr_PDF->SetMarkerSize(0);
  gr_PDF->SetMarkerColor(PDFCOLOR);
  gr_PDF->SetLineWidth(3);
  gr_PDF->SetLineColor(PDFCOLOR);
  gr_PDF->SetFillColor(PDFCOLOR);
  gr_PDF->SetFillStyle(1001);
  // gr_PDF->GetXaxis()->SetTitle("");
  // gr_PDF->GetXaxis()->SetLabelFont(43);
  // gr_PDF->GetXaxis()->SetLabelSize(14);
  // gr_PDF->GetYaxis()->SetTitle("");
  // gr_PDF->GetYaxis()->SetTitleOffset(1);
  // gr_PDF->GetYaxis()->SetTitleFont(font);
  // gr_PDF->GetYaxis()->SetTitleSize(14);
  // gr_PDF->GetYaxis()->SetLabelFont(font);
  // gr_PDF->GetYaxis()->SetLabelSize(14);

  int TOTALCOLOR = kMagenta +3;
  gr_Total->SetTitle("");
  gr_Total->SetMarkerStyle(25);
  gr_Total->SetMarkerSize(0);
  gr_Total->SetMarkerColor(TOTALCOLOR);
  gr_Total->SetLineWidth(3);
  gr_Total->SetLineColor(TOTALCOLOR);
  gr_Total->SetFillColor(TOTALCOLOR);
  gr_Total->SetFillStyle(1001);
  gr_Total->GetXaxis()->SetTitle("");
  gr_Total->GetXaxis()->SetLabelFont(font);
  gr_Total->GetXaxis()->SetLabelSize(16);
  gr_Total->GetYaxis()->SetTitle("Production Cross Section #sigma * Branching Ratio [fb]");
  gr_Total->GetYaxis()->CenterTitle(kTRUE);
  gr_Total->GetYaxis()->SetTitleOffset(1);
  gr_Total->GetYaxis()->SetTitleFont(font);
  gr_Total->GetYaxis()->SetTitleSize(26);
  gr_Total->GetYaxis()->SetLabelFont(font);
  gr_Total->GetYaxis()->SetLabelSize(16);

  Double_t xlo = cProcess[0]-eProcess[0];
  Double_t xhi = cProcess[nProcess-1]+eProcess[nProcess-1];
  
  gr_Total->GetHistogram()->GetXaxis()->Set(nProcess, xlo, xhi);
  gr_PDF  ->GetHistogram()->GetXaxis()->Set(nProcess, xlo, xhi);

  for(int k=0; k<nProcess; k++)
    gr_Total->GetHistogram()->GetXaxis()->
      SetBinLabel(k+1, processName[k].data());

  gr_Total->Draw("ae2");
  gr_PDF  ->Draw("e2"); 

  TLegend *gleg = new TLegend(0.687, 0.70, 0.904, 0.81);
 
  gleg->SetFillStyle(1001);
  gleg->SetFillColor(10);
  gleg->SetTextFont(font);
  gleg->SetTextSize(15);
  gleg->SetBorderSize(1);
  gleg->AddEntry(gr_Total, "Total Uncertainty"); 
  gleg->AddEntry(gr_PDF,   "PDF Uncertainty");
  gleg->Draw();
 
  TPaveText *pavetex = new TPaveText(0.687, 0.54, 0.904, 0.69, "NDCBR");

  pavetex->SetFillStyle(1001);
  pavetex->SetFillColor(10);
  pavetex->SetTextFont(font);
  pavetex->SetTextSize(15);
  pavetex->SetBorderSize(1);
  pavetex->SetTextAlign(12);
  pavetex->AddText("#splitline{#diamond   The decay modes of W and Z}{      bosons are all leptonic.}");
  pavetex->AddText("#splitline{#diamond   All dileptons mass cut (M_{i j})}{      are in units of GeV.}");
  pavetex->Draw();

  //gph->Print("output.eps");
  gph->Print("output.pdf");
  //gph->Print("output.png");

}
