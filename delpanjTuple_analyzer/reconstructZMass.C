#include <string>
#include <iostream>
#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "untuplizer.h"

void reconstructZMass(){

  TreeReader data("/home/henry/Desktop/work_area/XtoZH_study/delpanjTuple_background/DY/delpanj_v4_DYJetsToLL_PtZ-100.root");

  TH1F* h_ZMmu = new TH1F("h_ZMmu", "Z mass in muon channel",100, 40, 140);

  // begin of event loop
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev+1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t nMu = data.GetInt("nMu");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    TLorentzVector mu1(0,0,0,0);
    TLorentzVector mu2(0,0,0,0);
    TLorentzVector Zmu(0,0,0,0);

    for(Int_t i = 0; i < nMu; i++){
      for(Int_t j = 0; j < i; j++){

	mu1.SetPtEtaPhiM(muPt[i],muEta[i],muPhi[i],muM[i]);
	mu2.SetPtEtaPhiM(muPt[j],muEta[j],muPhi[j],muM[j]);

        Zmu = mu1 + mu2;
	h_ZMmu->Fill(Zmu.M());

      }
    }

  } // end of event loop

  h_ZMmu->SetXTitle("mass [GeV]");
  h_ZMmu->SetYTitle("event number");
  h_ZMmu->SetFillColor(kCyan);
  h_ZMmu->Draw();

}
