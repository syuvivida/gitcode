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

void combineBKG(){

  TFile *f = TFile::Open("backgEstimate.root");

  TH1D* h_dy70side  = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70sign  = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100sign = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dataside  = (TH1D*)(f->Get("sideZpMass_data_DoubleMu_A.root"));
  TH1D* h_datasign  = (TH1D*)(f->Get("signZpMass_data_DoubleMu_A.root"));

  h_dataside->Sumw2();
  h_datasign->Sumw2();

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  // scale = 20fb^-1 / luminosity
  Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
  Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100

  TH1D* h_combineSide = new TH1D("h_combineSide", "Side-band Zprime Mass", nvarBins, varBins);
  h_combineSide->Sumw2();
  h_combineSide->GetXaxis()->SetTitle("Mass");
  h_combineSide->GetYaxis()->SetTitle("Event Number");

  TH1D* h_combineSign = new TH1D("h_combineSign", "Signal region Zprime Mass", nvarBins, varBins);
  h_combineSign->Sumw2();
  h_combineSign->GetXaxis()->SetTitle("Mass");
  h_combineSign->GetYaxis()->SetTitle("Event Number");

  TH1D* h_alpha = new TH1D("h_alpha", "Alpha ratio", nvarBins, varBins);
  h_alpha->Sumw2();
  h_alpha->GetXaxis()->SetTitle("Zprime mass");
  h_alpha->GetYaxis()->SetTitle("Alpha Ratio");


  h_combineSide->Add(h_dy70side, scale1);
  h_combineSide->Add(h_dy100side, scale2);

  h_combineSign->Add(h_dy70sign, scale1);
  h_combineSign->Add(h_dy100sign, scale2);

  cout << "Total event of bkg side band region: " << h_combineSide->Integral() << endl;
  cout << "Total event of bkg signal region: " << h_combineSign->Integral() << endl;
  cout << "Total event of data side band region: " << h_dataside->Integral() << endl;
  cout << "Total event of data signal region: " << h_datasign->Integral() << endl;

  gStyle->SetOptStat(1111);

  TCanvas* c = new TCanvas("c", "", 0, 0, 1920, 1080);
  c->Divide(3,2);

  c->cd(1);
  h_combineSide->SetLineColor(1);
  h_combineSide->SetFillColor(kMagenta-4);
  h_combineSide->SetFillStyle(3001);
  h_combineSide->Draw("histe");

  c->cd(2);
  h_combineSign->SetLineColor(1);
  h_combineSign->SetFillColor(kRed-4);
  h_combineSign->SetFillStyle(3001);
  h_combineSign->Draw("histe");

  c->cd(4);
  h_dataside->SetLineColor(1);
  h_dataside->SetFillColor(kGreen-4);
  h_dataside->SetFillStyle(3001);
  h_dataside->Draw("histe");

  c->cd(5);
  h_datasign->SetLineColor(1);
  h_datasign->SetFillColor(kBlue-4);
  h_datasign->SetFillStyle(3001);
  h_datasign->Draw("histe");

  c->cd(6);
  h_alpha->Divide(h_combineSign, h_combineSide);
  h_alpha->Draw();


  /*
  TLegend *leg2 = new TLegend(0.65, 0.55, 0.9, 0.7);

  leg2->SetFillStyle(1001);
  leg2->SetFillColor(10);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_data, "data"); 
  leg2->AddEntry(h_combineBKG, "DY MCbackground");
  leg2->Draw();
  */


  c->Print("backgroundEstimation.gif");

}
