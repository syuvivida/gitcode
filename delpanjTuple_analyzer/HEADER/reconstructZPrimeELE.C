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
#include "passElectronID.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);
Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);

void reconstructZPrime(TreeReader &data, Double_t *ZprimeMass){

  Int_t    CA8nJet    = data.GetInt("CA8nJet"); 
  Int_t*   CA8jetPassID = data.GetPtrInt("CA8jetPassID");
  Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
  Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
  Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
  Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");

  Int_t    nEle   = data.GetInt("nEle");
  Int_t*   elePassID = data.GetPtrInt("elePassID");
  Float_t* elePt  = data.GetPtrFloat("elePt");
  Float_t* eleEta = data.GetPtrFloat("eleEta");
  Float_t* elePhi = data.GetPtrFloat("elePhi");
  Float_t* eleM   = data.GetPtrFloat("eleM");

  Float_t* muPt  = data.GetPtrFloat("muPt");


  //-----------------------------------------------------------------------------------//
  // choose the primary electron

  Int_t stMuPtIndex  = -1;
  Int_t ndMuPtIndex  = -1;
  Int_t stElePtIndex = -1;
  Int_t ndElePtIndex = -1;

  specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		   &stElePtIndex, &ndElePtIndex);

  if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
      (stElePtIndex < 0 || ndElePtIndex < 0 )  ) return; 
  
  if( stMuPtIndex > 0 && stElePtIndex > 0 ){
    
    if( (elePt[stElePtIndex] - muPt[stMuPtIndex]) < 1e-6 ) 
      return;

  }
    
  //-----------------------------------------------------------------------------------//
  // sorting electron and pass the electron ID

  vector<Int_t> howManyEle;

  typedef map<Float_t, Int_t, std::greater<Float_t> > eleMap;
  eleMap sortElePt;
  typedef eleMap::iterator mapEleIter;

  for(Int_t i = 0; i < nEle; i++){

    sortElePt.insert(std::pair<Float_t, Int_t>(elePt[i], i));

  }

  for(mapEleIter it_part = sortElePt.begin(); it_part != sortElePt.end(); ++it_part){

    Int_t sortEleIndex = it_part->second;

    if( elePassID[sortEleIndex] <= 0 ) continue;
    if( elePt[sortEleIndex] <= 40 ) continue; 

    howManyEle.push_back(sortEleIndex);

  }

  Int_t stRecoEleIndex, ndRecoEleIndex;

  if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) )
    return;


  //-----------------------------------------------------------------------------------//   
  // reconstruct Z mass

  TLorentzVector stRecoEle, ndRecoEle;  
 
  stRecoEle.SetPtEtaPhiM(elePt[stRecoEleIndex], 
			eleEta[stRecoEleIndex], 
			elePhi[stRecoEleIndex],
			eleM[stRecoEleIndex]);  
  
  ndRecoEle.SetPtEtaPhiM(elePt[ndRecoEleIndex], 
			eleEta[ndRecoEleIndex],
			elePhi[ndRecoEleIndex], 
			eleM[ndRecoEleIndex]); 
    
  TLorentzVector Z = stRecoEle + ndRecoEle;


  //-----------------------------------------------------------------------------------//
  // pass boosted-jet ID, removing overlap electrons

  typedef map<double, int, std::greater<double> > jetMap;
  jetMap sortJetPt;
  typedef jetMap::iterator mapJetIter;

  for(Int_t i = 0; i < CA8nJet; i++){

    sortJetPt.insert(std::pair<Float_t, Int_t>(CA8jetPt[i], i));

  }

  vector<Int_t> maxJetIndex;
  TLorentzVector boostedJet, basicElectron;

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

      for(size_t j = 0; j < howManyEle.size(); j++){

	basicElectron.SetPtEtaPhiM(elePt[howManyEle[j]], 
			       eleEta[howManyEle[j]], 
			       elePhi[howManyEle[j]],
			       eleM[howManyEle[j]]);

	if( basicElectron.DeltaR(boostedJet) < 0.8 ){
	  isolatedStats = false;
	  break;

	}

      } // end of howManyEle for loop

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

  TLorentzVector Zprime = Z + newBoostedJet;

  *ZprimeMass = Zprime.M();  

  
}
