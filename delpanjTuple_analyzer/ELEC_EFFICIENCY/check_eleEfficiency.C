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
#include "../HEADER/recoGenEleMatching.C"

void recoGenEleMatching(TreeReader& ,Int_t*, Int_t*, Int_t*, Int_t*);

void check_eleEfficiency(string inputFile){

  TreeReader data(inputFile.data());

  // Declare the histogram of Pt

  TH1D* h_Pt = new TH1D("h_Pt", "Transverse momentum", 100, 0, 800);
  h_Pt->GetXaxis()->SetTitle("Pt");
  h_Pt->GetYaxis()->SetTitle("Event number");

  TH1D* h_st_elePt_denom = (TH1D*)h_Pt->Clone("h_st_elePt_denom");
  TH1D* h_st_elePt_numer = (TH1D*)h_Pt->Clone("h_st_elePt_numer");
  TH1D* h_manual_st_elePt_numer = (TH1D*)h_Pt->Clone("h_manual_st_elePt_numer");

  TH1D* h_Eff_Pt = new TH1D("h_Eff_Pt", "Electron ID Efficiency", 100, 0, 800); h_Eff_Pt->Sumw2();
  h_Eff_Pt->GetXaxis()->SetTitle("Pt");
  h_Eff_Pt->GetYaxis()->SetTitle("Efficiency");

  // Declare the histogram of efficiency

  TH1D* h_IDeff_st_elePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_st_elePt");
  TH1D* h_IDeff_manual_st_elePt = (TH1D*)h_Eff_Pt->Clone("h_IDeff_manual_st_elePt");

  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 600);
  c->Divide(3,1);


  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);
    
    Int_t* elePassID = data.GetPtrInt("elePassID");
    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* eleEta = data.GetPtrFloat("eleEta");
    Float_t* eleScEta = data.GetPtrFloat("eleScEta");

    Int_t this_firstRecoIndex  = -1;
    Int_t this_secondRecoIndex = -1;			
    Int_t this_firstGenIndex   = -1;
    Int_t this_secondGenIndex  = -1;

    recoGenEleMatching(data, &this_firstRecoIndex, &this_secondRecoIndex, 
		       &this_firstGenIndex, &this_secondGenIndex);


    // highest electron pt

    Int_t*   elePassConv = data.GetPtrInt("elePassConv");
    Float_t* eleMissingHits = data.GetPtrFloat("eleMissingHits");
    Float_t* eleDelEtaIn = data.GetPtrFloat("eleDelEtaIn");
    Float_t* eleDelPhiIn = data.GetPtrFloat("eleDelPhiIn");
    Float_t* eleSigIhIh  = data.GetPtrFloat("eleSigIhIh");
    Float_t* eleHoE    = data.GetPtrFloat("eleHoE");
    Float_t* eleDxy    = data.GetPtrFloat("eleDxy");
    Float_t* eleDz     = data.GetPtrFloat("eleDz");
    Float_t* eleEoverP = data.GetPtrFloat("eleEoverP");


    // barrel selections
    if( fabs(eleScEta[this_firstRecoIndex]) > 0 && fabs(eleScEta[this_firstRecoIndex]) < 1.442 ){

      h_st_elePt_denom->Fill(elePt[this_firstRecoIndex]);

      // electron (auto pass function)
      if( elePassID[this_firstRecoIndex] > 0 )
	h_st_elePt_numer->Fill(elePt[this_firstRecoIndex]);

      c->cd(1);
      h_IDeff_st_elePt->Divide(h_st_elePt_numer, h_st_elePt_denom, 1, 1, "B");
      h_IDeff_st_elePt->Draw();

      // electron (manual pass function) // barrel cut
      if( eleDelEtaIn[this_firstRecoIndex] >= 0.004 ) continue;
      if( eleDelPhiIn[this_firstRecoIndex] >= 0.03) continue;
      if( eleSigIhIh[this_firstRecoIndex] >= 0.01 ) continue;
      if( eleHoE[this_firstRecoIndex] >= 0.12 ) continue;
      if( eleDxy[this_firstRecoIndex] >= 0.02 ) continue;
      if( eleDz[this_firstRecoIndex] >= 0.1 ) continue;
      if( eleEoverP[this_firstRecoIndex] >= 0.05 ) continue;
      if( elePassConv[this_firstRecoIndex] < 1e-6 ) continue;
      if( eleMissingHits[this_firstRecoIndex] > 0 ) continue;
      if( elePt[this_firstRecoIndex] <= 10 ) continue;
      if( fabs(eleEta[this_firstRecoIndex]) >= 2.5 ) continue;

      h_manual_st_elePt_numer->Fill(elePt[this_firstRecoIndex]);

      c->cd(2);
      h_IDeff_manual_st_elePt->Divide(h_manual_st_elePt_numer, h_st_elePt_denom, 1, 1, "B");
      h_IDeff_manual_st_elePt->Draw();

    }

    c->cd(3);
    h_IDeff_st_elePt->Draw(); 
    h_IDeff_manual_st_elePt->SetLineColor(2);
    h_IDeff_manual_st_elePt->Draw("same");


  } 

  // end of event loop

  fprintf(stderr, "Processed all events\n");

  c->Print("checkEleIDEff.gif");

}
