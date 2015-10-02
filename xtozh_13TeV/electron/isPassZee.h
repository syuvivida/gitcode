#include <vector>
#include <string>
#include <iostream>
#include <TMath.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include "untuplizer.h"

bool isPassZee(TreeReader &data, vector<Int_t>* ZeeId){
  
  Int_t    nEle       = data.GetInt("nEle");
  Int_t*   eleCharge  = data.GetPtrInt("eleCharge");
  Float_t* eleScEt    = data.GetPtrFloat("eleScEt");
  Float_t* eleScEta   = data.GetPtrFloat("eleScEta");
  Float_t* eleMiniIso = data.GetPtrFloat("eleMiniIso");
  TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");
  vector<bool>& eleEcalDrivenSeed  = *((vector<bool>*) data.GetPtr("eleEcalDrivenSeed"));
  vector<bool>& eleIsPassHEEPNoIso = *((vector<bool>*) data.GetPtr("eleIsPassHEEPNoIso"));

  // select good electrons
        
  std::vector<Int_t> goodElectrons;

  for(Int_t ie = 0; ie < nEle; ie++){

    if( !(fabs(eleScEta[ie]) < 1.442 || fabs(eleScEta[ie]) > 1.566) ) continue;
    if( fabs(eleScEta[ie]) > 2.5 ) continue;
    if( eleScEt[ie] <= 35 ) continue;
    if( !eleEcalDrivenSeed[ie] ) continue;
    if( !eleIsPassHEEPNoIso[ie] ) continue;
    if( eleMiniIso[ie] >= 0.1 ) continue;

    goodElectrons.push_back(ie);

  } // end of ele loop

  // select good Z boson

  bool findEPair = false;
  TLorentzVector* thisEle = NULL;
  TLorentzVector* thatEle = NULL;

  for(unsigned int i = 0; i < goodElectrons.size(); i++){

    Int_t ie = goodElectrons[i];
    thisEle = (TLorentzVector*)eleP4->At(ie);

    for(unsigned int j = 0; j < i; j++){

      Int_t je = goodElectrons[j];
      thatEle = (TLorentzVector*)eleP4->At(je);

      Float_t pt1 = thisEle->Pt();
      Float_t pt2 = thatEle->Pt();
      Float_t mll = (*thisEle+*thatEle).M();

      if( eleCharge[ie]*eleCharge[je] > 0 ) continue;   
      if( mll < 60 || mll > 120 ) continue;
      if( !findEPair ) ZeeId->push_back( (pt1 > pt2) ? ie : je );

      findEPair = true;
      break;

    }
  }

  if( !findEPair ) return false;
  return true;

}
