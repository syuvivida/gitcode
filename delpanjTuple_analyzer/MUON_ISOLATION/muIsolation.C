#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TStyle.h>
#include <TChain.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/passMuonID.C"

Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);

void muIsolation(std::string inputFile){

  TreeReader data(inputFile.data());

  gStyle->SetOptStat(0);

  // Declare the histogram

  TH1D* h_DeltaR = new TH1D("h_DeltaR", "Delta R", 100, 0, 2);
  TH1D* h_DeltaR_beforeIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_beforeIsoCut");
  TH1D* h_DeltaR_TrkIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_TrkIsoCut");

  TH1D* h_Vtx = new TH1D("h_Vtx", "info_nVtx", 100, 0, 50);
  TH1D* h_Vtx_beforeIsoCut = (TH1D*)h_Vtx->Clone("h_Vtx_beforeIsoCut");
  TH1D* h_Vtx_TrkIsoCut = (TH1D*)h_Vtx->Clone("h_Vtx_TrkIsoCut");

  TH1D* h_Iso = new TH1D("h_Iso", "Isolation variables", 100, 0, 1000);
  TH1D* h_muCorrTrkIso = (TH1D*)h_Iso->Clone("h_muCorrTrkIso");
  TH1D* h_muTrkIso = (TH1D*)h_Iso->Clone("h_muTrkIso");


  // Declare the histogram of efficiency

  TGraphAsymmErrors* h_EffIsoCut = new TGraphAsymmErrors();
  h_EffIsoCut->SetMarkerColor(4);
  h_EffIsoCut->SetMarkerStyle(20);
  h_EffIsoCut->SetMarkerSize(0.5);

  TGraphAsymmErrors* h_Eff_TrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TrkIsoCut");
  TGraphAsymmErrors* h_Eff_VtxTrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_VtxTrkIsoCut");


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Int_t info_nVtx = data.GetInt("info_nVtx");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* muTrkIso = data.GetPtrFloat("muTrkIso");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

    Int_t stRecoMuIndex, ndRecoMuIndex;

    // please check if the isolation cut in this function has been comment or not
    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
      continue;


    TLorentzVector stRecoMu, ndRecoMu;   
    stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], muEta[stRecoMuIndex], muPhi[stRecoMuIndex], muM[stRecoMuIndex]);    
    ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], muEta[ndRecoMuIndex], muPhi[ndRecoMuIndex], muM[ndRecoMuIndex]); 
    
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    h_Vtx_beforeIsoCut->Fill(info_nVtx);
    h_Vtx_beforeIsoCut->Fill(info_nVtx);

   
    // if pass muCorrTrkIso cut

    Double_t cutValue = 0.1;

    if( ( muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex] ) < cutValue ){ 
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
      h_Vtx_TrkIsoCut->Fill(info_nVtx);
    }

    if( ( muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex] ) < cutValue ){
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
      h_Vtx_TrkIsoCut->Fill(info_nVtx);
    }

    // fill isolation variables

    h_muCorrTrkIso->Fill(muCorrTrkIso[stRecoMuIndex]);
    h_muCorrTrkIso->Fill(muCorrTrkIso[ndRecoMuIndex]);

    h_muTrkIso->Fill(muTrkIso[stRecoMuIndex]);
    h_muTrkIso->Fill(muTrkIso[ndRecoMuIndex]);

  }

  // end of event loop

  // -- START of TCanvas -- //

  TCanvas* c_allMuEff = new TCanvas("c_allMuEff", "", 0, 0, 1920, 560);
  c_allMuEff->Divide(3,1);

  // -- plot muCorrTrkIso efficiency as a function of Delta R -- //

  c_allMuEff->cd(1);
  h_Eff_TrkIsoCut->Divide(h_DeltaR_TrkIsoCut, h_DeltaR_beforeIsoCut);
  h_Eff_TrkIsoCut->SetTitle("muCorrTrkIso");
  h_Eff_TrkIsoCut->GetXaxis()->SetTitle("Delta R");
  h_Eff_TrkIsoCut->GetYaxis()->SetTitle("Iso/Pt cut Efficiency");
  h_Eff_TrkIsoCut->Draw();

  // -- plot muCorrTrkIso efficiency as a function of nVtx -- //

  c_allMuEff->cd(2);
  h_Eff_VtxTrkIsoCut->Divide(h_Vtx_TrkIsoCut, h_Vtx_beforeIsoCut);
  h_Eff_VtxTrkIsoCut->SetTitle("muCorrVtxTrkIso");
  h_Eff_VtxTrkIsoCut->GetXaxis()->SetTitle("nVtx");
  h_Eff_VtxTrkIsoCut->GetYaxis()->SetTitle("Iso/Pt cut Efficiency");
  h_Eff_VtxTrkIsoCut->Draw();

  // -- plot the distribution of each isolation variable -- //

  c_allMuEff->cd(3)->SetLogy();

  h_muCorrTrkIso->SetFillStyle(3001);
  h_muCorrTrkIso->SetFillColor(kBlue);
  h_muCorrTrkIso->SetLineColor(kBlack);
  h_muCorrTrkIso->Scale(1./h_muCorrTrkIso->Integral(0,101));
  h_muCorrTrkIso->GetXaxis()->SetTitle("Isolation");
  h_muCorrTrkIso->GetYaxis()->SetTitle("Normalize distribution");
  h_muCorrTrkIso->Draw();

  h_muTrkIso->SetFillStyle(3001);
  h_muTrkIso->SetFillColor(kGreen);
  h_muTrkIso->SetLineColor(kBlack);
  h_muTrkIso->Scale(1./h_muTrkIso->Integral(0,101));
  h_muTrkIso->GetXaxis()->SetTitle("Isolation");
  h_muTrkIso->GetYaxis()->SetTitle("Normalize distribution");
  h_muTrkIso->Draw("same");

  TLegend *leg = new TLegend(0.5, 0.75, 0.9, 0.9);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_muCorrTrkIso, "muCorrTrkIso"); 
  leg->AddEntry(h_muTrkIso, "muTrkIso");
  leg->Draw();

  // -- END of TCanvas -- //

  fprintf(stderr, "Processed all events\n");
  
  c_allMuEff->Print("muIsoCutEff_qqllM2000.gif");
  

}
