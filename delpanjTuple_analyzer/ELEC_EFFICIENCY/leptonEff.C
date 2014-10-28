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

void leptonEff(std::string inputFile, std::string massName){

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

    Float_t  eleRho = data.GetFloat("eleRho");
    Float_t* eleEt  = data.GetPtrFloat("eleEt");
    Float_t* eleScEta = data.GetPtrFloat("eleScEta");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");

    // when using passElectronID at here, please make sure that 
    // you comment the isolation cut in the code

    Bool_t isRecoElectron = false;
    Int_t stRecoEleIndex, ndRecoEleIndex;

    if( passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) ) isRecoElectron = true;

    Bool_t isRecoIsoLedElectron = false;
    Bool_t isRecoIsoSubElectron = false;
    Int_t stRecoEleIsoIndex, ndRecoEleIsoIndex;
    
    if( passElectronID(data, &stRecoEleIsoIndex, &ndRecoEleIsoIndex) ){
      if( eleUserTrkIso[stRecoEleIsoIndex] < 5 && eleUserTrkIso[ndRecoEleIsoIndex] < 5 ){


	Double_t isoLedCutValue = 2+(0.03*eleEt[stRecoEleIsoIndex]);

	if( fabs(eleScEta[stRecoEleIsoIndex]) > 0 && fabs(eleScEta[stRecoEleIsoIndex]) < 1.4442 ){
	  if( (eleUserCalIso[stRecoEleIsoIndex]-(0.06205*eleRho)) < isoLedCutValue ) 
	    isRecoIsoLedElectron = true;
	} // end of barrel(st)

	else if( fabs(eleScEta[stRecoEleIsoIndex]) > 1.566 && fabs(eleScEta[stRecoEleIsoIndex]) < 2.5 ){
	  if( eleEt[stRecoEleIsoIndex] < 50 ){
	    if( (eleUserCalIso[stRecoEleIsoIndex]-(0.06205*eleRho)) < 2.5 ) 
	      isRecoIsoLedElectron = true;
	  }
	  else if( eleEt[stRecoEleIsoIndex] > 50 ){
	    if( (eleUserCalIso[stRecoEleIsoIndex]-(0.06205*eleRho)) < (isoLedCutValue+0.5) ) 
	      isRecoIsoLedElectron = true;
	  }
	} // end of endcap(st)
	

	Double_t isoSubCutValue = 2+(0.03*eleEt[ndRecoEleIsoIndex]);

	if( fabs(eleScEta[ndRecoEleIsoIndex]) > 0 && fabs(eleScEta[ndRecoEleIsoIndex]) < 1.4442 ){
	  if( (eleUserCalIso[ndRecoEleIsoIndex]-(0.06205*eleRho)) < isoSubCutValue ) 
	    isRecoIsoSubElectron = true;
	} // end of barrel(nd)

	else if( fabs(eleScEta[ndRecoEleIsoIndex]) > 1.566 && fabs(eleScEta[ndRecoEleIsoIndex]) < 2.5 ){
	  if( eleEt[ndRecoEleIsoIndex] < 50 ){
	    if( (eleUserCalIso[ndRecoEleIsoIndex]-(0.06205*eleRho)) < 2.5 ) 
	      isRecoIsoSubElectron = true;
	  }
	  else if( eleEt[ndRecoEleIsoIndex] > 50 ){
	    if( (eleUserCalIso[ndRecoEleIsoIndex]-(0.06205*eleRho)) < (isoSubCutValue+0.5) ) 
	      isRecoIsoSubElectron = true;
	  }
	} // end of endcap(nd)


      } // end of pass TrkIso
    } // end of pass electronID
    
    if(isGenElectron) genEventCounter++;
    if(isRecoElectron) recoEventCounter++;
    if(isRecoIsoLedElectron && isRecoIsoSubElectron) recoIsoEventCounter++;

   
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
