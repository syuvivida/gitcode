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
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/passElectronID.C"

Double_t fitFunc(Double_t*, Double_t*);
Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);

void eleIsolation(std::string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of Delta R

  gStyle->SetOptStat(0);

  TH1D* h_DeltaR = new TH1D("h_DeltaR", "Delta R", 100, 0, 2);
  TH1D* h_DeltaR_beforeIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_beforeIsoCut");
  TH1D* h_DeltaR_TrkIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TrkIsoCut");
  TH1D* h_DeltaR_CalIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_CalIsoCut");

  TH1D* h_Vtx = new TH1D("h_Vtx", "info_nVtx", 100, 0, 50);
  TH1D* h_Vtx_beforeIsoCut = (TH1D*)h_Vtx->Clone("h_Vtx_beforeIsoCut");
  TH1D* h_Vtx_TrkIsoCut = (TH1D*)h_Vtx->Clone("h_Vtx_TrkIsoCut");
  TH1D* h_Vtx_CalIsoCut = (TH1D*)h_Vtx->Clone("h_Vtx_CalIsoCut");

  // Declare the histogram of efficiency

  TCanvas* c_allEleEff = new TCanvas("c_allEleEff", "", 0, 0, 1920, 1080);
  c_allEleEff->Divide(3,2);

  TGraphAsymmErrors* h_EffIsoCut = new TGraphAsymmErrors();
  h_EffIsoCut->SetMarkerColor(4);
  h_EffIsoCut->SetMarkerStyle(20);
  h_EffIsoCut->SetMarkerSize(0.5);

  TGraphAsymmErrors* h_Eff_TrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_TrkIsoCut");
  TGraphAsymmErrors* h_Eff_CalIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_CalIsoCut");
  TGraphAsymmErrors* h_Eff_VtxTrkIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_VtxTrkIsoCut");
  TGraphAsymmErrors* h_Eff_VtxCalIsoCut = (TGraphAsymmErrors*)h_EffIsoCut->Clone("h_Eff_VtxCalIsoCut");

  TProfile* p_CalIso_Rho = new TProfile("p_CalIso_Rho", "eleUserCalIso vs eleRho", 100, 0, 20, 0, 10);


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Int_t info_nVtx = data.GetInt("info_nVtx");
    Float_t  eleRho = data.GetFloat("eleRho");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleEt  = data.GetPtrFloat("eleEt");
    Float_t* eleM   = data.GetPtrFloat("eleM");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");

    Int_t stRecoEleIndex, ndRecoEleIndex;

    // please check if the isolation cut in this function has been comment or not 
    if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) ) 
      continue;

    TLorentzVector stRecoEle, ndRecoEle;   
    stRecoEle.SetPtEtaPhiM(elePt[stRecoEleIndex], eleEta[stRecoEleIndex], elePhi[stRecoEleIndex], eleM[stRecoEleIndex]);    
    ndRecoEle.SetPtEtaPhiM(elePt[ndRecoEleIndex], eleEta[ndRecoEleIndex], elePhi[ndRecoEleIndex], eleM[ndRecoEleIndex]); 
    
    h_DeltaR_beforeIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
    h_DeltaR_beforeIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
   
    h_Vtx_beforeIsoCut->Fill(info_nVtx);
    h_Vtx_beforeIsoCut->Fill(info_nVtx);

    // if pass eleUserTrkIso cut

    Double_t stcutValue = 2 + ( 0.03 * eleEt[stRecoEleIndex] );
    Double_t ndcutValue = 2 + ( 0.03 * eleEt[ndRecoEleIndex] );

    if( ( eleUserTrkIso[stRecoEleIndex] ) < stcutValue ){ 
      h_DeltaR_TrkIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
      h_Vtx_TrkIsoCut->Fill(info_nVtx);
    }

    if( ( eleUserTrkIso[ndRecoEleIndex] ) < ndcutValue ){
      h_DeltaR_TrkIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
      h_Vtx_TrkIsoCut->Fill(info_nVtx);
    }

    // if pass eleUserCalIso cut

    if( ( eleUserCalIso[stRecoEleIndex] - (0.06205 * eleRho) ) < stcutValue ){ 
      h_DeltaR_CalIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
      h_Vtx_CalIsoCut->Fill(info_nVtx);
    }

    if( ( eleUserCalIso[ndRecoEleIndex] - (0.06205 * eleRho) ) < ndcutValue ){
      h_DeltaR_CalIsoCut->Fill(stRecoEle.DeltaR(ndRecoEle));
      h_Vtx_CalIsoCut->Fill(info_nVtx);
    }


    // Isolation cut efficiency
    
    c_allEleEff->cd(1);
    h_Eff_TrkIsoCut->Divide(h_DeltaR_TrkIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_TrkIsoCut->SetTitle("eleUserTrkIso");
    h_Eff_TrkIsoCut->GetXaxis()->SetTitle("Delta R");
    h_Eff_TrkIsoCut->GetYaxis()->SetTitle("TrkIso cut Efficiency");
    h_Eff_TrkIsoCut->Draw();

    c_allEleEff->cd(2);
    h_Eff_VtxTrkIsoCut->Divide(h_Vtx_TrkIsoCut, h_Vtx_beforeIsoCut);
    h_Eff_VtxTrkIsoCut->SetTitle("eleCorrVtxTrkIso");
    h_Eff_VtxTrkIsoCut->GetXaxis()->SetTitle("nVtx");
    h_Eff_VtxTrkIsoCut->GetYaxis()->SetTitle("TrkIso cut Efficiency");
    h_Eff_VtxTrkIsoCut->Draw();

    c_allEleEff->cd(4);
    h_Eff_CalIsoCut->Divide(h_DeltaR_CalIsoCut, h_DeltaR_beforeIsoCut);
    h_Eff_CalIsoCut->SetTitle("eleUserCalIso");
    h_Eff_CalIsoCut->GetXaxis()->SetTitle("Delta R");
    h_Eff_CalIsoCut->GetYaxis()->SetTitle("CalIso cut Efficiency");
    h_Eff_CalIsoCut->Draw();

    c_allEleEff->cd(5);
    h_Eff_VtxCalIsoCut->Divide(h_Vtx_CalIsoCut, h_Vtx_beforeIsoCut);
    h_Eff_VtxCalIsoCut->SetTitle("eleCorrVtxCalIso");
    h_Eff_VtxCalIsoCut->GetXaxis()->SetTitle("nVtx");
    h_Eff_VtxCalIsoCut->GetYaxis()->SetTitle("CalIso cut Efficiency");
    h_Eff_VtxCalIsoCut->Draw();

    c_allEleEff->cd(6);
    p_CalIso_Rho->Fill(eleUserCalIso[stRecoEleIndex], eleRho);
    p_CalIso_Rho->Fill(eleUserCalIso[ndRecoEleIndex], eleRho);
    p_CalIso_Rho->Draw();
    
  } 

  // end of event loop

  // fitting function

  c_allEleEff->cd(6);

  TF1* fitLine = new TF1("fitLine", fitFunc, 0, 20, 2);
  fitLine->SetParameters(1, 1);
  p_CalIso_Rho->Fit("fitLine", "","", 0, 20);
  p_CalIso_Rho->GetXaxis()->SetTitle("eleRho");
  p_CalIso_Rho->GetYaxis()->SetTitle("eleUserCalIso");
  p_CalIso_Rho->Draw("same");

  fprintf(stderr, "Processed all events\n");

  c_allEleEff->Print("eleIsoCutEff_qqllM2000.gif");
  

}


Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*x+par[1];

}
