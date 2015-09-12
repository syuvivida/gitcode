#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TProfile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "untuplizer.h"

// 25ns: root -q -b ZeeVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/DoubleEG_Run2015C-PromptReco-v1/\"\,0\)

void eleMiniIso(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;
 
  std::string outputFile[3] = {"DoubleEG_Run2015C-PromptReco-v1"};

  TSystemDirectory *base = new TSystemDirectory("root","root");
  base->SetDirectory(inputFile.data());
  TList *listOfFiles = base->GetListOfFiles();
  TIter fileIt(listOfFiles);
  TFile *fileH = new TFile();
  Long64_t nfiles = 0;

  while( (fileH = (TFile*)fileIt()) ){
    
    std::string fileN = fileH->GetName();
    std::string baseString = "NCUGlobal";
    if( fileN.find("fail") != std::string::npos ) continue;

    if( fileH->IsFolder() ){
    
      std::string newDir = inputFile+fileN;
      base->SetDirectory(newDir.data());
      TList *listOfFiles2 = base->GetListOfFiles();
      TIter fileIt2(listOfFiles2);
      TFile *fileH2 = new TFile(); 
      
      while( (fileH2 = (TFile*)fileIt2()) ){

	std::string fileN2 = fileH2->GetName();
	if( fileH2->IsFolder() ) continue;
	if( fileN2.find("fail") != std::string::npos ) continue;
	if( fileN2.find(baseString) == std::string::npos ) continue;
	infiles.push_back(Form("%s/%s",newDir.data(),fileN2.data()));
	nfiles++;

      }
    }
  }
  
  TreeReader data(infiles);
  
  // Declare the histogram

  TProfile* p_miniIsonVtx = new TProfile("p_miniIsonVtx", "miniIso vs nVtx", 10, 0, 2, 0, 1);
  TProfile* p_corrIsonVtx = new TProfile("p_corrIsonVtx", "corrIso vs nVtx", 10, 0, 2, 0, 1);
     
  TH1D* h_nVtxPassHEEPIso = new TH1D("h_nVtxPassHEEPIso", "nVtxPassHEEPIso", 30, -0.5, 29.5);
  TH1D* h_nVtxPassMiniIso = new TH1D("h_nVtxPassMiniIso", "nVtxPassMiniIso", 30, -0.5, 29.5);
  TH1D* h_nVtxPassCorrIso = new TH1D("h_nVtxPassCorrIso", "nVtxPassCorrIso", 30, -0.5, 29.5);
  TH1D* h_EffHEEPMini     = new TH1D("h_EffHEEPMini", "EffHEEPMini", 30, -0.5, 29.5);
  TH1D* h_EffHEEPCorr     = new TH1D("h_EffHEEPCorr", "EffHEEPCorr", 30, -0.5, 29.5);

  h_nVtxPassHEEPIso->Sumw2();
  h_nVtxPassMiniIso->Sumw2();
  h_nVtxPassCorrIso->Sumw2();
  h_EffHEEPMini    ->Sumw2();
  h_EffHEEPCorr    ->Sumw2();
  
  h_nVtxPassHEEPIso->GetXaxis()->SetTitle("nVtxPassHEEPIso"); 
  h_nVtxPassMiniIso->GetXaxis()->SetTitle("nVtxPassMiniIso");   
  h_nVtxPassCorrIso->GetXaxis()->SetTitle("nVtxPassCorrIso");    
  h_EffHEEPMini    ->GetXaxis()->SetTitle("EffHEEPMini");
  h_EffHEEPCorr    ->GetXaxis()->SetTitle("EffHEEPCorr");  
    
  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 10000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t    nVtx       = data.GetInt("nVtx");
    Int_t    nEle       = data.GetInt("nEle");
    Float_t  eleRho      = data.GetFloat("eleRho");
    Float_t* eleScEt    = data.GetPtrFloat("eleScEt");
    Float_t* eleScEta   = data.GetPtrFloat("eleScEta");
    Float_t* eleMiniIso = data.GetPtrFloat("eleMiniIso");
    //TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");
    vector<bool>& eleEcalDrivenSeed  = *((vector<bool>*) data.GetPtr("eleEcalDrivenSeed"));
    vector<bool>& eleIsPassHEEPNoIso = *((vector<bool>*) data.GetPtr("eleIsPassHEEPNoIso"));

    if( nVtx < 1 ) continue;

    // data trigger cut (electron channel)

    std::string* trigName = data.GetPtrString("hlt_trigName");
    vector<bool> &trigResult = *((vector<bool>*) data.GetPtr("hlt_trigResult"));
    const Int_t nsize = data.GetPtrStringSize();    
    bool passTrigger = false;
    
    for(Int_t it = 0; it < nsize; it++){
    
      std::string thisTrig = trigName[it];
      bool results = trigResult[it];
      
      if( thisTrig.find("HLT_DoubleEle33") != std::string::npos && results==1 ){
	passTrigger = true;
	break;
      }
      
    }

    if( !passTrigger ) continue;

    // select good electrons

    Float_t EA = 1.0;
    Float_t dR = 0.3;
        
    for(Int_t ie = 0; ie < nEle; ie++){

      if( !(fabs(eleScEta[ie]) < 1.442 || fabs(eleScEta[ie]) > 1.566) ) continue;
      if( fabs(eleScEta[ie]) > 2.5 ) continue;
      if( eleScEt[ie] <= 35 ) continue;
      if( !eleEcalDrivenSeed[ie] ) continue;
      if( !eleIsPassHEEPNoIso[ie] ) continue;

      h_nVtxPassHEEPIso->Fill(nVtx);

      if( eleMiniIso[ie] >= 0.1 ) continue;

      h_nVtxPassMiniIso->Fill(nVtx);

      Float_t eleCorrIso = eleMiniIso[ie]-eleRho*EA*TMath::Power((dR/0.3),2);

      p_miniIsonVtx->Fill(eleMiniIso[ie], nVtx);
      p_corrIsonVtx->Fill(eleCorrIso, nVtx);

      if( eleCorrIso >= 0.1 ) continue;

      h_nVtxPassCorrIso->Fill(nVtx);

    } // end of ele loop

    h_EffHEEPMini->Divide(h_nVtxPassMiniIso, h_nVtxPassHEEPIso, 1, 1, "B");
    h_EffHEEPCorr->Divide(h_nVtxPassCorrIso, h_nVtxPassHEEPIso, 1, 1, "B");

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  TFile* outFile = new TFile(Form("%s_eleMiniIso.root",outputFile[num].c_str()), "recreate");
  
  p_miniIsonVtx    ->Write("miniIsonVtx");
  p_corrIsonVtx    ->Write("corrIsonVtx");
  h_nVtxPassHEEPIso->Write("nVtxPassHEEPIso");
  h_nVtxPassMiniIso->Write("nVtxPassMiniIso");
  h_nVtxPassCorrIso->Write("nVtxPassCorrIso");
  h_EffHEEPMini    ->Write("EffHEEPMini");
  h_EffHEEPCorr    ->Write("EffHEEPCorr");

  outFile->Write();
  
}
