#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
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

const Int_t totalNEvent1 = 11765438;
const Int_t totalNEvent2 = 12511326;
const Double_t crossSection1 = 63.5*1000;
const Double_t crossSection2 = 39.4*1000;

void stcEleVariable(){

  TFile *f = TFile::Open("muVariable.root");

  gStyle->SetOptStat(1111);

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 600);
  c->Divide(2,1);

  // scale = ? fb^-1 / luminosity
  Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
  Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TH1D* h_nVtx_dy70  = (TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_nVtx_dy100 = (TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-100.root"));
  TH1D* h_nVtx_data  = (TH1D*)(f->Get("nVtx_data_DoubleMu_A.root"));
  TH1D* h_nVtx_sign  = (TH1D*)(f->Get("nVtx_ZPrime_hZ_qqll_LHC8_M1000.root"));

  h_nVtx_data->Sumw2();
  h_nVtx_data->SetTitle("Data nVtx");

  h_nVtx_sign->SetTitle("Signal MC nVtx");

  h_nVtx_dy70->Scale(scale1);
  h_nVtx_dy70->SetFillColor(kCyan-4);
  h_nVtx_dy70->SetLineColor(kBlack);

  h_nVtx_dy100->Scale(scale2);
  h_nVtx_dy100->SetFillColor(kAzure+9);
  h_nVtx_dy100->SetLineColor(kBlack);

  THStack *h_nVtx_stack = new THStack("h_nVtx_stack", "Mu nVtx distribution");
  h_nVtx_stack->Add(h_nVtx_dy70);
  h_nVtx_stack->Add(h_nVtx_dy100);

  c->cd(1);
  h_nVtx_stack->Draw();
  h_nVtx_stack->GetXaxis()->SetTitle("nVtx");
  h_nVtx_stack->GetYaxis()->SetTitle("Event number");

  h_nVtx_sign->SetFillColor(kOrange+1);
  h_nVtx_sign->SetFillStyle(3353);
  h_nVtx_sign->SetLineColor(kBlack);
  h_nVtx_sign->Draw("same");

  h_nVtx_data->SetLineColor(1);
  h_nVtx_data->SetMarkerStyle(8);
  h_nVtx_data->SetMarkerSize(0.5);
  h_nVtx_data->Draw("e1same");
  
  TLegend *leg1 = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_nVtx_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg1->AddEntry(h_nVtx_dy100, "DYJetsToLL_PtZ-100", "f"); 
  leg1->AddEntry(h_nVtx_sign, "ZPrime_hZ_qqll_LHC8_M1000", "f"); 
  leg1->AddEntry(h_nVtx_data, "Data", "lp");
  leg1->Draw();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TH1D* h_corrTrkIso_dy70  = (TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_corrTrkIso_dy100 = (TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-100.root"));
  TH1D* h_corrTrkIso_data  = (TH1D*)(f->Get("corrTrkIso_data_DoubleMu_A.root"));
  TH1D* h_corrTrkIso_sign  = (TH1D*)(f->Get("corrTrkIso_ZPrime_hZ_qqll_LHC8_M1000.root"));

  h_corrTrkIso_data->Sumw2();
  h_corrTrkIso_data->SetTitle("Data corrTrkIso");

  h_corrTrkIso_sign->SetTitle("Signal MC corrTrkIso");

  h_corrTrkIso_dy70->Scale(scale1);
  h_corrTrkIso_dy70->SetFillColor(kCyan-4);
  h_corrTrkIso_dy70->SetLineColor(kBlack);

  h_corrTrkIso_dy100->Scale(scale2);
  h_corrTrkIso_dy100->SetFillColor(kAzure+9);
  h_corrTrkIso_dy100->SetLineColor(kBlack);

  THStack *h_corrTrkIso_stack = new THStack("h_corrTrkIso_stack", "Mu corrTrkIso distribution");
  h_corrTrkIso_stack->Add(h_corrTrkIso_dy70);
  h_corrTrkIso_stack->Add(h_corrTrkIso_dy100);

  c->cd(2)->SetLogy();
  h_corrTrkIso_stack->Draw();
  h_corrTrkIso_stack->GetXaxis()->SetTitle("corrTrkIso");
  h_corrTrkIso_stack->GetYaxis()->SetTitle("Event number");

  h_corrTrkIso_sign->SetFillColor(kOrange+1);
  h_corrTrkIso_sign->SetFillStyle(3353);
  h_corrTrkIso_sign->SetLineColor(kBlack);
  h_corrTrkIso_sign->Draw("same");

  h_corrTrkIso_data->SetLineColor(1);
  h_corrTrkIso_data->SetMarkerStyle(8);
  h_corrTrkIso_data->SetMarkerSize(0.5);
  h_corrTrkIso_data->Draw("e1same");
  
  TLegend *leg2 = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg2->SetFillStyle(1001);
  leg2->SetFillColor(10);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_corrTrkIso_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg2->AddEntry(h_corrTrkIso_dy100, "DYJetsToLL_PtZ-100", "f"); 
  leg2->AddEntry(h_corrTrkIso_sign, "ZPrime_hZ_qqll_LHC8_M1000", "f"); 
  leg2->AddEntry(h_corrTrkIso_data, "Data", "lp");
  leg2->Draw();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  c->Print("muVariable.gif");

}
