#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TMath.h>
#include "untuplizer.h"

using namespace std;

void uncertainty(){
  
  const Float_t scalefactor[2][3] = {{1.0043, 1.0074, 1.022}, {1.0012, 1.0043, 1.014}};
  const Float_t uncertainty[2][3] = {{0.0004, 0.0005, 0.001}, {0.0004, 0.0004, 0.001}};

  Double_t etaX[4] = {0, 0.8, 2.1, 2.4};
  
  TH1D* h_pass[2][3];

  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 3; j++){

      h_pass[i][j] = new TH1D(Form("h_pass%d%d",i,j), "", 40, 0, 1200);

    }
  }

  TH1D* h_muPt = new TH1D("h_muPt", "", 40, 0, 1200);

  TreeReader data("delpanj_v4_AbelianZPrime_ZH_lljj_M1000.root");

  for(Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    data.GetEntry(ev);

    Int_t  nMu  = data.GetInt("nMu");
    Int_t* muPassID = data.GetPtrInt("muPassID");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");

    for(Int_t i = 0; i < nMu; i++){

      if( muPt[i] > 20 ) h_muPt->Fill(muPt[i]);
      if( muPassID[i] <= 0 ) continue;

      for(Int_t j = 1; j < 4; j++){

	if( muPt[i] > 20 && muPt[i] < 40 && fabs(muEta[i]) > etaX[j-1] && fabs(muEta[i]) < etaX[j] )
	  h_pass[0][j-1]->Fill(muPt[i]);

	if( muPt[i] > 40 && fabs(muEta[i]) > etaX[j-1] && fabs(muEta[i]) < etaX[j] ) 
	  h_pass[1][j-1]->Fill(muPt[i]);
	
      } // etaBin loop
    } // muon loop
  } // event loop

  Double_t muTotal = h_muPt->Integral();
  Double_t muPass = 0.;
  Double_t muPassPlus = 0.;

  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 3; j++){

      muPass += h_pass[i][j]->Integral()*scalefactor[i][j];
      muPassPlus += h_pass[i][j]->Integral()*(scalefactor[i][j]+uncertainty[i][j]);

    }
  }

  Double_t efficiency = muPass/muTotal;

  // 100% correlated uncertainty

  Double_t sysUncertainty = muPassPlus/muTotal - efficiency;
  
  cout << "Muon ID efficiency with 100% correlated uncertainty: " << endl;
  cout << "===== " << efficiency << " +- " << sysUncertainty << " =====" << endl;

  // 100% uncorrelated uncertainty
  
  Double_t sigmaSqr = 0.;

  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 3; j++){

      Double_t stterm = 0.;

      for(Int_t k = 0; k < 2; k++){
	for(Int_t l = 0; l < 3; l++){

	  if( k==i && l==j ) continue;
	  stterm += h_pass[k][l]->Integral()*scalefactor[k][l];

	}
      }

      Double_t ndterm = h_pass[i][j]->Integral()*(scalefactor[i][j]+uncertainty[i][j]);
      sigmaSqr += TMath::Power(((stterm + ndterm)/muTotal - efficiency), 2);

    }
  }
  
  Double_t unCorsysUncertainty = TMath::Sqrt(sigmaSqr);

  cout << "Muon ID efficiency with 100% uncorrelated uncertainty: " << endl;
  cout << "===== " << efficiency << " +- " << unCorsysUncertainty << " =====" << endl;

}
