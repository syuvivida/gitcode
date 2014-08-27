#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TStyle.h>
#include <TChain.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/specificLeptonPt.C"
#include "../HEADER/passMuonID.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);
Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);

void backgStack(std::string inputFile, std::string outName){

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


    Int_t    CA8nJet    = data.GetInt("CA8nJet"); 
    Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
    Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
    Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
    Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");
    Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");

    Int_t    nMu   = data.GetInt("nMu");
    Int_t*   muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    Float_t* elePt  = data.GetPtrFloat("elePt");
    

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
     
	// electron channel
	/*
	  if( thisTrig.find("HLT_DoubleEle33") != std::string::npos && results == 1 ){

	  passTrigger = true;
	  break;

	  }
	*/

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
  
    if( stMuPtIndex > 0 && stElePtIndex > 0 ){
    
      if( (muPt[stMuPtIndex] - elePt[stElePtIndex]) < 1e-6 ) 
	continue;

    }
    
    
    //-----------------------------------------------------------------------------------//
    // sorting muon and pass the muon ID

    vector<Int_t> howManyMu;

    typedef map<Float_t, Int_t, std::greater<Float_t> > muMap;
    muMap sortMuPt;
    typedef muMap::iterator mapMuIter;

    for(Int_t i = 0; i < nMu; i++){

      sortMuPt.insert(std::pair<Float_t, Int_t>(muPt[i], i));

    }

    for(mapMuIter it_part = sortMuPt.begin(); it_part != sortMuPt.end(); ++it_part){

      Int_t sortMuIndex = it_part->second;

      if( !(muPassID[sortMuIndex] & 4) ) continue;
      if( muPt[sortMuIndex] <= 20 ) continue; 

      howManyMu.push_back(sortMuIndex);

    }

    Int_t stRecoMuIndex, ndRecoMuIndex;

    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
      continue;
    

    //-----------------------------------------------------------------------------------//
    // pass boosted-jet ID, removing overlap muons

    typedef map<double, int, std::greater<double> > jetMap;
    jetMap sortJetPt;
    typedef jetMap::iterator mapJetIter;

    for(Int_t i = 0; i < CA8nJet; i++){

      sortJetPt.insert(std::pair<Float_t, Int_t>(CA8jetPt[i], i));

    }

    vector<Int_t> maxJetIndex;
    TLorentzVector boostedJet, basicMuon;

    for(mapJetIter it_part = sortJetPt.begin(); it_part != sortJetPt.end(); ++it_part){

      Int_t sortJetIndex = it_part->second;

      if( CA8jetPt[sortJetIndex] <= 30 ) continue;
      if( fabs(CA8jetEta[sortJetIndex]) >= 2.4 ) continue;


      boostedJet.SetPtEtaPhiM(CA8jetPt[sortJetIndex], 
			      CA8jetEta[sortJetIndex], 
			      CA8jetPhi[sortJetIndex], 
			      CA8jetMass[sortJetIndex]);


      Bool_t isolatedStats = true; 
	
      for(size_t j = 0; j < howManyMu.size(); j++){

	basicMuon.SetPtEtaPhiM(muPt[howManyMu[j]], 
			       muEta[howManyMu[j]], 
			       muPhi[howManyMu[j]],
			       muM[howManyMu[j]]);

	if( basicMuon.DeltaR(boostedJet) < 0.5 ){
	  isolatedStats = false;
	  break;

	}

      } // end of howManyMu for loop
	
      if( isolatedStats ) 
	maxJetIndex.push_back(sortJetIndex);
	

    } // end of CA8nJet for loop
  

    if( maxJetIndex.size() < 1 ) continue;

    if( CA8jetPrunedMass[maxJetIndex[0]] > 40 && CA8jetPrunedMass[maxJetIndex[0]] < 240 )
      h_prunedJetMass->Fill(CA8jetPrunedMass[maxJetIndex[0]]);

  }
   
  // end of event loop

  fprintf(stderr, "Processed all events\n");

  // draw results

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 760);
  c->Divide(2,2);

  c->cd(1)->SetLogy();
  h_prunedJetMass->Draw();


  // output file
  
  std::string sideName = "prunedJetMass_" + outName.substr(11);
  TFile* outFile = new TFile("backgStack.root", "update");
  h_prunedJetMass->Write(sideName.data());
  outFile->Write();
  
}
