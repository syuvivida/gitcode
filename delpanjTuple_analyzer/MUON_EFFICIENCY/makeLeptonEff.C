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

  Double_t mass[12];
  Double_t eff[12];
  Double_t isoeff[12];

  ifstream fin;
  fin.open(inputFile.data());

  Int_t nBIN = 1;
  fin >> mass[0] >> eff[0] >> isoeff[0];

  while(!fin.eof()){

    fin >> mass[nBIN] >> eff[nBIN] >> isoeff[nBIN]; 
    nBIN++;

  }

  TGraphErrors *h_eff = new TGraphErrors(nBIN, mass, eff);
  h_eff->GetXaxis()->SetTitle("Zprime mass");
  h_eff->GetYaxis()->SetTitle("Efficiency");
  h_eff->GetYaxis()->SetTitleOffset(1.5);
  h_eff->SetMinimum(0);
  h_eff->SetTitle("Muon");
  h_eff->SetMarkerStyle(8);
  h_eff->SetMarkerSize(1);

  TGraphErrors *h_isoeff = new TGraphErrors(nBIN, mass, isoeff);
  h_isoeff->GetXaxis()->SetTitle("Zprime mass");
  h_isoeff->GetYaxis()->SetTitle("Efficiency");
  h_isoeff->GetYaxis()->SetTitleOffset(1.5);
  h_isoeff->SetTitle("Muon");
  h_isoeff->SetMarkerStyle(8);
  h_isoeff->SetMarkerSize(1);
  
  TCanvas *c = new TCanvas("c", "", 700, 700);
  h_eff->Draw();
  h_isoeff->Draw("elsame");
  
  TLegend *leg = new TLegend(0.6, 0.78, 0.9, 0.9);
  leg->SetBorderSize(1);
  leg->SetFillColor(10);
  leg->SetFillStyle(1001);
  leg->SetTextSize(0.03);
  leg->AddEntry(h_eff, "basic cut", "lp");
  leg->AddEntry(h_isoeff, "basic cut+iso cut", "lp");
  leg->Draw("same");

}
