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

const Int_t totalNEvent1 = 11765438;
const Int_t totalNEvent2 = 12511326;
const Double_t crossSection1 = 63.5*1000;
const Double_t crossSection2 = 39.4*1000;

void combineBkgEst(){

  TFile *f = TFile::Open("backgEstimate.root");

  TH1D* h_dy70side   = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70sign   = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70zmass  = (TH1D*)(f->Get("ZMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side  = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100sign  = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100zmass = (TH1D*)(f->Get("ZMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dataside   = (TH1D*)(f->Get("sideZpMass_data_DoubleMu_A.root"));
  TH1D* h_datasign   = (TH1D*)(f->Get("signZpMass_data_DoubleMu_A.root"));
  TH1D* h_datazmass  = (TH1D*)(f->Get("ZMass_data_DoubleMu_A.root"));

  h_dataside->Sumw2();
  h_datasign->Sumw2();
  h_datazmass->Sumw2();

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  // scale = ? fb^-1 / luminosity
  Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
  Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100

  TH1D* h_combineSide = new TH1D("h_combineSide", "Side-band region Zprime Mass", nvarBins, varBins);
  h_combineSide->Sumw2();
  h_combineSide->GetXaxis()->SetTitle("Mass");
  h_combineSide->GetYaxis()->SetTitle("Event Number");
  h_combineSide->Add(h_dy70side, scale1);
  h_combineSide->Add(h_dy100side, scale2);

  TH1D* h_combineSign = new TH1D("h_combineSign", "Signal region Zprime Mass", nvarBins, varBins);
  h_combineSign->Sumw2();
  h_combineSign->GetXaxis()->SetTitle("Mass");
  h_combineSign->GetYaxis()->SetTitle("Event Number");
  h_combineSign->Add(h_dy70sign, scale1);
  h_combineSign->Add(h_dy100sign, scale2);

  cout << "Total event of bkg side band region:  " << h_combineSide->Integral() << endl;
  cout << "Total event of bkg signal region:     " << h_combineSign->Integral() << endl;
  cout << "Total event of data side band region: " << h_dataside->Integral() << endl;
  cout << "Total event of data signal region:    " << h_datasign->Integral() << endl;

  TH1D* h_combineZMass = new TH1D("h_combineZMass", "Z Mass", 100, 70, 110);
  h_combineZMass->Sumw2();
  h_combineZMass->GetXaxis()->SetTitle("Mass");
  h_combineZMass->GetYaxis()->SetTitle("Event Number");
  h_combineZMass->Add(h_dy70zmass, scale1);
  h_combineZMass->Add(h_dy100zmass, scale2);

  TH1D* h_alpha = new TH1D("h_alpha", "Alpha ratio", nvarBins, varBins);
  h_alpha->Sumw2();
  h_alpha->GetXaxis()->SetTitle("Zprime mass");
  h_alpha->GetYaxis()->SetTitle("Alpha Ratio");

  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("c", "", 0, 0, 1920, 1080);
  c->Divide(3,2);

  c->cd(1);
  h_combineSide->SetLineColor(1);
  h_combineSide->SetFillColor(kMagenta-4);
  h_combineSide->SetFillStyle(3001);
  h_combineSide->Draw("histe");

  TLegend *leg1 = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_combineSide, "DYJetsToLL", "f"); 
  leg1->Draw();

  c->cd(2);
  h_combineSign->SetLineColor(1);
  h_combineSign->SetFillColor(kRed-4);
  h_combineSign->SetFillStyle(3001);
  h_combineSign->Draw("histe");

  TLegend *leg2 = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg2->SetFillStyle(1001);
  leg2->SetFillColor(10);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_combineSign, "DYJetsToLL", "f"); 
  leg2->Draw();

  c->cd(3);
  h_datazmass->SetMarkerStyle(8);
  h_datazmass->SetMarkerSize(0.5);
  h_datazmass->Draw();
  h_combineZMass->SetLineColor(1);
  h_combineZMass->SetFillColor(kBlue-4);
  h_combineZMass->SetFillStyle(3001);
  h_combineZMass->Draw("histesame");

  TLegend *leg3 = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg3->SetFillStyle(1001);
  leg3->SetFillColor(10);
  leg3->SetBorderSize(1);
  leg3->AddEntry(h_datazmass, "Data", "p"); 
  leg3->AddEntry(h_combineZMass, "DYJetsToLL", "f"); 
  leg3->Draw();

  c->cd(4);
  h_dataside->SetLineColor(1);
  h_dataside->SetFillColor(kMagenta-4);
  h_dataside->SetFillStyle(3001);
  h_dataside->Draw("histe");

  TLegend *leg4 = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg4->SetFillStyle(1001);
  leg4->SetFillColor(10);
  leg4->SetBorderSize(1);
  leg4->AddEntry(h_dataside, "Data", "f"); 
  leg4->Draw();

  c->cd(5);
  h_datasign->SetLineColor(1);
  h_datasign->SetFillColor(kRed-4);
  h_datasign->SetFillStyle(3001);
  h_datasign->Draw("histe");

  TLegend *leg5 = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg5->SetFillStyle(1001);
  leg5->SetFillColor(10);
  leg5->SetBorderSize(1);
  leg5->AddEntry(h_datasign, "Data", "f"); 
  leg5->Draw();

  c->cd(6);
  h_alpha->Divide(h_combineSign, h_combineSide);
  h_alpha->SetLineColor(1);
  h_alpha->SetMarkerStyle(8);
  h_alpha->SetMarkerSize(0.8);
  h_alpha->SetMinimum(0);
  h_alpha->SetMaximum(1);
  h_alpha->Draw();

  c->Print("backgroundEstimation.gif");

}
