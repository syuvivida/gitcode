#include <iostream>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>

void combine13TeV(){

  TFile *f_df = TFile::Open("13TeVmuRecoEff_default.root");
  TFile *f_nr = TFile::Open("13TeVmuRecoEff_narrow.root");

  TGraphAsymmErrors* h_df = (TGraphAsymmErrors*)(f_df->Get("twoGlbMu"));
  TGraphAsymmErrors* h_nr = (TGraphAsymmErrors*)(f_nr->Get("twoGlbMu"));

  TCanvas* c = new TCanvas("c", "", 0, 0, 800, 800);
  TLegend *leg = new TLegend(0.6, 0.85, 0.9, 0.9);

  c->cd(1);

  h_df->SetMarkerColor(kRed);
  h_df->SetMarkerStyle(23);
  h_nr->SetMarkerStyle(22);
  h_df->Draw("ap");
  h_nr->Draw("psame");

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_df, "delpanj_v4_default_my.root", "lp"); 
  leg->AddEntry(h_nr, "delpanj_v4_narrow_my.root", "lp");
  leg->Draw();

  c->Print("13TeVmuRecoEff_2GlbMu.gif");

}
