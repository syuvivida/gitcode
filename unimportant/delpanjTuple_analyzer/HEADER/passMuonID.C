#include <map>
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

struct muMap{
  Int_t index;
  Float_t pt;
};

Bool_t muPtGreater(muMap i, muMap j){ 
  return (i.pt>j.pt); 
}

Bool_t passMuonID(TreeReader &data, 
		  Int_t *stRecoMuIndex, Int_t *ndRecoMuIndex){


  Int_t    nMu   = data.GetInt("nMu"); 
  Int_t*   muPassID = data.GetPtrInt("muPassID");
  Float_t* muPt  = data.GetPtrFloat("muPt");
  Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

  vector<Int_t> howManyMu;
  vector<Int_t> basicMuIndex;
  vector<Int_t> onlyGlobalIndex;
  vector<Int_t> onlyTightPtIdex;
  vector<Int_t> globalTightPtIndex;

  // sorting muon and pass the muon ID

  vector<muMap> sortMuPt;
  for(Int_t i = 0; i < nMu; i++){

    muMap temp;
    temp.index = i;
    temp.pt = muPt[i];
    sortMuPt.push_back(temp);

  }
  std::sort(sortMuPt.begin(),sortMuPt.end(),muPtGreater);

  Int_t nSortMu = sortMuPt.size();

  for(Int_t i = 0; i < nSortMu; i++){

    // at least two muons
    // must be tracker mu, at least one is global mu
    // pt must greater than 20, one must greater than 40

    Int_t muIndex = sortMuPt[i].index;

    if( !(muPassID[muIndex] & 4) ) continue;
    if( muPt[muIndex] <= 20 ) continue; 
    if( (muCorrTrkIso[muIndex] / muPt[muIndex]) >= 0.1 ) continue;

    howManyMu.push_back(muIndex);

    if( !(muPassID[muIndex] & 2) && muPt[muIndex] < 40 )
      basicMuIndex.push_back(muIndex);

    if( (muPassID[muIndex] & 2) && muPt[muIndex] < 40 )
      onlyGlobalIndex.push_back(muIndex);

    if( !(muPassID[muIndex] & 2) && muPt[muIndex] > 40 )
      onlyTightPtIdex.push_back(muIndex);

    if( (muPassID[muIndex] & 2) && muPt[muIndex] > 40 )
      globalTightPtIndex.push_back(muIndex);

  } // end of for loop

  if( howManyMu.size() < 2 ) return false;

  if( onlyGlobalIndex.size() < 1 && 
      onlyTightPtIdex.size() < 1 && 
      globalTightPtIndex.size() < 1 ) return false;

  if( globalTightPtIndex.size() < 1 && 
      onlyGlobalIndex.size() < 1 ) return false;

  if( globalTightPtIndex.size() < 1 && 
      onlyTightPtIdex.size() < 1 ) return false;
    

  // filling index 

  *stRecoMuIndex = -1;
  *ndRecoMuIndex = -1;

  if( globalTightPtIndex.size() > 0 ){

    *stRecoMuIndex = globalTightPtIndex[0];

    if( globalTightPtIndex.size() > 1 )
      *ndRecoMuIndex = globalTightPtIndex[1];

    else if( onlyGlobalIndex.size() > 1 )
      *ndRecoMuIndex = onlyGlobalIndex[0];

    else if( onlyTightPtIdex.size() > 1 )
      *ndRecoMuIndex = onlyTightPtIdex[0];

    else if( basicMuIndex.size() > 0 )
      *ndRecoMuIndex = basicMuIndex[0];

  }   

  if( globalTightPtIndex.size() < 1 ){

    *stRecoMuIndex = onlyGlobalIndex[0];
    *ndRecoMuIndex = onlyTightPtIdex[0];
       
  } 

  if( *stRecoMuIndex < 0 || *ndRecoMuIndex < 0 ) return false;

  return true;

}
