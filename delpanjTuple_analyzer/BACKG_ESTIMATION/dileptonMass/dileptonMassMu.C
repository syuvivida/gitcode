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
#include "../../HEADER/passElectronID.C"

Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);
Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);

void dileptonMassMu(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  TH1D* h_ZMass = new TH1D("h_ZMass", "Z mass by dimuon", 100, 70, 110);
  h_ZMass->GetXaxis()->SetTitle("Mass");
  h_ZMass->GetYaxis()->SetTitle("Event number");
 
  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Int_t    nMu   = data.GetInt("nMu");
    Int_t*   muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Int_t    nEle  = data.GetInt("nEle");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    
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
   
    // choose muon event and pass muon ID
    Int_t stMuPtIndex, ndMuPtIndex;
    Int_t stElePtIndex, ndElePtIndex;

    passMuonID(data, &stMuPtIndex, &ndMuPtIndex);
    passElectronID(data, &stElePtIndex, &ndElePtIndex);

    Bool_t muonEvent = false;
    
    if( nMu > 0 && nEle > 0 ){

      if( muPt[stMuPtIndex] > elePt[stElePtIndex] ) 
	muonEvent = true;

      else if( elePt[stElePtIndex] > muPt[stMuPtIndex] )
	muonEvent = false;

    }
    
    else if( nEle == 0 && nMu > 0 ) 
      muonEvent = true;

    else if( nMu == 0 && nEle > 0 )
      muonEvent = false;
    
    if( !muonEvent ) continue;
    
    Int_t stRecoMuIndex, ndRecoMuIndex;
    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
      continue;
    
    // reconstruct Z mass
    TLorentzVector stRecoMu(0,0,0,0), ndRecoMu(0,0,0,0), Z(0,0,0,0);  
 
    stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], 
			  muEta[stRecoMuIndex], 
			  muPhi[stRecoMuIndex],
			  muM[stRecoMuIndex]);  
  
    ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], 
			  muEta[ndRecoMuIndex],
			  muPhi[ndRecoMuIndex], 
			  muM[ndRecoMuIndex]); 

    Z = stRecoMu + ndRecoMu;

    if( Z.E() <= 1e-6) continue;
    if( Z.M() <= 70 ) continue;
    if( Z.M() >= 110 ) continue;
    if( Z.Pt() <= 80) continue;

    h_ZMass->Fill(Z.M());
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  // output file
  std::string ZMassName = "ZMass_" + outName.substr(11);

  TFile* outFile = new TFile("dileptonMassMu.root", "update");

  h_ZMass->Write(ZMassName.data());

  outFile->Write();
  
}
