#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include "../../HEADER/untuplizer.h"
#include "../../HEADER/passMuonID.C"
#include "../../HEADER/passJetID.h"
#include "../../HEADER/loadingbar.C"

// csvl=> 0:no b-tag or tau21 cuts, 1:b-tagging only, 2:tau21 only

void step1(std::string inputFile, Int_t csvlMode, Int_t scaleMode){

  if( abs(scaleMode) > 1 ) return;

  TreeReader data(inputFile.data());

  inputFile = inputFile.erase(inputFile.find(".root"));
  inputFile = inputFile.substr(inputFile.find("v4")+3);

  // variable bin
  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,1250,1400,1600,1800,2000,2400};
  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  TH1D* h_ZpMass[2]; // [side-band][signal]

  h_ZpMass[0] = new TH1D("h_ZpMass0", "Side-band region Zprime Mass", nvarBins, varBins);
  h_ZpMass[1] = new TH1D("h_ZpMass1", "Signal region Zprime Mass", nvarBins, varBins);

  std::string textFile;
  
  if( inputFile.find("data") != std::string::npos )
    textFile = "../../HEADER/FT_53_V21_AN4_Uncertainty_AK7PFchs.txt";
  else
    textFile = "../../HEADER/START53_V23_Uncertainty_AK7PFchs.txt";

  corrJetV corrJet(textFile);
  
  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    data.GetEntry(ev);

    printProgBar(ev/(Double_t)data.GetEntriesFast()*100);

    Float_t* CA8jetPrunedMass = data.GetPtrFloat("CA8jetPrunedMass");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    // data trigger cut (muon)
    if ( inputFile.find("DoubleMu") != std::string::npos ){
      std::string* trigName = data.GetPtrString("hlt_trigName");
      Int_t* trigResult = data.GetPtrInt("hlt_trigResult");
      const Int_t nsize = data.GetPtrStringSize();
      Bool_t passTrigger = false;
      for(int it = 0; it < nsize; it++){
	std::string thisTrig = trigName[it];
	Int_t results = trigResult[it];
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
    TLorentzVector stRecoMu(0,0,0,0);
    TLorentzVector ndRecoMu(0,0,0,0);  
 
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
    Int_t maxJetIndex = -1;
    TLorentzVector tempVector(0,0,0,0);

    if( !passJetID(data, corrJet, csvlMode, scaleMode, &maxJetIndex, &tempVector) )
      continue;
 
    TLorentzVector Higgs = tempVector;
    TLorentzVector Zprime = Z + Higgs;

    if( Z.E() <= 0.  || Higgs.E() <= 0. ) continue;
    if( Z.M() <= 70. || Z.M() >= 110. ) continue;
    if( Z.Pt() <= 80. ) continue;

    if( CA8jetPrunedMass[maxJetIndex] > 50 && CA8jetPrunedMass[maxJetIndex] < 110 )
      h_ZpMass[0]->Fill(Zprime.M());

    if( CA8jetPrunedMass[maxJetIndex] > 110 && CA8jetPrunedMass[maxJetIndex] < 140 )
      h_ZpMass[1]->Fill(Zprime.M());
    
  } // end of event loop

  std::cout << "\nProcessed all events\n";

  TFile* outFile[2];
  
  outFile[0] = new TFile("SideZpMMu.root", "update");
  h_ZpMass[0]->Write((Form("side_MX_%d_",scaleMode)+inputFile).data());

  outFile[1] = new TFile("SignalZpMMu.root", "update");
  h_ZpMass[1]->Write((Form("signal_MX_%d_",scaleMode)+inputFile).data());

  outFile[0]->Write();
  outFile[1]->Write();
  
}
