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
const Int_t totalNEvent3 = 10783509;
const Double_t crossSection1 = 63.5*1000;
const Double_t crossSection2 = 39.4*1000;
const Double_t crossSection3 = 25.8*1000;

void combineBKG(){

  TFile *f = TFile::Open("backgMwindowMu.root");

  TH1D* h_bk1 = (TH1D*)(f->Get("MwindowTrk_cut2000_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_bk2 = (TH1D*)(f->Get("MwindowTrk_cut2000_DYJetsToLL_PtZ-100.root"));
  TH1D* h_bk3 = (TH1D*)(f->Get("MwindowTrk_cut2000_TTTo2L2Nu2B.root"));
  TH1D* h_sgn = (TH1D*)(f->Get("MwindowTrk_cut0_ZPrime_hZ_qqll_LHC8_M2000.root"));

  h_sgn->SetTitle("Iso/Pt of Total Background");
  h_sgn->GetYaxis()->SetTitle("Normalized distribution");

  // scale = 20fb^-1 / luminosity
  Double_t scale1 = 20 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
  Double_t scale2 = 20 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100
  Double_t scale3 = 20 / (totalNEvent3 / crossSection3); // TTTo2L2Nu2B

  TH1D* h_combineBKG = new TH1D("h_combineBKG", "Iso/Pt of Total Background", 40, 0, 0.2);
  h_combineBKG->GetXaxis()->SetTitle("Iso/Pt");
  h_combineBKG->GetYaxis()->SetTitle("Normalized distribution");

  h_combineBKG->Add(h_bk1, scale1);
  h_combineBKG->Add(h_bk2, scale2);
  h_combineBKG->Add(h_bk3, scale3);


  //-------------------------------------------------------------------------------------


  Double_t bkgtotalPassEvent = h_combineBKG->Integral(0,41);
  Double_t sigtotalPassEvent = h_sgn->Integral(0,41);

  h_sgn->Scale(1/sigtotalPassEvent);
  h_combineBKG->Scale(1/bkgtotalPassEvent);

  vector<Double_t> B;
  vector<Double_t> S;
  vector<Double_t> bkgisolationEff;
  vector<Double_t> sigisolationEff;
  vector<Double_t> OneSubtrBkgEff;
  vector<Double_t> SgnOverBkg;
  vector<Double_t> SOverSqrtSplusB;
  vector<Double_t> SEffOverOneplusSqrtB;
  vector<Double_t> upperThreshold;

  for(Int_t bin = 0; bin < 21; bin++){

    upperThreshold.push_back(bin*(0.2/40));

    B.push_back(h_combineBKG->Integral(0,bin));
    S.push_back(h_sgn->Integral(0,bin));

    bkgisolationEff.push_back( B[bin] );
    sigisolationEff.push_back( S[bin] );

    OneSubtrBkgEff.push_back(1-bkgisolationEff[bin]);
    SgnOverBkg.push_back(S[bin]/B[bin]);
    SOverSqrtSplusB.push_back(S[bin]/TMath::Sqrt(S[bin]+B[bin]));
    SEffOverOneplusSqrtB.push_back(sigisolationEff[bin]/(1+TMath::Sqrt(B[bin])));

  }


  //-------------------------------------------------------------------------------------


  const Double_t lineColor = 4;
  const Double_t lineWidth = 2;
  const Double_t markStyle = 8;
  const Double_t markSize  = 0.5;

  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadGridX(kTRUE);

  TGraph* h_OneSubtrBkgEff = new TGraph(sigisolationEff.size(), sigisolationEff.data(), OneSubtrBkgEff.data());
  h_OneSubtrBkgEff->SetTitle("(1-BKG Eff) over SIG Eff");
  h_OneSubtrBkgEff->GetYaxis()->SetTitle("1 - Background Efficiency");
  h_OneSubtrBkgEff->GetXaxis()->SetTitle("Signal Efficiency");
  h_OneSubtrBkgEff->SetLineColor(lineColor);
  h_OneSubtrBkgEff->SetLineWidth(lineWidth);
  h_OneSubtrBkgEff->SetMarkerStyle(markStyle);
  h_OneSubtrBkgEff->SetMarkerSize(markSize);


  TGraph* h_SgnOverBkg = new TGraph(upperThreshold.size(), upperThreshold.data(), SgnOverBkg.data());
  h_SgnOverBkg->SetTitle("S/B over Iso/Pt upper threshold");
  h_SgnOverBkg->GetYaxis()->SetTitle("S/B");
  h_SgnOverBkg->GetXaxis()->SetTitle("Iso/Pt upper threshold");
  h_SgnOverBkg->SetLineColor(lineColor);
  h_SgnOverBkg->SetLineWidth(lineWidth);
  h_SgnOverBkg->SetMarkerStyle(markStyle);
  h_SgnOverBkg->SetMarkerSize(markSize);


  TGraph* h_SOverSqrtSplusB = new TGraph(upperThreshold.size(), upperThreshold.data(), SOverSqrtSplusB.data());
  h_SOverSqrtSplusB->SetTitle("S/sqrt(S+B) over Iso/Pt upper threshold");
  h_SOverSqrtSplusB->GetYaxis()->SetTitle("S/sqrt(S+B)");
  h_SOverSqrtSplusB->GetXaxis()->SetTitle("Iso/Pt upper threshold");
  h_SOverSqrtSplusB->SetLineColor(lineColor);
  h_SOverSqrtSplusB->SetLineWidth(lineWidth);
  h_SOverSqrtSplusB->SetMarkerStyle(markStyle);
  h_SOverSqrtSplusB->SetMarkerSize(markSize);


  TGraph* h_SEffOverOneplusSqrtB = new TGraph(upperThreshold.size(), upperThreshold.data(), SEffOverOneplusSqrtB.data());
  h_SEffOverOneplusSqrtB->SetTitle("SIG Eff/(1+sqrt(B)) over Iso/Pt upper threshold");
  h_SEffOverOneplusSqrtB->GetYaxis()->SetTitle("effs/(1+sqrt(B))");
  h_SEffOverOneplusSqrtB->GetXaxis()->SetTitle("Iso/Pt upper threshold");
  h_SEffOverOneplusSqrtB->SetLineColor(lineColor);
  h_SEffOverOneplusSqrtB->SetLineWidth(lineWidth);
  h_SEffOverOneplusSqrtB->SetMarkerStyle(markStyle);
  h_SEffOverOneplusSqrtB->SetMarkerSize(markSize);


  TGraph* h_bkgtotalIsoEff = new TGraph(upperThreshold.size(), upperThreshold.data(), bkgisolationEff.data());
  h_bkgtotalIsoEff->SetTitle("Efficiency over Iso/Pt upper threshold");
  h_bkgtotalIsoEff->GetYaxis()->SetTitle("Efficiency");
  h_bkgtotalIsoEff->GetXaxis()->SetTitle("Iso/Pt upper threshold");
  h_bkgtotalIsoEff->SetLineColor(2);
  h_bkgtotalIsoEff->SetLineWidth(lineWidth);
  h_bkgtotalIsoEff->SetMarkerStyle(markStyle);
  h_bkgtotalIsoEff->SetMarkerSize(markSize);

  TGraph* h_sigtotalIsoEff = new TGraph(upperThreshold.size(), upperThreshold.data(), sigisolationEff.data());
  h_sigtotalIsoEff->SetTitle("Efficiency over Iso/Pt upper threshold");
  h_sigtotalIsoEff->GetYaxis()->SetTitle("Efficiency");
  h_sigtotalIsoEff->GetXaxis()->SetTitle("Iso/Pt upper threshold");
  h_sigtotalIsoEff->SetLineColor(lineColor);
  h_sigtotalIsoEff->SetLineWidth(lineWidth);
  h_sigtotalIsoEff->SetMarkerStyle(markStyle);
  h_sigtotalIsoEff->SetMarkerSize(markSize);


  //-------------------------------------------------------------------------------------


  TCanvas* c_allPlots = new TCanvas("c_allPlots", "", 0, 0, 1920, 1080);
  c_allPlots->Divide(3,2);

  c_allPlots->cd(1)->SetLogy();
  h_combineBKG->SetLineColor(1);
  h_combineBKG->SetFillColor(2);
  h_combineBKG->SetFillStyle(3001);

  if( h_sgn->GetMaximum() > h_combineBKG->GetMaximum() ){
    h_sgn->Draw();
    h_combineBKG->Draw("same");
  }else{
    h_combineBKG->Draw();
    h_sgn->Draw("same");
  }

  TLegend *leg1 = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_combineBKG, "backgr"); 
  leg1->AddEntry(h_sgn, "signal");
  leg1->Draw();

  c_allPlots->cd(2);
  h_bkgtotalIsoEff->Draw();
  h_sigtotalIsoEff->Draw("cpsame");

  TLegend *leg2 = new TLegend(0.65, 0.55, 0.9, 0.7);

  leg2->SetFillStyle(1001);
  leg2->SetFillColor(10);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_bkgtotalIsoEff, "backgr"); 
  leg2->AddEntry(h_sigtotalIsoEff, "signal");
  leg2->Draw();

  c_allPlots->cd(3);
  h_OneSubtrBkgEff->Draw();

  c_allPlots->cd(4);
  h_SgnOverBkg->Draw();

  c_allPlots->cd(5);
  h_SOverSqrtSplusB->Draw();

  c_allPlots->cd(6);
  h_SEffOverOneplusSqrtB->Draw();

  c_allPlots->Print("backgroundOptimize_cut2000.gif");

}
