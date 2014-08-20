#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/reconstructZPrime.C"

void reconstructZPrime(TreeReader&, Double_t*);

void IsolationdvPt(std::string inputFile, std::string outName){

  TreeReader data(inputFile.data());

  // Declare the histogram of Delta R

  TH1D* h_DeltaR = new TH1D("h_DeltaR", "Delta R", 400, 0, 2);
  h_DeltaR->GetXaxis()->SetTitle("DeltaR");
  h_DeltaR->GetYaxis()->SetTitle("Event number");

  TH1D* h_DeltaR_beforeIsoCut = (TH1D*)h_DeltaR->Clone("h_DeltaR_beforeIsoCut");
  TH1D* h_DeltaR_TrkIsoCut    = (TH1D*)h_DeltaR->Clone("h_DeltaR_TrkIsoCut");

  TH1D* h_IsoPt = new TH1D("h_IsoPt", "Iso/Pt", 40, 0, 0.2);
  h_IsoPt->GetXaxis()->SetTitle("Iso/Pt");
  h_IsoPt->GetYaxis()->SetTitle("Event number");

  TH1D* h_ZprimeMass = new TH1D("h_ZprimeMass", "Zprime Mass", 100, 0, 2000);
  h_ZprimeMass->GetXaxis()->SetTitle("Mass");
  h_ZprimeMass->GetYaxis()->SetTitle("Event number");

  //________________________________________________________________________________________//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){ // begin of event loop

    // print progress
    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

    Int_t stRecoMuIndex = -1;
    Int_t ndRecoMuIndex = -1;

    passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex);

    if( stRecoMuIndex < 0 || ndRecoMuIndex < 0 ) continue;

    
    TLorentzVector stRecoMu, ndRecoMu;   
    stRecoMu.SetPtEtaPhiM(muPt[stRecoMuIndex], muEta[stRecoMuIndex], muPhi[stRecoMuIndex], muM[stRecoMuIndex]);    
    ndRecoMu.SetPtEtaPhiM(muPt[ndRecoMuIndex], muEta[ndRecoMuIndex], muPhi[ndRecoMuIndex], muM[ndRecoMuIndex]); 
    
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
    h_DeltaR_beforeIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
   
    // if pass muCorrTrkIso cut

    const Double_t cutValue = 0.1;
    
    if( ( muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex] ) < cutValue ) 
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));

    if( ( muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex] ) < cutValue )
      h_DeltaR_TrkIsoCut->Fill(stRecoMu.DeltaR(ndRecoMu));
    

    // reconstructed Zprime mass

    Double_t ZprimeMass = 0;

    reconstructZPrime(data, &ZprimeMass);

    if(ZprimeMass <= 1e-6) continue;

    h_ZprimeMass->Fill(ZprimeMass);


    // Select Zprime mass window and fill isolation/pt

    Double_t genZprimeMass = -1;

    // for background sample
    if( outName.find("DY") != std::string::npos || 
	outName.find("TT") != std::string::npos ){

      genZprimeMass = 2000;

      if( (ZprimeMass > genZprimeMass*(1-0.12)) &&  
	  (ZprimeMass < genZprimeMass*(1+0.12)) ){

	h_IsoPt->Fill(muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex]);
	h_IsoPt->Fill(muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex]);

	

      }

    }

    // for signal sample
    if ( outName.find("_M") != std::string::npos ){

      if( outName.find("1000") != std::string::npos )
	genZprimeMass = 1000;
      else if( outName.find("1500") != std::string::npos )
	genZprimeMass = 1500;
      else if( outName.find("2000") != std::string::npos )
	genZprimeMass = 2000;

      if( (ZprimeMass > genZprimeMass*(1-0.12)) &&  
	  (ZprimeMass < genZprimeMass*(1+0.12)) ){

	h_IsoPt->Fill(muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex]);
	h_IsoPt->Fill(muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex]);

      }

    }


    //__________________________________________________________
    
  } // end of event loop

  fprintf(stderr, "Processed all events\n");
  
  std::string histoName = "IsoPt_" + outName.substr(11);
  TFile* outFile = new TFile("backg_IsoPt.root", "update");

  h_IsoPt->Write(histoName.data());

  outFile->Write();
  
}
