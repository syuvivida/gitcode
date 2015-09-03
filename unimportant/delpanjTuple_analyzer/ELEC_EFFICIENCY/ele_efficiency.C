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
#include "untuplizer.h"
#include "specific_leptonPt.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);

void ele_efficiency(string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of Pt

  TH1D* h_Pt = new TH1D("h_Pt", "Transverse momentum", 40, 0, 800);
  h_Pt->GetXaxis()->SetTitle("Pt");
  h_Pt->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_barelePt_denom = (TH1D*)h_Pt->Clone("h_st_barelePt_denom");
  TH1D* h_st_barelePt_numer = (TH1D*)h_Pt->Clone("h_st_barelePt_numer");
  TH1D* h_nd_barelePt_denom = (TH1D*)h_Pt->Clone("h_nd_barelePt_denom");
  TH1D* h_nd_barelePt_numer = (TH1D*)h_Pt->Clone("h_nd_barelePt_numer");

  TH1D* h_st_endelePt_denom = (TH1D*)h_Pt->Clone("h_st_endelePt_denom");
  TH1D* h_st_endelePt_numer = (TH1D*)h_Pt->Clone("h_st_endelePt_numer");
  TH1D* h_nd_endelePt_denom = (TH1D*)h_Pt->Clone("h_nd_endelePt_denom");
  TH1D* h_nd_endelePt_numer = (TH1D*)h_Pt->Clone("h_nd_endelePt_numer");

  // Declare the histogram of efficiency

  TH1D* h_Eff_Pt = new TH1D("h_Eff_Pt", "Efficiency", 40, 0, 800); h_Eff_Pt->Sumw2();
  h_Eff_Pt->GetXaxis()->SetTitle("Pt");
  h_Eff_Pt->GetYaxis()->SetTitle("Efficiency");

  TH1D* h_IDeff_st_barelePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_barelePt");
  TH1D* h_IDeff_nd_barelePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_nd_barelePt");

  TH1D* h_IDeff_st_endelePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_endelePt");
  TH1D* h_IDeff_nd_endelePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_nd_endelePt");

  // Declare the histogram of Eta

  TH1D* h_Eta = new TH1D("h_Eta", "Eta", 100, -4, 4);
  h_Eta->GetXaxis()->SetTitle("Eta");
  h_Eta->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_bareleEta_denom = (TH1D*)h_Eta->Clone("h_st_bareleEta_denom");
  TH1D* h_st_bareleEta_numer = (TH1D*)h_Eta->Clone("h_st_bareleEta_numer");
  TH1D* h_nd_bareleEta_denom = (TH1D*)h_Eta->Clone("h_nd_bareleEta_denom");
  TH1D* h_nd_bareleEta_numer = (TH1D*)h_Eta->Clone("h_nd_bareleEta_numer");

  TH1D* h_st_endeleEta_denom = (TH1D*)h_Eta->Clone("h_st_endeleEta_denom");
  TH1D* h_st_endeleEta_numer = (TH1D*)h_Eta->Clone("h_st_endeleEta_numer");
  TH1D* h_nd_endeleEta_denom = (TH1D*)h_Eta->Clone("h_nd_endeleEta_denom");
  TH1D* h_nd_endeleEta_numer = (TH1D*)h_Eta->Clone("h_nd_endeleEta_numer");

  // Declare the histogram of efficiency

  TH1D* h_Eff_Eta = new TH1D("h_Eff_Eta", "Efficiency", 100, -4, 4); h_Eff_Eta->Sumw2();
  h_Eff_Eta->GetXaxis()->SetTitle("Eta");
  h_Eff_Eta->GetYaxis()->SetTitle("Efficiency");

  TH1D* h_IDeff_st_bareleEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_st_bareleEta");
  TH1D* h_IDeff_nd_bareleEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_nd_bareleEta");

  TH1D* h_IDeff_st_endeleEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_st_endeleEta");
  TH1D* h_IDeff_nd_endeleEta = (TH1D*)h_Eff_Eta->Clone("h_IDeff_nd_endeleEta");

  //________________________________________________________________________________________//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){ // begin of event loop

    // print progress
    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    //__________________________________________________________
    // choosing the primary reconstructed muon

    Int_t* elePassID  = data.GetPtrInt("elePassID");
    Float_t* elePt    = data.GetPtrFloat("elePt");
    Float_t* eleEta   = data.GetPtrFloat("eleEta");
    Float_t* eleScEta = data.GetPtrFloat("eleScEta");
    Float_t* muPt     = data.GetPtrFloat("muPt");

    Int_t max_muPt_index     = -1;
    Int_t second_muPt_index  = -1;
    Int_t max_elePt_index    = -1;
    Int_t second_elePt_index = -1;

    specificLeptonPt(data, &max_muPt_index, &second_muPt_index, 
		     &max_elePt_index, &second_elePt_index);

    if( (max_muPt_index < 0 || second_muPt_index < 0 ) && 
	(max_elePt_index < 0 || second_elePt_index < 0 ) continue;

    if( (elePt[max_elePt_index] - muPt[max_muPt_index]) < 1e-6 ) continue;

    //__________________________________________________________
    // barrel electron

    if( fabs(eleScEta[max_elePt_index]) > 0 && fabs(eleScEta[max_elePt_index]) < 1.442 ){

      // highest electron pt

      h_st_barelePt_denom ->Fill(elePt[max_elePt_index]);
      h_st_bareleEta_denom->Fill(eleEta[max_elePt_index]);

      if( elePassID[max_elePt_index] > 0 ){

	h_st_barelePt_numer ->Fill(elePt[max_elePt_index]);
	h_st_bareleEta_numer->Fill(eleEta[max_elePt_index]);

      }
     
      h_IDeff_st_barelePt ->Divide(h_st_barelePt_numer,  h_st_barelePt_denom,  1, 1, "B");
      h_IDeff_st_bareleEta->Divide(h_st_bareleEta_numer, h_st_bareleEta_denom, 1, 1, "B");

      // second highest electron pt

      h_nd_barelePt_denom ->Fill(elePt[second_elePt_index]);
      h_nd_bareleEta_denom->Fill(eleEta[second_elePt_index]);

      if( elePassID[second_elePt_index] > 0 ){

	h_nd_barelePt_numer ->Fill(elePt[second_elePt_index]);
	h_nd_bareleEta_numer->Fill(eleEta[second_elePt_index]);

      }  
    
      h_IDeff_nd_barelePt ->Divide(h_nd_barelePt_numer,  h_nd_barelePt_denom,  1, 1, "B");
      h_IDeff_nd_bareleEta->Divide(h_nd_bareleEta_numer, h_nd_bareleEta_denom, 1, 1, "B");

    }

    //__________________________________________________________
    // endcap electron

    if( fabs(eleScEta[max_elePt_index]) > 1.566 && fabs(eleScEta[max_elePt_index]) < 2.5 ){
    
      // highest electron pt

      h_st_endelePt_denom ->Fill(elePt[max_elePt_index]);
      h_st_endeleEta_denom->Fill(eleEta[max_elePt_index]);

      if( elePassID[max_elePt_index] > 0 ){

	h_st_endelePt_numer ->Fill(elePt[max_elePt_index]);
	h_st_endeleEta_numer->Fill(eleEta[max_elePt_index]);

      }
     
      h_IDeff_st_endelePt ->Divide(h_st_endelePt_numer,  h_st_endelePt_denom,  1, 1, "B");
      h_IDeff_st_endeleEta->Divide(h_st_endeleEta_numer, h_st_endeleEta_denom, 1, 1, "B");

      // second highest electron pt

      h_nd_endelePt_denom ->Fill(elePt[second_elePt_index]);
      h_nd_endeleEta_denom->Fill(eleEta[second_elePt_index]);

      if( elePassID[second_elePt_index] > 0 ){

	h_nd_endelePt_numer ->Fill(elePt[second_elePt_index]);
	h_nd_endeleEta_numer->Fill(eleEta[second_elePt_index]);

      }  
    
      h_IDeff_nd_endelePt ->Divide(h_nd_endelePt_numer,  h_nd_endelePt_denom,  1, 1, "B");
      h_IDeff_nd_endeleEta->Divide(h_nd_endeleEta_numer, h_nd_endeleEta_denom, 1, 1, "B");

    }

    //__________________________________________________________

  } // end of event loop


  fprintf(stderr, "Processed all events\n");

  std::string outName = "electronIDeff_" + inputFile;
  TFile* outFile = new TFile(outName.data(), "recreate");

  h_IDeff_st_barelePt ->Write();
  h_IDeff_nd_barelePt ->Write();
  h_IDeff_st_endelePt ->Write();
  h_IDeff_nd_endelePt ->Write();

  h_IDeff_st_bareleEta->Write();
  h_IDeff_nd_bareleEta->Write();
  h_IDeff_st_endeleEta->Write();
  h_IDeff_nd_endeleEta->Write();

  outFile->Write();

}
