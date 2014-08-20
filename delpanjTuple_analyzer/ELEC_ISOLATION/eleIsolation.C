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
#include "../HEADER/passElectronID.C"

void passElectronID(TreeReader&, Int_t*, Int_t*);

void eleIsolation(std::string inputFile, const Double_t cutValue){

  TreeReader data(inputFile.data());

  // Declare the histogram of Delta R

  TH1D* h_DeltaR = new TH1D("h_DeltaR", "Delta R", 100, 0, 2);
  h_DeltaR->GetXaxis()->SetTitle("DeltaR");
  h_DeltaR->GetYaxis()->SetTitle("Event number");

  TH1D* h_DeltaR_beforeIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_beforeIsoCut");
  TH1D* h_DeltaR_TrkIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TrkIsoCut");
  TH1D* h_DeltaR_CalIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_CalIsoCut");
  TH1D* h_DeltaR_PfIsoCut     = (TH1D*)h_DeltaR->Clone("h_DeltaR_PfIsoCut");
  TH1D* h_DeltaR_TauIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TauIsoCut");

  // Declare the histogram of efficiency

  TCanvas* c_allEleEff = new TCanvas("c_allEleEff", "", 0, 0, 1280, 720);
  c_allEleEff->Divide(2,2);

  TGraphAsymmErrors* h_EffIsoCut = new TGraphAsymmErrors();
  h_EffIsoCut->SetMarkerColor(4);
  h_EffIsoCut->SetMarkerStyle(20);
  h_EffIsoCut->SetMarkerSize(0.5);

  TGraphAsymmErrors* h_Eff_TrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TrkIsoCut");
  TGraphAsymmErrors* h_Eff_CalIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_CalIsoCut");
  TGraphAsymmErrors* h_Eff_PfIsoCut  = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_PfIsoCut");
  TGraphAsymmErrors* h_Eff_TauIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TauIsoCut");

  //________________________________________________________________________________________//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){ // begin of event loop

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleM   = data.GetPtrFloat("eleM");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");
    Float_t* eleCorrPfIso  = data.GetPtrFloat("eleCorrPfIso");
    Float_t* eleTauCorrPfIso = data.GetPtrFloat("eleTauCorrPfIso");

    Int_t stRecoEleIndex = -1;
    Int_t ndRecoEleIndex = -1;

    passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex);

    if( stRecoEleIndex < 0 || ndRecoEleIndex < 0 ) continue;

    TLorentzVector stRecoEle, ndRecoEle;   
    stRecoEle.SetPtEtaPhiM(elePt[stRecoEleIndex], eleEta[stRecoEleIndex], elePhi[stRecoEleIndex], eleM[stRecoEleIndex]);    
    ndRecoEle.SetPtEtaPhiM(elePt[ndRecoEleIndex], eleEta[ndRecoEleIndex], elePhi[ndRecoEleIndex], eleM[ndRecoEleIndex]); 
    
    h_DeltaR_beforeIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
    h_DeltaR_beforeIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
   
    // if pass eleUserTrkIso cut

    if( ( eleUserTrkIso[stRecoEleIndex] / elePt[stRecoEleIndex] ) < cutValue ) 
      h_DeltaR_TrkIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    if( ( eleUserTrkIso[ndRecoEleIndex] / elePt[ndRecoEleIndex] ) < cutValue )
      h_DeltaR_TrkIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    // if pass eleUserCalIso cut

    if( ( eleUserCalIso[stRecoEleIndex] / elePt[stRecoEleIndex] ) < cutValue ) 
      h_DeltaR_CalIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    if( ( eleUserCalIso[ndRecoEleIndex] / elePt[ndRecoEleIndex] ) < cutValue )
      h_DeltaR_CalIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    // if pass eleCorrPfIso cut

    if( ( eleCorrPfIso[stRecoEleIndex] / elePt[stRecoEleIndex] ) < cutValue ) 
      h_DeltaR_PfIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    if( ( eleCorrPfIso[ndRecoEleIndex] / elePt[ndRecoEleIndex] ) < cutValue )
      h_DeltaR_PfIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    // if pass eleTauCorrPfIso cut

    if( ( eleTauCorrPfIso[stRecoEleIndex] / elePt[stRecoEleIndex] ) < cutValue ) 
      h_DeltaR_TauIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));

    if( ( eleTauCorrPfIso[ndRecoEleIndex] / elePt[ndRecoEleIndex] ) < cutValue )
      h_DeltaR_TauIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));


    // Isolation cut efficiency

    c_allEleEff->cd(1);
    h_Eff_TrkIsoCut->Divide(h_DeltaR_TrkIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_TrkIsoCut->SetTitle("eleUserTrkIso");
    h_Eff_TrkIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_TrkIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_TrkIsoCut->Draw();

    c_allEleEff->cd(2);
    h_Eff_CalIsoCut->Divide(h_DeltaR_CalIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_CalIsoCut->SetTitle("eleUserCalIso");
    h_Eff_CalIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_CalIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_CalIsoCut->Draw();

    c_allEleEff->cd(3);
    h_Eff_PfIsoCut->Divide(h_DeltaR_PfIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_PfIsoCut->SetTitle("eleCorrPfIso");
    h_Eff_PfIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_PfIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_PfIsoCut->Draw();

    c_allEleEff->cd(4);
    h_Eff_TauIsoCut->Divide(h_DeltaR_TauIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_TauIsoCut->SetTitle("eleTauCorrPfIso");
    h_Eff_TauIsoCut->GetXaxis()->SetTitle("Iso/Pt");
    h_Eff_TauIsoCut->GetYaxis()->SetTitle("Efficiency");
    h_Eff_TauIsoCut->Draw();


    //__________________________________________________________
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  c_allEleEff->Print("eleIsoCutEff_M2000.gif");
  

}
