#include <vector>
#include <string>
#include <iostream>
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
#include "../HEADER/untuplizer.h"
#include "../HEADER/recoGenMuonMatching.C"

void recoGenMuonMatching(TreeReader& ,Int_t*, Int_t*, Int_t*, Int_t*);

void check_muEfficiency(string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of Pt

  TH1D* h_Pt = new TH1D("h_Pt", "Transverse momentum", 100, 0, 800);
  h_Pt->GetXaxis()->SetTitle("Pt");
  h_Pt->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_muPt_denom    = (TH1D*)h_Pt->Clone("h_st_muPt_denom");
  TH1D* h_st_gblmuPt_numer = (TH1D*)h_Pt->Clone("h_st_gblmuPt_numer");
  TH1D* h_manual_st_gblmuPt_numer = (TH1D*)h_Pt->Clone("h_manual_st_gblmuPt_numer");

  TH1D* h_Eff_Pt = new TH1D("h_Eff_Pt", "Efficiency", 100, 0, 800); h_Eff_Pt->Sumw2();
  h_Eff_Pt->GetXaxis()->SetTitle("Pt");
  h_Eff_Pt->GetYaxis()->SetTitle("Efficiency");

  // Declare the histogram of efficiency

  TH1D* h_IDeff_st_gblmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_gblmuPt");
  TH1D* h_IDeff_manual_st_gblmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_manual_st_gblmuPt");

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 600);
  c->Divide(3,1);


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Int_t* muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");

    Int_t this_firstRecoIndex  = -1;
    Int_t this_secondRecoIndex = -1;			
    Int_t this_firstGenIndex   = -1;
    Int_t this_secondGenIndex  = -1;

    recoGenMuonMatching(data, &this_firstRecoIndex, &this_secondRecoIndex, 
			&this_firstGenIndex, &this_secondGenIndex);


    // highest muon pt

    Int_t*   isGlobalMuon = data.GetPtrInt("isGlobalMuon");
    Int_t*   muHits       = data.GetPtrInt("muHits");
    Int_t*   muMatches    = data.GetPtrInt("muMatches");
    Int_t*   muPixelHits  = data.GetPtrInt("muPixelHits");
    Int_t*   muTrkLayers  = data.GetPtrInt("muTrkLayers");
    Float_t* muPtErrx     = data.GetPtrFloat("muPtErrx");
    Float_t* mudxy        = data.GetPtrFloat("mudxy");
    Float_t* mudz         = data.GetPtrFloat("mudz");

    // global selection
    if( isGlobalMuon[this_firstRecoIndex] ){

      h_st_muPt_denom->Fill(muPt[this_firstRecoIndex]);

      // global muon (auto pass function)
      if( muPassID[this_firstRecoIndex] & 2 )
	h_st_gblmuPt_numer->Fill(muPt[this_firstRecoIndex]);

      c->cd(1);
      h_IDeff_st_gblmuPt->Divide(h_st_gblmuPt_numer, h_st_muPt_denom, 1, 1, "B");
      h_IDeff_st_gblmuPt->Draw();

      // global muon (manual pass loop)
      if( muHits[this_firstRecoIndex] <= 0 ) continue;
      if( muMatches[this_firstRecoIndex] <= 1 ) continue;
      if( fabs(mudxy[this_firstRecoIndex]) >= 0.2 ) continue;
      if( fabs(mudz[this_firstRecoIndex]) >= 0.5 ) continue;
      if( muPixelHits[this_firstRecoIndex] <= 0 ) continue;
      if( muTrkLayers[this_firstRecoIndex] <= 5 ) continue;
      if( muPt[this_firstRecoIndex] <= 10 ) continue;
      if( muPtErrx[this_firstRecoIndex] >= 0.3 ) continue;
      if( muEta[this_firstRecoIndex] >= 2.4 ) continue;
      if( muPhi[this_firstRecoIndex] >= 3.2 )  continue;

      h_manual_st_gblmuPt_numer->Fill(muPt[this_firstRecoIndex]);

      c->cd(2);
      h_IDeff_manual_st_gblmuPt->Divide(h_manual_st_gblmuPt_numer, h_st_muPt_denom, 1, 1, "B");
      h_IDeff_manual_st_gblmuPt->Draw();

    }

    c->cd(3);
    h_IDeff_st_gblmuPt->Draw(); 
    h_IDeff_manual_st_gblmuPt->SetLineColor(2);
    h_IDeff_manual_st_gblmuPt->Draw("same");


  } 

  // end of event loop

  fprintf(stderr, "Processed all events\n");

  c->Print("checkMuIDEff.gif");

}
