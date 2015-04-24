#include <iostream>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "untuplizer.h"

void gravitonPythia(std::string fileName){

  gStyle->SetOptStat(0);

  TreeReader data(fileName.data());

  TH1F* h_cosTh = new TH1F("h_cosTh","RS Graviton by Pythia8",100,-1,1);
  
  h_cosTh->SetXTitle("cos#theta_1 in the W rest frame");
  h_cosTh->SetMinimum(0);
  h_cosTh->Sumw2();

  for (Long64_t ev = 0 ; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev+1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t nGenPar = data.GetInt("nGenPar"); 
    Int_t* genMo1 = data.GetPtrInt("genMo1");
    Int_t* genParId = data.GetPtrInt("genParId");
    Int_t* genMomParId = data.GetPtrInt("genMomParId");
    Int_t* genParSt = data.GetPtrInt("genParSt");
    Float_t* genParPt = data.GetPtrFloat("genParPt");
    Float_t* genParEta = data.GetPtrFloat("genParEta");
    Float_t* genParPhi = data.GetPtrFloat("genParPhi");
    Float_t* genParM = data.GetPtrFloat("genParM");
      
    Int_t chgLepID = -1;
    Int_t WbID = -1;

    for(Int_t i = 0; i < nGenPar; i++){

      if( abs(genParId[i])==11 || abs(genParId[i])==13 || abs(genParId[i])==15 ) 
	
	chgLepID = i;

      if( (abs(genParId[i])==11 || abs(genParId[i])==13 || abs(genParId[i])==15 || 
	   abs(genParId[i])==12 || abs(genParId[i])==14 || abs(genParId[i])==16 ) 
	  && abs(genMomParId[i])==24 )

	WbID = genMo1[i];

      if( genParSt[chgLepID]!=23 || genParSt[WbID]!=22 ) continue;

    }

    TLorentzVector chgLep;
    TLorentzVector Wb;

    chgLep.SetPtEtaPhiM(genParPt[chgLepID], 
			genParEta[chgLepID], 
			genParPhi[chgLepID], 
			genParM[chgLepID]);

    Wb.SetPtEtaPhiM(genParPt[WbID], 
		    genParEta[WbID], 
		    genParPhi[WbID], 
		    genParM[WbID]);


    TVector3 WbP = Wb.Vect();
    TVector3 bv = -Wb.BoostVector();

    Wb.Boost(bv);
    chgLep.Boost(bv);
    
    TVector3 chgLepP = chgLep.Vect();
    
    Double_t cosTh = TMath::Cos(chgLepP.Angle(WbP));
   
    h_cosTh->Fill(cosTh);

  }

  TCanvas* c = new TCanvas("c","",0,0,1024,768);
  c->cd();

  h_cosTh->Draw();

  c->Print("RSgravitonPythia.pdf");
    
}
