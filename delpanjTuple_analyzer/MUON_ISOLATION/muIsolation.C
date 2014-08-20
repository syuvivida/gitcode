#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/passMuonID.C"

void passMuonID(TreeReader&, Int_t*, Int_t*);

void muIsolation(std::string inputFile, const Double_t cutValue){

  TreeReader data(inputFile.data());

  // Declare the histogram of Delta R

  TH1D* h_DeltaR = new TH1D("h_DeltaR", "Delta R", 100, 0, 2);
  h_DeltaR->GetXaxis()->SetTitle("DeltaR");
  h_DeltaR->GetYaxis()->SetTitle("Event number");

  TH1D* h_DeltaR_beforeIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_beforeIsoCut");
  TH1D* h_DeltaR_TrkIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TrkIsoCut");
  TH1D* h_DeltaR_PfIsoCut     = (TH1D*)h_DeltaR->Clone("h_DeltaR_PfIsoCut");
  TH1D* h_DeltaR_TauIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TauIsoCut");

  // Declare the histogram of efficiency

  TCanvas* c_allMuEff = new TCanvas("c_allMuEff", "", 0, 0, 1280, 720);
  c_allMuEff->Divide(2,2);

  TGraphAsymmErrors* h_EffIsoCut = new TGraphAsymmErrors();
  h_EffIsoCut->SetMarkerColor(4);
  h_EffIsoCut->SetMarkerStyle(20);
  h_EffIsoCut->SetMarkerSize(0.5);

  TGraphAsymmErrors* h_Eff_TrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TrkIsoCut");
  TGraphAsymmErrors* h_Eff_PfIsoCut  = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_PfIsoCut");
  TGraphAsymmErrors* h_Eff_TauIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TauIsoCut");

  //________________________________________________________________________________________//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){ // begin of event loop

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");
    Float_t* muCorrPfIso = data.GetPtrFloat("muCorrPfIso");
    Float_t* muTauCorrPfIso = data.GetPtrFloat("muTauCorrPfIso");

    Int_t stRecoMuIndex = -1;
    Int_t ndRecoMuIndex = -1;

    passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex);

    if( stRecoMuIndex < 0 || ndRecoMuIndex < 0 ) continue;

    TLorentzVector stRecoMu, ndRecoMu;   
    stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], muEta[stRecoMuIndex], muPhi[stRecoMuIndex], muM[stRecoMuIndex]);    
    ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], muEta[ndRecoMuIndex], muPhi[ndRecoMuIndex], muM[ndRecoMuIndex]); 
    
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
   
    // if pass muCorrTrkIso cut

    if( ( muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex] ) < cutValue ) 
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    if( ( muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex] ) < cutValue )
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    // if pass muCorrPfIso cut

    if( ( muCorrPfIso[stRecoMuIndex] / muPt[stRecoMuIndex] ) < cutValue ) 
      h_DeltaR_PfIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    if( ( muCorrPfIso[ndRecoMuIndex] / muPt[ndRecoMuIndex] ) < cutValue )
      h_DeltaR_PfIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    // if pass muTauCorrPfIso cut

    if( ( muTauCorrPfIso[stRecoMuIndex] / muPt[stRecoMuIndex] ) < cutValue ) 
      h_DeltaR_TauIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    if( ( muTauCorrPfIso[ndRecoMuIndex] / muPt[ndRecoMuIndex] ) < cutValue )
      h_DeltaR_TauIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));


    // Isolation cut efficiency

    c_allMuEff->cd(1);
    h_Eff_TrkIsoCut->Divide(h_DeltaR_TrkIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_TrkIsoCut->SetTitle("muCorrTrkIso");
    h_Eff_TrkIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_TrkIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_TrkIsoCut->Draw();

    c_allMuEff->cd(2);
    h_Eff_PfIsoCut->Divide(h_DeltaR_PfIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_PfIsoCut->SetTitle("muCorrPfIso");
    h_Eff_PfIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_PfIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_PfIsoCut->Draw();

    c_allMuEff->cd(3);
    h_Eff_TauIsoCut->Divide(h_DeltaR_TauIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_TauIsoCut->SetTitle("muTauCorrPfIso");
    h_Eff_TauIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_TauIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_TauIsoCut->Draw();


    //__________________________________________________________
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");
  
  c_allMuEff->Print("muIsoCutEff_M2000.gif");
  
}
