#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TPad.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TVectorT.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>


void zpMasstest(){

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("backgZpMass.root");

  TH1D* h_ttto2l2nu2b   = (TH1D*)(f->Get("ZpMass_TTTo2L2Nu2B.root"));
  TH1D* h_ttjetsfulll = (TH1D*)(f->Get("ZpMass_TTJets_FullLeptMGDecays_8TeV_filtered.root"));


  h_ttto2l2nu2b->Scale(1/h_ttto2l2nu2b->Integral());
  h_ttjetsfulll->Scale(1/h_ttjetsfulll->Integral());


  Double_t vmax = 0.3;

  h_ttto2l2nu2b->SetMaximum(vmax);
  h_ttjetsfulll->SetMaximum(vmax);

  h_ttto2l2nu2b->SetLineColor(1);
  h_ttjetsfulll->SetLineColor(1);

  h_ttto2l2nu2b->SetFillColor(kRed);
  h_ttjetsfulll->SetFillColor(kBlue+1);

  h_ttto2l2nu2b->SetFillStyle(3004);
  h_ttjetsfulll->SetFillStyle(3005);


  TCanvas* c = new TCanvas("c", "", 0, 0, 1000, 800);

  c->cd(1);
  h_ttto2l2nu2b->Draw();
  h_ttjetsfulll->Draw("same");


  TLegend *leg = new TLegend(0.7, 0.8, 0.9, 0.9);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_ttto2l2nu2b, "ttto2l2nu2b", "f");
  leg->AddEntry(h_ttjetsfulll, "ttjetsfullleptmgdecays", "f");
  leg->Draw();

  c->Print("zpMasstest.gif");

}
