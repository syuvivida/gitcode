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
// scale = ? fb^-1 / luminosity
Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100


void myPlot(TH1D*, TH1D*, TH1D*, TH1D*);

void stcMuVariable(){

  TFile *f = TFile::Open("muVariable.root");

  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("c", "", 0, 0, 1920, 1080);
  c->Divide(2,2);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
 

  c->cd(1);
  myPlot( ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(f->Get("nVtx_data_DoubleMu_A.root"))), 
	  ((TH1D*)(f->Get("nVtx_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  c->cd(2)->SetLogy();
  myPlot( ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(f->Get("corrTrkIso_data_DoubleMu_A.root"))), 
	  ((TH1D*)(f->Get("corrTrkIso_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  c->Print("muVariable.gif");

}


void myPlot(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_data, TH1D* h_sign){

  h_data->Sumw2();

  h_dy70->Scale(scale1);
  h_dy70->SetFillColor(kCyan-4);
  h_dy70->SetLineColor(kBlack);

  h_dy100->Scale(scale2);
  h_dy100->SetFillColor(kAzure+9);
  h_dy100->SetLineColor(kBlack);

  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_dy70);
  h_stack->Add(h_dy100);

  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->Draw("e1"); 
  h_stack->Draw("same");
  h_data->Draw("e1same");
  /*
  h_sign->SetFillColor(kOrange+1);
  h_sign->SetFillStyle(3353);
  h_sign->SetLineColor(kBlack);
  h_sign->Draw("same");
  */
  TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg->AddEntry(h_dy100,"DYJetsToLL_PtZ-100", "f"); 
  //leg->AddEntry(h_sign, "ZPrime_hZ_qqll_LHC8_M1000", "f"); 
  leg->AddEntry(h_data, "Data", "lp");
  leg->Draw();

}
