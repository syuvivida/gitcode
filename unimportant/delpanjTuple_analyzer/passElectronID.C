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

struct eleMap{
  Int_t index;
  Float_t pt;
};

Bool_t elePtGreater(eleMap i, eleMap j){ 
  return (i.pt>j.pt); 
}

Bool_t passElectronID(TreeReader &data, 
		      Int_t *stRecoEleIndex, Int_t *ndRecoEleIndex){


  Int_t    nEle   = data.GetInt("nEle"); 
  Int_t*   elePassID = data.GetPtrInt("elePassID");
  Float_t  eleRho = data.GetFloat("eleRho");
  Float_t* elePt  = data.GetPtrFloat("eleEt");
  Float_t* eleScEta = data.GetPtrFloat("eleScEta");
  Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
  Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");

  vector<Int_t> tightEleIndex;

  // sorting electron and pass the electron ID

  vector<eleMap> sortElePt;
  for(Int_t i = 0; i < nEle; i++){

    eleMap temp;
    temp.index = i;
    temp.pt = elePt[i];
    sortElePt.push_back(temp);

  }
  std::sort(sortElePt.begin(),sortElePt.end(),elePtGreater);

  Int_t nSortEle = sortElePt.size();

  for(Int_t i = 0; i < nSortEle; i++){

    // at least two electrons
    // pt of these electrons must greater than 40

    Int_t eleIndex = sortElePt[i].index;

    Double_t isoCutValue = 2+(0.03*elePt[eleIndex]);

    if( elePassID[eleIndex] <= 0 ) continue;
    if( elePt[eleIndex] <= 40 ) continue;
    if( eleUserTrkIso[eleIndex] >= 5 ) continue;
    
    // barrel selection
    if( fabs(eleScEta[eleIndex]) > 0 && fabs(eleScEta[eleIndex]) < 1.4442 ){

      if( (eleUserCalIso[eleIndex]-(0.06205*eleRho)) >= isoCutValue ) 
	continue;

    }

    // endcap selection
    if( fabs(eleScEta[eleIndex]) > 1.566 && fabs(eleScEta[eleIndex]) < 2.5 ){

      if( elePt[eleIndex] < 50 ){
	if( (eleUserCalIso[eleIndex]-(0.06205*eleRho)) >= 2.5 ) 
	  continue;
      }

      if( elePt[eleIndex] > 50 ){
	if( (eleUserCalIso[eleIndex]-(0.06205*eleRho)) >= (isoCutValue+0.5) ) 
	  continue;
      }
    
    }
    
    tightEleIndex.push_back(eleIndex);

  }

  if( tightEleIndex.size() < 2 ) return false;


  // filling index 

  *stRecoEleIndex = -1;
  *ndRecoEleIndex = -1;

  if( tightEleIndex.size() > 0 ){

    *stRecoEleIndex = tightEleIndex[0];
    *ndRecoEleIndex = tightEleIndex[1];

  }   

  if( *stRecoEleIndex < 0 || *ndRecoEleIndex < 0 ) return false;

  return true;

}
