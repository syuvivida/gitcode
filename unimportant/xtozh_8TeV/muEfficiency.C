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
#include "../HEADER/specificLeptonPt.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);

void muEfficiency(std::string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of Pt

  TH1D* h_Pt = new TH1D("h_Pt", "Transverse momentum", 80, 0, 800);
  h_Pt->GetXaxis()->SetTitle("Pt");
  h_Pt->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_gblmuPt_denom = (TH1D*)h_Pt->Clone("h_st_gblmuPt_denom");
  TH1D* h_st_gblmuPt_numer = (TH1D*)h_Pt->Clone("h_st_gblmuPt_numer");
  TH1D* h_st_trkmuPt_denom = (TH1D*)h_Pt->Clone("h_st_trkmuPt_denom");
  TH1D* h_st_trkmuPt_numer = (TH1D*)h_Pt->Clone("h_st_trkmuPt_numer");

  TH1D* h_nd_gblmuPt_denom = (TH1D*)h_Pt->Clone("h_nd_gblmuPt_denom");
  TH1D* h_nd_gblmuPt_numer = (TH1D*)h_Pt->Clone("h_nd_gblmuPt_numer");
  TH1D* h_nd_trkmuPt_denom = (TH1D*)h_Pt->Clone("h_nd_trkmuPt_denom");
  TH1D* h_nd_trkmuPt_numer = (TH1D*)h_Pt->Clone("h_nd_trkmuPt_numer");

  // Declare the histogram of efficiency

  TH1D* h_Eff_Pt = new TH1D("h_Eff_Pt", "Efficiency", 80, 0, 800); h_Eff_Pt->Sumw2();
  h_Eff_Pt->GetXaxis()->SetTitle("Pt");
  h_Eff_Pt->GetYaxis()->SetTitle("Efficiency");

  TH1D* h_IDeff_st_gblmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_gblmuPt");
  TH1D* h_IDeff_st_trkmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_trkmuPt");

  TH1D* h_IDeff_nd_gblmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_nd_gblmuPt");
  TH1D* h_IDeff_nd_trkmuPt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_nd_trkmuPt");

  // Declare the histogram of Eta

  TH1D* h_Eta = new TH1D("h_Eta", "Eta", 80, -4, 4);
  h_Eta->GetXaxis()->SetTitle("Eta");
  h_Eta->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_gblmuEta_denom = (TH1D*)h_Eta->Clone("h_st_gblmuEta_denom");
  TH1D* h_st_gblmuEta_numer = (TH1D*)h_Eta->Clone("h_st_gblmuEta_numer");
  TH1D* h_st_trkmuEta_denom = (TH1D*)h_Eta->Clone("h_st_trkmuEta_denom");
  TH1D* h_st_trkmuEta_numer = (TH1D*)h_Eta->Clone("h_st_trkmuEta_numer");

  TH1D* h_nd_gblmuEta_denom = (TH1D*)h_Eta->Clone("h_nd_gblmuEta_denom");
  TH1D* h_nd_gblmuEta_numer = (TH1D*)h_Eta->Clone("h_nd_gblmuEta_numer");
  TH1D* h_nd_trkmuEta_denom = (TH1D*)h_Eta->Clone("h_nd_trkmuEta_denom");
  TH1D* h_nd_trkmuEta_numer = (TH1D*)h_Eta->Clone("h_nd_trkmuEta_numer");

  // Declare the histogram of efficiency

  TH1D* h_Eff_Eta = new TH1D("h_Eff_Eta", "Efficiency", 80, -4, 4); h_Eff_Eta->Sumw2();
  h_Eff_Eta->GetXaxis()->SetTitle("Eta");
  h_Eff_Eta->GetYaxis()->SetTitle("Efficiency");

  TH1D* h_IDeff_st_gblmuEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_st_gblmuEta");
  TH1D* h_IDeff_st_trkmuEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_st_trkmuEta");

  TH1D* h_IDeff_nd_gblmuEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_nd_gblmuEta");
  TH1D* h_IDeff_nd_trkmuEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_nd_trkmuEta");

  //________________________________________________________________________________________//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){ // begin of event loop

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    //__________________________________________________________
    // choosing the primary reconstructed muon

    Int_t* isGlobalMuon  = data.GetPtrInt("isGlobalMuon");
    Int_t* isTrackerMuon = data.GetPtrInt("isTrackerMuon");
    Int_t* muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt   = data.GetPtrFloat("muPt");
    Float_t* muEta  = data.GetPtrFloat("muEta");

    Float_t* elePt   = data.GetPtrFloat("elePt");

    Int_t stMuPtIndex  = -1;
    Int_t ndMuPtIndex  = -1;
    Int_t stElePtIndex = -1;
    Int_t ndElePtIndex = -1;

    specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		     &stElePtIndex, &ndElePtIndex);

    if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
	(stElePtIndex < 0 || ndElePtIndex < 0 )  ) continue; 
  
    if( stMuPtIndex >= 0 && stElePtIndex >= 0 ){
    
      if( (muPt[stMuPtIndex] - elePt[stElePtIndex]) < 1e-6 ) 
	continue;

    }

    //__________________________________________________________
    // highest muon pt

    //----------------------------------------------------------
    // global muons

    if( isGlobalMuon[stMuPtIndex] ){

      h_st_gblmuPt_denom ->Fill(muPt[stMuPtIndex]);
      h_st_gblmuEta_denom->Fill(muEta[stMuPtIndex]);

      if( muPassID[stMuPtIndex] & 2 ){

	h_st_gblmuPt_numer ->Fill(muPt[stMuPtIndex]);
	h_st_gblmuEta_numer->Fill(muEta[stMuPtIndex]);
      
      }
    }

    h_IDeff_st_gblmuPt ->Divide(h_st_gblmuPt_numer,  h_st_gblmuPt_denom,  1, 1, "B");
    h_IDeff_st_gblmuEta->Divide(h_st_gblmuEta_numer, h_st_gblmuEta_denom, 1, 1, "B");

    //----------------------------------------------------------
    // tracker muons

    if( isTrackerMuon[stMuPtIndex] ){

      h_st_trkmuPt_denom ->Fill(muPt[stMuPtIndex]);
      h_st_trkmuEta_denom->Fill(muEta[stMuPtIndex]);
    
      if( muPassID[stMuPtIndex] & 4 ){

	h_st_trkmuPt_numer ->Fill(muPt[stMuPtIndex]);
	h_st_trkmuEta_numer->Fill(muEta[stMuPtIndex]);

      }
    }

    h_IDeff_st_trkmuPt ->Divide(h_st_trkmuPt_numer,  h_st_trkmuPt_denom,  1, 1, "B");
    h_IDeff_st_trkmuEta->Divide(h_st_trkmuEta_numer, h_st_trkmuEta_denom, 1, 1, "B");  
    
    //__________________________________________________________
    // second highest muon pt

    //----------------------------------------------------------
    // global muons  

    if( isGlobalMuon[ndMuPtIndex] ){

      h_nd_gblmuPt_denom ->Fill(muPt[ndMuPtIndex]);
      h_nd_gblmuEta_denom->Fill(muEta[ndMuPtIndex]);

      if( muPassID[ndMuPtIndex] & 2 ){

	h_nd_gblmuPt_numer ->Fill(muPt[ndMuPtIndex]);
	h_nd_gblmuEta_numer->Fill(muEta[ndMuPtIndex]);
      
      }
    }

    h_IDeff_nd_gblmuPt ->Divide(h_nd_gblmuPt_numer,  h_nd_gblmuPt_denom,  1, 1, "B");
    h_IDeff_nd_gblmuEta->Divide(h_nd_gblmuEta_numer, h_nd_gblmuEta_denom, 1, 1, "B");

    //----------------------------------------------------------
    // tracker muons

    if( isTrackerMuon[ndMuPtIndex] ){

      h_nd_trkmuPt_denom ->Fill(muPt[ndMuPtIndex]);
      h_nd_trkmuEta_denom->Fill(muEta[ndMuPtIndex]);

      if( muPassID[ndMuPtIndex] & 4 ){

	h_nd_trkmuPt_numer ->Fill(muPt[ndMuPtIndex]);
	h_nd_trkmuEta_numer->Fill(muEta[ndMuPtIndex]);
      
      }
    }

    h_IDeff_nd_trkmuPt ->Divide(h_nd_trkmuPt_numer,  h_nd_trkmuPt_denom,  1, 1, "B");
    h_IDeff_nd_trkmuEta->Divide(h_nd_trkmuEta_numer, h_nd_trkmuEta_denom, 1, 1, "B");
    
    //__________________________________________________________
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");
  
  std::string outName = "muonIDeff_" + inputFile;
  TFile* outFile = new TFile(outName.data(), "recreate");

  h_IDeff_st_trkmuPt ->Write();
  h_IDeff_st_gblmuPt ->Write();
  h_IDeff_nd_trkmuPt ->Write();
  h_IDeff_nd_gblmuPt ->Write();
  
  h_IDeff_st_trkmuEta->Write();
  h_IDeff_st_gblmuEta->Write();
  h_IDeff_nd_trkmuEta->Write();
  h_IDeff_nd_gblmuEta->Write();
  
  outFile->Write();
  
}
