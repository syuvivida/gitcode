#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TStyle.h>
#include <TChain.h>
#include <TSystem.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "../../HEADER/untuplizer.h"
#include "../../HEADER/passElectronID.C"
#include "../../HEADER/passJetID.C"

Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);
Bool_t passJetID(TreeReader&, Int_t&, Int_t*);

void prunedJetMEle(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // Declare the histogram
  TH1D* h_prunedJetMass = new TH1D("h_prunedJetMass", "Pruned Jet Mass", 100, 40, 240);
  h_prunedJetMass->GetXaxis()->SetTitle("Pruned jet mass");
  h_prunedJetMass->GetYaxis()->SetTitle("Event number");

  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
    Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
    Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
    Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");
    Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleM   = data.GetPtrFloat("eleM");
    Float_t* muPt   = data.GetPtrFloat("muPt");

    // data trigger cut
    if ( outName.find("DoubleEl") != std::string::npos ){

      std::string* trigName = data.GetPtrString("hlt_trigName");
      Int_t* trigResult = data.GetPtrInt("hlt_trigResult");
      const Int_t nsize = data.GetPtrStringSize();

      Bool_t passTrigger = false;

      for(int it = 0; it < nsize; it++){

	std::string thisTrig = trigName[it];
	Int_t results = trigResult[it];

	// electron channel
	if( thisTrig.find("HLT_DoubleEle33") != std::string::npos && results == 1 ){

	  passTrigger = true;
	  break;

	}

      }
   
      if( !passTrigger ) continue;

    }

    // pass electron ID
    Int_t stRecoEleIndex, ndRecoEleIndex;
    if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) )
      continue;

    // pass boosted-jet ID
    Int_t mode = 2; // 1:b-tagging only, 2:tau21 only
    Int_t maxJetIndex;

    if( !passJetID(data, mode, &maxJetIndex) )
      continue;

    if( CA8jetPrunedMass[maxJetIndex] > 40 && CA8jetPrunedMass[maxJetIndex] < 240 )
      h_prunedJetMass->Fill(CA8jetPrunedMass[maxJetIndex]);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  // output file
  TFile* outFile = new TFile("prunedJetMEl.root", "update");
  std::string sideName = "prunedJetM_" + outName.substr(11);
  h_prunedJetMass->Write(sideName.data());
  outFile->Write();
  
}
