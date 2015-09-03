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
#include <TGraphAsymmErrors.h>
#include "../HEADER/untuplizer.h"

void reconstructEff13TeV(std::string inputFile, std::string outName){

  gStyle->SetOptStat(0);

  TreeReader data(inputFile.data());

  TGraphAsymmErrors* h_reconstrcEff[3]; // glbglb,glbtrc,trctrc
  TH1D* h_DeltaRtemp = new TH1D("h_DeltaRtemp", "", 50, 0, 2);
  TH1D* h_DeltaR[4]; // denom,glbglb,glbtrc,trctrc 

  for(Int_t i = 0; i < 4; i++){

    h_DeltaR[i] = (TH1D*)h_DeltaRtemp->Clone(Form("h_DeltaR%d",i));

  }
  
  // Begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 50000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());
    data.GetEntry(ev);

    // To pass the generator muons ID

    Int_t    nGenPar     = data.GetInt("nGenPar"); 
    Int_t*   genParId    = data.GetPtrInt("genParId");
    Int_t*   genMomParId = data.GetPtrInt("genMomParId");
    Int_t*   genParSt    = data.GetPtrInt("genParSt");
    Float_t* genParPt    = data.GetPtrFloat("genParPt");
    Float_t* genParEta   = data.GetPtrFloat("genParEta");
    Float_t* genParPhi   = data.GetPtrFloat("genParPhi");
    Float_t* genParM     = data.GetPtrFloat("genParM");
      
    Int_t genMuIndex[2][2] = {{-1,-1},{-1,-1}};

    for(Int_t i = 0; i < nGenPar; i++){

      if( abs(genParId[i]) != 13 ) continue;
      if( genParSt[i] != 1 ) continue;
      if( genMomParId[i] != 23 && genMomParId[i] != 13 ) continue;
      if( genParPt[i] <= 20 ) continue;
      if( fabs(genParEta[i]) >= 2.4 ) continue;

      if( genMuIndex[0][0] < 0 ) genMuIndex[0][0] = i;
      else if( genMuIndex[0][1] < 0 ) genMuIndex[0][1] = i;
      else if( genMuIndex[1][0] < 0 ) genMuIndex[1][0] = i; 
      else if( genMuIndex[1][1] < 0 ) genMuIndex[1][1] = i;

      if( genMuIndex[0][0] >= 0 && genMuIndex[0][1] >= 0 &&
	  genMuIndex[1][0] >= 0 && genMuIndex[1][1] >= 0 ) break;

    }

    if( genMuIndex[0][0] < 0 || genMuIndex[0][1] < 0 ) continue;

    Int_t npair = (genMuIndex[1][0] >= 0 && genMuIndex[1][1] >= 0) ? 2 : 1;
    
    TLorentzVector genMuon[2][2] = {{TLorentzVector(0,0,0,0),TLorentzVector(0,0,0,0)},
				    {TLorentzVector(0,0,0,0),TLorentzVector(0,0,0,0)}};

    for(Int_t i = 0; i < npair; i++){
      for(Int_t j = 0; j < 2; j++){

	genMuon[i][j].SetPtEtaPhiM(genParPt[genMuIndex[i][j]], 
				   genParEta[genMuIndex[i][j]], 
				   genParPhi[genMuIndex[i][j]], 
				   genParM[genMuIndex[i][j]]
				   );

      } 
    }

    Double_t gendR[2];

    for(Int_t gm = 0; gm < npair; gm++){

      gendR[gm] = genMuon[gm][0].DeltaR(genMuon[gm][1]);
      h_DeltaR[0]->Fill(gendR[gm]);

    }

    // Overloop all reconstructed muons that have possible matches

    Int_t    nMu   = data.GetInt("nMu");
    Int_t*   isGlobalMuon  = data.GetPtrInt("isGlobalMuon");
    Int_t*   isTrackerMuon = data.GetPtrInt("isTrackerMuon");
    Float_t* muPt  = data.GetPtrFloat("muPt");
    Float_t* muEta = data.GetPtrFloat("muEta");
    Float_t* muPhi = data.GetPtrFloat("muPhi");
    Float_t* muM   = data.GetPtrFloat("muM");

    TLorentzVector recoMuoni, recoMuonj;

    Double_t dRMin = 0.1;
    Bool_t findPair[2][3] = {{},{false, false, false}}; //gg,gt,tt

    for(Int_t gm = 0; gm < npair; gm++){ // genmu for loop

      for(Int_t i = 0; i < nMu; i++){ // i for loop

	if( muPt[i] <= 10 ) continue;
	if( fabs(muEta[i]) >= 2.4 ) continue;
	if( !isGlobalMuon[i] && !isTrackerMuon[i] )continue;
     
	TLorentzVector stRecoMu(0,0,0,0);

	stRecoMu.SetPtEtaPhiM(muPt[i], muEta[i], muPhi[i], muM[i]);
      
	Double_t dR_reco0gen0 = stRecoMu.DeltaR(genMuon[gm][0]);
	Double_t dR_reco0gen1 = stRecoMu.DeltaR(genMuon[gm][1]);

	if( dR_reco0gen0 > dRMin && dR_reco0gen1 > dRMin )continue;

	for(Int_t j = 0; j < i; j++){ // j for loop

	  if( muPt[j] <= 10 ) continue;
	  if( fabs(muEta[j]) >= 2.4 ) continue;
	  if( !isGlobalMuon[j] && !isTrackerMuon[j] )continue;
     
	  TLorentzVector ndRecoMu(0,0,0,0);

	  ndRecoMu.SetPtEtaPhiM(muPt[j], muEta[j], muPhi[j], muM[j]);
      
	  Double_t dR_reco1gen0 = ndRecoMu.DeltaR(genMuon[gm][0]);
	  Double_t dR_reco1gen1 = ndRecoMu.DeltaR(genMuon[gm][1]);

	  if( dR_reco1gen0 > dRMin && dR_reco1gen1 > dRMin )continue;

	  // Pass 3 cases

	  Bool_t glbglb = (isGlobalMuon[i] && isGlobalMuon[j]);
	  Bool_t atleastglb = (isGlobalMuon[i] && isGlobalMuon[j]) || (isGlobalMuon[i] && isTrackerMuon[j]) || (isGlobalMuon[j] && isTrackerMuon[i]);
	  Bool_t trktrk = (isTrackerMuon[i] && isTrackerMuon[j]);
	  Bool_t muMatching = ((dR_reco0gen0 < dRMin && dR_reco1gen1 < dRMin) || (dR_reco0gen1 < dRMin && dR_reco1gen0 < dRMin));

	  if( glbglb && muMatching ) 
	    findPair[gm][0] = true;

	  if( atleastglb && muMatching )	   
	    findPair[gm][1] = true;

	  if( trktrk && muMatching )
	    findPair[gm][2] = true;
	
	} // end of j for loop

      } // end of i for loop

    } // end of genmu loop    

    // Fill histogram with 3 cases
    
    for(Int_t gm = 0; gm < npair; gm++){
      for(Int_t i = 0; i < 3; i++){

	if(findPair[gm][i]) h_DeltaR[i+1]->Fill(gendR[gm]);

      } 
    }
       
  } // End of event loop

  fprintf(stderr, "Processed all events\n");

  TFile* outFile = new TFile(outName.data(), "recreate");
  TCanvas* c = new TCanvas("c", "", 0, 0, 1280, 720);
  c->Divide(2,2);

  std::string title[3]={"Two Global muons","1 Global muon + 1 Tracker muon","Two Tracker muons"};
  std::string namee[3]={"twoGlbMu","oneGlbMuoneTrkMu","twoTrkMu"};

  for(Int_t he = 0; he < 3; he++){

    c->cd(he+1);
    h_reconstrcEff[he] = new TGraphAsymmErrors();
    h_reconstrcEff[he]->SetTitle(title[he].data());
    h_reconstrcEff[he]->SetMarkerColor(kBlue);
    h_reconstrcEff[he]->SetMarkerStyle(20);
    h_reconstrcEff[he]->SetMarkerSize(0.7);
    h_reconstrcEff[he]->BayesDivide(h_DeltaR[he+1], h_DeltaR[0]);
    h_reconstrcEff[he]->GetXaxis()->SetTitle("DeltaR");
    h_reconstrcEff[he]->GetYaxis()->SetTitle("Efficiency");
    h_reconstrcEff[he]->SetMinimum(0);
    h_reconstrcEff[he]->SetMaximum(1.2);
    h_reconstrcEff[he]->Draw("ap");
    h_reconstrcEff[he]->Write(namee[he].data());

  }

  c->Print("13TeVreconstructEff.png");
  outFile->Write();

}
