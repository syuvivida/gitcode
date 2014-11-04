#include <vector>
#include <string>
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

const Int_t totalNEvent_dy70 = 11764538;
const Int_t totalNEvent_dy100 = 12511326;
const Double_t crossSection_dy70 = 63.5*1000;
const Double_t crossSection_dy100 = 39.4*1000;
// scale = data_luminosity / bkg_luminosity
Double_t scale_dy70 = 1 / (totalNEvent_dy70 / crossSection_dy70);
Double_t scale_dy100 = 1 / (totalNEvent_dy100 / crossSection_dy100);

Double_t fitFunc(Double_t*, Double_t*);

void combineBkgEstConstBin(){

  TFile *f = TFile::Open("backgEstimate.root");

  TH1D* h_dy70side  = (TH1D*)(f->Get("sideZpMass_constBin_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70sign  = (TH1D*)(f->Get("signZpMass_constBin_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side = (TH1D*)(f->Get("sideZpMass_constBin_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100sign = (TH1D*)(f->Get("signZpMass_constBin_DYJetsToLL_PtZ-100.root"));

  TH1D* h_combineBkgSide = new TH1D("h_combineBkgSide", "Side-band Zp Mass", 43, 680, 2400);
  h_combineBkgSide->Sumw2();
  h_combineBkgSide->GetXaxis()->SetTitle("Mass");
  h_combineBkgSide->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSide->Add(h_dy70side, scale_dy70);
  h_combineBkgSide->Add(h_dy100side, scale_dy100);

  TH1D* h_combineBkgSign = new TH1D("h_combineBkgSign", "Signal-band Zp Mass", 43, 680, 2400);
  h_combineBkgSign->Sumw2();
  h_combineBkgSign->GetXaxis()->SetTitle("Mass");
  h_combineBkgSign->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSign->Add(h_dy70sign, scale_dy70);
  h_combineBkgSign->Add(h_dy100sign, scale_dy100);

  cout << "Total event of bkg side band region: " << h_combineBkgSide->Integral() << endl;
  cout << "Total event of bkg signal region:    " << h_combineBkgSign->Integral() << endl;

  TH1D* h_alpha = new TH1D("h_alpha", "Alpha ratio", 43, 680, 2400);
  h_alpha->Sumw2();
  h_alpha->GetXaxis()->SetTitle("Zprime mass");
  h_alpha->GetYaxis()->SetTitle("Alpha Ratio");

  TF1* fitCurveSide = new TF1("fitCurveSide", fitFunc, 680, 2400, 3);
  TF1* fitCurveSign = new TF1("fitCurveSign", fitFunc, 680, 2400, 3);
  
  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("c", "", 0, 0, 1280, 800);
  c->Divide(2,2);

  c->cd(1);
  h_combineBkgSide->SetMarkerStyle(8);
  h_combineBkgSide->SetMarkerSize(0.7);
  h_combineBkgSide->SetMarkerColor(1);
  h_combineBkgSide->Fit("fitCurveSide", "", "", 680, 2400);
  h_combineBkgSide->Draw();

  c->cd(2);
  h_combineBkgSign->SetMarkerStyle(8);
  h_combineBkgSign->SetMarkerSize(0.7);
  h_combineBkgSign->SetMarkerColor(1);
  h_combineBkgSign->Fit("fitCurveSign", "", "", 680, 2400);
  h_combineBkgSign->Draw();
  
  c->cd(3);
  h_alpha->Divide(h_combineBkgSign, h_combineBkgSide);
  h_alpha->SetMarkerColor(1);
  h_alpha->SetMarkerStyle(8);
  h_alpha->SetMarkerSize(0.8);
  h_alpha->SetMinimum(0);
  h_alpha->SetMaximum(1);
  h_alpha->Draw();

  c->Print("aplhaRatio_constBin.gif");

}

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x + par[2]/x);
  
}
