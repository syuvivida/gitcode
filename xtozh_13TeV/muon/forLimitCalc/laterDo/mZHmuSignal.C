#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include "../untuplizer.h"
#include "../isPassZmumu.h"
#include "../readSample.h"

/* 
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph.root\"\,5\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph.root\"\,6\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph.root\"\,7\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph.root\"\,8\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph.root\"\,9\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph.root\"\,10\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph.root\"\,11\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph.root\"\,12\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph.root\"\,13\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph.root\"\,14\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph.root\"\,15\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph.root\"\,16\);
root -q -b mZHmuSignal.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph.root\"\,17\);
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/crab_SingleMuon-Run2015C-PromptReco-v1/150830_214159/0000/\"\,18\);
*/

void mZHmuSignal(std::string inputFile, std::string outputFile){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  if( inputFile.find(".root") != std::string::npos )  
    infiles.push_back(inputFile.data());

  else
    readSample(inputFile, infiles);
  
  TreeReader data(infiles);
  
  // Declare the histogram
     
  TH1D* h_mZprime          = new TH1D("h_mZprime", "mZprime", 100, 400, 5000);
  TH1D* h_eventWeight      = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_mZprime->Sumw2();
  h_mZprime->GetXaxis()->SetTitle("mZprime");
      
  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t          nVtx              = data.GetInt("nVtx");
    Float_t        mcWeight          = data.GetFloat("mcWeight");    
    TClonesArray*  muP4              = (TClonesArray*) data.GetPtrTObject("muP4");
    Int_t          FATnJet           = data.GetInt("FATnJet");    
    Int_t*         FATnSubSDJet      = data.GetPtrInt("FATnSubSDJet");
    Float_t*       FATjetCISVV2      = data.GetPtrFloat("FATjetCISVV2");
    Float_t*       FATjetSDmass      = data.GetPtrFloat("FATjetSDmass");
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
      if( FATjetCISVV2[ij] < 0.605 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( FATnSubSDJet[ij] < 2 ) continue;
      if( thisJet->DeltaR(*thisMu) < 0.8 || thisJet->DeltaR(*thatMu) < 0.8 ) continue;

      for(Int_t is = 0; is < FATnSubSDJet[ij]; is++)
	if( FATsubjetSDCSV[ij][is] < 0.605 ) continue;

      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue;
    
    Float_t mZll = (*thisMu+*thatMu+*thisJet).M();

    h_mZprime->Fill(mZll,eventWeight);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[2] = {"mZprime","eventWeight"};

  TFile* outFile = new TFile(Form("%s_mZHmuSignal.root",outputFile.c_str()), "recreate");

  h_mZprime         ->Write(h_name[0].data());
  h_eventWeight     ->Write(h_name[1].data());

  outFile->Write();
  
}
