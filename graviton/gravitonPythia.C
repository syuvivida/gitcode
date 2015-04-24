#include <iostream>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "untuplizer.h"

void gravitonPythia(){

  gStyle->SetOptStat(0);

  TreeReader data("pythia8_RS_WW.root");

  TH1F* h_cosTh = new TH1F("h_cosTh","RS Graviton by Pythia8",100,-1,1);
  h_cosTh->SetMinimum(0);
  h_cosTh->Sumw2();

  TH1F *h_cosThStar = (TH1F*)h_cosTh->Clone("h_cosThStar");
  
  for(Long64_t ev = 0 ; ev < data.GetEntriesFast(); ev++){

    data.GetEntry(ev);

    Int_t nGenPar = data.GetInt("nGenPar"); 
    Int_t* genParId = data.GetPtrInt("genParId");
    Int_t* genParSt = data.GetPtrInt("genParSt");
    Float_t* genParPt = data.GetPtrFloat("genParPt");
    Float_t* genParEta = data.GetPtrFloat("genParEta");
    Float_t* genParPhi = data.GetPtrFloat("genParPhi");
    Float_t* genParM = data.GetPtrFloat("genParM");


    // cos#theta_1 in the W rest frame
      
    Int_t chgLepID = -1;
    Int_t neuLepID = -1;

    TLorentzVector chgLep(0,0,0,0);
    TLorentzVector neuLep(0,0,0,0);

    for(Int_t i = 0; i < nGenPar; i++){

      if( genParSt[i] != 23 ) continue; 
      if( abs(genParId[i]) == 11 || abs(genParId[i]) == 13 || abs(genParId[i]) == 15 ) chgLepID = i;
      if( abs(genParId[i]) == 12 || abs(genParId[i]) == 14 || abs(genParId[i]) == 16 ) neuLepID = i;

      chgLep.SetPtEtaPhiM(genParPt[chgLepID], 
			  genParEta[chgLepID], 
			  genParPhi[chgLepID], 
			  genParM[chgLepID]);

      neuLep.SetPtEtaPhiM(genParPt[neuLepID], 
			  genParEta[neuLepID], 
			  genParPhi[neuLepID], 
			  genParM[neuLepID]);

      TLorentzVector  Wb = chgLep + neuLep;

      TVector3 WbP = Wb.Vect();
      TVector3 bv = -Wb.BoostVector();

      chgLep.Boost(bv);
    
      TVector3 chgLepP = chgLep.Vect();
    
      Double_t cosTh = TMath::Cos(chgLepP.Angle(WbP));
   
      h_cosTh->Fill(cosTh);

    }



    // cos#theta* in the RSG rest frame

    Int_t WplusID = -1;
    Int_t WminusID = -1;

    TLorentzVector Wplus(0,0,0,0);
    TLorentzVector Wminus(0,0,0,0);

    for(Int_t i = 0; i < nGenPar; i++){

      if( genParSt[i] != 22 ) continue;
      if( genParId[i] == +24 ) WplusID = i;
      if( genParId[i] == -24 ) WminusID = i;

      Wplus.SetPtEtaPhiM(genParPt[WplusID], 
			 genParEta[WplusID], 
			 genParPhi[WplusID], 
			 genParM[WplusID]);

      Wplus.SetPtEtaPhiM(genParPt[WminusID], 
			 genParEta[WminusID], 
			 genParPhi[WminusID], 
			 genParM[WminusID]);

      TLorentzVector RSG = Wplus + Wminus;

      TVector3 RSGP = RSG.Vect();
      TVector3 gv = -RSG.BoostVector();

      Wplus.Boost(gv);

      TVector3 WplusP = Wplus.Vect();

      Double_t cosThStar = TMath::Cos(WplusP.Angle(RSGP));

      h_cosThStar->Fill(cosThStar);

    }


  }

  TCanvas* c = new TCanvas("c","",0,0,1000,600);
  c->Divide(2,1);

  c->cd(1); 
  h_cosTh->SetXTitle("cos#theta_1 in the W rest frame");
  h_cosTh->Draw();

  c->cd(2); 
  h_cosThStar->SetXTitle("cos#theta* in the RSG rest frame");
  h_cosThStar->Draw();

  c->Print("RSgravitonPythia.pdf");
    
}
