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

void recoGenEleMatching(TreeReader &data, 
			Int_t *this_firstRecoIndex, Int_t *this_secondRecoIndex, 
			Int_t *this_firstGenIndex, Int_t *this_secondGenIndex){


  Float_t* muPt  = data.GetPtrFloat("muPt");
  Float_t* elePt  = data.GetPtrFloat("elePt");
  Float_t* eleEta = data.GetPtrFloat("eleEta");
  Float_t* elePhi = data.GetPtrFloat("elePhi");
  Float_t* eleM   = data.GetPtrFloat("eleM");

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


  // to pass the generator electrons

  Int_t    nGenPar     = data.GetInt("nGenPar"); 
  Int_t*   genParId    = data.GetPtrInt("genParId");
  Int_t*   genMomParId = data.GetPtrInt("genMomParId");
  Int_t*   genParSt    = data.GetPtrInt("genParSt");
  Float_t* genParPt    = data.GetPtrFloat("genParPt");
  Float_t* genParEta   = data.GetPtrFloat("genParEta");
  Float_t* genParPhi   = data.GetPtrFloat("genParPhi");
  Float_t* genParM     = data.GetPtrFloat("genParM");
      
  Int_t gen_ElectronID = -1;
  Int_t gen_antiElectronID = -1;

  for(Int_t i = 0; i < nGenPar; i++){

    if( genMomParId[i] == 23 && genParSt[i] == 1 && genParId[i] == 11 ) gen_ElectronID = i;
    if( genMomParId[i] == 23 && genParSt[i] == 1 && genParId[i] == -11 ) gen_antiElectronID = i; 
    if( gen_ElectronID > -1 && gen_antiElectronID > -1 ) break;
    
  }

  if( gen_ElectronID < 0 || gen_antiElectronID < 0 ) return;

  TLorentzVector gen_Electron, gen_antiElectron;

  gen_Electron.SetPtEtaPhiM(genParPt[gen_ElectronID], genParEta[gen_ElectronID], genParPhi[gen_ElectronID], genParM[gen_ElectronID]);
  gen_antiElectron.SetPtEtaPhiM(genParPt[gen_antiElectronID], genParEta[gen_antiElectronID], genParPhi[gen_antiElectronID], genParM[gen_antiElectronID]);


  //-----------------------------------------------------------------------------------//


  // to check if reconstructed electron match generator electron
    
  TLorentzVector reco_stElectron, reco_ndElectron;

  reco_stElectron.SetPtEtaPhiM(elePt[stElePtIndex], eleEta[stElePtIndex], elePhi[stElePtIndex], eleM[stElePtIndex]);
  reco_ndElectron.SetPtEtaPhiM(elePt[ndElePtIndex], eleEta[ndElePtIndex], elePhi[ndElePtIndex], eleM[ndElePtIndex]);

  Double_t recostElectron_with_genElectronDeltaR = reco_stElectron.DeltaR(gen_Electron);
  Double_t recostElectron_with_genantiElectronDeltaR = reco_stElectron.DeltaR(gen_antiElectron);
  Double_t recondElectron_with_genElectronDeltaR = reco_ndElectron.DeltaR(gen_Electron);
  Double_t recondElectron_with_genantiElectronDeltaR = reco_ndElectron.DeltaR(gen_antiElectron);

  *this_firstRecoIndex = -1;
  *this_secondRecoIndex = -1;
  *this_firstGenIndex = -1;
  *this_secondGenIndex = -1;

  if( recostElectron_with_genElectronDeltaR < 0.3 || recostElectron_with_genantiElectronDeltaR < 0.3 ){
    *this_firstRecoIndex = stElePtIndex;
    *this_firstGenIndex = gen_ElectronID;
  }
  
  if( recondElectron_with_genElectronDeltaR < 0.3 || recondElectron_with_genantiElectronDeltaR < 0.3 ){
    *this_secondRecoIndex = ndElePtIndex;
    *this_secondGenIndex = gen_antiElectronID;
  }

  if( *this_firstRecoIndex < 0 || *this_secondRecoIndex < 0 || 
      *this_firstGenIndex < 0  || *this_secondGenIndex < 0  ) return;
 

}
