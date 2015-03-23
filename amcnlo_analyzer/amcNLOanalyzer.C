#include <TLorentzVector.h>
#include <TH1F.h>
#include <iostream>
#include "untuplizer.h"

using namespace std;

Float_t deltaPhi(Float_t phi1, Float_t phi2){

  Float_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
  if (dPhi < - TMath::Pi()) dPhi += 2*TMath::Pi();
  return dPhi;

}

Float_t deltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2){

  Float_t dEta = eta1 - eta2;
  Float_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2*TMath::Pi();
  return sqrt(pow(dEta,2) + pow(dPhi,2));

}

void printProgBar(Int_t percent){

  string bar;

  for (Int_t i = 0; i < 50; i++){

    if (i < (percent/2))
      bar.replace(i,1,"=");
    else if(i == (percent/2))
      bar.replace(i,1,">");
    else
      bar.replace(i,1," ");

  }

  cout << "\r" "  [" << bar << "] ";
  cout.width(3);
  cout << percent << "%     " << flush;

}

void amcNLOanalyzer(string inputName){

  cout << " Reading input file: " << inputName.data() << endl;

  TreeReader data(inputName.data());

  string outputName = "analyz_" + inputName.substr(7);

  TFile *outFile = new TFile(outputName.data(), "RECREATE");

  TH1F *hJetPt = new TH1F("hJetPt", "pt leading jet", 100, 0, 500);
  TH1F *hnJet  = new TH1F("hnJet", "number of leading jet", 10, 0, 10);
  TH1F *hlepPt = new TH1F("hlepPt", "hlepPt", 100, 0 , 300);
  TH1F *hmet   = new TH1F("hmet", "hmet", 100, 0, 350);
  TH1F *hplusYlepJet  = new TH1F("hplusYlepJet", "hplusYlepJet", 30, -3, 3);
  TH1F *hminusYlepJet = new TH1F("hminusYlepJet", "hminusYlepJet", 30, -3, 3);
  TH1F *hplusYWJet  = new TH1F("hplusYWJet", "hplusYWJet", 30, -3, 3);
  TH1F *hminusYWJet = new TH1F("hminusYWJet", "hminusYWJet", 30, -3, 3);

  Int_t   nLep_;
  Float_t LepPt_;
  Float_t LepEta_;
  Float_t LepPhi_;
  Int_t   LepCh_;
  Int_t   LepId_;
  Float_t LepY_;
  Float_t metPt_;
  Int_t   nJet_;
  Float_t JetPt_;
  Float_t JetEta_;
  Float_t JetPhi_;
  Float_t GenJetPt_;
  Float_t GenJetEta_;
  Float_t JetY_;
  Float_t WY_;
  Float_t plusY_;
  Float_t minusY_;
  Float_t plusYW_;
  Float_t minusYW_;
  Float_t weight_;

  TTree* tree = new TTree("tree", "tree");

  tree->Branch("nLep", &nLep_, "nLep/I");
  tree->Branch("LepPt", &LepPt_, "LepPt/F");
  tree->Branch("LepEta", &LepEta_, "LepEta/F");
  tree->Branch("LepPhi", &LepPhi_, "LepPhi/F");
  tree->Branch("LepCh", &LepCh_, "LepCh/I");
  tree->Branch("LepId", &LepId_, "LepId/I");
  tree->Branch("LepY", &LepY_, "LepY/F");
  tree->Branch("metPt", &metPt_, "metPt/F");
  tree->Branch("nJet", &nJet_, "nJet/I");
  tree->Branch("JetPt", &JetPt_, "JetPt/F");
  tree->Branch("JetEta", &JetEta_, "JetEta/F");
  tree->Branch("JetPhi", &JetPhi_, "JetPhi/F");
  tree->Branch("GenJetPt", &GenJetPt_, "GenJetPt/F");
  tree->Branch("GenJetEta", &GenJetEta_, "GenJetEta/F");
  tree->Branch("JetY", &JetY_, "JetY/F");
  tree->Branch("WY", &WY_, "WY/F");
  tree->Branch("plusYLepJet", &plusY_, "plusYLepJet/F");
  tree->Branch("minusYLepJet", &minusY_, "minusYLepJet/F");
  tree->Branch("plusYWJet", &plusYW_, "plusYWJet/F");
  tree->Branch("minusYWJet", &minusYW_, "minusYWJet/F");
  tree->Branch("weight", &weight_, "weight/F");

  cout << " Processing Event Loop:" << endl;

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    data.GetEntry(ev);

    Float_t  mcWeight  = data.GetFloat("mcWeight");
    Int_t    nGenPar   = data.GetInt("nGenPar");
    Float_t* genParE   = data.GetPtrFloat("genParE");
    Float_t* genParPt  = data.GetPtrFloat("genParPt");
    Float_t* genParEta = data.GetPtrFloat("genParEta");
    Float_t* genParPhi = data.GetPtrFloat("genParPhi");
    Float_t* genParM   = data.GetPtrFloat("genParM");
    Int_t*   genParQ   = data.GetPtrInt("genParQ");
    Int_t*   genParId  = data.GetPtrInt("genParId");
    Int_t*   genParSt  = data.GetPtrInt("genParSt");
    Int_t*   genMomParId = data.GetPtrInt("genMomParId");
    Int_t*   genParIndex = data.GetPtrInt("genParIndex");
    Int_t    nGenJet   = data.GetInt("nGenJet");
    Float_t* genJetE   = data.GetPtrFloat("genJetE");
    Float_t* genJetPt  = data.GetPtrFloat("genJetPt");
    Float_t* genJetEta = data.GetPtrFloat("genJetEta");
    Float_t* genJetPhi = data.GetPtrFloat("genJetPhi");

    nLep_   = 0;
    LepPt_  = 0.;
    LepEta_ = 0.;
    LepPhi_ = 0.;
    LepCh_  = 0;
    metPt_  = 0.;
    JetPt_  = 0.;
    JetEta_ = 0.;
    JetPhi_ = 0.;
    GenJetPt_  = 0.;
    GenJetEta_ = 0.;
    weight_ = mcWeight;

    // select highest pt lepton
    Int_t nlep  = 0;
    Int_t lepIdMax = -1;
    Int_t lepcharge  = -999;
    Float_t lepPtmax = -999.;
 
    for (Int_t ilep = 0; ilep < nGenPar; ilep++){

      if (genParSt[ilep] != 1) continue;
      if (abs(genParId[ilep]) == 11 || abs(genParId[ilep]) == 13 || abs(genParId[ilep]) == 15){

	if (genParId[ilep] == 11 || genParId[ilep] == 13 || abs(genParId[ilep]) == 15) lepcharge = -1;
	else if (genParId[ilep] == -11 || genParId[ilep] == -13 || abs(genParId[ilep]) == -15) lepcharge = 1;

	if (genParPt[ilep] > lepPtmax){

	  lepPtmax = genParPt[ilep];
	  lepIdMax = ilep;
	  nlep++;

	}
      }
    }

    TLorentzVector lepton(0,0,0,0);

    if (lepIdMax >= 0) 
      lepton.SetPtEtaPhiE(genParPt[lepIdMax], genParEta[lepIdMax], genParPhi[lepIdMax], genParE[lepIdMax]);

    nLep_   = nlep;
    LepPt_  = lepton.Pt();
    LepEta_ = lepton.Eta();
    LepPhi_ = lepton.Phi();
    LepY_   = lepton.Rapidity();
    LepCh_  = lepcharge;
    LepId_  = genParId[lepIdMax];

    hlepPt->Fill(LepPt_, mcWeight);

    // select highest pt MET
    Int_t metIdMax = -1;
    Float_t metPtmax = -999.;

    for (Int_t ilep = 0; ilep < nGenPar; ilep++){

      if (genParSt[ilep] != 1) continue;
      if (abs(genParId[ilep]) == 12 || abs(genParId[ilep]) == 14 || abs(genParId[ilep]) == 16){

	if (genParPt[ilep] > metPtmax){
	  
	  metPtmax = genParPt[ilep];
	  metIdMax = ilep;

	}
      }
    } 

    TLorentzVector neutrino(0,0,0,0);

    if (metIdMax >= 0) 
      neutrino.SetPtEtaPhiE(genParPt[metIdMax], genParEta[metIdMax], genParPhi[metIdMax], genParE[metIdMax]);

    metPt_ = metPtmax;

    hmet->Fill(metPt_, mcWeight);

    TLorentzVector W = lepton + neutrino;

    WY_ = W.Rapidity();

    //select leading jet
    Float_t ptJetMax = -999.;
    Int_t njet = 0;
    Int_t jetIdMax = -1;

    for (Int_t ijet =0; ijet < nGenJet; ijet++){

      if (deltaR(LepEta_, LepPhi_, genJetEta[ijet], genJetPhi[ijet]) < 0.5) continue;
      if (genJetPt[ijet] < 30 || abs(genJetEta[ijet]) > 2.4 ) continue;

      njet++;

      if (genJetPt[ijet] > ptJetMax){
	ptJetMax = genJetPt[ijet];
	jetIdMax = ijet;
      }

    }

    hnJet->Fill(njet, mcWeight);

    TLorentzVector Jet(0,0,0,0);
    if (jetIdMax < 0) continue;

    Jet.SetPtEtaPhiE(genJetPt[jetIdMax], genJetEta[jetIdMax], genJetPhi[jetIdMax], genJetE[jetIdMax]);

    nJet_    = njet;
    JetPt_   = Jet.Pt();
    JetEta_  = Jet.Eta();
    JetPhi_  = Jet.Phi();
    JetY_    = Jet.Rapidity();
    plusY_   = (0.5*(lepton.Rapidity() + Jet.Rapidity()));
    minusY_  = (0.5*(lepton.Rapidity() - Jet.Rapidity()));
    plusYW_  = (0.5*(W.Rapidity() + Jet.Rapidity()));
    minusYW_ = (0.5*(W.Rapidity() - Jet.Rapidity()));

    hplusYlepJet->Fill((LepY_ + JetY_)/2, mcWeight);
    hminusYlepJet->Fill((LepY_ - JetY_)/2, mcWeight);
    hplusYWJet->Fill((WY_ + JetY_)/2, mcWeight);
    hminusYWJet->Fill((WY_ - JetY_)/2, mcWeight);
    hJetPt->Fill(JetPt_, mcWeight);
    tree->Fill();

    printProgBar(ev/(Double_t)data.GetEntriesFast()*100);
      
  } // end of event loop

  cout << "\n Complete!\n Output file: " << outputName.data() << endl;;

  outFile->Write();
  outFile->Close();

}
