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

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);

void muVariable(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // Declare the histogram (global muon)

  TH1D* hglobal_muHits      = new TH1D("hglobal_muHits", "global muHits", 100, 0, 10);
  TH1D* hglobal_muMatches   = new TH1D("hglobal_muMatches", "global muMatches", 100, 0, 5);
  TH1D* hglobal_muPixelHits = new TH1D("hglobal_muPixelHits", "global muPixelHits", 100, 0, 6);
  TH1D* hglobal_muTrkLayers = new TH1D("hglobal_muTrkLayers", "global muTrkLayers", 100, 0, 16);
  TH1D* hglobal_muPtErrx    = new TH1D("hglobal_muPtErrx", "global muPtErrx", 100, 0, 0.1);
  TH1D* hglobal_mudxy       = new TH1D("hglobal_mudxy", "global mudxy", 100, -0.1, 0.1);
  TH1D* hglobal_mudz        = new TH1D("hglobal_mudz", "global mudz", 100, -0.1, 0.1);
  TH1D* hglobal_muPt        = new TH1D("hglobal_muPt", "global muPt", 100, 40, 200);
  TH1D* hglobal_muEta       = new TH1D("hglobal_muEta", "global muEta", 100, -3, 3);
  TH1D* hglobal_muPhi       = new TH1D("hglobal_muPhi", "global muPhi", 100, -4, 4);
  TH1D* hglobal_muCorrTrkIso = new TH1D("hglobal_corrTrkIso", "global muCorrTrkIso", 100, 0, 0.1);
  TH1D* hglobal_muTrkIso    = new TH1D("hglobal_muTrkIso", "global muTrkIso", 100, 0, 5);

  hglobal_muHits->GetXaxis()->SetTitle("Hits"); 
  hglobal_muHits->GetYaxis()->SetTitle("Event number"); 
  hglobal_muMatches->GetXaxis()->SetTitle("Matches"); 
  hglobal_muMatches->GetYaxis()->SetTitle("Event number"); 
  hglobal_muPixelHits->GetXaxis()->SetTitle("PixelHits");
  hglobal_muPixelHits->GetYaxis()->SetTitle("Event number");
  hglobal_muTrkLayers->GetXaxis()->SetTitle("TrkLayers");
  hglobal_muTrkLayers->GetYaxis()->SetTitle("Event number");
  hglobal_muPtErrx->GetXaxis()->SetTitle("PtErrx");
  hglobal_muPtErrx->GetYaxis()->SetTitle("Event number");
  hglobal_mudxy->GetXaxis()->SetTitle("Dxy");
  hglobal_mudxy->GetYaxis()->SetTitle("Event number");  
  hglobal_mudz->GetXaxis()->SetTitle("Dz"); 
  hglobal_mudz->GetYaxis()->SetTitle("Event number");   
  hglobal_muPt->GetXaxis()->SetTitle("Pt");
  hglobal_muPt->GetYaxis()->SetTitle("Event number");    
  hglobal_muEta->GetXaxis()->SetTitle("Eta"); 
  hglobal_muEta->GetYaxis()->SetTitle("Event number");   
  hglobal_muPhi->GetXaxis()->SetTitle("Phi");   
  hglobal_muPhi->GetYaxis()->SetTitle("Event number"); 
  hglobal_muCorrTrkIso->GetXaxis()->SetTitle("CorrTrkIso");
  hglobal_muCorrTrkIso->GetYaxis()->SetTitle("Event number");
  hglobal_muTrkIso->GetXaxis()->SetTitle("TrkIso");
  hglobal_muTrkIso->GetYaxis()->SetTitle("Event number"); 

  // Declare the histogram (tracker muon)

  TH1D* htracker_muHits      = new TH1D("htracker_muHits", "tracker muHits", 100, 0, 10);
  TH1D* htracker_muMatches   = new TH1D("htracker_muMatches", "tracker muMatches", 100, 0, 5);
  TH1D* htracker_muPixelHits = new TH1D("htracker_muPixelHits", "tracker muPixelHits", 100, 0, 6);
  TH1D* htracker_muTrkLayers = new TH1D("htracker_muTrkLayers", "tracker muTrkLayers", 100, 0, 16);
  TH1D* htracker_muPtErrx    = new TH1D("htracker_muPtErrx", "tracker muPtErrx", 100, 0, 0.1);
  TH1D* htracker_mudxy       = new TH1D("htracker_mudxy", "tracker mudxy", 100, -0.1, 0.1);
  TH1D* htracker_mudz        = new TH1D("htracker_mudz", "tracker mudz", 100, -0.1, 0.1);
  TH1D* htracker_muPt        = new TH1D("htracker_muPt", "tracker muPt", 100, 40, 200);
  TH1D* htracker_muEta       = new TH1D("htracker_muEta", "tracker muEta", 100, -3, 3);
  TH1D* htracker_muPhi       = new TH1D("htracker_muPhi", "tracker muPhi", 100, -4, 4);
  TH1D* htracker_muCorrTrkIso = new TH1D("htracker_corrTrkIso", "tracker muCorrTrkIso", 100, 0, 0.1);
  TH1D* htracker_muTrkIso    = new TH1D("htracker_muTrkIso", "tracker muTrkIso", 100, 0, 5);

  htracker_muHits->GetXaxis()->SetTitle("Hits"); 
  htracker_muHits->GetYaxis()->SetTitle("Event number"); 
  htracker_muMatches->GetXaxis()->SetTitle("Matches"); 
  htracker_muMatches->GetYaxis()->SetTitle("Event number"); 
  htracker_muPixelHits->GetXaxis()->SetTitle("PixelHits");
  htracker_muPixelHits->GetYaxis()->SetTitle("Event number");
  htracker_muTrkLayers->GetXaxis()->SetTitle("TrkLayers");
  htracker_muTrkLayers->GetYaxis()->SetTitle("Event number");
  htracker_muPtErrx->GetXaxis()->SetTitle("PtErrx");
  htracker_muPtErrx->GetYaxis()->SetTitle("Event number");
  htracker_mudxy->GetXaxis()->SetTitle("Dxy");
  htracker_mudxy->GetYaxis()->SetTitle("Event number");  
  htracker_mudz->GetXaxis()->SetTitle("Dz"); 
  htracker_mudz->GetYaxis()->SetTitle("Event number");   
  htracker_muPt->GetXaxis()->SetTitle("Pt");
  htracker_muPt->GetYaxis()->SetTitle("Event number");    
  htracker_muEta->GetXaxis()->SetTitle("Eta"); 
  htracker_muEta->GetYaxis()->SetTitle("Event number");   
  htracker_muPhi->GetXaxis()->SetTitle("Phi");   
  htracker_muPhi->GetYaxis()->SetTitle("Event number"); 
  htracker_muCorrTrkIso->GetXaxis()->SetTitle("CorrTrkIso");
  htracker_muCorrTrkIso->GetYaxis()->SetTitle("Event number");
  htracker_muTrkIso->GetXaxis()->SetTitle("TrkIso");
  htracker_muTrkIso->GetYaxis()->SetTitle("Event number"); 


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);


    Int_t*   isGlobalMuon  = data.GetPtrInt("isGlobalMuon");
    Int_t*   isTrackerMuon = data.GetPtrInt("isTrackerMuon");
    Int_t*   muHits       = data.GetPtrInt("muHits");
    Int_t*   muMatches    = data.GetPtrInt("muMatches");
    Int_t*   muPixelHits  = data.GetPtrInt("muPixelHits");
    Int_t*   muTrkLayers  = data.GetPtrInt("muTrkLayers");
    Float_t* muPtErrx     = data.GetPtrFloat("muPtErrx");
    Float_t* mudxy        = data.GetPtrFloat("mudxy");
    Float_t* mudz         = data.GetPtrFloat("mudz");
    Float_t* muPt         = data.GetPtrFloat("muPt");
    Float_t* muEta        = data.GetPtrFloat("muEta");
    Float_t* muPhi        = data.GetPtrFloat("muPhi");
    Float_t* muTrkIso     = data.GetPtrFloat("muTrkIso");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");
    Float_t* muM          = data.GetPtrFloat("muM");
    Float_t* elePt        = data.GetPtrFloat("elePt");


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

    if( stMuPtIndex < 0 || ndMuPtIndex < 0 ) continue;


    //-----------------------------------------------------------------------------------//   
    // reconstruct Z mass
    
    TLorentzVector stRecoMu, ndRecoMu;  
 
    stRecoMu.SetPtEtaPhiM(muPt[stMuPtIndex], 
			  muEta[stMuPtIndex], 
			  muPhi[stMuPtIndex],
			  muM[stMuPtIndex]);  
  
    ndRecoMu.SetPtEtaPhiM(muPt[ndMuPtIndex], 
			  muEta[ndMuPtIndex],
			  muPhi[ndMuPtIndex], 
			  muM[ndMuPtIndex]); 
    
    TLorentzVector Z = stRecoMu + ndRecoMu;

    if(Z.Pt() <= 80) continue;


    //-----------------------------------------------------------------------------------//
    // muon selections and cuts

    Int_t muIndex[2] = {stMuPtIndex, ndMuPtIndex};

    for(Int_t flag = 0; flag <= 10; flag++){

      for(Int_t im = 0; im < 2; im++){

	if( isGlobalMuon[muIndex[im]] ){ // global selections and global cuts

	  if( muHits[muIndex[im]]      <= 0   && flag != 0 ) continue;
	  if( muMatches[muIndex[im]]   <= 1   && flag != 1 ) continue;
	  if( muPixelHits[muIndex[im]] <= 0   && flag != 2 ) continue;
	  if( muTrkLayers[muIndex[im]] <= 5   && flag != 3 ) continue;
	  if( muPtErrx[muIndex[im]]    >= 0.3 && flag != 4 ) continue;
	  if( fabs(mudxy[muIndex[im]]) >= 0.2 && flag != 5 ) continue;
	  if( fabs(mudz[muIndex[im]])  >= 0.5 && flag != 6 ) continue;
	  if( muPt[muIndex[im]]        <= 10  && flag != 7 ) continue;
	  if( fabs(muEta[muIndex[im]]) >= 2.4 && flag != 8 ) continue;
	  if( fabs(muPhi[muIndex[im]]) >= 3.2 && flag != 9 ) continue;
	  if( (muCorrTrkIso[muIndex[im]]/muPt[muIndex[im]]) >= 0.1 && flag != 10 ) continue;

	  switch(flag){

	  case 0: hglobal_muHits->Fill(muHits[muIndex[im]]); break;
	  case 1: hglobal_muMatches->Fill(muMatches[muIndex[im]]); break;
	  case 2: hglobal_muPixelHits->Fill(muPixelHits[muIndex[im]]); break;
       	  case 3: hglobal_muTrkLayers->Fill(muTrkLayers[muIndex[im]]); break;
	  case 4: hglobal_muPtErrx->Fill(muPtErrx[muIndex[im]]); break;
	  case 5: hglobal_mudxy->Fill(mudxy[muIndex[im]]); break;
	  case 6: hglobal_mudz->Fill(mudz[muIndex[im]]); break;
	  case 7: hglobal_muPt->Fill(muPt[muIndex[im]]); break;
	  case 8: hglobal_muEta->Fill(muEta[muIndex[im]]); break;
	  case 9: hglobal_muPhi->Fill(muPhi[muIndex[im]]); break;
	  case 10: hglobal_muCorrTrkIso->Fill(muCorrTrkIso[muIndex[im]]/muPt[muIndex[im]]); break;


	  } // end of switch

	} // end of global loop

	hglobal_muTrkIso->Fill(muTrkIso[muIndex[im]]/muPt[muIndex[im]]); 
     
      } // end of two muons

    } // end of flag loop 


      //-----------------------------------------------------------------------------------//


    for(Int_t flag = 0; flag <= 10; flag++){

      for(Int_t im = 0; im < 2; im++){

	if( isTrackerMuon[muIndex[im]] ){ // tracker selections and tracker cuts

	  if( muHits[muIndex[im]]      <= 0   && flag != 0 ) continue;
	  if( muMatches[muIndex[im]]   <= 1   && flag != 1 ) continue;
	  if( muPixelHits[muIndex[im]] <= 0   && flag != 2 ) continue;
	  if( muTrkLayers[muIndex[im]] <= 5   && flag != 3 ) continue;
	  if( muPtErrx[muIndex[im]]    >= 0.3 && flag != 4 ) continue;
	  if( fabs(mudxy[muIndex[im]]) >= 0.2 && flag != 5 ) continue;
	  if( fabs(mudz[muIndex[im]])  >= 0.5 && flag != 6 ) continue;
	  if( muPt[muIndex[im]]        <= 10  && flag != 7 ) continue;
	  if( fabs(muEta[muIndex[im]]) >= 2.4 && flag != 8 ) continue;
	  if( fabs(muPhi[muIndex[im]]) >= 3.2 && flag != 9 ) continue;
	  if( (muCorrTrkIso[muIndex[im]]/muPt[muIndex[im]]) >= 0.1 && flag != 10 ) continue;

	  switch(flag){

	  case 0: htracker_muHits->Fill(muHits[muIndex[im]]); break;
	  case 1: htracker_muMatches->Fill(muMatches[muIndex[im]]); break;
	  case 2: htracker_muPixelHits->Fill(muPixelHits[muIndex[im]]); break;
	  case 3: htracker_muTrkLayers->Fill(muTrkLayers[muIndex[im]]); break;
	  case 4: htracker_muPtErrx->Fill(muPtErrx[muIndex[im]]); break;
	  case 5: htracker_mudxy->Fill(mudxy[muIndex[im]]); break;
	  case 6: htracker_mudz->Fill(mudz[muIndex[im]]); break;
	  case 7: htracker_muPt->Fill(muPt[muIndex[im]]); break;
	  case 8: htracker_muEta->Fill(muEta[muIndex[im]]); break;
	  case 9: htracker_muPhi->Fill(muPhi[muIndex[im]]); break;
	  case 10: htracker_muCorrTrkIso->Fill(muCorrTrkIso[muIndex[im]]/muPt[muIndex[im]]); break;


	  } // end of switch

	} // end of tracker loop
      
	htracker_muTrkIso->Fill(muTrkIso[muIndex[im]]/muPt[muIndex[im]]); 
      
      } // end of two muons loop 
    
    } // end of flag loop

    
  } // end of event loop


  fprintf(stderr, "Processed all events\n");

  TFile* globalOutFile = new TFile("globalMuVariable.root", "update");

  std::string histoNameG0 = "global_Hits_" + outName.substr(11);
  std::string histoNameG1 = "global_Matches_" + outName.substr(11);
  std::string histoNameG2 = "global_PixelHits_" + outName.substr(11);
  std::string histoNameG3 = "global_TrkLayers_" + outName.substr(11);
  std::string histoNameG4 = "global_PtErrx_" + outName.substr(11);
  std::string histoNameG5 = "global_Dxy_" + outName.substr(11);
  std::string histoNameG6 = "global_Dz_" + outName.substr(11);
  std::string histoNameG7 = "global_Pt_" + outName.substr(11);
  std::string histoNameG8 = "global_Eta_" + outName.substr(11);
  std::string histoNameG9 = "global_Phi_" + outName.substr(11);
  std::string histoNameG10 = "global_CorrTrkIso_" + outName.substr(11);
  std::string histoNameG11 = "global_TrkIso_" + outName.substr(11);

  hglobal_muHits      ->Write(histoNameG0.data());
  hglobal_muMatches   ->Write(histoNameG1.data());
  hglobal_muPixelHits ->Write(histoNameG2.data());
  hglobal_muTrkLayers ->Write(histoNameG3.data());
  hglobal_muPtErrx    ->Write(histoNameG4.data());
  hglobal_mudxy       ->Write(histoNameG5.data());
  hglobal_mudz        ->Write(histoNameG6.data());
  hglobal_muPt        ->Write(histoNameG7.data());
  hglobal_muEta       ->Write(histoNameG8.data());
  hglobal_muPhi       ->Write(histoNameG9.data());
  hglobal_muCorrTrkIso->Write(histoNameG10.data());
  hglobal_muTrkIso    ->Write(histoNameG11.data());

  globalOutFile->Write();

  TFile* trackerOutFile = new TFile("trackerMuVariable.root", "update");

  std::string histoNameT0 = "tracker_Hits_" + outName.substr(11);
  std::string histoNameT1 = "tracker_Matches_" + outName.substr(11);
  std::string histoNameT2 = "tracker_PixelHits_" + outName.substr(11);
  std::string histoNameT3 = "tracker_TrkLayers_" + outName.substr(11);
  std::string histoNameT4 = "tracker_PtErrx_" + outName.substr(11);
  std::string histoNameT5 = "tracker_Dxy_" + outName.substr(11);
  std::string histoNameT6 = "tracker_Dz_" + outName.substr(11);
  std::string histoNameT7 = "tracker_Pt_" + outName.substr(11);
  std::string histoNameT8 = "tracker_Eta_" + outName.substr(11);
  std::string histoNameT9 = "tracker_Phi_" + outName.substr(11);
  std::string histoNameT10 = "tracker_CorrTrkIso_" + outName.substr(11);
  std::string histoNameT11 = "tracker_TrkIso_" + outName.substr(11);

  htracker_muHits      ->Write(histoNameT0.data());
  htracker_muMatches   ->Write(histoNameT1.data());
  htracker_muPixelHits ->Write(histoNameT2.data());
  htracker_muTrkLayers ->Write(histoNameT3.data());
  htracker_muPtErrx    ->Write(histoNameT4.data());
  htracker_mudxy       ->Write(histoNameT5.data());
  htracker_mudz        ->Write(histoNameT6.data());
  htracker_muPt        ->Write(histoNameT7.data());
  htracker_muEta       ->Write(histoNameT8.data());
  htracker_muPhi       ->Write(histoNameT9.data());
  htracker_muCorrTrkIso->Write(histoNameT10.data());
  htracker_muTrkIso    ->Write(histoNameT11.data());

  trackerOutFile->Write(); 

}
