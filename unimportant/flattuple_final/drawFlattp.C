#include <iostream>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TROOT.h>

Color_t fillColor[] = {kGreen-7,kRed+1};
Color_t markColor[] = {kBlack,kPink-7,kBlue,kRed};
Int_t fillStyle[] = {3001,3005};
Int_t markStyle[] = {20,21,22,23};
Double_t markSize = 0.8;

void drawHisto(TLegend *leg, TH1F *h[], Int_t *i){

  h[*i]->GetYaxis()->SetTitle("a.u");
  h[*i]->GetXaxis()->SetTitleSize(0.035);

  if( *i < 2 ){
    h[*i]->SetFillStyle(fillStyle[*i]);
    h[*i]->SetFillColor(fillColor[*i]);
    h[*i]->SetLineColor(fillColor[*i]);
  }else{
    h[*i]->SetMarkerSize(markSize);
    h[*i]->SetMarkerStyle(markStyle[*i-2]);
    h[*i]->SetMarkerColor(markColor[*i-2]);
    h[*i]->SetLineColor(markColor[*i-2]);
  }

  if( *i < 1 ) h[*i]->Draw();
  else h[*i]->Draw("same");

  leg->SetTextSize(0.025);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetShadowColor(0);
  leg->Draw();

}

void drawFlattp(){

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.08);

  std::string inputName[] = { "analyz_W01J_5F_NLO_FXFX.root",
			      "analyz_W0J_5F_NLO.root",
			      "analyz_W0J_5F_NLO_Herwig.root",
			      "analyz_W01JetLO_5F.root",
			      "analyz_W0Jet_5F_LO.root",
			      "analyz_W0Jet_5F_LO_Herwig.root" };

  std::string legName[] = { "W+0/1 jet NLO with Pythia 8",
			    "W+0 jet NLO with Pythia 8", 
			    "W+0 jet NLO with Herwig++",
			    "W+0/1 jet LO with Pythia 8", 
			    "W+0 jet LO with Pythia 8",   
			    "W+0 jet LO with Herwig++" };
		        
  Int_t N = sizeof(inputName)/sizeof(inputName[0]);

  TFile *f[N];
  TTree *t[N];
  TH1F *hnJet[N];
  TH1F *hjetPt[N];
  TH1F *hjetEta[N];
  TH1F *hMet[N]; 
  TH1F *hlepPt[N];
  TH1F *hplusYlepjet[N];
  TH1F *hminusYlepjet[N];
  TH1F *hplusYWjet[N];
  TH1F *hminusYWjet[N];

  for(Int_t i = 0; i < N; i++){

    f[i] = TFile::Open(inputName[i].data());
    t[i] = (TTree*) f[i]->Get("tree");
    hnJet[i]   = (TH1F*) f[i]->Get("hnJet");
    hjetPt[i]  = new TH1F(Form("hjetPt%d",i), "", 40, 0, 200);
    hjetEta[i] = new TH1F(Form("hjetEta%d",i), "", 20, -3, 3);
    hMet[i]    = new TH1F(Form("hMet%d",i), "", 40, 0, 200);
    hlepPt[i]  = new TH1F(Form("hlepPt%d",i), "", 40, 0, 200);
    hplusYlepjet[i]  = new TH1F(Form("hplusYlepjet%d",i), "", 50, -5, 5);
    hminusYlepjet[i] = new TH1F(Form("hminusYlepjet%d",i), "", 50, -5, 5);
    hplusYWjet[i]    = new TH1F(Form("hplusYWjet%d",i), "", 50, -5, 5);
    hminusYWjet[i]   = new TH1F(Form("hminusYWjet%d",i), "", 50, -5, 5);

    if( i > 1 ){

      hnJet[i]->Sumw2();
      hjetPt[i]->Sumw2();
      hjetEta[i]->Sumw2();
      hMet[i]->Sumw2();
      hlepPt[i]->Sumw2();
      hplusYlepjet[i]->Sumw2();
      hminusYlepjet[i]->Sumw2();
      hplusYWjet[i]->Sumw2();
      hminusYWjet[i]->Sumw2();

    }

    hnJet[i]->Scale(1./hnJet[i]->Integral());

    t[i]->Draw(Form("JetPt>>hjetPt%d",i), "(nJet>0)*weight");
    hjetPt[i]->Scale(1./hjetPt[i]->Integral());
    
    t[i]->Draw(Form("JetEta>>hjetEta%d",i), "(nJet>0)*weight");
    hjetEta[i]->Scale(1./hjetEta[i]->Integral());
  
    t[i]->Draw(Form("metPt>>hMet%d",i), "(nJet>0)*weight");
    hMet[i]->Scale(1./hMet[i]->Integral());

    t[i]->Draw(Form("LepPt>>hlepPt%d",i), "abs(LepId)==11", "(nJet>0)*weight");
    hlepPt[i]->Scale(1./hlepPt[i]->Integral());

    t[i]->Draw(Form("plusYLepJet>>hplusYlepjet%d",i), "(nJet>0)*weight");
    hplusYlepjet[i]->Scale(1./hplusYlepjet[i]->Integral());
   
    t[i]->Draw(Form("minusYLepJet>>hminusYlepjet%d",i), "(nJet>0)*weight");
    hminusYlepjet[i]->Scale(1./hminusYlepjet[i]->Integral());

    t[i]->Draw(Form("plusYWJet>>hplusYWjet%d",i), "(nJet>0)*weight");
    hplusYWjet[i]->Scale(1./hplusYWjet[i]->Integral());
   
    t[i]->Draw(Form("minusYWJet>>hminusYWjet%d",i), "(nJet>0)*weight");
    hminusYWjet[i]->Scale(1./hminusYWjet[i]->Integral());
    
  }

  TLegend *leg = new TLegend(0.65, 0.65, 0.89, 0.89);
  TCanvas *c0 = new TCanvas("c0", "", 800, 600);
  TCanvas *c1 = new TCanvas("c1", "", 1300, 800);
  TCanvas *c2 = new TCanvas("c2", "", 1300, 800);

  c1->Divide(2,2);
  c2->Divide(2,2);

  for(Int_t i = 0; i < N; i++){

    if( i < 2 ) leg->AddEntry(hnJet[i], legName[i].data(), "f");
    else leg->AddEntry(hnJet[i], legName[i].data(), "pl");

    c0->cd()->SetLogy();
    hnJet[i]->SetTitle("Jet multiplicity");
    hnJet[i]->GetXaxis()->SetTitle("Jet multiplicity");
    drawHisto(leg, hnJet, &i);

    c1->cd(1);
    hjetPt[i]->SetTitle("Leading Jet p_{T}");
    hjetPt[i]->GetXaxis()->SetTitle("Leading Jet p_{T} [GeV]");
    hjetPt[i]->GetYaxis()->SetRangeUser(0, 0.31);
    drawHisto(leg, hjetPt, &i);
   
    c1->cd(2);
    hjetEta[i]->SetTitle("Leading Jet #eta");
    hjetEta[i]->GetXaxis()->SetTitle("Leading Jet #eta");
    hjetEta[i]->GetYaxis()->SetRangeUser(0, 0.075);
    drawHisto(leg, hjetEta, &i);
 
    c1->cd(3);
    hMet[i]->SetTitle("Leading #slashE_{T}");
    hMet[i]->GetXaxis()->SetTitle("Leading #slashE_{T} [GeV]");
    hMet[i]->GetYaxis()->SetRangeUser(0, 0.3);
    drawHisto(leg, hMet, &i);

    c1->cd(4);
    hlepPt[i]->SetTitle("Leading p_{T}^{e}");
    hlepPt[i]->GetXaxis()->SetTitle("Leading p_{T}^{e} [GeV]");
    hlepPt[i]->GetYaxis()->SetRangeUser(0, 0.31);
    drawHisto(leg, hlepPt, &i);

    c2->cd(1);
    hplusYlepjet[i]->SetTitle("[Y_{lep} + Y_{jet}]/2");
    hplusYlepjet[i]->GetXaxis()->SetTitle("[Y_{lep} + Y_{jet}]/2");
    hplusYlepjet[i]->GetYaxis()->SetRangeUser(0, 0.055);
    drawHisto(leg, hplusYlepjet, &i);
 
    c2->cd(2);
    hminusYlepjet[i]->SetTitle("[Y_{lep} - Y_{jet}]/2");
    hminusYlepjet[i]->GetXaxis()->SetTitle("[Y_{lep} - Y_{jet}]/2");
    hminusYlepjet[i]->GetYaxis()->SetRangeUser(0, 0.13);
    drawHisto(leg, hminusYlepjet, &i);

    c2->cd(3);
    hplusYWjet[i]->SetTitle("[Y_{W} + Y_{jet}]/2");
    hplusYWjet[i]->GetXaxis()->SetTitle("[Y_{W} + Y_{jet}]/2");
    hplusYWjet[i]->GetYaxis()->SetRangeUser(0, 0.055);
    drawHisto(leg, hplusYWjet, &i);
 
    c2->cd(4);
    hminusYWjet[i]->SetTitle("[Y_{W} - Y_{jet}]/2");
    hminusYWjet[i]->GetXaxis()->SetTitle("[Y_{W} - Y_{jet}]/2");
    hminusYWjet[i]->GetYaxis()->SetRangeUser(0, 0.25);
    drawHisto(leg, hminusYWjet, &i);
    
  }
  
  c0->Print("WplusJet.pdf(", "pdf");
  c1->Print("WplusJet.pdf", "pdf");
  c2->Print("WplusJet.pdf)", "pdf");

}
