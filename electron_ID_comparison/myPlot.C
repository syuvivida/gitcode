#include <iostream>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>

const Int_t totalNEvent_DY = 28436690;
const Int_t totalNEvent_ttbar = 19494441;
const Double_t crossSection_DY = 6025.2; //pb
const Double_t crossSection_ttbar = 831.76; //pb
const Double_t dataLumi = 12.; //ele
// scale = data_luminosity / bkg_luminosity
Double_t scale_DY = dataLumi / (totalNEvent_DY / crossSection_DY);
Double_t scale_ttbar = dataLumi / (totalNEvent_ttbar / crossSection_ttbar);

void myPlot(TH1D* h_DY, TH1D* h_ttbar, TH1D* h_data){

  h_data->Sumw2();

  h_DY->Scale(scale_DY);
  h_DY->SetFillColor(kCyan-4);
  h_DY->SetLineColor(kBlack);

  h_ttbar->Scale(scale_ttbar);
  h_ttbar->SetFillColor(kGreen);
  h_ttbar->SetLineColor(kBlack);
  
  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_DY);
  h_stack->Add(h_ttbar);

  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->GetXaxis()->SetTitle("");
  h_data->GetXaxis()->SetLabelOffset(999);
  h_data->GetXaxis()->SetLabelSize(0);
  //h_data->Draw("e1"); 
  h_stack->Draw();
  h_stack->GetHistogram()->GetXaxis()->SetTickLength(0);
  h_stack->GetHistogram()->GetXaxis()->SetLabelOffset(999);
  h_data->Draw("e1same");

  TLegend *leg = new TLegend(0.55, 0.8, 0.9, 0.9);

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_DY, "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns", "f"); 
  leg->AddEntry(h_ttbar,"TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803", "f");
  leg->AddEntry(h_data, "SingleElectron_Run2015C-PromptReco-v1", "lp");
  leg->Draw();

}
