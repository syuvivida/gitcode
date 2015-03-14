#include <iostream>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TROOT.h>

void drawFlattp(){

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  Int_t N = 6;

  std::string name[N] = {"analyz_W01J_5F_NLO_FXFX.root",
			 "analyz_W01JetLO_5F.root",
			 "analyz_W0J_5F_NLO.root",
			 "analyz_W0Jet_5F_LO.root",
			 "analyz_W0J_5F_NLO_Herwig.root",
			 "analyz_W0Jet_5F_LO_Herwig.root"};

  TFile *f[N];
  TTree *t[N];
  TH1F *hnJet[N];
  TH1F *hjetPt[N];
  TH1F *hjetEta[N];
  TH1F *hMet[N]; 
  TH1F *hlepPt[N];
  TH1F *hplusYlepjet[N];
  TH1F *hminusYlepjet[N];

  for(Int_t i = 0; i < N; i++){

    f[i] = TFile::Open(name[i].data());
    t[i] = (TTree*) f[i]->Get("tree");
    hnJet[i]   = (TH1F*) f[i]->Get("hnJet");
    hjetPt[i]  = new TH1F(Form("hjetPt%d",i), "", 40, 0, 200);
    hjetEta[i] = new TH1F(Form("hjetEta%d",i), "", 20, -3, 3);
    hMet[i]    = new TH1F(Form("hMet%d",i), "", 40, 0, 200);
    hlepPt[i]  = new TH1F(Form("hlepPt%d",i), "", 40, 0, 200);
    hplusYlepjet[i]  = new TH1F(Form("hplusYlepjet%d",i), "", 30, -3, 3);
    hminusYlepjet[i] = new TH1F(Form("hminusYlepjet%d",i), "", 30, -3, 3);

    hnJet[i]->Sumw2();
    hnJet[i]->Scale(1./hnJet[i]->Integral());

    hjetPt[i]->Sumw2();
    t[i]->Draw("JetPt>>hjetPt[i]", "(nJet>0)*weight");
    hjetPt[i]->Scale(1./hjetPt[i]->Integral());

    hjetEta[i]->Sumw2();
    t[i]->Draw("JetEta>>hjetEta[i]", "(nJet>0)*weight");
    hjetEta[i]->Scale(1./hjetEta[i]->Integral());

    hMet[i]->Sumw2();
    t[i]->Draw("metPt>>hMet[i]", "(nJet>0)*weight");
    hMet[i]->Scale(1./hMet[i]->Integral());

    hlepPt[i]->Sumw2();
    t[i]->Draw("LepPt>>hlepPt[i]", "abs(LepId)==11", "(nJet>0)*weight");
    hlepPt[i]->Scale(1./hlepPt[i]->Integral());

    hplusYlepjet[i]->Sumw2();
    t[i]->Draw("plusYLepJet>>hplusYlepjet[i]", "(nJet>0)*weight");
    hplusYlepjet[i]->Scale(1./hplusYlepjet[i]->Integral());

    hminusYlepjet[i]->Sumw2();
    t[i]->Draw("minusYLepJet>>hminusYlepjet[i]", "(nJet>0)*weight");
    hminusYlepjet[i]->Scale(1./hminusYlepjet[i]->Integral());

  }

  
  Int_t Color[N] = {1,3,5,7,9,11};
  Int_t markerStyle[N] = {21,23,25,27,33,31};
  /*
  TCanvas *c0 = new TCanvas("c0", "", 800, 600);

  c0->cd()->SetLogy();
  for(Int_t i = 0; i < N; i++){

    hnJet[i]->GetYaxis()->SetTitle("a.u");
    hnJet[i]->GetXaxis()->SetTitle("Jet multiplicity");
    hnJet[i]->SetMarkerSize(1);
    hnJet[i]->SetMarkerStyle(markerStyle[i]);
    hnJet[i]->SetMarkerColor(Color[i]);
    hnJet[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hnJet[i]->Draw();
    else
      hnJet[i]->Draw("same");

  }
  */
  TCanvas *c = new TCanvas("c", "", 1300, 800);
  c->Divide(3,2);

  c->cd(1);
  for(Int_t i = 0; i < N; i++){

    hjetPt[i]->GetYaxis()->SetTitle("a.u");
    hjetPt[i]->GetXaxis()->SetTitle("Leading Jet p_{T} [GeV]");
    hjetPt[i]->SetMarkerSize(1);
    hjetPt[i]->SetMarkerStyle(markerStyle[i]);
    hjetPt[i]->SetMarkerColor(Color[i]);
    hjetPt[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hjetPt[i]->Draw();
    else
      hjetPt[i]->Draw("same");

  }
  /*
  c->cd(2);
  for(Int_t i = 0; i < N; i++){

    hjetEta[i]->GetYaxis()->SetTitle("a.u");
    hjetEta[i]->GetXaxis()->SetTitle("Leading Jet #eta");
    hjetEta[i]->SetMarkerSize(1);
    hjetEta[i]->SetMarkerStyle(markerStyle[i]);
    hjetEta[i]->SetMarkerColor(Color[i]);
    hjetEta[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hjetEta[i]->Draw();
    else
      hjetEta[i]->Draw("same");

  }

  c->cd(3);
  for(Int_t i = 0; i < N; i++){

    hMet[i]->GetYaxis()->SetTitle("a.u");
    hMet[i]->GetXaxis()->SetTitle("Leading #slashE_{T} [GeV]");
    hMet[i]->SetMarkerSize(1);
    hMet[i]->SetMarkerStyle(markerStyle[i]);
    hMet[i]->SetMarkerColor(Color[i]);
    hMet[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hMet[i]->Draw();
    else
      hMet[i]->Draw("same");

  }

  c->cd(4);
  for(Int_t i = 0; i < N; i++){

    hlepPt[i]->GetYaxis()->SetTitle("a.u");
    hlepPt[i]->GetXaxis()->SetTitle("Leading p_{T}^{e} [GeV]");
    hlepPt[i]->SetMarkerSize(1);
    hlepPt[i]->SetMarkerStyle(markerStyle[i]);
    hlepPt[i]->SetMarkerColor(Color[i]);
    hlepPt[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hlepPt[i]->Draw();
    else
      hlepPt[i]->Draw("same");

  }

  c->cd(5);
  for(Int_t i = 0; i < N; i++){

    hplusYlepjet[i]->GetYaxis()->SetTitle("a.u");
    hplusYlepjet[i]->GetXaxis()->SetTitle("[Y_{lep} + Y_{jet}]/2");
    hplusYlepjet[i]->SetMarkerSize(1);
    hplusYlepjet[i]->SetMarkerStyle(markerStyle[i]);
    hplusYlepjet[i]->SetMarkerColor(Color[i]);
    hplusYlepjet[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hplusYlepjet[i]->Draw();
    else
      hplusYlepjet[i]->Draw("same");

  }

  c->cd(6);
  for(Int_t i = 0; i < N; i++){

    hminusYlepjet[i]->GetYaxis()->SetTitle("a.u");
    hminusYlepjet[i]->GetXaxis()->SetTitle("[Y_{lep} - Y_{jet}]/2");
    hminusYlepjet[i]->SetMarkerSize(1);
    hminusYlepjet[i]->SetMarkerStyle(markerStyle[i]);
    hminusYlepjet[i]->SetMarkerColor(Color[i]);
    hminusYlepjet[i]->SetLineColor(Color[i]);
    if( i < 1 )
      hminusYlepjet[i]->Draw();
    else
      hminusYlepjet[i]->Draw("same");

  }
  */
  //c0->Draw();
  c->Draw();
  //c0->SaveAs("bbb.png");
  //c->SaveAs("aaaa.png");
  
}

    /*
      TLegend *leg0 = new TLegend(0.6, 0.7, 0.85, 0.9);
      leg0->AddEntry(hjetmulNLO, "0 jet NLO", "pl");
      leg0->AddEntry(h1jetmulNLO, "0/1 jet NLO", "pl");
      leg0->AddEntry(h0jetmulLO, "0 jet LO", "f");
      leg0->AddEntry(hjetmulLO, "0/1 jet LO", "f");
      leg0->SetTextSize(0.04);
      leg0->SetTextFont(42);
      leg0->SetFillColor(0);
      leg0->SetLineColor(0);
      leg0->SetShadowColor(0);
      leg0->Draw();
    */
    /*
      TLegend *leg1 = new TLegend(0.5, 0.6, 0.75, 0.85);
      leg1->AddEntry(hjetptNLO, "0 jet NLO", "pl");
      leg1->AddEntry(h1jetptNLO, "0/1 jet NLO", "pl");
      leg1->AddEntry( hjetPt[i], "0 jet LO", "f");
      leg1->AddEntry(hjetptLO, "0/1 jet LO", "f");
      leg1->SetTextSize(0.045);
      leg1->SetTextFont(42);
      leg1->SetFillColor(0);
      leg1->SetLineColor(0);
      leg1->SetShadowColor(0);
      leg1->Draw();
    */
   /*
      TLegend *leg2 = new TLegend(0.4, 0.3, 0.6, 0.6);
      leg2->AddEntry(hplusYlepjetNLO, "0 jet NLO", "pl");
      leg2->AddEntry(hplusYlep1jetNLO, "0/1 jet NLO", "pl");
      leg2->AddEntry(hplusYlep0jetLO, "0 jet LO", "f");
      leg2->AddEntry(hplusYlepjetLO, "0/1 jet LO", "f");
      leg2->SetTextSize(0.045);
      leg2->SetTextFont(42);
      leg2->SetFillColor(0);
      leg2->SetLineColor(0);
      leg2->SetShadowColor(0);
      leg2->Draw();
    */
