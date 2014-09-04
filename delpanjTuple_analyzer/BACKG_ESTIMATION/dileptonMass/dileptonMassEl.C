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
#include "../HEADER/passElectronID.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);
Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);

void dileptonMassEl(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  TH1D* h_ZMass = new TH1D("h_ZMass", "Z mass by dielectron", 100, 70, 110);
  h_ZMass->GetXaxis()->SetTitle("Mass");
  h_ZMass->GetYaxis()->SetTitle("Event number");
 
  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);


    Int_t    nEle  = data.GetInt("nEle");
    Int_t*   elePassID = data.GetPtrInt("elePassID");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleM   = data.GetPtrFloat("eleM");

    Float_t* muPt  = data.GetPtrFloat("muPt");


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
    // choose the primary electron
    
    Int_t stMuPtIndex  = -1;
    Int_t ndMuPtIndex  = -1;
    Int_t stElePtIndex = -1;
    Int_t ndElePtIndex = -1;

    specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		     &stElePtIndex, &ndElePtIndex);

    if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
	(stElePtIndex < 0 || ndElePtIndex < 0 )  ) continue; 
  
    if( stMuPtIndex > 0 && stElePtIndex > 0 ){
    
      if( (elePt[stElePtIndex] - muPt[stMuPtIndex]) < 1e-6 ) 
	continue;

    }
    
    
    //-----------------------------------------------------------------------------------//
    // sorting muon and pass the muon ID

    vector<Int_t> howManyEle;

    typedef map<Float_t, Int_t, std::greater<Float_t> > eleMap;
    eleMap sortElePt;
    typedef eleMap::iterator mapEleIter;

    for(Int_t i = 0; i < nEle; i++){

      sortElePt.insert(std::pair<Float_t, Int_t>(elePt[i], i));

    }

    for(mapEleIter it_part = sortElePt.begin(); it_part != sortElePt.end(); ++it_part){

      Int_t sortEleIndex = it_part->second;

      if( elePassID[sortEleIndex] <= 0 ) continue;
      if( elePt[sortEleIndex] <= 35 ) continue; 

      howManyEle.push_back(sortEleIndex);

    }

    Int_t stRecoEleIndex, ndRecoEleIndex;

    if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) )
      continue;


    //-----------------------------------------------------------------------------------//   
    // reconstruct Z mass
    
    TLorentzVector stRecoEle, ndRecoEle;  
 
    stRecoEle.SetPtEtaPhiM(elePt[stRecoEleIndex], 
			  eleEta[stRecoEleIndex], 
			  elePhi[stRecoEleIndex],
			  eleM[stRecoEleIndex]);  
  
    ndRecoEle.SetPtEtaPhiM(elePt[ndRecoEleIndex], 
			  eleEta[ndRecoEleIndex],
			  elePhi[ndRecoEleIndex], 
			  eleM[ndRecoEleIndex]); 
    
    TLorentzVector Z = stRecoEle + ndRecoEle;

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

  TFile* outFile = new TFile("dileptonMassEl.root", "update");

  h_ZMass->Write(ZMassName.data());

  outFile->Write();
  
}
