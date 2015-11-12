#include <string>
#include <iostream>
#include <TH1F.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "HEADER/untuplizer.h"
#include "HEADER/passMuonID.C"
#include "HEADER/passElectronID.C"
#include "HEADER/loadingbar.C"

void reconstructZMass(){

  TreeReader data("/home/henry/Desktop/work_area/XtoZH_study/delpanjTuple_background/DY/delpanj_v4_DYJetsToLL_PtZ-100.root");

  TH1F* h_ZM = new TH1F("h_ZM", "Z mass in muon channel",100, 40, 140);
  TH1F* h_ZMmu = (TH1F*)h_ZM->Clone("h_ZMmu");

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    printProgBar(ev/(Double_t)data.GetEntriesFast()*100);

    data.GetEntry(ev);

    Int_t nMu = data.GetInt("nMu");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    
    // no pass selection
    
    if( nMu > 1 ){
	
      TLorentzVector mu1(0,0,0,0);
      TLorentzVector mu2(0,0,0,0);
      TLorentzVector Zmu(0,0,0,0);

      mu1.SetPtEtaPhiM(muPt[0],muEta[0],muPhi[0],muM[0]);
      mu2.SetPtEtaPhiM(muPt[1],muEta[1],muPhi[1],muM[1]);
      
      Zmu = mu1 + mu2;
      h_ZMmu->Fill(Zmu.M());
	
    }
      
    // pass selection

    Int_t stId = -1;
    Int_t ndId = -1;
      
    if( passMuonID(data,&stId,&ndId) ){

      TLorentzVector lep1(0,0,0,0);
      TLorentzVector lep2(0,0,0,0);
      TLorentzVector Z(0,0,0,0);

      lep1.SetPtEtaPhiM(muPt[stId],muEta[stId],muPhi[stId],muM[stId]);
      lep2.SetPtEtaPhiM(muPt[ndId],muEta[ndId],muPhi[ndId],muM[ndId]);
    
      Z = lep1 + lep2;
      h_ZM->Fill(Z.M());

    }

  } // end of event loop

  std::cout << "efficiency: " << h_ZM->Integral(1,99)/h_ZMmu->Integral(1,99)*100 << std::endl;

  h_ZM->SetXTitle("mass [GeV]");
  h_ZM->SetYTitle("event number");
  h_ZM->SetFillColor(kCyan);

  h_ZMmu->Draw();
  h_ZM->Draw("same");
 
}

/*
  TLorentzVector mu1(0,0,0,0);
  TLorentzVector mu2(0,0,0,0);
  TLorentzVector Zmu(0,0,0,0);
      
  for(Int_t i = 0; i < nMu; i++){
  for(Int_t j = 0; j < i; j++){

  mu1.SetPtEtaPhiM(muPt[i],muEta[i],muPhi[i],muM[i]);
  mu2.SetPtEtaPhiM(muPt[j],muEta[j],muPhi[j],muM[j]);

  Zmu = mu1 + mu2;
	  
  }
  }
*/
