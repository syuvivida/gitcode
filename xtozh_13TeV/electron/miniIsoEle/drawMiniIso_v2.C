#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>

void drawMiniIso_v2(){

  TFile *file = TFile::Open("outputMiniIso/ZprimeToZhToZlephbb_eleMiniIso.root");
   
  gStyle->SetOptStat(0);

  std::string h_name[] = {"miniIsonVtx","corrIsonVtx","EffHEEPMini","EffHEEPCorr",
			  "deltaR","eleRho","eleMiniIso","eleCorrIso","effectiveArea"};

  TProfile* p[2];
  p[0] = (TProfile*)(file->Get(h_name[0].data())) ;
  p[1] = (TProfile*)(file->Get(h_name[1].data()));

  TGraphAsymmErrors* a[2];
  a[0] = (TGraphAsymmErrors*)(file->Get(h_name[2].data()));
  a[1] = (TGraphAsymmErrors*)(file->Get(h_name[3].data()));

  TH1D* h[5];
  h[0] = (TH1D*)(file->Get(h_name[4].data()));
  h[1] = (TH1D*)(file->Get(h_name[5].data()));
  h[2] = (TH1D*)(file->Get(h_name[6].data()));
  h[3] = (TH1D*)(file->Get(h_name[7].data()));
  h[4] = (TH1D*)(file->Get(h_name[8].data()));

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  c->cd();
  p[0]->SetLineColor(kBlue);
  p[0]->Draw();
  p[1]->SetLineColor(kRed);
  p[1]->Draw("same");

  TLegend *legp = new TLegend(0.25, 0.25, 0.55, 0.35);

  legp->SetFillStyle(0);
  legp->SetBorderSize(1);
  legp->AddEntry(p[0], "miniIsonVtx", "le");
  legp->AddEntry(p[1], "corrIsonVtx", "le");
  legp->Draw();

  c->Print("eleMiniIso_v2.pdf(");  

  c->cd(); h[0]->Draw(); c->Print("eleMiniIso_v2.pdf");
  c->cd(); h[1]->Draw(); c->Print("eleMiniIso_v2.pdf");
  c->cd(); h[2]->Draw(); c->Print("eleMiniIso_v2.pdf");
  c->cd(); h[3]->Draw(); c->Print("eleMiniIso_v2.pdf");
  c->cd(); h[4]->Draw(); c->Print("eleMiniIso_v2.pdf");

  TCanvas* d = new TCanvas("d","",0,0,1000,800);

  d->cd();
  a[0]->SetLineColor(kBlue);
  a[0]->Draw("AP");
  a[1]->SetLineColor(kRed);
  a[1]->Draw("sameP");

  TLegend *lega = new TLegend(0.25, 0.25, 0.55, 0.35);

  lega->SetFillStyle(0);
  lega->SetBorderSize(1);
  lega->AddEntry(a[0], "EffHEEPMini", "le");
  lega->AddEntry(a[1], "EffHEEPCorr", "le");
  lega->Draw();

  d->Print("eleMiniIso_v2.pdf)");

}
