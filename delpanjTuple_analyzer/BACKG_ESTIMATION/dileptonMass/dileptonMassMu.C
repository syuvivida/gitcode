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

    if(Z.E() <= 1e-6) continue;
    if(Z.Pt() <= 80) continue;

    h_ZMass->Fill(Z.M());

    
  }
   
  // end of event loop

  fprintf(stderr, "Processed all events\n");

  // draw results

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 760);
  c->Divide(2,2);

  c->cd(1);
  h_ZMass->Draw();


  // output file

  std::string ZMassName = "ZMass_" + outName.substr(11);

  TFile* outFile = new TFile("dileptonMassMu.root", "update");

  h_ZMass->Write(ZMassName.data());

  outFile->Write();
  
}
