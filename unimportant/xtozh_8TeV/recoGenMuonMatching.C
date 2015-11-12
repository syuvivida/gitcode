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
#include "untuplizer.h"
#include "specificLeptonPt.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);

void recoGenMuonMatching(TreeReader &data, 
			  Int_t *this_firstRecoIndex, Int_t *this_secondRecoIndex, 
			  Int_t *this_firstGenIndex, Int_t *this_secondGenIndex){


  Float_t* elePt  = data.GetPtrFloat("elePt");
  Float_t* muPt  = data.GetPtrFloat("muPt");
  Float_t* muEta = data.GetPtrFloat("muEta");
  Float_t* muPhi = data.GetPtrFloat("muPhi");
  Float_t* muM   = data.GetPtrFloat("muM");

  Int_t stMuPtIndex  = -1;
  Int_t ndMuPtIndex  = -1;
  Int_t stElePtIndex = -1;
  Int_t ndElePtIndex = -1;

  specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		   &stElePtIndex, &ndElePtIndex);

  if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
      (stElePtIndex < 0 || ndElePtIndex < 0 )  ) return; 
  
  if( stMuPtIndex > 0 && stElePtIndex > 0 ){
    
    if( (muPt[stMuPtIndex] - elePt[stElePtIndex]) < 1e-6 ) 
      return;

  }


  //-----------------------------------------------------------------------------------//
  // to pass the generator muons

  Int_t    nGenPar     = data.GetInt("nGenPar"); 
  Int_t*   genParId    = data.GetPtrInt("genParId");
  Int_t*   genMomParId = data.GetPtrInt("genMomParId");
  Int_t*   genParSt    = data.GetPtrInt("genParSt");
  Float_t* genParPt    = data.GetPtrFloat("genParPt");
  Float_t* genParEta   = data.GetPtrFloat("genParEta");
  Float_t* genParPhi   = data.GetPtrFloat("genParPhi");
  Float_t* genParM     = data.GetPtrFloat("genParM");
      
  Int_t gen_MuonID = -1;
  Int_t gen_antiMuonID = -1;

  for(Int_t i = 0; i < nGenPar; i++){

    if( genMomParId[i] == 23 && genParSt[i] == 1 && genParId[i] == 13 ) gen_MuonID = i;
    if( genMomParId[i] == 23 && genParSt[i] == 1 && genParId[i] == -13 ) gen_antiMuonID = i; 
    if( gen_MuonID > -1 && gen_antiMuonID > -1 ) break;
    
  }

  if( gen_MuonID < 0 || gen_antiMuonID < 0 ) return;

  TLorentzVector gen_Muon, gen_antiMuon;

  gen_Muon.SetPtEtaPhiM(genParPt[gen_MuonID], genParEta[gen_MuonID], genParPhi[gen_MuonID], genParM[gen_MuonID]);
  gen_antiMuon.SetPtEtaPhiM(genParPt[gen_antiMuonID], genParEta[gen_antiMuonID], genParPhi[gen_antiMuonID], genParM[gen_antiMuonID]);


  //-----------------------------------------------------------------------------------//
  // to check if reconstructed muon match generator muon
    
  TLorentzVector reco_stMuon, reco_ndMuon;

  reco_stMuon.SetPtEtaPhiM(muPt[stMuPtIndex], muEta[stMuPtIndex], muPhi[stMuPtIndex], muM[stMuPtIndex]);
  reco_ndMuon.SetPtEtaPhiM(muPt[ndMuPtIndex], muEta[ndMuPtIndex], muPhi[ndMuPtIndex], muM[ndMuPtIndex]);

  Double_t recostMuon_with_genMuonDeltaR = reco_stMuon.DeltaR(gen_Muon);
  Double_t recostMuon_with_genantiMuonDeltaR = reco_stMuon.DeltaR(gen_antiMuon);
  Double_t recondMuon_with_genMuonDeltaR = reco_ndMuon.DeltaR(gen_Muon);
  Double_t recondMuon_with_genantiMuonDeltaR = reco_ndMuon.DeltaR(gen_antiMuon);

  *this_firstRecoIndex = -1;
  *this_secondRecoIndex = -1;
  *this_firstGenIndex = -1;
  *this_secondGenIndex = -1;

  if( recostMuon_with_genMuonDeltaR < 0.3 || recostMuon_with_genantiMuonDeltaR < 0.3 ){
    *this_firstRecoIndex = stMuPtIndex;
    *this_firstGenIndex = gen_MuonID;
  }

  if( recondMuon_with_genMuonDeltaR < 0.3 || recondMuon_with_genantiMuonDeltaR < 0.3 ){
    *this_secondRecoIndex = ndMuPtIndex;
    *this_secondGenIndex = gen_antiMuonID;
  }

  if( *this_firstRecoIndex < 0 || *this_secondRecoIndex < 0 || 
      *this_firstGenIndex < 0  || *this_secondGenIndex < 0  ) return;  

}
