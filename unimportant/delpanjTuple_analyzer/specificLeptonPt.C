#include <map>
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
#include "untuplizer.h"

void specificLeptonPt(TreeReader &data, Int_t *stMuPtIndex, Int_t *ndMuPtIndex, 
		      Int_t *stElePtIndex, Int_t *ndElePtIndex){


  // specific muons pt

  Int_t    nMu   = data.GetInt("nMu"); 
  Float_t* muPt  = data.GetPtrFloat("muPt");
  Float_t* muEta = data.GetPtrFloat("muEta");

  typedef map<Float_t, Int_t, std::greater<Float_t> > muonMap;
  muonMap sortMuonPt;
  typedef muonMap::iterator mapMuIter;
  
  for(Int_t i = 0; i < nMu; i++){

    sortMuonPt.insert(std::pair<Float_t, Int_t>(muPt[i], i));

  }

  *stMuPtIndex = -1;
  *ndMuPtIndex = -1;

  for(mapMuIter it_part = sortMuonPt.begin(); it_part != sortMuonPt.end(); ++it_part){

    Int_t sortMuIndex = it_part->second;

    if( fabs(muEta[sortMuIndex]) >= 2.4 ) continue;
    if( muPt[sortMuIndex] <= 20 ) continue;

    if( *stMuPtIndex == -1 )
      *stMuPtIndex = sortMuIndex;
    else if( *ndMuPtIndex == -1 )
      *ndMuPtIndex = sortMuIndex;

    if( *stMuPtIndex >= 0 && *ndMuPtIndex >= 0 ) break;

  }


  //-----------------------------------------------------------------------------------//


  // specific electrons pt

  Int_t    nEle   = data.GetInt("nEle");
  Float_t* elePt  = data.GetPtrFloat("elePt");
  Float_t* eleEta = data.GetPtrFloat("eleEta");

  typedef map<Float_t, Int_t, std::greater<Float_t> > eleMap;
  eleMap sortElePt;
  typedef eleMap::iterator mapEleIter;
  
  for(Int_t i = 0; i < nEle; i++){

    sortElePt.insert(std::pair<Float_t, Int_t>(elePt[i], i));

  }

  *stElePtIndex = -1;
  *ndElePtIndex = -1;

  for(mapEleIter it_part = sortElePt.begin(); it_part != sortElePt.end(); ++it_part){

    Int_t sortEleIndex = it_part->second;

    if( fabs(eleEta[sortEleIndex]) >= 2.5 ) continue;
    if( elePt[sortEleIndex] <= 35 ) continue;
      
    if( *stElePtIndex == -1 )
      *stElePtIndex = sortEleIndex;
    else if( *ndElePtIndex == -1 )
      *ndElePtIndex = sortEleIndex;

    if( *stElePtIndex >= 0 && *ndElePtIndex >= 0 ) break;

  }


  //-----------------------------------------------------------------------------------//

  
}
