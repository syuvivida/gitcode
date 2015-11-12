#include <map>
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
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "untuplizer.h"
#include "specificLeptonPt.C"
#include "passMuonID.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);
Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);

void reconstructZPrime(TreeReader &data, Double_t *ZprimeMass, Double_t *PrunedjetMass){

  Int_t    CA8nJet    = data.GetInt("CA8nJet"); 
  Int_t*   CA8jetPassID = data.GetPtrInt("CA8jetPassID");
  Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
  Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
  Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
  Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");
  Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");

  Int_t    nMu   = data.GetInt("nMu");
  Int_t*   muPassID = data.GetPtrInt("muPassID");
  Float_t* muPt  = data.GetPtrFloat("muPt");
  Float_t* muEta = data.GetPtrFloat("muEta");
  Float_t* muPhi = data.GetPtrFloat("muPhi");
  Float_t* muM   = data.GetPtrFloat("muM");

  Float_t* elePt  = data.GetPtrFloat("elePt");


  //-----------------------------------------------------------------------------------//
  // choose the primary muon

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
  // sorting muon and pass the muon ID

  vector<Int_t> howManyMu;

  typedef map<double, int, std::greater<double> > muMap;
  muMap sortMuPt;
  typedef muMap::iterator mapMuIter;

  for(Int_t i = 0; i < nMu; i++){

    sortMuPt.insert(std::pair<Float_t, Int_t>(muPt[i], i));

  }

  for(mapMuIter it_part = sortMuPt.begin(); it_part != sortMuPt.end(); ++it_part){

    Int_t sortMuIndex = it_part->second;

    if( !(muPassID[sortMuIndex] & 4) ) continue;
    if( muPt[sortMuIndex] <= 20 ) continue; 

    howManyMu.push_back(sortMuIndex);


  }

  Int_t stRecoMuIndex, ndRecoMuIndex;
  
  if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
    return;
  

  //-----------------------------------------------------------------------------------//   
  // reconstruct Z mass

  TLorentzVector stRecoMu, ndRecoMu;  
 
  stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], 
			muEta[stRecoMuIndex], 
			muPhi[stRecoMuIndex],
			muM[stRecoMuIndex]);  
  
  ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], 
			muEta[ndRecoMuIndex],
			muPhi[ndRecoMuIndex], 
			muM[ndRecoMuIndex]); 
    
  TLorentzVector Z = stRecoMu + ndRecoMu;


  //-----------------------------------------------------------------------------------//
  // pass boosted-jet ID, removing overlap muons

  typedef map<Float_t, Int_t, std::greater<Float_t> > jetMap;
  jetMap sortJetPt;
  typedef jetMap::iterator mapJetIter;

  for(Int_t i = 0; i < CA8nJet; i++){

    sortJetPt.insert(std::pair<Float_t, Int_t>(CA8jetPt[i], i));

  }

  vector<Int_t> maxJetIndex;
  TLorentzVector boostedJet, basicMuon;

  for(mapJetIter it_part = sortJetPt.begin(); it_part != sortJetPt.end(); ++it_part){

    Int_t sortJetIndex = it_part->second;

    if( CA8jetPt[sortJetIndex] <= 30 ) continue;
    if( fabs(CA8jetEta[sortJetIndex]) >= 2.4 ) continue;

    if( CA8jetPassID[sortJetIndex] > 0 ){

      boostedJet.SetPtEtaPhiM(CA8jetPt[sortJetIndex], 
			      CA8jetEta[sortJetIndex], 
			      CA8jetPhi[sortJetIndex], 
			      CA8jetMass[sortJetIndex]);


      Bool_t isolatedStats = true; 

      for(size_t j = 0; j < howManyMu.size(); j++){

	basicMuon.SetPtEtaPhiM(muPt[howManyMu[j]], 
			       muEta[howManyMu[j]], 
			       muPhi[howManyMu[j]],
			       muM[howManyMu[j]]);

	if( basicMuon.DeltaR(boostedJet) < 0.8 ){
	  isolatedStats = false;
	  break;

	}

      } // end of howManyMu for loop

      if( isolatedStats ) 
	maxJetIndex.push_back(sortJetIndex);
	
    } // end of CA8jetPassID if loop

  } // end of CA8nJet for loop
  

  //-----------------------------------------------------------------------------------//
  // choose the highest jet pt
    
  if( maxJetIndex.size() < 1 ) return;

  TLorentzVector newBoostedJet;

  newBoostedJet.SetPtEtaPhiM(CA8jetPt[maxJetIndex[0]],
			     CA8jetEta[maxJetIndex[0]],
			     CA8jetPhi[maxJetIndex[0]],
			     CA8jetMass[maxJetIndex[0]]);

    
  //-----------------------------------------------------------------------------------//
  // reconstruct Z prime mass

  if( Z.E() <= 1e-6 || newBoostedJet.E() <= 1e-6 ) return;

  // signal region
  //if( CA8jetPrunedMass[maxJetIndex[0]] <= 110 || CA8jetPrunedMass[maxJetIndex[0]] >= 140 )
  //  return;

  TLorentzVector Zprime = Z + newBoostedJet;

  *ZprimeMass = Zprime.M();
  *PrunedjetMass = CA8jetPrunedMass[maxJetIndex[0]];  
 
}
