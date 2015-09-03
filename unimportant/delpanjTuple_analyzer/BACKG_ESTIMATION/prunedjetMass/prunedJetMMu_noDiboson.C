#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include "../../HEADER/untuplizer.h"
#include "../../HEADER/passMuonID.C"
#include "../../HEADER/passJetID.h"
#include "../../HEADER/loadingbar.C"

void prunedJetMMu_noDiboson(std::string inputFile){

  TreeReader data(inputFile.data());

  inputFile = inputFile.erase(inputFile.find(".root"));
  inputFile = inputFile.substr(inputFile.find("v4")+3);

  Int_t csvlMode = 2;
  Int_t scaleMode = 0;

  // Declare the histogram
  TH1D* h_prunedJetMass = new TH1D("h_prunedJetMass", "Pruned Jet Mass", 100, 40, 240);
  h_prunedJetMass->GetXaxis()->SetTitle("Pruned jet mass");
  h_prunedJetMass->GetYaxis()->SetTitle("Event number");

  std::string textFile;
  
  if( inputFile.find("data") != std::string::npos )
    textFile = "../../HEADER/FT_53_V21_AN4_Uncertainty_AK7PFchs.txt";
  else
    textFile = "../../HEADER/START53_V23_Uncertainty_AK7PFchs.txt";

  corrJetV corrJet(textFile);

  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    data.GetEntry(ev);
    
    printProgBar(ev/(Double_t)data.GetEntriesFast()*100);

    Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
    Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
    Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
    Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");
    Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* elePt = data.GetPtrFloat("elePt");

    // data trigger cut (muon)
    if ( inputFile.find("DoubleMu") != std::string::npos ){
      std::string* trigName = data.GetPtrString("hlt_trigName");
      Int_t* trigResult = data.GetPtrInt("hlt_trigResult");
      const Int_t nsize = data.GetPtrStringSize();
      Bool_t passTrigger = false;
      for(int it = 0; it < nsize; it++){
	std::string thisTrig = trigName[it];
	Int_t results = trigResult[it];
	if( thisTrig.find("HLT_Mu22_TkMu8") != std::string::npos && results == 1 ){
	  passTrigger = true;
	  break;
	}
      }
      if( !passTrigger ) continue;
    }

    // pass muon ID
    Int_t stRecoMuIndex, ndRecoMuIndex;
    
    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
      continue;

    // pass boosted-jet ID
    Int_t maxJetIndex = -1;
    TLorentzVector tempVector(0,0,0,0);

    if( !passJetID(data, corrJet, csvlMode, scaleMode, &maxJetIndex, &tempVector) )
      continue;

    h_prunedJetMass->Fill(CA8jetPrunedMass[maxJetIndex]);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");
  
  TFile* outFile = new TFile("prunedJetMMu_noDiboson.root", "update");
  h_prunedJetMass->Write(("prunedJetMMu_"+inputFile).data());
  outFile->Write();

}
