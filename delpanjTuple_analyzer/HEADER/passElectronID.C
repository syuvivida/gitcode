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

void passElectronID(TreeReader &data, 
		    Int_t *stRecoEleIndex, Int_t *ndRecoEleIndex){


  Int_t    nEle   = data.GetInt("nEle"); 
  Int_t*   elePassID = data.GetPtrInt("elePassID");
  Float_t* elePt  = data.GetPtrFloat("elePt");

  // sorting eleon and pass the eleon ID

  vector<Int_t> tightEleIndex;

  typedef map<double, int, std::greater<double> > eleMap;
  eleMap sortElePt;
  typedef eleMap::iterator mapEleIter;

  for(Int_t i = 0; i < nEle; i++){

    sortElePt.insert(std::pair<Float_t, Int_t>(elePt[i], i));

  }

  for(mapEleIter it_part = sortElePt.begin(); it_part != sortElePt.end(); ++it_part){

    // at least two electrons
    // pt of these electrons must greater than 40

    Int_t sortEleIndex = it_part->second;

    if( elePassID[sortEleIndex] <= 0 ) continue;
    if( elePt[sortEleIndex] <= 40 ) continue; 

    tightEleIndex.push_back(sortEleIndex);

  }

  if( tightEleIndex.size() < 2 ) return;
    

  //-----------------------------------------------------------------------------------//


  // filling index 

  *stRecoEleIndex = -1;
  *ndRecoEleIndex = -1;

  if( tightEleIndex.size() > 0 ){

    *stRecoEleIndex = tightEleIndex[0];
    *ndRecoEleIndex = tightEleIndex[1];

  }   

  if( *stRecoEleIndex < 0 || *ndRecoEleIndex < 0 ) return;

}
