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

void eleVariable(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // Declare the histogram (barrel)

  TH1D* hbarrel_eleDelEtaIn = new TH1D("hbarrel_eleDelEtaIn", "barrel eleDelEtaIn", 100, -0.05, 0.05);
  TH1D* hbarrel_eleDelPhiIn = new TH1D("hbarrel_eleDelPhiIn", "barrel eleDelPhiIn", 100, -0.05, 0.05);
  TH1D* hbarrel_eleSigIhIh  = new TH1D("hbarrel_eleSigIhIh", "barrel eleSigIhIh", 100, 0, 0.03);
  TH1D* hbarrel_eleHoE      = new TH1D("hbarrel_eleHoE", "barrel eleHoE", 100, 0, 0.2); 
  TH1D* hbarrel_eleDxy      = new TH1D("hbarrel_eleDxy", "barrel eleDx", 100, -0.04, 0.04);  
  TH1D* hbarrel_eleDz       = new TH1D("hbarrel_eleDz", "barrel eleDz", 100, -0.04, 0.04);   
  TH1D* hbarrel_eleEoverP   = new TH1D("hbarrel_eleEoverP", "barrel eleEoverP", 100, 0, 0.1);
  TH1D* hbarrel_elePassConv = new TH1D("hbarrel_elePassConv", "barrel elePassConv", 100, 0.8, 1.2);
  TH1D* hbarrel_eleMissingHits = new TH1D("hbarrel_eleMissingHits", "barrel eleMissingHits", 100, -0.05, 0.05);
  TH1D* hbarrel_elePt       = new TH1D("hbarrel_elePt", "barrel elePt", 100, 40, 140);
  TH1D* hbarrel_eleEta      = new TH1D("hbarrel_eleEta", "barrel eleEta", 100, -2, 2);
  TH1D* hbarrel_eleUserTrkIso  = new TH1D("hbarrel_eleUserTrkIso", "barrel eleUserTrkIso", 100, 0, 6);
  TH1D* hbarrel_eleUserCalIso  = new TH1D("hbarrel_eleUserCalIso", "barrel eleUserCalIso", 100, 0, 10);

  hbarrel_eleDelEtaIn->GetXaxis()->SetTitle("DelEtaIn");
  hbarrel_eleDelEtaIn->GetYaxis()->SetTitle("Event number");
  hbarrel_eleDelPhiIn->GetXaxis()->SetTitle("DelPhiIn");
  hbarrel_eleDelPhiIn->GetYaxis()->SetTitle("Event number");
  hbarrel_eleSigIhIh->GetXaxis()->SetTitle("SigIhIh");
  hbarrel_eleSigIhIh->GetYaxis()->SetTitle("Event number");
  hbarrel_eleHoE->GetXaxis()->SetTitle("HoE");
  hbarrel_eleHoE->GetYaxis()->SetTitle("Event number");
  hbarrel_eleDxy->GetXaxis()->SetTitle("Dxy");
  hbarrel_eleDxy->GetYaxis()->SetTitle("Event number");
  hbarrel_eleDz->GetXaxis()->SetTitle("Dz");
  hbarrel_eleDz->GetYaxis()->SetTitle("Event number");
  hbarrel_eleEoverP->GetXaxis()->SetTitle("EoverP");
  hbarrel_eleEoverP->GetYaxis()->SetTitle("Event number");
  hbarrel_elePassConv->GetXaxis()->SetTitle("PassConv");
  hbarrel_elePassConv->GetYaxis()->SetTitle("Event number");
  hbarrel_eleMissingHits->GetXaxis()->SetTitle("MissingHits");
  hbarrel_eleMissingHits->GetYaxis()->SetTitle("Event number");
  hbarrel_elePt->GetXaxis()->SetTitle("Pt");
  hbarrel_elePt->GetYaxis()->SetTitle("Event number");
  hbarrel_eleEta->GetXaxis()->SetTitle("Eta");
  hbarrel_eleEta->GetYaxis()->SetTitle("Event number");
  hbarrel_eleUserTrkIso->GetXaxis()->SetTitle("UserTrkIso");
  hbarrel_eleUserTrkIso->GetYaxis()->SetTitle("Event number");
  hbarrel_eleUserCalIso->GetXaxis()->SetTitle("UserCalIso");
  hbarrel_eleUserCalIso->GetYaxis()->SetTitle("Event number");


  // Declare the histogram (endcap)

  TH1D* hendcap_eleDelEtaIn = new TH1D("hendcap_eleDelEtaIn", "endcap eleDelEtaIn", 100, -0.05, 0.05);
  TH1D* hendcap_eleDelPhiIn = new TH1D("hendcap_eleDelPhiIn", "endcap eleDelPhiIn", 100, -0.05, 0.05);
  TH1D* hendcap_eleSigIhIh  = new TH1D("hendcap_eleSigIhIh", "endcap eleSigIhIh", 100, 0.01, 0.05);
  TH1D* hendcap_eleHoE      = new TH1D("hendcap_eleHoE", "endcap eleHoE", 100, 0, 0.2); 
  TH1D* hendcap_eleDxy      = new TH1D("hendcap_eleDxy", "endcap eleDx", 100, -0.04, 0.04);  
  TH1D* hendcap_eleDz       = new TH1D("hendcap_eleDz", "endcap eleDz", 100, -0.04, 0.04);   
  TH1D* hendcap_eleEoverP   = new TH1D("hendcap_eleEoverP", "endcap eleEoverP", 100, 0, 0.1);
  TH1D* hendcap_elePassConv = new TH1D("hendcap_elePassConv", "endcap elePassConv", 100, 0.8, 1.2);
  TH1D* hendcap_eleMissingHits = new TH1D("hendcap_eleMissingHits", "endcap eleMissingHits", 100, -0.05, 0.05);
  TH1D* hendcap_elePt       = new TH1D("hendcap_elePt", "endcap elePt", 100, 40, 140);
  TH1D* hendcap_eleEta      = new TH1D("hendcap_eleEta", "endcap eleEta", 100, -3, 3);
  TH1D* hendcap_eleUserTrkIso  = new TH1D("hendcap_eleUserTrkIso", "endcap eleUserTrkIso", 100, 0, 6);
  TH1D* hendcap_eleUserCalIso  = new TH1D("hendcap_eleUserCalIso", "endcap eleUserCalIso", 100, 0, 10);

  hendcap_eleDelEtaIn->GetXaxis()->SetTitle("DelEtaIn");
  hendcap_eleDelEtaIn->GetYaxis()->SetTitle("Event number");
  hendcap_eleDelPhiIn->GetXaxis()->SetTitle("DelPhiIn");
  hendcap_eleDelPhiIn->GetYaxis()->SetTitle("Event number");
  hendcap_eleSigIhIh->GetXaxis()->SetTitle("SigIhIh");
  hendcap_eleSigIhIh->GetYaxis()->SetTitle("Event number");
  hendcap_eleHoE->GetXaxis()->SetTitle("HoE");
  hendcap_eleHoE->GetYaxis()->SetTitle("Event number");
  hendcap_eleDxy->GetXaxis()->SetTitle("Dxy");
  hendcap_eleDxy->GetYaxis()->SetTitle("Event number");
  hendcap_eleDz->GetXaxis()->SetTitle("Dz");
  hendcap_eleDz->GetYaxis()->SetTitle("Event number");
  hendcap_eleEoverP->GetXaxis()->SetTitle("EoverP");
  hendcap_eleEoverP->GetYaxis()->SetTitle("Event number");
  hendcap_elePassConv->GetXaxis()->SetTitle("PassConv");
  hendcap_elePassConv->GetYaxis()->SetTitle("Event number");
  hendcap_eleMissingHits->GetXaxis()->SetTitle("MissingHits");
  hendcap_eleMissingHits->GetYaxis()->SetTitle("Event number");
  hendcap_elePt->GetXaxis()->SetTitle("Pt");
  hendcap_elePt->GetYaxis()->SetTitle("Event number");
  hendcap_eleEta->GetXaxis()->SetTitle("Eta");
  hendcap_eleEta->GetYaxis()->SetTitle("Event number");
  hendcap_eleUserTrkIso->GetXaxis()->SetTitle("UserTrkIso");
  hendcap_eleUserTrkIso->GetYaxis()->SetTitle("Event number");
  hendcap_eleUserCalIso->GetXaxis()->SetTitle("UserCalIso");
  hendcap_eleUserCalIso->GetYaxis()->SetTitle("Event number");


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    
    Int_t*   elePassConv = data.GetPtrInt("elePassConv");
    Float_t* eleDelEtaIn = data.GetPtrFloat("eleDelEtaIn");
    Float_t* eleDelPhiIn = data.GetPtrFloat("eleDelPhiIn");
    Float_t* eleSigIhIh  = data.GetPtrFloat("eleSigIhIh");
    Float_t* eleHoE      = data.GetPtrFloat("eleHoE");
    Float_t* eleDxy      = data.GetPtrFloat("eleDxy");
    Float_t* eleDz       = data.GetPtrFloat("eleDz");
    Float_t* eleEoverP   = data.GetPtrFloat("eleEoverP");
    Float_t* eleMissingHits = data.GetPtrFloat("eleMissingHits");
    Float_t* elePt       = data.GetPtrFloat("elePt");
    Float_t  eleRho      = data.GetFloat("eleRho");
    Float_t* eleEta      = data.GetPtrFloat("eleEta");
    Float_t* eleScEta    = data.GetPtrFloat("eleScEta");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");
    Float_t* elePhi      = data.GetPtrFloat("elePhi");
    Float_t* eleM        = data.GetPtrFloat("eleM");
    Float_t* muPt        = data.GetPtrFloat("muPt");


    //-----------------------------------------------------------------------------------//
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

    //-----------------------------------------------------------------------------------//
    // choosing the primary electron

    Int_t stMuPtIndex  = -1;
    Int_t ndMuPtIndex  = -1;
    Int_t stElePtIndex = -1;
    Int_t ndElePtIndex = -1;

    specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		     &stElePtIndex, &ndElePtIndex);

    if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
	(stElePtIndex < 0 || ndElePtIndex < 0 )  ) continue; 
  
    if( stMuPtIndex > -1 && stElePtIndex > -1 ){
    
      if( (elePt[stElePtIndex] - muPt[stMuPtIndex]) < 1e-6 ) 
	continue;

    }

    if( stElePtIndex < 0 ) continue;


    //-----------------------------------------------------------------------------------//   
    // reconstruct Z mass
    
    TLorentzVector stRecoEle, ndRecoEle;  
 
    stRecoEle.SetPtEtaPhiM(elePt[stElePtIndex], 
			   eleEta[stElePtIndex], 
			   elePhi[stElePtIndex],
			   eleM[stElePtIndex]);  
  
    ndRecoEle.SetPtEtaPhiM(elePt[ndElePtIndex], 
			   eleEta[ndElePtIndex],
			   elePhi[ndElePtIndex], 
			   eleM[ndElePtIndex]); 
    
    TLorentzVector Z = stRecoEle + ndRecoEle;

    if(Z.Pt() <= 80) continue;


    Double_t isoCutValue = 2+(0.03*elePt[stElePtIndex]);

    //-----------------------------------------------------------------------------------//
    // barrel selections and barrel cuts

    if( fabs(eleScEta[stElePtIndex]) > 0 && fabs(eleScEta[stElePtIndex]) < 1.4442 ){

      for(Int_t flag = 0; flag <= 10; flag++){

	if( eleDelEtaIn[stElePtIndex] >= 0.004 && flag != 0 ) continue;
	if( eleDelPhiIn[stElePtIndex] >= 0.03  && flag != 1 ) continue;
	if( eleSigIhIh[stElePtIndex]  >= 0.01  && flag != 2 ) continue;
	if( eleHoE[stElePtIndex]      >= 0.12  && flag != 3 ) continue;
	if( eleDxy[stElePtIndex]      >= 0.02  && flag != 4 ) continue;
	if( eleDz[stElePtIndex]       >= 0.1   && flag != 5 ) continue;
	if( eleEoverP[stElePtIndex]   >= 0.05  && flag != 6 ) continue;
	if( elePassConv[stElePtIndex] < 1e-6   && flag != 7 ) continue;
	if( eleMissingHits[stElePtIndex] > 0   && flag != 8 ) continue;
	if( elePt[stElePtIndex]         <= 10  && flag != 9 ) continue;
	if( fabs(eleEta[stElePtIndex])  >= 2.5 && flag != 10) continue;
	if( eleUserTrkIso[stElePtIndex] >= 5   && flag != 11) continue;
	if( (eleUserCalIso[stElePtIndex]-(0.06205*eleRho)) >= isoCutValue && 
	    flag != 12 ) continue;

	    
	switch(flag){

	case 0:  hbarrel_eleDelEtaIn->Fill(eleDelEtaIn[stElePtIndex]);
	case 1:  hbarrel_eleDelPhiIn->Fill(eleDelPhiIn[stElePtIndex]);
	case 2:  hbarrel_eleSigIhIh ->Fill(eleSigIhIh[stElePtIndex]);
	case 3:  hbarrel_eleHoE     ->Fill(eleHoE[stElePtIndex]);
	case 4:  hbarrel_eleDxy     ->Fill(eleDxy[stElePtIndex]);
	case 5:  hbarrel_eleDz      ->Fill(eleDz[stElePtIndex]);
	case 6:  hbarrel_eleEoverP  ->Fill(eleEoverP[stElePtIndex]);
	case 7:  hbarrel_elePassConv->Fill(elePassConv[stElePtIndex]);
	case 8:  hbarrel_eleMissingHits->Fill(eleMissingHits[stElePtIndex]);
	case 9:  hbarrel_elePt      ->Fill(elePt[stElePtIndex]);
	case 10: hbarrel_eleEta     ->Fill(eleEta[stElePtIndex]);
	case 11: hbarrel_eleUserTrkIso->Fill(eleUserTrkIso[stElePtIndex]);
	case 12: hbarrel_eleUserCalIso->Fill(eleUserCalIso[stElePtIndex]);


	} // end of switch
    
      } // end of flag loop

    } // end of barrel


    //-----------------------------------------------------------------------------------//
    // endcap selections and endcap cuts

    if( fabs(eleScEta[stElePtIndex]) > 1.566 && fabs(eleScEta[stElePtIndex]) < 2.5 ){

      for(Int_t flag = 0; flag <= 10; flag++){

	if( eleDelEtaIn[stElePtIndex] >= 0.005 && flag != 0 ) continue;
	if( eleDelPhiIn[stElePtIndex] >= 0.02  && flag != 1 ) continue;
	if( eleSigIhIh[stElePtIndex]  >= 0.03  && flag != 2 ) continue;
	if( eleHoE[stElePtIndex]      >= 0.1   && flag != 3 ) continue;
	if( eleDxy[stElePtIndex]      >= 0.02  && flag != 4 ) continue;
	if( eleDz[stElePtIndex]       >= 0.1   && flag != 5 ) continue;
	if( eleEoverP[stElePtIndex]   >= 0.05  && flag != 6 ) continue;
	if( elePassConv[stElePtIndex] < 1e-6   && flag != 7 ) continue;
	if( eleMissingHits[stElePtIndex] > 0   && flag != 8 ) continue;
	if( elePt[stElePtIndex]         <= 10  && flag != 9 ) continue;
	if( fabs(eleEta[stElePtIndex])  >= 2.5 && flag != 10) continue;
	if( eleUserTrkIso[stElePtIndex] >= 5  && flag != 11) continue;
	if( elePt[stElePtIndex] < 50 )
	  if( (eleUserCalIso[stElePtIndex]-(0.06205*eleRho)) >= 2.5 && 
	      flag != 12 ) continue;
	if( elePt[stElePtIndex] > 50 )
	  if( (eleUserCalIso[stElePtIndex]-(0.06205*eleRho)) >= (isoCutValue+0.5) &&
	      flag != 12 ) continue;
	
	
	switch(flag){

	case 0:  hendcap_eleDelEtaIn->Fill(eleDelEtaIn[stElePtIndex]);
	case 1:  hendcap_eleDelPhiIn->Fill(eleDelPhiIn[stElePtIndex]);
	case 2:  hendcap_eleSigIhIh ->Fill(eleSigIhIh[stElePtIndex]);
	case 3:  hendcap_eleHoE     ->Fill(eleHoE[stElePtIndex]);
	case 4:  hendcap_eleDxy     ->Fill(eleDxy[stElePtIndex]);
	case 5:  hendcap_eleDz      ->Fill(eleDz[stElePtIndex]);
	case 6:  hendcap_eleEoverP  ->Fill(eleEoverP[stElePtIndex]);
	case 7:  hendcap_elePassConv->Fill(elePassConv[stElePtIndex]);
	case 8:  hendcap_eleMissingHits->Fill(eleMissingHits[stElePtIndex]);
	case 9:  hendcap_elePt      ->Fill(elePt[stElePtIndex]);
	case 10: hendcap_eleEta     ->Fill(eleEta[stElePtIndex]);
	case 11: hendcap_eleUserTrkIso->Fill(eleUserTrkIso[stElePtIndex]);
	case 12: hendcap_eleUserCalIso->Fill(eleUserCalIso[stElePtIndex]);

	
	} // end of switch
	
      } // end of flag loop

    } // end of endcap
    

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  TFile* barrelOutFile = new TFile("barrelEleVariable.root", "update");

  std::string histoNameB0 = "barrel_DelEtaIn_" + outName.substr(11);
  std::string histoNameB1 = "barrel_DelPhiIn_" + outName.substr(11);
  std::string histoNameB2 = "barrel_SigIhIh_" + outName.substr(11);
  std::string histoNameB3 = "barrel_HoE_" + outName.substr(11);
  std::string histoNameB4 = "barrel_Dxy_" + outName.substr(11);
  std::string histoNameB5 = "barrel_Dz_" + outName.substr(11);
  std::string histoNameB6 = "barrel_EoverP_" + outName.substr(11);
  std::string histoNameB7 = "barrel_PassConv_" + outName.substr(11);
  std::string histoNameB8 = "barrel_MissingHits_" + outName.substr(11);
  std::string histoNameB9 = "barrel_Pt_" + outName.substr(11);
  std::string histoNameB10 = "barrel_Eta_" + outName.substr(11);
  std::string histoNameB11 = "barrel_UserTrkIso_" + outName.substr(11);
  std::string histoNameB12 = "barrel_UserCalIso_" + outName.substr(11);
  
  hbarrel_eleDelEtaIn->Write(histoNameB0.data());
  hbarrel_eleDelPhiIn->Write(histoNameB1.data());
  hbarrel_eleSigIhIh ->Write(histoNameB2.data());  
  hbarrel_eleHoE     ->Write(histoNameB3.data());   
  hbarrel_eleDxy     ->Write(histoNameB4.data());   
  hbarrel_eleDz      ->Write(histoNameB5.data());   
  hbarrel_eleEoverP  ->Write(histoNameB6.data());
  hbarrel_elePassConv->Write(histoNameB7.data());
  hbarrel_eleMissingHits->Write(histoNameB8.data());
  hbarrel_elePt      ->Write(histoNameB9.data());  
  hbarrel_eleEta     ->Write(histoNameB10.data());   
  hbarrel_eleUserTrkIso ->Write(histoNameB11.data());
  hbarrel_eleUserCalIso ->Write(histoNameB12.data());

  barrelOutFile->Write();

  TFile* endcapOutFile = new TFile("endcapEleVariable.root", "update");

  std::string histoNameE0 = "endcap_DelEtaIn_" + outName.substr(11);
  std::string histoNameE1 = "endcap_DelPhiIn_" + outName.substr(11);
  std::string histoNameE2 = "endcap_SigIhIh_" + outName.substr(11);
  std::string histoNameE3 = "endcap_HoE_" + outName.substr(11);
  std::string histoNameE4 = "endcap_Dxy_" + outName.substr(11);
  std::string histoNameE5 = "endcap_Dz_" + outName.substr(11);
  std::string histoNameE6 = "endcap_EoverP_" + outName.substr(11);
  std::string histoNameE7 = "endcap_PassConv_" + outName.substr(11);
  std::string histoNameE8 = "endcap_MissingHits_" + outName.substr(11);
  std::string histoNameE9 = "endcap_Pt_" + outName.substr(11);
  std::string histoNameE10 = "endcap_Eta_" + outName.substr(11);
  std::string histoNameE11 = "endcap_UserTrkIso_" + outName.substr(11);
  std::string histoNameE12 = "endcap_UserCalIso_" + outName.substr(11);

  hendcap_eleDelEtaIn->Write(histoNameE0.data());
  hendcap_eleDelPhiIn->Write(histoNameE1.data());
  hendcap_eleSigIhIh ->Write(histoNameE2.data());  
  hendcap_eleHoE     ->Write(histoNameE3.data());   
  hendcap_eleDxy     ->Write(histoNameE4.data());   
  hendcap_eleDz      ->Write(histoNameE5.data());   
  hendcap_eleEoverP  ->Write(histoNameE6.data());
  hendcap_elePassConv->Write(histoNameE7.data());
  hendcap_eleMissingHits->Write(histoNameE8.data());
  hendcap_elePt      ->Write(histoNameE9.data());  
  hendcap_eleEta     ->Write(histoNameE10.data());   
  hendcap_eleUserTrkIso ->Write(histoNameE11.data());
  hendcap_eleUserCalIso ->Write(histoNameE12.data());

  endcapOutFile->Write();
  
}
