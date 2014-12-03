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


void zpMasstest(){

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("backgZpMass.root");

  TH1D* h_ttto2l2nu2b[5];
  TH1D* h_ttjetsfulll[5];

  TCanvas* c = new TCanvas("c", "", 0, 0, 1400, 900);
  c->Divide(3,2);

  TLegend *leg[5];

  h_ttto2l2nu2b[0] = (TH1D*)(f->Get("ZpMass_TTTo2L2Nu2B.root"));
  h_ttto2l2nu2b[1] = (TH1D*)(f->Get("ZPt_TTTo2L2Nu2B.root"));
  h_ttto2l2nu2b[2] = (TH1D*)(f->Get("ZEta_TTTo2L2Nu2B.root"));
  h_ttto2l2nu2b[3] = (TH1D*)(f->Get("HiggsPt_TTTo2L2Nu2B.root"));
  h_ttto2l2nu2b[4] = (TH1D*)(f->Get("HiggsEta_TTTo2L2Nu2B.root"));

  h_ttjetsfulll[0] = (TH1D*)(f->Get("ZpMass_TTJets_FullLeptMGDecays_8TeV_filtered.root"));
  h_ttjetsfulll[1] = (TH1D*)(f->Get("ZPt_TTJets_FullLeptMGDecays_8TeV_filtered.root"));
  h_ttjetsfulll[2] = (TH1D*)(f->Get("ZEta_TTJets_FullLeptMGDecays_8TeV_filtered.root"));
  h_ttjetsfulll[3] = (TH1D*)(f->Get("HiggsPt_TTJets_FullLeptMGDecays_8TeV_filtered.root"));
  h_ttjetsfulll[4] = (TH1D*)(f->Get("HiggsEta_TTJets_FullLeptMGDecays_8TeV_filtered.root"));

  for(Int_t i = 0; i < 5; i++){

    h_ttto2l2nu2b[i]->Scale(1/h_ttto2l2nu2b[i]->Integral());
    h_ttjetsfulll[i]->Scale(1/h_ttjetsfulll[i]->Integral());

    h_ttto2l2nu2b[i]->GetYaxis()->SetTitleOffset(1.4);
    h_ttjetsfulll[i]->GetYaxis()->SetTitleOffset(1.4);

    h_ttto2l2nu2b[i]->SetMaximum(0.2);
    h_ttjetsfulll[i]->SetMaximum(0.2);

    h_ttto2l2nu2b[i]->SetLineColor(1);
    h_ttjetsfulll[i]->SetLineColor(1);

    h_ttto2l2nu2b[i]->SetFillColor(kRed);
    h_ttjetsfulll[i]->SetFillColor(kBlue+1);

    h_ttto2l2nu2b[i]->SetFillStyle(3002);
    h_ttjetsfulll[i]->SetFillStyle(3002);

    c->cd(i+1);
    h_ttto2l2nu2b[i]->Draw();
    h_ttjetsfulll[i]->Draw("same");

    leg[i] = new TLegend(0.5, 0.8, 0.9, 0.9);
    leg[i]->SetFillStyle(1001);
    leg[i]->SetFillColor(10);
    leg[i]->SetBorderSize(1);
    leg[i]->AddEntry(h_ttto2l2nu2b[i], "ttto2l2nu2b", "f");
    leg[i]->AddEntry(h_ttjetsfulll[i], "ttjetsfullleptmgdecays", "f");
    leg[i]->Draw();
    
  }

  c->Print("zpMasstest.gif");

}
