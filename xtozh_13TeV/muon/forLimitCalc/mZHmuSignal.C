#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include "../../untuplizer.h"
#include "../../isPassZmumu.h"
#include "../../readSample.h"

void mZHmuSignal(std::string inputFile, std::string outputFile){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  if( inputFile.find(".root") != std::string::npos )  
    infiles.push_back(inputFile.data());

  else
    readSample(inputFile, infiles);
  
  TreeReader data(infiles);
  
  // Declare the histogram
     
  TH1D* h_mZprime     = new TH1D("h_mZprime", "mZprime", 100, 400, 5000);
  TH1D* h_eventWeight = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_mZprime->Sumw2();
  h_mZprime->GetXaxis()->SetTitle("mZprime");
      
  // begin of event loop

  for( Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

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

    Double_t eventWeight = mcWeight;
    if( inputFile.find("DYJets") != std::string::npos ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else
      eventWeight = 1;
    
    h_eventWeight->Fill(0.,eventWeight);
    
    if( nVtx < 1 ) continue;

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
      if( fabs(thisJet->Eta()) > 2.4 ) continue;
      if( corrPRmass[ij] < 105 || corrPRmass[ij] > 135 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( FATnSubSDJet[ij] != 2 ) continue;
      if( FATsubjetSDCSV[ij][0] < 0.605 || FATsubjetSDCSV[ij][1] < 0.605 ) continue;
      if( thisJet->DeltaR(*thisMu) < 0.8 || thisJet->DeltaR(*thatMu) < 0.8 ) continue;

      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue;
    
    Float_t mllbb = (*thisMu+*thatMu+*thisJet).M();

    if( mllbb < 700 ) continue;

    h_mZprime->Fill(mllbb,eventWeight);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[2] = {"mZprime","eventWeight"};

  TFile* outFile = new TFile(Form("%s_mZHmuSignal.root",outputFile.c_str()), "recreate");

  h_mZprime    ->Write(h_name[0].data());
  h_eventWeight->Write(h_name[1].data());

  outFile->Write();
  
}
