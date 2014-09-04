#include <vector>
#include <string>
#include <iostream>
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
#include "../HEADER/untuplizer.h"
#include "../HEADER/specificLeptonPt.C"
#include "../HEADER/passMuonID.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);
Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);

void muVariable(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // Declare the histogram

  TH1D* h_corrTrkIso = new TH1D("h_corrTrkIso", "muCorrTrkIso", 50, 0, 0.1);
  TH1D* h_nVtx = new TH1D("h_nVtx", "nVtx", 50, 0, 50);

  h_corrTrkIso->GetXaxis()->SetTitle("corrTrkIso");
  h_corrTrkIso->GetYaxis()->SetTitle("Event number");
  h_nVtx->GetXaxis()->SetTitle("nVtx");
  h_nVtx->GetYaxis()->SetTitle("Event number");


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Int_t info_nVtx = data.GetInt("info_nVtx");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");
    Float_t* elePt = data.GetPtrFloat("elePt");


    //-----------------------------------------------------------------------------------//
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

    //-----------------------------------------------------------------------------------//
    // choose the primary muon

    Int_t stMuPtIndex  = -1;
    Int_t ndMuPtIndex  = -1;
    Int_t stElePtIndex = -1;
    Int_t ndElePtIndex = -1;

    specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		     &stElePtIndex, &ndElePtIndex);

    if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
	(stElePtIndex < 0 || ndElePtIndex < 0 )  ) continue; 
  
    if( stMuPtIndex > -1 && stElePtIndex > -1 ){
    
      if( (muPt[stMuPtIndex] - elePt[stElePtIndex]) < 1e-6 ) 
	continue;

    }

    Int_t stRecoMuIndex, ndRecoMuIndex;

    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) ) continue;


    //-----------------------------------------------------------------------------------//   
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

    if(Z.Pt() <= 80) continue;


    h_corrTrkIso->Fill(muCorrTrkIso[stRecoMuIndex]/muPt[stRecoMuIndex]);
    h_corrTrkIso->Fill(muCorrTrkIso[ndRecoMuIndex]/muPt[ndRecoMuIndex]);

    h_nVtx->Fill(info_nVtx);


  } 

  // end of event loop

  fprintf(stderr, "Processed all events\n");
  
  std::string histoName1 = "corrTrkIso_" + outName.substr(11);
  std::string histoName2 = "nVtx_" + outName.substr(11); 

  TFile* outFile = new TFile("muVariable.root", "update");
  
  h_corrTrkIso->Write(histoName1.data());
  h_nVtx->Write(histoName2.data());

  outFile->Write();
  
}
