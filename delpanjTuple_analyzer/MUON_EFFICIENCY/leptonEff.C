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
#include "../HEADER/passMuonID.C"

Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);

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

    Bool_t isGenMuon = false;

    for(Int_t i = 0; i < nGenPar; i++){

      if( genParSt[i] == 3 && abs(genParId[i]) == 13 ){

	isGenMuon = true;
	break;

      }
    
    }

    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

    // when using passMuonID at here, please make sure that 
    // you comment the isolation cut in the code

    Bool_t isRecoMuon = false;
    Int_t stRecoMuIndex, ndRecoMuIndex;

    if( passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) ) isRecoMuon = true;

    Bool_t isRecoIsoMuon = false;
    Int_t stRecoMuIsoIndex, ndRecoMuIsoIndex;
    
    if( passMuonID(data, &stRecoMuIsoIndex, &ndRecoMuIsoIndex) ){

      Double_t stReIso = muCorrTrkIso[stRecoMuIsoIndex] / muPt[stRecoMuIsoIndex];
      Double_t ndReIso = muCorrTrkIso[ndRecoMuIsoIndex] / muPt[ndRecoMuIsoIndex];

      if( stReIso < 0.1 && ndReIso < 0.1 ) isRecoIsoMuon = true;

    }
    
    if(isGenMuon) genEventCounter++;
    if(isRecoMuon) recoEventCounter++;
    if(isRecoIsoMuon) recoIsoEventCounter++;

   
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  Double_t eventEff = (Double_t)recoEventCounter/genEventCounter;
  Double_t eventIsoEff = (Double_t)recoIsoEventCounter/genEventCounter;

  std::ofstream fout;
  fout.open("massptEff.txt", ios::app);
  fout << massName.data() << "\t" << eventEff << "\t" << eventIsoEff << endl;
  fout.close();
  
}
