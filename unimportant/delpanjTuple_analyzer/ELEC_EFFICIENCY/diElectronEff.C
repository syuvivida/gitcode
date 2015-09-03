#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "../HEADER/passElectronID.C"

Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);

void diElectronEff(std::string inputFile, std::string massName){

  TreeReader data(inputFile.data());

  Int_t genEventCounter = 0;
  Int_t recoEventCounter = 0;
  Int_t recoIsoEventCounter = 0;

  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Int_t  nGenPar  = data.GetInt("nGenPar"); 
    Int_t* genParId = data.GetPtrInt("genParId");
    Int_t* genParSt = data.GetPtrInt("genParSt");

    Bool_t isGenElectron = false;

    for(Int_t i = 0; i < nGenPar; i++){

      if( genParSt[i] == 3 && abs(genParId[i]) == 11 ){

	isGenElectron = true;
	break;

      }
    
    }

    if(!isGenElectron) continue;
    genEventCounter++;

    Float_t  eleRho   = data.GetFloat("eleRho");
    Float_t* eleEt    = data.GetPtrFloat("eleEt");
    Float_t* eleScEta = data.GetPtrFloat("eleScEta");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");

    // when using passElectronID at here, please make sure that 
    // you comment the isolation cut in the code

    Int_t stRecoEleIndex = -1; 
    Int_t ndRecoEleIndex = -1;

    if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) ) continue;
    recoEventCounter++;

    if( stRecoEleIndex < 0 || ndRecoEleIndex < 0 ) continue;

    Bool_t isRecoIsoElectron[2] = {false, false};

    Int_t eleIndex[2] = {stRecoEleIndex, ndRecoEleIndex};

    if( eleUserTrkIso[stRecoEleIndex] < 5 && eleUserTrkIso[ndRecoEleIndex] < 5 ){

      for(Int_t ie = 0; ie < 2; ie++){

	Double_t isoCutValue = 2+(0.03*eleEt[eleIndex[ie]]);

	if( fabs(eleScEta[eleIndex[ie]]) > 0 && fabs(eleScEta[eleIndex[ie]]) < 1.4442 ){

	  if( (eleUserCalIso[eleIndex[ie]]-(0.06205*eleRho)) < isoCutValue ) 

	    isRecoIsoElectron[ie] = true;

	} else if( fabs(eleScEta[eleIndex[ie]]) > 1.566 && fabs(eleScEta[eleIndex[ie]]) < 2.5 ){

	  if( eleEt[eleIndex[ie]] < 50 ){

	    if( (eleUserCalIso[eleIndex[ie]]-(0.06205*eleRho)) < 2.5 ) 

	      isRecoIsoElectron[ie] = true;

	  } else if( eleEt[eleIndex[ie]] > 50 ){

	    if( (eleUserCalIso[eleIndex[ie]]-(0.06205*eleRho)) < (isoCutValue+0.5) ) 

	      isRecoIsoElectron[ie] = true;

	  }

	}
	
      }

    } // end of pass TrkIso

    if(!isRecoIsoElectron[0] || !isRecoIsoElectron[1]) continue;
    recoIsoEventCounter++;

   
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  Double_t eventEff = (Double_t)recoEventCounter/genEventCounter;
  Double_t eventEffError = TMath::Sqrt((1-eventEff)*eventEff/genEventCounter);

  Double_t eventIsoEff = (Double_t)recoIsoEventCounter/genEventCounter;
  Double_t eventIsoEffError = TMath::Sqrt((1-eventIsoEff)*eventIsoEff/genEventCounter);

  std::ofstream fout;
  fout.open("massEleptEff.txt", ios::app);
  fout << massName.data() << "\t" 
       << eventEff << "\t" 
       << eventEffError << "\t" 
       << eventIsoEff << "\t" 
       << eventIsoEffError << endl;

  fout.close();
  
}
