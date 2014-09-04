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
#include <TStyle.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/reconstructZPrimeELE.C"

void reconstructZPrime(TreeReader&, Double_t*);

void eleMassWindow(std::string inputFile, std::string outName, 
		   Double_t bgenZprimeMass){

  TreeReader data(inputFile.data());

  gStyle->SetOptStat(1111);

  // Declare the histogram

  TH1D* h_TrkIso = new TH1D("h_TrkIso", "Iso", 100, 0, 50);
  h_TrkIso->GetXaxis()->SetTitle("Iso");
  h_TrkIso->GetYaxis()->SetTitle("Event number");

  TH1D* h_CalIso = new TH1D("h_CalIso", "Iso", 100, 0, 50);
  h_CalIso->GetXaxis()->SetTitle("Iso");
  h_CalIso->GetYaxis()->SetTitle("Event number");

  TH1D* h_ZprimeMass = new TH1D("h_ZprimeMass", "Zprime Mass", 100, 0, 2000);
  h_ZprimeMass->GetXaxis()->SetTitle("Mass");
  h_ZprimeMass->GetYaxis()->SetTitle("Event number");


  // begin of event loop
  
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Float_t  eleRho = data.GetFloat("eleRho");
    Float_t* eleEt  = data.GetPtrFloat("eleEt");
    Float_t* eleScEta = data.GetPtrFloat("eleScEta");
    Float_t* eleUserTrkIso = data.GetPtrFloat("eleUserTrkIso");
    Float_t* eleUserCalIso = data.GetPtrFloat("eleUserCalIso");

    Int_t stRecoEleIndex, ndRecoEleIndex;

    if( !passElectronID(data, &stRecoEleIndex, &ndRecoEleIndex) )
      continue;


    // reconstructed Zprime mass

    Double_t ZprimeMass = 0;

    reconstructZPrime(data, &ZprimeMass);

    if(ZprimeMass <= 1e-6) continue;

    h_ZprimeMass->Fill(ZprimeMass);


    // Select Zprime mass window and fill eleIsolation

    Double_t genZprimeMass = 0;


    // barrel selections
    if( (fabs(eleScEta[stRecoEleIndex]) > 0 && fabs(eleScEta[stRecoEleIndex]) < 1.4442) ||
	(fabs(eleScEta[ndRecoEleIndex]) > 0 && fabs(eleScEta[ndRecoEleIndex]) < 1.4442) ){

      // for background sample
      if( outName.find("DY") != std::string::npos || 
	  outName.find("TT") != std::string::npos ){

	if( (ZprimeMass > bgenZprimeMass*(1-0.12)) &&  
	    (ZprimeMass < bgenZprimeMass*(1+0.12)) ){

	  h_TrkIso->Fill(eleUserTrkIso[stRecoEleIndex]-(0.03*eleEt[stRecoEleIndex]));
	  h_TrkIso->Fill(eleUserTrkIso[ndRecoEleIndex]-(0.03*eleEt[ndRecoEleIndex]));

	  h_CalIso->Fill(eleUserCalIso[stRecoEleIndex]-(0.06205*eleRho)-(0.03*eleEt[stRecoEleIndex]));
	  h_CalIso->Fill(eleUserCalIso[ndRecoEleIndex]-(0.06205*eleRho)-(0.03*eleEt[ndRecoEleIndex]));

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

	  h_TrkIso->Fill(eleUserTrkIso[stRecoEleIndex]-(0.03*eleEt[stRecoEleIndex]));
	  h_TrkIso->Fill(eleUserTrkIso[ndRecoEleIndex]-(0.03*eleEt[stRecoEleIndex]));

	  h_CalIso->Fill(eleUserCalIso[stRecoEleIndex]-(0.06205*eleRho)-(0.03*eleEt[stRecoEleIndex]));
	  h_CalIso->Fill(eleUserCalIso[ndRecoEleIndex]-(0.06205*eleRho)-(0.03*eleEt[ndRecoEleIndex]));

	}

      } // end of for signal sample

    } // end of barrel selection

  } 

  // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::stringstream convert; 
  convert << bgenZprimeMass;
  
  std::string trkHistoName = "MwindowTrk_cut" + convert.str() + "_" + outName.substr(11);
  std::string calHistoName = "MwindowCal_cut" + convert.str() + "_" + outName.substr(11);

  TFile* outFile = new TFile("backgMwindowEle.root", "update");

  h_TrkIso->Write(trkHistoName.data());
  h_CalIso->Write(calHistoName.data());

  outFile->Write();
  
}
