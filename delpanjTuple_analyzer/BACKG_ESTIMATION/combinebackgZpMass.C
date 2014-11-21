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

const Int_t NEvent_ttto2l2nu2b   = 10783509;
const Int_t NEvent_wwjetsto2l2nu = 1918235;
const Int_t NEvent_wzjetsto2l2q  = 3170990;
const Int_t NEvent_wzjetsto3lnu  = 1997979;
const Int_t NEvent_zzjetsto2l2nu = 954911;
const Int_t NEvent_zzjetsto2l2q  = 1931727;
const Int_t NEvent_wwpythia = 9850431;
const Int_t NEvent_wzpythia = 9740283;
const Int_t NEvent_zzpythia = 9674908;

const Double_t xSection_ttto2l2nu2b   = 25.8*1000;
const Double_t xSection_wwjetsto2l2nu = 5.8*1000;
const Double_t xSection_wzjetsto2l2q  = 2.22*1000;
const Double_t xSection_wzjetsto3lnu  = 1.07*1000;
const Double_t xSection_zzjetsto2l2nu = 0.395*1000;
const Double_t xSection_zzjetsto2l2q  = 1.38*1000;
const Double_t xSection_wwpythia = 56.0*1000;
const Double_t xSection_wzpythia = 22.4*1000;
const Double_t xSection_zzpythia = 7.6*1000;

Double_t scale_ttto2l2nu2b   = 1 / (NEvent_ttto2l2nu2b / xSection_ttto2l2nu2b);
Double_t scale_wwjetsto2l2nu = 1 / (NEvent_wwjetsto2l2nu / xSection_wwjetsto2l2nu);
Double_t scale_wzjetsto2l2q  = 1 / (NEvent_wzjetsto2l2q / xSection_wzjetsto2l2q);
Double_t scale_wzjetsto3lnu  = 1 / (NEvent_wzjetsto3lnu / xSection_wzjetsto3lnu);
Double_t scale_zzjetsto2l2nu = 1 / (NEvent_zzjetsto2l2nu / xSection_zzjetsto2l2nu);
Double_t scale_zzjetsto2l2q  = 1 / (NEvent_zzjetsto2l2q / xSection_zzjetsto2l2q);
Double_t scale_wwpythia = 1 / (NEvent_wwpythia / xSection_wwpythia);
Double_t scale_wzpythia = 1 / (NEvent_wzpythia / xSection_wzpythia);
Double_t scale_zzpythia = 1 / (NEvent_zzpythia / xSection_zzpythia);

void combineBkgZpMass(){

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("backgZpMass.root");

  TH1D* h_ttto2l2nu2b   = (TH1D*)(f->Get("ZpMass_TTTo2L2Nu2B.root"));
  TH1D* h_wwjetsto2l2nu = (TH1D*)(f->Get("ZpMass_WWJetsTo2L2Nu_TuneZ2star_8TeV_filtered.root"));
  TH1D* h_wzjetsto2l2q  = (TH1D*)(f->Get("ZpMass_WZJetsTo2L2Q_TuneZ2star_8TeV_filtered.root"));
  TH1D* h_wzjetsto3lnu  = (TH1D*)(f->Get("ZpMass_WZJetsTo3LNu_TuneZ2_8TeV_filtered.root"));
  TH1D* h_zzjetsto2l2nu = (TH1D*)(f->Get("ZpMass_ZZJetsTo2L2Nu_TuneZ2star_8TeV_filtered.root"));
  TH1D* h_zzjetsto2l2q  = (TH1D*)(f->Get("ZpMass_ZZJetsTo2L2Q_TuneZ2star_8TeV_filtered.root"));
  TH1D* h_wwpythia = (TH1D*)(f->Get("ZpMass_WW_pythia_filtered.root"));
  TH1D* h_wzpythia = (TH1D*)(f->Get("ZpMass_WZ_pythia_filtered.root"));
  TH1D* h_zzpythia = (TH1D*)(f->Get("ZpMass_ZZ_pythia_filtered.root"));

  h_ttto2l2nu2b->Scale(scale_ttto2l2nu2b);
  h_wwjetsto2l2nu->Scale(scale_wwjetsto2l2nu);
  h_wzjetsto2l2q->Scale(scale_wzjetsto2l2q);
  h_wzjetsto3lnu->Scale(scale_wzjetsto3lnu);
  h_zzjetsto2l2nu->Scale(scale_zzjetsto2l2nu);
  h_zzjetsto2l2q->Scale(scale_zzjetsto2l2q);
  h_wwpythia->Scale(scale_wwpythia);
  h_wzpythia->Scale(scale_wzpythia);
  h_zzpythia->Scale(scale_zzpythia);

  h_ttto2l2nu2b->SetMaximum(0.01);
  h_wwjetsto2l2nu->SetMaximum(0.01);
  h_wzjetsto2l2q->SetMaximum(0.01);
  h_wzjetsto3lnu->SetMaximum(0.01);
  h_zzjetsto2l2nu->SetMaximum(0.01);
  h_zzjetsto2l2q->SetMaximum(0.01);
  h_wwpythia->SetMaximum(0.01);
  h_wzpythia->SetMaximum(0.01);
  h_zzpythia->SetMaximum(0.01);

  h_ttto2l2nu2b->SetLineColor(1);
  h_wwjetsto2l2nu->SetLineColor(1);
  h_wzjetsto2l2q->SetLineColor(1);
  h_wzjetsto3lnu->SetLineColor(1);
  h_zzjetsto2l2nu->SetLineColor(1);
  h_zzjetsto2l2q->SetLineColor(1);
  h_wwpythia->SetLineColor(1);
  h_wzpythia->SetLineColor(1);
  h_zzpythia->SetLineColor(1);

  h_ttto2l2nu2b->SetFillColor(kRed);
  h_wwjetsto2l2nu->SetFillColor(kOrange+7);
  h_wzjetsto2l2q->SetFillColor(kYellow);
  h_wzjetsto3lnu->SetFillColor(kGreen+1);
  h_zzjetsto2l2nu->SetFillColor(kCyan);
  h_zzjetsto2l2q->SetFillColor(kBlue+1);
  h_wwpythia->SetFillColor(kViolet+2);
  h_wzpythia->SetFillColor(kPink+10);
  h_zzpythia->SetFillColor(kPink-7);

  h_ttto2l2nu2b->SetFillStyle(3244);
  h_wwjetsto2l2nu->SetFillStyle(3244);
  h_wzjetsto2l2q->SetFillStyle(3244);
  h_wzjetsto3lnu->SetFillStyle(3244);
  h_zzjetsto2l2nu->SetFillStyle(3244);
  h_zzjetsto2l2q->SetFillStyle(3244);
  h_wwpythia->SetFillStyle(3244);
  h_wzpythia->SetFillStyle(3244);
  h_zzpythia->SetFillStyle(3244);


  TCanvas* c = new TCanvas("c", "", 0, 0, 1000, 800);

  c->cd(1);
  h_ttto2l2nu2b->Draw();
  h_wwjetsto2l2nu->Draw("same");
  h_wzjetsto2l2q->Draw("same");
  h_wzjetsto3lnu->Draw("same");
  h_zzjetsto2l2nu->Draw("same");
  h_zzjetsto2l2q->Draw("same");
  h_wwpythia->Draw("same");
  h_wzpythia->Draw("same");
  h_zzpythia->Draw("same");

  TLegend *leg = new TLegend(0.7, 0.55, 0.9, 0.9);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_ttto2l2nu2b, "ttto2l2nu2b", "f");
  leg->AddEntry(h_wwjetsto2l2nu, "wwjetsto2l2nu", "f");
  leg->AddEntry(h_wzjetsto2l2q, "wzjetsto2l2q", "f");
  leg->AddEntry(h_wzjetsto3lnu, "wzjetsto3lnu", "f");
  leg->AddEntry(h_zzjetsto2l2nu, "zzjetsto2l2nu", "f");
  leg->AddEntry(h_zzjetsto2l2q, "zzjetsto2l2q", "f");
  leg->AddEntry(h_wwpythia, "wwpythia", "f");
  leg->AddEntry(h_wzpythia, "wzpythia", "f");
  leg->AddEntry(h_zzpythia, "zzpythia", "f");
  leg->Draw();

  c->Print("BkgZpMass.gif");


}
