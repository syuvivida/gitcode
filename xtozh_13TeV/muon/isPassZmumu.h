#include <vector>
#include <string>
#include <iostream>
#include <TMath.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include "untuplizer.h"

bool isPassZmumu(TreeReader &data, vector<Int_t>* ZmumuId){

  Int_t    nMu       = data.GetInt("nMu");
  Int_t*   muCharge  = data.GetPtrInt("muCharge");
  Float_t* muMiniIso = data.GetPtrFloat("muMiniIso");
  TClonesArray* muP4 = (TClonesArray*) data.GetPtrTObject("muP4");
  vector<bool>& isHighPtMuon = *((vector<bool>*) data.GetPtr("isHighPtMuon"));
  vector<bool>& isCustomTrackerMuon = *((vector<bool>*) data.GetPtr("isCustomTrackerMuon"));

  // select good muons
      
  std::vector<Int_t> goodMuons;
  
  for(Int_t im = 0; im < nMu; im++){
      
    TLorentzVector* myMu = (TLorentzVector*)muP4->At(im);

    if( fabs(myMu->Eta()) > 2.1 ) continue;
    if( myMu->Pt() < 20 ) continue;
    if( muMiniIso[im] >= 0.1 ) continue;
    if( !isHighPtMuon[im] && !isCustomTrackerMuon[im] ) continue;

    goodMuons.push_back(im);

  }	

  // select good Z boson

  bool findMPair = false;
  TLorentzVector* thisMu = NULL;
  TLorentzVector* thatMu = NULL;

  for(unsigned int i = 0; i < goodMuons.size(); i++){

    Int_t im = goodMuons[i];
    thisMu = (TLorentzVector*)muP4->At(im);

    for(unsigned int j = 0; j < i; j++){

      Int_t jm = goodMuons[j];
      thatMu = (TLorentzVector*)muP4->At(jm);

      Float_t pt1   = thisMu->Pt();
      Float_t pt2   = thatMu->Pt();
      Float_t ptMax = TMath::Max(pt1,pt2);
      Float_t mll   = (*thisMu+*thatMu).M();

      if( muCharge[im]*muCharge[jm] > 0 ) continue;
      if( mll < 60 || mll > 120 ) continue;
      if( ptMax < 50 ) continue;
      if( !( (isHighPtMuon[im] && isCustomTrackerMuon[jm]) || (isHighPtMuon[jm] && isCustomTrackerMuon[im]) ) ) continue;
      if( !findMPair ) ZmumuId->push_back( (pt1 > pt2) ? im : jm );

      findMPair = true;
      break;

    }
  }

  if( !findMPair ) return false;
  return true;

}
