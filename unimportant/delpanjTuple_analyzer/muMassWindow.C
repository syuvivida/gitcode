#include <vector>
#include <string>
#include <sstream>
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
#include "../HEADER/reconstructZPrimeMu.C"

void reconstructZPrime(TreeReader&, Double_t*);

void muMassWindow(std::string inputFile, std::string outName, 
		  Double_t bgenZprimeMass){

  TreeReader data(inputFile.data());

  // Declare the histogram

  TH1D* h_IsoPt = new TH1D("h_IsoPt", "Iso/Pt", 40, 0, 0.2);
  h_IsoPt->GetXaxis()->SetTitle("Iso/Pt");
  h_IsoPt->GetYaxis()->SetTitle("Event number");

  TH1D* h_ZprimeMass = new TH1D("h_ZprimeMass", "Zprime Mass", 100, 0, 2000);
  h_ZprimeMass->GetXaxis()->SetTitle("Mass");
  h_ZprimeMass->GetYaxis()->SetTitle("Event number");


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muCorrTrkIso = data.GetPtrFloat("muCorrTrkIso");

    Int_t stRecoMuIndex, ndRecoMuIndex;

    if( !passMuonID(data, &stRecoMuIndex, &ndRecoMuIndex) )
      continue;
    

    // reconstructed Zprime mass

    Double_t ZprimeMass = 0;

    reconstructZPrime(data, &ZprimeMass);

    if(ZprimeMass <= 1e-6) continue;

    h_ZprimeMass->Fill(ZprimeMass);


    // Select Zprime mass window and fill muIsolation/mupt

    Double_t genZprimeMass = -1;

    // for background sample
    if( outName.find("DY") != std::string::npos || 
	outName.find("TT") != std::string::npos ){

      if( (ZprimeMass > bgenZprimeMass*(1-0.12)) &&  
	  (ZprimeMass < bgenZprimeMass*(1+0.12)) ){

	h_IsoPt->Fill(muCorrTrkIso[stRecoMuIndex] / muPt[stRecoMuIndex]);
	h_IsoPt->Fill(muCorrTrkIso[ndRecoMuIndex] / muPt[ndRecoMuIndex]);	

      }

    } // end of for background sample


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

    } // end of for signal sample
    
  } 

  // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::stringstream convert; 
  convert << bgenZprimeMass;
  
  std::string histoName = "MwindowTrk_cut" + convert.str() + "_" + outName.substr(11);

  TFile* outFile = new TFile("backgMwindowMu.root", "update");

  h_IsoPt->Write(histoName.data());

  outFile->Write();
  
}
