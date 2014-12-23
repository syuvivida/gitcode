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
#include "../../HEADER/passMuonID.C"
#include "../../HEADER/passJetID.C"

Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);
Bool_t passJetID(TreeReader&, Int_t&, Int_t*);

void sideSigZpMMu(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // variable bin
  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  TH1D* h_ZpMass[2]; // [side,signal]
  std::string htitle[2] = {"Side-band region Zprime Mass","Signal region Zprime Mass"};

  for(Int_t i = 0; i < 2; i++){

    h_ZpMass[i] = new TH1D(Form("h_sideZprimeMass%d",i), "", nvarBins, varBins);
    h_ZpMass[i]->SetTitle(htitle[i].data());
    h_ZpMass[i]->GetXaxis()->SetTitle("Zprime mass");
    h_ZpMass[i]->GetYaxis()->SetTitle("Event number");

  }

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
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* elePt = data.GetPtrFloat("elePt");

    // data trigger cut
    if ( outName.find("DoubleMu") != std::string::npos ){

      std::string* trigName = data.GetPtrString("hlt_trigName");
      Int_t* trigResult = data.GetPtrInt("hlt_trigResult");
      const Int_t nsize = data.GetPtrStringSize();

      Bool_t passTrigger = false;

      for(int it = 0; it < nsize; it++){

	std::string thisTrig = trigName[it];
	Int_t results = trigResult[it];

	// muon channel
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
    
    // reconstruct Z mass
    TLorentzVector stRecoMu, ndRecoMu;  
 
    stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], 
			  muEta[stRecoMuIndex], 
			  muPhi[stRecoMuIndex],
			  muM[stRecoMuIndex]);  
  
    ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], 
			  muEta[ndRecoMuIndex],
			  muPhi[ndRecoMuIndex], 
			  muM[ndRecoMuIndex]); 
    
    TLorentzVector Z = stRecoMu + ndRecoMu;
    
    // pass boosted-jet ID
    Int_t mode = 2; // 1:b-tagging only, 2:tau21 only
    Int_t maxJetIndex;

    if( !passJetID(data, mode, &maxJetIndex) )
      continue;
 
    TLorentzVector Higgs;
    Higgs.SetPtEtaPhiM(CA8jetPt[maxJetIndex],
		       CA8jetEta[maxJetIndex],
		       CA8jetPhi[maxJetIndex],
		       CA8jetMass[maxJetIndex]);

    if( Z.E() <= 1e-6 || Higgs.E() <= 1e-6 ) continue;
    if( Z.M() <= 70 || Z.M() >=110 ) continue;
    if( Z.Pt() <= 80 ) continue;

    // side band region
    if( CA8jetPrunedMass[maxJetIndex] > 50 && CA8jetPrunedMass[maxJetIndex] < 110 ){

      TLorentzVector Zprime = Z + Higgs; 
      h_ZpMass[0]->Fill(Zprime.M());

    }
    
    // signal region
    if( CA8jetPrunedMass[maxJetIndex] > 110 && CA8jetPrunedMass[maxJetIndex] < 140 ){

      TLorentzVector Zprime = Z + Higgs; 
      h_ZpMass[1]->Fill(Zprime.M());
   
    }
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  // output file
  TFile* outFile = new TFile("sideSigZpMMu.root", "update");

  std::string writeName[2] = {"sideZpMass_"+outName.substr(11),
			      "signZpMass_"+outName.substr(11)};

  for(Int_t i = 0; i < 2; i++){
    h_ZpMass[i]->Write(writeName[i].data());
  }

  outFile->Write();
  
}
