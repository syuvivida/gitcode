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
#include <TGraphAsymmErrors.h>
#include "untuplizer.h"

// 25ns: root -q -b eleMiniIso.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/DoubleEG_Run2015C-PromptReco-v1/\"\,0\)
// signal: root -q -b eleMiniIso.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/\"\,1\)

void eleMiniIso(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  bool isSignal = false;
  if( num == 1 ) isSignal = true;

  std::vector<string> infiles;
 
  std::string outputFile[2] = {"DoubleEG_Run2015C-PromptReco-v1","ZprimeToZhToZlephbb"};

  TSystemDirectory *base = new TSystemDirectory("root","root");
  base->SetDirectory(inputFile.data());
  TList *listOfFiles = base->GetListOfFiles();
  TIter fileIt(listOfFiles);
  TFile *fileH = new TFile();
  Long64_t nfiles = 0;

  while( (fileH = (TFile*)fileIt()) ){
    
    std::string fileN = fileH->GetName();
    std::string baseString = isSignal ? "Zprime" : "NCUGlobal";

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

  TProfile* p_miniIsonVtx = new TProfile("p_miniIsonVtx", "miniIso vs nVtx", 50, 0, 50, 0, 1);
  TProfile* p_corrIsonVtx = new TProfile("p_corrIsonVtx", "corrIso vs nVtx", 50, 0, 50, 0, 1);
     
  TH1D* h_nVtxPassHEEPIso = new TH1D("h_nVtxPassHEEPIso", "nVtxPassHEEPIso", 50, 0, 50);
  TH1D* h_nVtxPassMiniIso = new TH1D("h_nVtxPassMiniIso", "nVtxPassMiniIso", 50, 0, 50);
  TH1D* h_nVtxPassCorrIso = new TH1D("h_nVtxPassCorrIso", "nVtxPassCorrIso", 50, 0, 50);
  TH1D* h_deltaR          = new TH1D("h_deltaR", "deltaR", 100, 0, 0.4);
  TH1D* h_eleRho          = new TH1D("h_eleRho", "eleRho", 100, 0, 100);
  TH1D* h_eleMiniIso      = new TH1D("h_eleMiniIso", "eleMiniIso", 100, -1, 1);
  TH1D* h_eleCorrIso      = new TH1D("h_eleCorrIso", "eleCorrIso", 100, -2, 2);
  TH1D* h_effectiveArea   = new TH1D("h_effectiveArea", "effectiveArea", 100, 0, 0.4);

  TGraphAsymmErrors* h_EffHEEPMini = new TGraphAsymmErrors();
  TGraphAsymmErrors* h_EffHEEPCorr = new TGraphAsymmErrors();

  h_nVtxPassHEEPIso->Sumw2();
  h_nVtxPassMiniIso->Sumw2();
  h_nVtxPassCorrIso->Sumw2();
  
  h_nVtxPassHEEPIso->GetXaxis()->SetTitle("nVtxPassHEEPIso"); 
  h_nVtxPassMiniIso->GetXaxis()->SetTitle("nVtxPassMiniIso");   
  h_nVtxPassCorrIso->GetXaxis()->SetTitle("nVtxPassCorrIso");    
  h_deltaR         ->GetXaxis()->SetTitle("deltaR");
  h_eleRho         ->GetXaxis()->SetTitle("eleRho");
  h_eleMiniIso     ->GetXaxis()->SetTitle("eleMiniIso");
  h_eleCorrIso     ->GetXaxis()->SetTitle("eleCorrIso");
  h_effectiveArea  ->GetXaxis()->SetTitle("effectiveArea");
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
    TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");
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

    for(Int_t ie = 0; ie < nEle; ie++){

      TLorentzVector* thisEle = (TLorentzVector*)eleP4->At(ie);

      Float_t effectiveArea = 0.0;

      if( fabs(eleScEta[ie]) > 0.0 && fabs(eleScEta[ie]) < 1.0 )
        effectiveArea = 0.1752;
      else if( fabs(eleScEta[ie]) > 1.0 && fabs(eleScEta[ie]) < 1.479 )
        effectiveArea = 0.1862;
      else if( fabs(eleScEta[ie]) > 1.479 && fabs(eleScEta[ie]) < 2.0 )
        effectiveArea = 0.1411;
      else if( fabs(eleScEta[ie]) > 2.0 && fabs(eleScEta[ie]) < 2.2 )
        effectiveArea = 0.1534;
      else if( fabs(eleScEta[ie]) > 2.2 && fabs(eleScEta[ie]) < 2.3 )
        effectiveArea = 0.1903;
      else if( fabs(eleScEta[ie]) > 2.3 && fabs(eleScEta[ie]) < 2.4 )
        effectiveArea = 0.2243;
      else if( fabs(eleScEta[ie]) > 2.4 && fabs(eleScEta[ie]) < 2.5 )
        effectiveArea = 0.2687;

      Float_t deltaR = TMath::Max(0.05,TMath::Min(0.2,(10.0/thisEle->Pt())));
      Float_t eleCorrIso = eleMiniIso[ie]-eleRho*effectiveArea*TMath::Power((deltaR/0.3),2);

      if( !(fabs(eleScEta[ie]) < 1.442 || fabs(eleScEta[ie]) > 1.566) ) continue;
      if( fabs(eleScEta[ie]) > 2.5 ) continue;
      if( eleScEt[ie] <= 35 ) continue;
      if( !eleEcalDrivenSeed[ie] ) continue;
      if( !eleIsPassHEEPNoIso[ie] ) continue;

      h_deltaR       ->Fill(deltaR);
      h_eleRho       ->Fill(eleRho);
      h_eleMiniIso   ->Fill(eleMiniIso[ie]);
      h_eleCorrIso   ->Fill(eleCorrIso);
      h_effectiveArea->Fill(effectiveArea);

      p_miniIsonVtx->Fill(nVtx, eleMiniIso[ie]);
      p_corrIsonVtx->Fill(nVtx, eleCorrIso);

      h_nVtxPassHEEPIso->Fill(nVtx);

      if( eleMiniIso[ie] < 0.1 )
	h_nVtxPassMiniIso->Fill(nVtx);

      if( eleCorrIso < 0.1 )
	h_nVtxPassCorrIso->Fill(nVtx);

    } // end of ele loop

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  h_EffHEEPMini->Divide(h_nVtxPassMiniIso, h_nVtxPassHEEPIso);
  h_EffHEEPCorr->Divide(h_nVtxPassCorrIso, h_nVtxPassHEEPIso);

  h_EffHEEPMini->SetTitle("passMiniIso dv passHEEPIso");
  h_EffHEEPCorr->SetTitle("passCorrIso dv passHEEPIso");

  TFile* outFile = new TFile(Form("%s_eleMiniIso.root",outputFile[num].c_str()), "recreate");
  
  p_miniIsonVtx  ->Write("miniIsonVtx");
  p_corrIsonVtx  ->Write("corrIsonVtx");
  h_EffHEEPMini  ->Write("EffHEEPMini");
  h_EffHEEPCorr  ->Write("EffHEEPCorr");
  h_deltaR       ->Write("deltaR");
  h_eleRho       ->Write("eleRho");
  h_eleMiniIso   ->Write("eleMiniIso");
  h_eleCorrIso   ->Write("eleCorrIso");
  h_effectiveArea->Write("effectiveArea");

  outFile->Write();
  
}
