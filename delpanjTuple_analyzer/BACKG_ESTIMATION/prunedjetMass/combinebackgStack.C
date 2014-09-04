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

void combineBkgStc(){

  TFile *f = TFile::Open("backgStack.root");

  gStyle->SetOptStat(1111);

  TH1D* h_dy70  = (TH1D*)(f->Get("prunedJetMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100 = (TH1D*)(f->Get("prunedJetMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_data  = (TH1D*)(f->Get("prunedJetMass_data_DoubleMu_A.root"));

  h_data->Sumw2();
  h_data->SetTitle("Data Pruned Jet Mass");

  // scale = ? fb^-1 / luminosity
  Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
  Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100

  h_dy70->Scale(scale1);
  h_dy70->SetFillColor(kPink+1);
  h_dy70->SetLineColor(kBlack);
  h_dy100->Scale(scale2);
  h_dy100->SetFillColor(kViolet+1);
  h_dy100->SetLineColor(kBlack);

  THStack *h_stack = new THStack("h_stack", "Pruned jet mass distribution");
  h_stack->Add(h_dy70);
  h_stack->Add(h_dy100);

  TCanvas* c = new TCanvas("c", "", 0, 0, 800, 600);

  c->cd(1)->SetLogy();
  h_stack->Draw();
  h_stack->GetXaxis()->SetTitle("Mass");
  h_stack->GetYaxis()->SetTitle("Event number");
  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->Draw("e1same");
  
  TLegend *leg1 = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg1->AddEntry(h_dy100, "DYJetsToLL_PtZ-100", "f"); 
  leg1->AddEntry(h_data, "Data", "p");
  leg1->Draw();
  
  c->Print("backgroundStack.gif");

}
