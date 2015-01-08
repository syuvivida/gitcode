#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TLine.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TChain.h>
#include <THStack.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TVectorT.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>

const Int_t totalNEvent_dy70 = 11764538;
const Int_t totalNEvent_dy100 = 12511326;
const Int_t totalNEvent_ttbar = 10783509;
const Int_t totalNEvent_ww = 9850431;
const Int_t totalNEvent_wz = 9840283;
const Int_t totalNEvent_zz = 9594908;

const Double_t crossSection_dy70 = 63.5;
const Double_t crossSection_dy100 = 39.4;
const Double_t crossSection_ttbar = 25.8;
const Double_t crossSection_ww = 56;
const Double_t crossSection_wz = 22.4;
const Double_t crossSection_zz = 7.6;
const Double_t dataLumi_totalD = 19712.225;//ele
//const Double_t dataLumi_totalD = 19671.225;//mu
// scale = data_luminosity / bkg_luminosity
Double_t scale_dy70 = dataLumi_totalD / (totalNEvent_dy70 / crossSection_dy70);
Double_t scale_dy100 = dataLumi_totalD / (totalNEvent_dy100 / crossSection_dy100);
Double_t scale_ttbar = dataLumi_totalD / (totalNEvent_ttbar / crossSection_ttbar);
Double_t scale_ww = dataLumi_totalD / (totalNEvent_ww / crossSection_ww);
Double_t scale_wz = dataLumi_totalD / (totalNEvent_wz / crossSection_wz);
Double_t scale_zz = dataLumi_totalD / (totalNEvent_zz / crossSection_zz);

void myPlot(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_ttbar, TH1D* h_ww, TH1D* h_wz, TH1D* h_zz, TH1D* h_data){

  h_data->Sumw2();

  h_dy70->Scale(scale_dy70);
  h_dy70->SetFillColor(kCyan-4);
  h_dy70->SetLineColor(kBlack);

  h_dy100->Scale(scale_dy100);
  h_dy100->SetFillColor(kAzure+9);
  h_dy100->SetLineColor(kBlack);

  h_ttbar->Scale(scale_ttbar);
  h_ttbar->SetFillColor(kGreen);
  h_ttbar->SetLineColor(kBlack);
  
  h_ww->Scale(scale_ww);
  h_ww->SetFillColor(kRed);
  h_ww->SetLineColor(kBlack);

  h_wz->Scale(scale_wz);
  h_wz->SetFillColor(kOrange+4);
  h_wz->SetLineColor(kBlack);

  h_zz->Scale(scale_zz);
  h_zz->SetFillColor(kYellow);
  h_zz->SetLineColor(kBlack);
  
  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_dy70);
  h_stack->Add(h_dy100);
  h_stack->Add(h_ttbar);
  h_stack->Add(h_ww);
  h_stack->Add(h_wz);
  h_stack->Add(h_zz);

  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->GetXaxis()->SetTitle("");
  h_data->GetXaxis()->SetLabelOffset(999);
  h_data->GetXaxis()->SetLabelSize(0);
  h_data->Draw("e1"); 
  h_stack->Draw("same");
  h_data->Draw("e1same");

  TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg->AddEntry(h_dy100,"DYJetsToLL_PtZ-100", "f");
  leg->AddEntry(h_ttbar,"TTTo2L2Nu2B", "f");
  leg->AddEntry(h_ww,"WW_pythia", "f");
  leg->AddEntry(h_wz,"WZ_pythia", "f");
  leg->AddEntry(h_zz,"ZZ_pythia", "f");
  leg->AddEntry(h_data, "Data", "lp");
  leg->Draw();

}
