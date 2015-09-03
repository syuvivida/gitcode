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

void diMuonEff(std::string inputFile, std::string massName){

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

    if(!isGenMuon) continue;
    genEventCounter++;

    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

    // when using passMuonID at here, please make sure that 
    // you comment the isolation cut in the code

    Bool_t isRecoMuon = false;
    Int_t stRecoMuIndex = -1; 
    Int_t ndRecoMuIndex = -1;

    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) ) continue;
    recoEventCounter++;

    if( stRecoMuIndex < 0 || ndRecoMuIndex < 0 ) continue;

    Bool_t isRecoIsoMuon = false;
   
    Double_t stReIso = muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex];
    Double_t ndReIso = muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex];
    
    if( stReIso < 0.1 && ndReIso < 0.1 ) isRecoIsoMuon = true;
    
    if(!isRecoIsoMuon) continue;
    recoIsoEventCounter++;

   
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  Double_t eventEff = (Double_t)recoEventCounter/genEventCounter;
  Double_t eventEffError = TMath::Sqrt((1-eventEff)*eventEff/genEventCounter);

  Double_t eventIsoEff = (Double_t)recoIsoEventCounter/genEventCounter;
  Double_t eventIsoEffError = TMath::Sqrt((1-eventIsoEff)*eventIsoEff/genEventCounter);

  std::ofstream fout;
  fout.open("massMuptEff.txt", ios::app);
  fout << massName.data() << "\t" 
       << eventEff << "\t" 
       << eventEffError << "\t" 
       << eventIsoEff << "\t" 
       << eventIsoEffError << endl;

  fout.close();
  
}
