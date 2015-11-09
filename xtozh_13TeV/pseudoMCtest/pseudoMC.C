#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include "../untuplizer.h"
#include "../readSample.h"
#include "../isPassZmumu.h"

void pseudoMC(std::string inputFile, std::string outputFile){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;
    
  readSample(inputFile, infiles);
  
  TreeReader data(infiles);

  // Declare the histogram

  const Double_t varBins[] = {600,800,1000,1200,1400,1600,1800,2000,2500,3000,3500,4000,4500};
  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;
     
  TH1D* h_ZprimeSign_pMC  = new TH1D("h_ZprimeSign_pMC", "ZprimeSign", nvarBins, varBins);
  TH1D* h_ZprimeSide_pMC  = new TH1D("h_ZprimeSide_pMC", "ZprimeSide", nvarBins, varBins);
  TH1D* h_ZprimeSign_pDA  = new TH1D("h_ZprimeSign_pDA", "ZprimeSign", nvarBins, varBins);
  TH1D* h_ZprimeSide_pDA  = new TH1D("h_ZprimeSide_pDA", "ZprimeSide", nvarBins, varBins);
  TH1D* h_corrPRmass_pDA  = new TH1D("h_corrPRmass_pDA", "corrPRmass", 20, 40, 240);
  TH1D* h_eventWeight_pMC = new TH1D("h_eventWeight_pMC", "eventWeight", 100, -1, 1);
  TH1D* h_eventWeight_pDA = new TH1D("h_eventWeight_pDA", "eventWeight", 100, -1, 1);

  h_ZprimeSign_pMC->Sumw2();
  h_ZprimeSide_pMC->Sumw2();
  h_ZprimeSign_pDA->Sumw2();
  h_ZprimeSide_pDA->Sumw2();
  h_corrPRmass_pDA->Sumw2();

  h_ZprimeSign_pMC ->GetXaxis()->SetTitle("ZprimeSign");
  h_ZprimeSide_pMC ->GetXaxis()->SetTitle("ZprimeSide");
  h_ZprimeSign_pDA ->GetXaxis()->SetTitle("ZprimeSign");
  h_ZprimeSide_pDA ->GetXaxis()->SetTitle("ZprimeSide");
  h_corrPRmass_pDA ->GetXaxis()->SetTitle("corrPRmass");
  h_eventWeight_pMC->GetXaxis()->SetTitle("eventWeight");
  h_eventWeight_pDA->GetXaxis()->SetTitle("eventWeight");

  // begin of event loop

  for(Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if( ev % 1000000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t          nVtx              = data.GetInt("nVtx");
    Float_t        mcWeight          = data.GetFloat("mcWeight");    
    TClonesArray*  muP4              = (TClonesArray*) data.GetPtrTObject("muP4");
    Int_t          FATnJet           = data.GetInt("FATnJet");    
    Int_t*         FATnSubSDJet      = data.GetPtrInt("FATnSubSDJet");
    Float_t*       corrPRmass        = data.GetPtrFloat("FATjetPRmassL2L3Corr");
    TClonesArray*  FATjetP4          = (TClonesArray*) data.GetPtrTObject("FATjetP4");
    vector<bool>&  FATjetPassIDLoose = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));
    vector<float>* FATsubjetSDCSV    = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);
  
    if( nVtx < 1 ) continue;

    Double_t eventWeight = mcWeight;
    if( inputFile.find("DYJets") != std::string::npos ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else
      eventWeight = 1;

    if( ev % 2 == 0 )
      h_eventWeight_pMC->Fill(0.,eventWeight);

    else if( ev % 2 == 1 )
      h_eventWeight_pDA->Fill(0.,eventWeight);
        
    // data trigger cut (muon channel)

    std::string* trigName = data.GetPtrString("hlt_trigName");
    vector<bool> &trigResult = *((vector<bool>*) data.GetPtr("hlt_trigResult"));
    const Int_t nsize = data.GetPtrStringSize();    
    bool passTrigger = false;
    
    for(Int_t it = 0; it < nsize; it++){
    
      std::string thisTrig = trigName[it];
      bool results = trigResult[it];
      
      if( thisTrig.find("HLT_Mu45") != std::string::npos && results==1 ){
	passTrigger = true;
	break;
      }
      
    }

    if( !passTrigger ) continue;

    // select good muons
      
    vector<Int_t> goodMuID;
    if( !isPassZmumu(data, goodMuID) ) continue;

    TLorentzVector* thisMu = (TLorentzVector*)muP4->At(goodMuID[0]);
    TLorentzVector* thatMu = (TLorentzVector*)muP4->At(goodMuID[1]);

    // select good FATjet

    Int_t goodFATJetID = -1;
    TLorentzVector* thisJet = NULL;

    for(Int_t ij = 0; ij < FATnJet; ij++){

      thisJet = (TLorentzVector*)FATjetP4->At(ij);

      if( thisJet->Pt() < 200 ) continue;
      if( fabs(thisJet->Eta()) > 2.5 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( FATnSubSDJet[ij] != 2 ) continue;
      if( thisJet->DeltaR(*thisMu) < 0.8 || thisJet->DeltaR(*thatMu) < 0.8 ) continue;
      //if( FATsubjetSDCSV[ij][0] < 0.605 || FATsubjetSDCSV[ij][1] < 0.605 ) continue;

      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue;

    Float_t mllbb = (*thisMu+*thatMu+*thisJet).M();  

    if( ev % 2 == 0 ){

      if( corrPRmass[goodFATJetID] > 40 && !(corrPRmass[goodFATJetID] > 65 && corrPRmass[goodFATJetID] < 150) )
	h_ZprimeSide_pMC->Fill(mllbb,eventWeight);

      if( corrPRmass[goodFATJetID] > 105 && corrPRmass[goodFATJetID] < 135 )
	h_ZprimeSign_pMC->Fill(mllbb,eventWeight);
  
    }

    else if( ev % 2 == 1 ){

      if( corrPRmass[goodFATJetID] > 40 && !(corrPRmass[goodFATJetID] > 65 && corrPRmass[goodFATJetID] < 150) ){
        h_ZprimeSide_pDA->Fill(mllbb,eventWeight);
        h_corrPRmass_pDA->Fill(corrPRmass[goodFATJetID],eventWeight);
      }

      if( corrPRmass[goodFATJetID] > 105 && corrPRmass[goodFATJetID] < 135 )
	h_ZprimeSign_pDA->Fill(mllbb,eventWeight);

    }

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  TFile* outFile = new TFile(Form("%s_pseudoTest.root",outputFile.c_str()), "recreate");
  
  h_ZprimeSign_pMC ->Write("ZprimeSign_pMC");
  h_ZprimeSide_pMC ->Write("ZprimeSide_pMC");
  h_ZprimeSign_pDA ->Write("ZprimeSign_pDA");
  h_ZprimeSide_pDA ->Write("ZprimeSide_pDA");
  h_corrPRmass_pDA ->Write("corrPRmass_pDA");
  h_eventWeight_pMC->Write("eventWeight_pMC");
  h_eventWeight_pDA->Write("eventWeight_pDA");

  outFile->Write();

}
