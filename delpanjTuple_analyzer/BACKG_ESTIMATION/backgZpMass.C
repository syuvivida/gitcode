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
#include "../HEADER/passMuonID.C"
#include "../HEADER/passElectronID.C"
#include "../HEADER/passJetID.C"

Bool_t passMuonID(TreeReader&, Int_t*, Int_t*);
Bool_t passElectronID(TreeReader&, Int_t*, Int_t*);
Bool_t passJetID(TreeReader&, Int_t*);

void backgZpMass(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  TH1D* h_ZprimeMass = new TH1D("h_ZprimeMass", "Signal region Zprime Mass", 66, 680, 2000);
  h_ZprimeMass->GetXaxis()->SetTitle("Zprime mass");
  h_ZprimeMass->GetYaxis()->SetTitle("Event number");


  // begin of event loop
  Int_t count = 0;

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

    Int_t    nMu   = data.GetInt("nMu");
    Int_t*   muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    Int_t    nEle   = data.GetInt("nEle");
    Int_t*   elePassID = data.GetPtrInt("elePassID");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* elePhi = data.GetPtrFloat("elePhi");
    Float_t* eleM   = data.GetPtrFloat("eleM");
    

    //-----------------------------------------------------------------------------------//
    // choose the primary lepton

    Int_t stMuPtIndex, ndMuPtIndex;
    Int_t stElePtIndex, ndElePtIndex;

    passMuonID(data, &stMuPtIndex, &ndMuPtIndex);
    passElectronID(data, &stElePtIndex, &ndElePtIndex);

    Bool_t muonEvent = false;
    Bool_t electronEvent = false;
    
    if( nMu > 0 && nEle > 0 ){

      if( muPt[stMuPtIndex] > elePt[stElePtIndex] ) 
	muonEvent = true;

      else if( elePt[stElePtIndex] > muPt[stMuPtIndex] )
	electronEvent = true;

    }

    else if( nEle == 0 && nMu > 0 ) 
      muonEvent = true;

    else if( nMu == 0 && nEle > 0 )
      electronEvent = true;


    TLorentzVector stRecoLepton(0,0,0,0), ndRecoLepton(0,0,0,0), Z(0,0,0,0); 
    

    //-----------------------------------------------------------------------------------// 
    // muon event, passMuonID, reconstruct Z, recnstruct Zp in signal region


    if( muonEvent ){

      Int_t stRecoMuIndex, ndRecoMuIndex;

      if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) ) continue;
 
      stRecoLepton.SetPtEtaPhiM(muPt[stRecoMuIndex], 
				muEta[stRecoMuIndex], 
				muPhi[stRecoMuIndex],
				muM[stRecoMuIndex]);  
  
      ndRecoLepton.SetPtEtaPhiM(muPt[ndRecoMuIndex], 
				muEta[ndRecoMuIndex],
				muPhi[ndRecoMuIndex], 
				muM[ndRecoMuIndex]); 
    
      Z = stRecoLepton + ndRecoLepton;

    } // end of muon event


    //-----------------------------------------------------------------------------------// 
    // electron event, passElectronID, reconstruct Z, recnstruct Zp in signal region


    else if( electronEvent ){

      Int_t stRecoEleIndex, ndRecoEleIndex;

      if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) ) continue;

      stRecoLepton.SetPtEtaPhiM(elePt[stRecoEleIndex], 
				eleEta[stRecoEleIndex], 
				elePhi[stRecoEleIndex],
				eleM[stRecoEleIndex]);  
  
      ndRecoLepton.SetPtEtaPhiM(elePt[ndRecoEleIndex], 
				eleEta[ndRecoEleIndex],
				elePhi[ndRecoEleIndex], 
				eleM[ndRecoEleIndex]); 
    
      Z = stRecoLepton + ndRecoLepton;

    } // end of electron event

    if( Z.E() <= 0 ) continue;
    if( Z.M() <= 70 ) continue;
    if( Z.M() >= 110 ) continue;
    if( Z.Pt() <= 80 ) continue;


    //-----------------------------------------------------------------------------------// 
    // passJetID, reconstruct boosted-jet, Z and boosted-jet cut


    Int_t jetIndex;

    // this passJetID cuts tau21 and also select signal region(cut prunedmass) already
    if( !passJetID(data, &jetIndex) ) continue;


    TLorentzVector Higgs(0,0,0,0);

    Higgs.SetPtEtaPhiM(CA8jetPt[jetIndex],
		       CA8jetEta[jetIndex],
		       CA8jetPhi[jetIndex],
		       CA8jetMass[jetIndex]);

    if( Higgs.E() <= 0 ) continue;
    if( Higgs.Pt() <= 80 ) continue;

    TLorentzVector Zprime = Z + Higgs;
    h_ZprimeMass->Fill(Zprime.M());

    count++;

    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  cout << "Total pass event: " << count << endl;

  // output file
  
  std::string Name = "ZpMass_" + outName.substr(11);
  TFile* outFile = new TFile("backgZpMass.root", "update");

  h_ZprimeMass->Write(Name.data());
  outFile->Write();
  
 
}
