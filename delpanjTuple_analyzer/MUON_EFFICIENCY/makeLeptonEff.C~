#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>

void makeLeptonEff(std::string inputFile){

  Int_t arrsize = 12;
  Int_t nBIN = 0;
  Double_t mass[arrsize];
  Double_t eff[arrsize];
  Double_t effErr[arrsize];
  Double_t isoeff[arrsize];
  Double_t isoeffErr[arrsize];

  ifstream fin;
  fin.open(inputFile.data(), ios::in);

  while(!fin.eof() && nBIN < arrsize){

    fin >> mass[nBIN] >> eff[nBIN] >> effErr[nBIN] >> isoeff[nBIN] >> isoeffErr[nBIN]; 
    nBIN++;

  }

  TGraphErrors *h_eff = new TGraphErrors(nBIN, mass, eff, 0, effErr);
  h_eff->GetXaxis()->SetTitle("Zprime mass");
  h_eff->GetYaxis()->SetTitle("Efficiency");
  h_eff->GetYaxis()->SetTitleOffset(1.5);
  h_eff->SetMinimum(0);
  h_eff->SetTitle("DiMuon Efficiency");
  h_eff->SetMarkerStyle(8);
  h_eff->SetMarkerSize(0.6);
  h_eff->SetLineWidth(2);
  h_eff->SetLineColor(kRed+1);

  TGraphErrors *h_isoeff = new TGraphErrors(nBIN, mass, isoeff, 0, isoeffErr);
  h_isoeff->GetXaxis()->SetTitle("Zprime mass");
  h_isoeff->GetYaxis()->SetTitle("Efficiency");
  h_isoeff->GetYaxis()->SetTitleOffset(1.5);
  h_isoeff->SetTitle("DiMuon Efficiency");
  h_isoeff->SetMarkerStyle(8);
  h_isoeff->SetMarkerSize(0.6);
  h_isoeff->SetLineWidth(2);
  h_isoeff->SetLineColor(kBlue+1);
  
  TCanvas *c = new TCanvas("c", "", 700, 700);
  h_eff->Draw();
  h_isoeff->Draw("cpsame");

  TLegend *leg = new TLegend(0.6, 0.78, 0.9, 0.9);
  leg->SetBorderSize(1);
  leg->SetFillColor(10);
  leg->SetFillStyle(1001);
  leg->SetTextSize(0.03);
  leg->AddEntry(h_eff, "basic cut", "lp");
  leg->AddEntry(h_isoeff, "basic cut+iso cut", "lp");
  leg->Draw("same");

  c->Print("dimuonEfficiency.png");

}
