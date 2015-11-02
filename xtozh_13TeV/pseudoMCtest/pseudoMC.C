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
     
  TH1D* h_ZprimeSign  = new TH1D("h_ZprimeSign", "ZprimeSign", nvarBins, varBins);
  TH1D* h_ZprimeSide  = new TH1D("h_ZprimeSide", "ZprimeSide", nvarBins, varBins);
  TH1D* h_eventWeight = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_ZprimeSign->Sumw2();
  h_ZprimeSide->Sumw2();

  h_ZprimeSign ->GetXaxis()->SetTitle("ZprimeSign");
  h_ZprimeSide ->GetXaxis()->SetTitle("ZprimeSide");
  h_eventWeight->GetXaxis()->SetTitle("eventWeight");  
    
  // begin of event loop

  Int_t mode = 0;

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 1000000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    if( mode == 0 && ev % 2 == 0 ) continue; // pseudo-data mode
    else if( mode == 1 && ev % 2 == 1 ) continue; // MC mode

    data.GetEntry(ev);

    Int_t          nVtx                 = data.GetInt("nVtx");
    Float_t        mcWeight             = data.GetFloat("mcWeight");    
    TClonesArray*  muP4                 = (TClonesArray*) data.GetPtrTObject("muP4");
    Int_t          FATnJet              = data.GetInt("FATnJet");    
    Int_t*         FATnSubSDJet         = data.GetPtrInt("FATnSubSDJet");
    Float_t*       FATjetSDmass         = data.GetPtrFloat("FATjetSDmass");
    Float_t*       FATjetPRmassL2L3Corr = data.GetPtrFloat("FATjetPRmassL2L3Corr");
    TClonesArray*  FATjetP4             = (TClonesArray*) data.GetPtrTObject("FATjetP4");
    vector<bool>&  FATjetPassIDLoose    = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));
    vector<float>* FATsubjetSDCSV       = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);
  
    if( nVtx < 1 ) continue;

    Double_t eventWeight = mcWeight;
    if( inputFile.find("DYJets") != std::string::npos ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else
      eventWeight = 1;
    
    h_eventWeight->Fill(0.,eventWeight);
    
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
      if( FATjetSDmass[ij] < 95 || FATjetSDmass[ij] > 130 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( FATnSubSDJet[ij] != 2 ) continue;
      if( thisJet->DeltaR(*thisMu) < 0.8 || thisJet->DeltaR(*thatMu) < 0.8 ) continue;
      if( FATsubjetSDCSV[ij][0] < 0.605 || FATsubjetSDCSV[ij][1] < 0.605 ) continue;

      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue;

    Float_t mllbb = (*thisMu+*thatMu+*thisJet).M();  

    if( FATjetPRmassL2L3Corr[goodFATJetID] > 40 && FATjetPRmassL2L3Corr[goodFATJetID] < 105 )
      h_ZprimeSide->Fill(mllbb,eventWeight);

    if( FATjetPRmassL2L3Corr[goodFATJetID] > 105 && FATjetPRmassL2L3Corr[goodFATJetID] < 135 )
      h_ZprimeSign->Fill(mllbb,eventWeight);
  
  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[3] = {"ZprimeSign","ZprimeSide","eventWeight"};

  std::string type;

  if( mode == 0 ) type = "pseudoData";
  else if( mode == 1 ) type = "pseudoMC";

  TFile* outFile = new TFile(Form("%s_%s.root",outputFile.c_str(),type.c_str()), "recreate");
  
  h_ZprimeSign ->Write(h_name[0].data());
  h_ZprimeSide ->Write(h_name[1].data());
  h_eventWeight->Write(h_name[2].data());

  outFile->Write();
  
}
