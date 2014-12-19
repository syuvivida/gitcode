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

void sideSigZpMEle(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // variable bin

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  TH1D* h_sideZprimeMass = new TH1D("h_sideZprimeMass", "Side-band region Zprime Mass", nvarBins, varBins);
  h_sideZprimeMass->GetXaxis()->SetTitle("Zprime mass");
  h_sideZprimeMass->GetYaxis()->SetTitle("Event number");

  TH1D* h_signZprimeMass = new TH1D("h_signZprimeMass", "Signal region Zprime Mass", nvarBins, varBins);
  h_signZprimeMass->GetXaxis()->SetTitle("Zprime mass");
  h_signZprimeMass->GetYaxis()->SetTitle("Event number");


  // constant bin

  TH1D* h_sideZprimeMass_constBin = new TH1D("h_sideZprimeMass_constBin", "Side-band region Zprime Mass",43 ,680, 2400);
  h_sideZprimeMass_constBin->GetXaxis()->SetTitle("Zprime mass");
  h_sideZprimeMass_constBin->GetYaxis()->SetTitle("Event number");

  TH1D* h_signZprimeMass_constBin = new TH1D("h_signZprimeMass_constBin", "Signal region Zprime Mass",43 ,680, 2400);
  h_signZprimeMass_constBin->GetXaxis()->SetTitle("Zprime mass");
  h_signZprimeMass_constBin->GetYaxis()->SetTitle("Event number");


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);


    Int_t    CA8nJet    = data.GetInt("CA8nJet"); 
    Int_t*   CA8jetPassID = data.GetPtrInt("CA8jetPassID");
    Float_t* CA8jetPt   = data.GetPtrFloat("CA8jetPt");
    Float_t* CA8jetEta  = data.GetPtrFloat("CA8jetEta");
    Float_t* CA8jetPhi  = data.GetPtrFloat("CA8jetPhi");
    Float_t* CA8jetMass = data.GetPtrFloat("CA8jetMass");
    Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");
    Float_t* CA8jetTau1 = data.GetPtrFloat("CA8jetTau1");
    Float_t* CA8jetTau2 = data.GetPtrFloat("CA8jetTau2");

    Int_t    nEle   = data.GetInt("nEle");
    Int_t*   elePassID = data.GetPtrInt("elePassID");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleM   = data.GetPtrFloat("eleM");

    Float_t* muPt  = data.GetPtrFloat("muPt");


    //-----------------------------------------------------------------------------------//
    // data trigger cut

    if ( outName.find("DoubleEle") != std::string::npos ){

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
    
      if( (elePt[stElePtIndex] - muPt[stElePtIndex]) < 1e-6 ) 
	continue;

    }
    
    
    //-----------------------------------------------------------------------------------//
    // sorting electron and pass the electron ID

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
      if( elePt[sortEleIndex] <= 40 ) continue; 

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
    

    //-----------------------------------------------------------------------------------//
    // pass boosted-jet ID, removing overlap electrons

    typedef map<double, int, std::greater<double> > jetMap;
    jetMap sortJetPt;
    typedef jetMap::iterator mapJetIter;

    for(Int_t i = 0; i < CA8nJet; i++){

      sortJetPt.insert(std::pair<Float_t, Int_t>(CA8jetPt[i], i));

    }

    vector<Int_t> maxJetIndex;
    TLorentzVector boostedJet, basicElectron;

    for(mapJetIter it_part = sortJetPt.begin(); it_part != sortJetPt.end(); ++it_part){

      Int_t sortJetIndex = it_part->second;

      if( CA8jetPt[sortJetIndex] <= 30 ) continue;
      if( fabs(CA8jetEta[sortJetIndex]) >= 2.4 ) continue;
      if( CA8jetTau2[sortJetIndex]/CA8jetTau1[sortJetIndex] >= 0.5 ) continue;

      if( CA8jetPassID[sortJetIndex] > 0 ){

	boostedJet.SetPtEtaPhiM(CA8jetPt[sortJetIndex], 
				CA8jetEta[sortJetIndex], 
				CA8jetPhi[sortJetIndex], 
				CA8jetMass[sortJetIndex]);


	Bool_t isolatedStats = true; 
	
	for(size_t j = 0; j < howManyEle.size(); j++){

	  basicElectron.SetPtEtaPhiM(elePt[howManyEle[j]], 
				     eleEta[howManyEle[j]], 
				     elePhi[howManyEle[j]],
				     eleM[howManyEle[j]]);

	  if( basicElectron.DeltaR(boostedJet) < 0.5 ){
	    isolatedStats = false;
	    break;

	  }

	} // end of howManyEle for loop
	
	if( isolatedStats ) 
	  maxJetIndex.push_back(sortJetIndex);
	
      } // end of CA8jetPassID if loop

    } // end of CA8nJet for loop
  

    //-----------------------------------------------------------------------------------//
    // choose the highest jet pt
    
    if( maxJetIndex.size() < 1 ) continue;

    TLorentzVector newBoostedJet;

    newBoostedJet.SetPtEtaPhiM(CA8jetPt[maxJetIndex[0]],
			       CA8jetEta[maxJetIndex[0]],
			       CA8jetPhi[maxJetIndex[0]],
			       CA8jetMass[maxJetIndex[0]]);

    
    //-----------------------------------------------------------------------------------//
    // reconstruct Z prime

    if( Z.E() <= 1e-6 || newBoostedJet.E() <= 1e-6 ) continue;

    //TLorentzVector Zprime = Z + newBoostedJet;  

    if(Z.M() <= 70 || Z.M() >=110 ) continue;
    if(Z.Pt() <= 80) continue;
    if(newBoostedJet.M() <= 40 ) continue;
    if(newBoostedJet.Pt() <= 80 ) continue;

    // side band region
    if( CA8jetPrunedMass[maxJetIndex[0]] > 50 && CA8jetPrunedMass[maxJetIndex[0]] < 110 ){

      TLorentzVector Zprime = Z + newBoostedJet; 
      h_sideZprimeMass->Fill(Zprime.M());
      h_sideZprimeMass_constBin->Fill(Zprime.M());

    }
    
    // signal region
    if( CA8jetPrunedMass[maxJetIndex[0]] > 110 && CA8jetPrunedMass[maxJetIndex[0]] < 140 ){

      TLorentzVector Zprime = Z + newBoostedJet; 
      h_signZprimeMass->Fill(Zprime.M());
      h_signZprimeMass_constBin->Fill(Zprime.M());
   
    }
    
  }
   
  // end of event loop

  fprintf(stderr, "Processed all events\n");


  // output file

  std::string sideName = "sideZpMass_" + outName.substr(11);
  std::string signName = "signZpMass_" + outName.substr(11);
  std::string sideNameCB = "sideZpMass_constBin_" + outName.substr(11);
  std::string signNameCB = "signZpMass_constBin_" + outName.substr(11);

  TFile* outFile = new TFile("sideBkgEle.root", "update");

  h_sideZprimeMass->Write(sideName.data());
  h_signZprimeMass->Write(signName.data());
  h_sideZprimeMass_constBin->Write(sideNameCB.data());
  h_signZprimeMass_constBin->Write(signNameCB.data());

  outFile->Write();
  
}
