#include <vector>
#include <string>
#include <iostream>
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
#include "../HEADER/untuplizer.h"
#include "../HEADER/specificLeptonPt.C"

void specificLeptonPt(TreeReader&, Int_t*, Int_t*, Int_t*, Int_t*);

void reconstructEff13TeV(){

  gStyle->SetOptStat(0);

  TreeReader data("../../tree_RSGravToZZ_kMpl01_M-4500_Tune4C_13TeV-pythia8_Phys14DR.root");

  // Declare the histogram

  TH1D* h_deltaRtemplate = new TH1D("h_deltaRtemplate", "", 50, 0, 2);

  TH1D* h_DeltaR_denom  = (TH1D*)h_deltaRtemplate->Clone("h_DeltaR_denom");
  TH1D* h_DeltaR_glbglb = (TH1D*)h_deltaRtemplate->Clone("h_DeltaR_glbglb");
  TH1D* h_DeltaR_glbtrc = (TH1D*)h_deltaRtemplate->Clone("h_DeltaR_glbtrc");
  TH1D* h_DeltaR_trctrc = (TH1D*)h_deltaRtemplate->Clone("h_DeltaR_trctrc");

  TH1D* h_efftemplate = new TH1D("h_efftemplate", "", 50, 0, 2);
  h_efftemplate->GetXaxis()->SetTitle("DeltaR");
  h_efftemplate->GetYaxis()->SetTitle("Efficiency");

  TH1D* h_reconstrcEff_glbglb = (TH1D*)h_efftemplate->Clone("h_reconstrcEff_glbglb");
  TH1D* h_reconstrcEff_glbtrc = (TH1D*)h_efftemplate->Clone("h_reconstrcEff_glbtrc");
  TH1D* h_reconstrcEff_trctrc = (TH1D*)h_efftemplate->Clone("h_reconstrcEff_trctrc");

  h_reconstrcEff_glbglb->Sumw2();
  h_reconstrcEff_glbtrc->Sumw2();
  h_reconstrcEff_trctrc->Sumw2();

  h_reconstrcEff_glbglb->SetTitle("Efficiency_glbglb");
  h_reconstrcEff_glbtrc->SetTitle("Efficiency_glbtrc");
  h_reconstrcEff_trctrc->SetTitle("Efficiency_trctrc");

  int count = 0;

  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);


    Float_t* elePt  = data.GetPtrFloat("elePt");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    Int_t stMuPtIndex  = -1;
    Int_t ndMuPtIndex  = -1;
    Int_t stElePtIndex = -1;
    Int_t ndElePtIndex = -1;

    specificLeptonPt(data, &stMuPtIndex, &ndMuPtIndex, 
		     &stElePtIndex, &ndElePtIndex);

    if( (stMuPtIndex  < 0 || ndMuPtIndex  < 0 ) && 
	(stElePtIndex < 0 || ndElePtIndex < 0 )  ) continue; 
  
    if( stMuPtIndex > 0 && stElePtIndex > 0 ){
    
      if( (muPt[stMuPtIndex] - elePt[stElePtIndex]) < 1e-6 ) 
	continue;

    }


    // to pass the generator muons

    Int_t    nGenPar     = data.GetInt("nGenPar"); 
    Int_t*   genParId    = data.GetPtrInt("genParId");
    Int_t*   genMomParId = data.GetPtrInt("genMomParId");
    Int_t*   genParSt    = data.GetPtrInt("genParSt");
    Float_t* genParPt    = data.GetPtrFloat("genParPt");
    Float_t* genParEta   = data.GetPtrFloat("genParEta");
    Float_t* genParPhi   = data.GetPtrFloat("genParPhi");
    Float_t* genParM     = data.GetPtrFloat("genParM");
      
    Int_t genMuonID = -1;
    Int_t genAntiMuonID = -1;

    for(Int_t i = 0; i < nGenPar; i++){

      if( (genMomParId[i] == 23 || abs(genMomParId[i]) == 13) && genParId[i] == +13 && genParSt[i] == 1) 
	genMuonID = i;

      if( (genMomParId[i] == 23 || abs(genMomParId[i]) == 13) && genParId[i] == -13 && genParSt[i] == 1) 
	genAntiMuonID = i; 

      if( genMuonID > -1 && genAntiMuonID > -1 ) break;
    
    }

    if( genMuonID < 0 || genAntiMuonID < 0 ) continue;


    // Pass basic cut

    if( genParPt[genMuonID] <= 20 || genParPt[genAntiMuonID] <= 20 ) continue; 
    if( fabs(genParEta[genMuonID]) >= 2.4 || fabs(genParEta[genAntiMuonID]) >= 2.4 ) continue;

    count++;

    TLorentzVector genMuon, genAntiMuon;

    genMuon.SetPtEtaPhiM(genParPt[genMuonID], genParEta[genMuonID], genParPhi[genMuonID], genParM[genMuonID]);
    genAntiMuon.SetPtEtaPhiM(genParPt[genAntiMuonID], genParEta[genAntiMuonID], genParPhi[genAntiMuonID], genParM[genAntiMuonID]);

    h_DeltaR_denom->Fill(genMuon.DeltaR(genAntiMuon));


    // to check if reconstructed muon match generator muon
    
    TLorentzVector reco_stMuon, reco_ndMuon;

    reco_stMuon.SetPtEtaPhiM(muPt[stMuPtIndex], muEta[stMuPtIndex], muPhi[stMuPtIndex], muM[stMuPtIndex]);
    reco_ndMuon.SetPtEtaPhiM(muPt[ndMuPtIndex], muEta[ndMuPtIndex], muPhi[ndMuPtIndex], muM[ndMuPtIndex]);

    Double_t recostMuon_with_genMuonDeltaR = reco_stMuon.DeltaR(genMuon);
    Double_t recostMuon_with_genantiMuonDeltaR = reco_stMuon.DeltaR(genAntiMuon);
    Double_t recondMuon_with_genMuonDeltaR = reco_ndMuon.DeltaR(genMuon);
    Double_t recondMuon_with_genantiMuonDeltaR = reco_ndMuon.DeltaR(genAntiMuon);

    Int_t stRecoIndex = -1;
    Int_t ndRecoIndex = -1;
    Int_t stGenIndex = -1;
    Int_t ndGenIndex = -1;

    if( recostMuon_with_genMuonDeltaR < 0.3 || recostMuon_with_genantiMuonDeltaR < 0.3 ){
      stRecoIndex = stMuPtIndex;
      stGenIndex = genMuonID;
    }

    if( recondMuon_with_genMuonDeltaR < 0.3 || recondMuon_with_genantiMuonDeltaR < 0.3 ){
      ndRecoIndex = ndMuPtIndex;
      ndGenIndex = genAntiMuonID;
    }

    if( stRecoIndex < 0 || ndRecoIndex < 0 || 
	stGenIndex < 0  || ndGenIndex < 0  ) continue; 


    // Fill histogram with 3 cases

    TLorentzVector stPassMuon, ndPassMuon;

    stPassMuon.SetPtEtaPhiM(genParPt[stGenIndex], genParEta[stGenIndex], genParPhi[stGenIndex], genParM[stGenIndex]);
    ndPassMuon.SetPtEtaPhiM(genParPt[ndGenIndex], genParEta[ndGenIndex], genParPhi[ndGenIndex], genParM[ndGenIndex]);

    Int_t* isGlobalMuon  = data.GetPtrInt("isGlobalMuon");
    Int_t* isTrackerMuon = data.GetPtrInt("isTrackerMuon");

    if( isGlobalMuon[stRecoIndex] && isGlobalMuon[ndRecoIndex] ){  
  
      h_DeltaR_glbglb->Fill(stPassMuon.DeltaR(ndPassMuon));
      h_reconstrcEff_glbglb->Divide(h_DeltaR_glbglb, h_DeltaR_denom, 1, 1, "B");

    }

    if( isGlobalMuon[stRecoIndex] || isGlobalMuon[ndRecoIndex] ){ 
   
      h_DeltaR_glbtrc->Fill(stPassMuon.DeltaR(ndPassMuon));
      h_reconstrcEff_glbtrc->Divide(h_DeltaR_glbtrc, h_DeltaR_denom, 1, 1, "B");

    }

    if( isTrackerMuon[stRecoIndex] && isTrackerMuon[ndRecoIndex] ){

      h_DeltaR_trctrc->Fill(stPassMuon.DeltaR(ndPassMuon));
      h_reconstrcEff_trctrc->Divide(h_DeltaR_trctrc, h_DeltaR_denom, 1, 1, "B");

    }


  } // end of event loop

  fprintf(stderr, "Processed all events\n");


  cout << count << endl;
  
  TCanvas* c = new TCanvas("c", "", 0, 0, 1000, 800);
  c->Divide(2,2);

  c->cd(1); 
  h_reconstrcEff_glbglb->Draw();

  c->cd(2);
  h_reconstrcEff_glbtrc->Draw();

  c->cd(3);
  h_reconstrcEff_trctrc->Draw();


}
