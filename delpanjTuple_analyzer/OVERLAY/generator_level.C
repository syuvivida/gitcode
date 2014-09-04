#include <vector>
#include <string>
#include <iostream>
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

void generator_level(string inputFile){

  TreeReader data(inputFile.data());                                           

  //__________________________________________________________

  TH1D* h_pt = new TH1D("h_pt", "Transverse momentum", 100, 0, 1000);
  h_pt->GetXaxis()->SetTitle("pt");
  h_pt->GetYaxis()->SetTitle("Event number");
  TH1D* h_pt_z_prime  = (TH1D*)h_pt->Clone("h_pt_z_prime");
  TH1D* h_pt_sm_higgs = (TH1D*)h_pt->Clone("h_pt_sm_higgs");
  TH1D* h_pt_sm_z     = (TH1D*)h_pt->Clone("h_pt_sm_z");

  //__________________________________________________________

  TH1D* h_rapidity  =new TH1D("h_rapidity", "Rapidity", 100, -4, 4);
  h_rapidity->GetXaxis()->SetTitle("Rapidity");
  h_rapidity->GetYaxis()->SetTitle("Event number");

  //__________________________________________________________

  TH1D* h_eta = new TH1D("h_eta", "Eta", 100, -4, 4);
  h_eta->GetXaxis()->SetTitle("Eta");
  h_eta->GetYaxis()->SetTitle("Event number");
  TH1D* h_eta_sm_higgs = (TH1D*)h_eta->Clone("h_eta_sm_higgs");
  TH1D* h_eta_sm_z     = (TH1D*)h_eta->Clone("h_eta_sm_z");

  //__________________________________________________________

  TH1D* h_deltaR = new TH1D("h_deltaR", "Delta_R", 100, 0, 5);
  h_deltaR->GetXaxis()->SetTitle("Delta_R");
  h_deltaR->GetYaxis()->SetTitle("Event number");
  TH1D* h_quarks_deltaR = (TH1D*)h_deltaR->Clone("h_quarks_deltaR");
  TH1D* h_leptons_deltaR = (TH1D*)h_deltaR->Clone("h_leptons_deltaR");

  //__________________________________________________________

  /********************************** begin of event loop **********************************/

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    Int_t    nGenPar     = data.GetInt("nGenPar"); 
    Int_t*   genParId    = data.GetPtrInt("genParId");
    Int_t*   genMomParId = data.GetPtrInt("genMomParId");
    Int_t*   genParSt    = data.GetPtrInt("genParSt");
    Float_t* genParPt    = data.GetPtrFloat("genParPt");
    Float_t* genParEta   = data.GetPtrFloat("genParEta");
    Float_t* genParPhi   = data.GetPtrFloat("genParPhi");
    Float_t* genParM     = data.GetPtrFloat("genParM");
   
    //----------------- plot variables of each object at generator level -----------------//

    // choose generator level object
    TLorentzVector z_prime;
    TLorentzVector quark, anti_quark;
    TLorentzVector lepton, anti_lepton;

    Bool_t quark_state = false;
    Bool_t anti_quark_state = false;

    Bool_t lepton_state = false;
    Bool_t anti_lepton_state = false;

    for(Int_t i = 0; i < nGenPar; i++){
      
      if( genParId[i] == 1023 && genParSt[i] == 3 ){
	z_prime.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	h_rapidity->Fill(z_prime.Rapidity());
        h_pt_z_prime->Fill(genParPt[i]);   
      }

      if( genParId[i] == 25 && genMomParId[i] == 1023 && genParSt[i] == 3 ){
	h_eta_sm_higgs->Fill(genParEta[i]);
 	h_pt_sm_higgs->Fill(genParPt[i]); 
      } 

      if( genParId[i] == 23 && genMomParId[i] == 1023 && genParSt[i] == 3 ){  
	h_eta_sm_z->Fill(genParEta[i]);
	h_pt_sm_z->Fill(genParPt[i]);
      }

      //__________________________________________________________
      // this part is valid for ZH->qqll samples only... 

      if(genMomParId[i] == 25 && genParSt[i] == 3){

	if( genParId[i] >= 1 && genParId[i] <= 5 ){
	  quark.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	  quark_state = true;
	}

	else if( genParId[i] <= -1 && genParId[i] >= -5 ){
	  anti_quark.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	  anti_quark_state = true;
	}

      }

      //__________________________________________________________
      
      if(genMomParId[i] == 23 && genParSt[i] == 3){

	if( /*genParId[i] == 11 || */genParId[i] == 13 ){
	  lepton.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	  lepton_state = true;
	}

	else if( /*genParId[i] == -11 || */genParId[i] == -13 ){
	  anti_lepton.SetPtEtaPhiM(genParPt[i], genParEta[i], genParPhi[i], genParM[i]);
	  anti_lepton_state = true;
	}

      }

    }

    if( quark_state && anti_quark_state )
      h_quarks_deltaR->Fill(quark.DeltaR(anti_quark));
    
    if( lepton_state && anti_lepton_state )
      h_leptons_deltaR->Fill(lepton.DeltaR(anti_lepton));

  }

  /********************************** end of event loop **********************************/

  fprintf(stderr, "Processed all events\n");


  TFile* outFile = new TFile("genLv.root", "recreate");

  h_pt_z_prime->Write();
  h_pt_sm_higgs->Write();
  h_pt_sm_z->Write();
  h_rapidity->Write();
  h_eta_sm_higgs->Write();	
  h_eta_sm_z->Write();
  h_quarks_deltaR->Write();
  h_leptons_deltaR->Write();

  outFile->Write();

}
