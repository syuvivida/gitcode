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
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "../HEADER/untuplizer.h"
#include "../HEADER/reconstructZPrimeMu.C"

Double_t fitFunc(Double_t*, Double_t*);
void reconstructZPrime(TreeReader&, Double_t*, Double_t*);

void ZPrimeMass(std::string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of reconstructed mass

  TH1D* h_Zprime_Mass = new TH1D("h_Zprime_Mass", "Reconstructed Zprime mass", 100, 0, 2500);
  h_Zprime_Mass->GetXaxis()->SetTitle("Mass");
  h_Zprime_Mass->GetYaxis()->SetTitle("Event number");

  TH1D* h_PrunedjetMass = new TH1D("h_PrunedjetMass","",100,0,300);

  TH1D* h_genZprime_Mass = new TH1D("h_genZprime_Mass", "Generator Zprime mass", 100, 900, 2100);
  h_genZprime_Mass->GetXaxis()->SetTitle("Mass");
  h_genZprime_Mass->GetYaxis()->SetTitle("Event number");

  TH1D* h_ratioZprime_Mass = new TH1D("h_ratioZprime_Mass", "Z prime mass ratio", 100, 0, 2);
  h_ratioZprime_Mass->GetXaxis()->SetTitle("Ratio");
  h_ratioZprime_Mass->GetYaxis()->SetTitle("Event number");


  // begin of event loop
  
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){


    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);
    

    Int_t    nGenPar   = data.GetInt("nGenPar"); 
    Int_t*   genParId  = data.GetPtrInt("genParId");
    Int_t*   genParSt  = data.GetPtrInt("genParSt");
    Float_t* genParPt  = data.GetPtrFloat("genParPt");
    Float_t* genParEta = data.GetPtrFloat("genParEta");
    Float_t* genParPhi = data.GetPtrFloat("genParPhi");
    Float_t* genParM   = data.GetPtrFloat("genParM");


    //-----------------------------------------------------------------------------------//
    // choose generator level Z prime and draw its mass  

    TLorentzVector genZprime;

    for(Int_t i = 0; i < nGenPar; i++){
      
      if( genParId[i] == 1023 && genParSt[i] == 3 ){

	genZprime.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	h_genZprime_Mass->Fill(genZprime.M());

      }

    }


    //-----------------------------------------------------------------------------------//

    Double_t ZprimeMass = -1;
    Double_t PrunedjetMass = -1;

    reconstructZPrime(data, &ZprimeMass, &PrunedjetMass);

    if( ZprimeMass < 1e-6 || PrunedjetMass < 1e-6 ) continue;

    h_Zprime_Mass->Fill(ZprimeMass);  
    h_PrunedjetMass->Fill(PrunedjetMass);

    //-----------------------------------------------------------------------------------//
    // to check the width of Zprime mass at reconstructed level 

    h_ratioZprime_Mass->Fill(ZprimeMass/genZprime.M());

    
  } 

  // end of event loop


  // Fitting the mass ratio
  
  TF1* fitRatio = new TF1("fitRatio", fitFunc, 0.8, 1.2, 3);
  fitRatio->SetParameters(100, 0.9, 0.2);
  fitRatio->SetParNames("Amplitude", "Mean", "Sigma");

  h_ratioZprime_Mass->Fit("fitRatio", "","", 0.8, 1.2);
 
  fprintf(stderr, "Processed all events\n");

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 760);
  c->Divide(2,2);

  c->cd(1);
  h_genZprime_Mass->Draw();

  c->cd(2);
  h_Zprime_Mass->Draw();

  c->cd(3);
  h_ratioZprime_Mass->Draw();

  c->cd(4);
  h_PrunedjetMass->Draw();

  
}

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Gaus(x,par[1],par[2]);

}
