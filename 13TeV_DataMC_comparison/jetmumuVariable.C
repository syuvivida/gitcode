#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "untuplizer.h"

// 25ns: root -q -b jetmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/crab_SingleMuon-Run2015C-PromptReco-v1/150830_214159/0000/\"\,0\)
// 25ns: root -q -b jetmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0830/150830_215828/0000/\"\,1\)
// 25ns: root -q -b jetmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830/150831_085116/\"\,2\)

void jetmumuVariable(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  std::string outputFile[3] = {"crab_SingleMuon-Run2015C-PromptReco-v1","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns",
			       "crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830"};

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
     
  TH1D* h_FATjetPt         = new TH1D("h_FATjetPt", "FATjetPt", 100, 100, 1000);
  TH1D* h_FATjetEta        = new TH1D("h_FATjetEta", "FATjetEta", 100, -4, 4);
  TH1D* h_FATjetCISVV2     = new TH1D("h_FATjetCISVV2", "FATjetCISVV2", 100, 0, 2);
  TH1D* h_FATjetSDmass     = new TH1D("h_FATjetSDmass", "FATjetSDmass", 100, 50, 200);
  TH1D* h_FATjetPRmass     = new TH1D("h_FATjetPRmass", "FATjetPRmass", 100, 50, 200);
  TH1D* h_FATjetTau1       = new TH1D("h_FATjetTau1", "FATjetTau1", 100, 0, 1);
  TH1D* h_FATjetTau2       = new TH1D("h_FATjetTau2", "FATjetTau2", 100, 0, 1);
  TH1D* h_FATjetTau2dvTau1 = new TH1D("h_FATjetTau2dvTau1", "FATjetTau2dvTau1", 100, 0, 1);
  TH1D* h_FATsubjetPt      = new TH1D("h_FATsubjetPt", "FATsubjetPt", 100, 40, 800);
  TH1D* h_FATsubjetEta     = new TH1D("h_FATsubjetEta", "FATsubjetEta", 100, -4, 4);
  TH1D* h_FATsubjetSDCSV   = new TH1D("h_FATsubjetSDCSV", "FATsubjetSDCSV", 100, 0, 2);
  TH1D* h_eventWeight      = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_FATjetPt        ->Sumw2();   
  h_FATjetEta       ->Sumw2();
  h_FATjetCISVV2    ->Sumw2();
  h_FATjetSDmass    ->Sumw2();
  h_FATjetPRmass    ->Sumw2();
  h_FATjetTau1      ->Sumw2();
  h_FATjetTau2      ->Sumw2();
  h_FATjetTau2dvTau1->Sumw2();
  h_FATsubjetPt     ->Sumw2();
  h_FATsubjetEta    ->Sumw2();
  h_FATsubjetSDCSV  ->Sumw2();

  h_FATjetPt        ->GetXaxis()->SetTitle("FATjetPt");
  h_FATjetEta       ->GetXaxis()->SetTitle("FATjetEta");
  h_FATjetCISVV2    ->GetXaxis()->SetTitle("FATjetCISVV2");
  h_FATjetSDmass    ->GetXaxis()->SetTitle("FATjetSDmass");
  h_FATjetPRmass    ->GetXaxis()->SetTitle("FATjetPRmass");
  h_FATjetTau1      ->GetXaxis()->SetTitle("FATjetTau1");
  h_FATjetTau2      ->GetXaxis()->SetTitle("FATjetTau2");
  h_FATjetTau2dvTau1->GetXaxis()->SetTitle("FATjetTau2dvTau1");
  h_FATsubjetPt     ->GetXaxis()->SetTitle("FATsubjetPt");
  h_FATsubjetEta    ->GetXaxis()->SetTitle("FATsubjetEta");
  h_FATsubjetSDCSV  ->GetXaxis()->SetTitle("FATsubjetSDCSV");
  h_eventWeight     ->GetXaxis()->SetTitle("eventWeight");  
    
  // begin of event loop

  Int_t nPass[4] = {0};

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t          nVtx                = data.GetInt("nVtx");
    Int_t          nMu                 = data.GetInt("nMu");
    Int_t*         muCharge            = data.GetPtrInt("muCharge");
    Float_t        mcWeight            = data.GetFloat("mcWeight");    
    Float_t*       muMiniIso           = data.GetPtrFloat("muMiniIso");
    TClonesArray*  muP4                = (TClonesArray*) data.GetPtrTObject("muP4");
    vector<bool>&  isHighPtMuon        = *((vector<bool>*) data.GetPtr("isHighPtMuon"));
    vector<bool>&  isCustomTrackerMuon = *((vector<bool>*) data.GetPtr("isCustomTrackerMuon"));
    Int_t          FATnJet             = data.GetInt("FATnJet");    
    Int_t*         FATnSubSDJet        = data.GetPtrInt("FATnSubSDJet");
    Float_t*       FATjetCISVV2        = data.GetPtrFloat("FATjetCISVV2");
    Float_t*       FATjetSDmass        = data.GetPtrFloat("FATjetSDmass");
    Float_t*       FATjetPRmass        = data.GetPtrFloat("FATjetPRmass");
    Float_t*       FATjetTau1          = data.GetPtrFloat("FATjetTau1");
    Float_t*       FATjetTau2          = data.GetPtrFloat("FATjetTau2");
    TClonesArray*  FATjetP4            = (TClonesArray*) data.GetPtrTObject("FATjetP4");
    vector<bool>&  FATjetPassIDLoose   = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));
    vector<float>* FATsubjetSDPx       = data.GetPtrVectorFloat("FATsubjetSDPx", FATnJet);
    vector<float>* FATsubjetSDPy       = data.GetPtrVectorFloat("FATsubjetSDPy", FATnJet);
    vector<float>* FATsubjetSDPz       = data.GetPtrVectorFloat("FATsubjetSDPz", FATnJet);
    vector<float>* FATsubjetSDE        = data.GetPtrVectorFloat("FATsubjetSDE", FATnJet);
    vector<float>* FATsubjetSDCSV      = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);

    if( nVtx < 1 ) continue;

    Double_t eventWeight = mcWeight;
    if( num == 1 ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else if( num == 0 || num == 2 )
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

    nPass[0]++;

    // select good muons
      
    std::vector<Int_t> goodMuons;
  
    for(Int_t im = 0; im < nMu; im++){
      
      TLorentzVector* myMu = (TLorentzVector*)muP4->At(im);

      if( fabs(myMu->Eta()) > 2.1 ) continue;
      if( myMu->Pt() < 20 ) continue;
      if( muMiniIso[im] >= 0.1 ) continue;
      if( !isHighPtMuon[im] && !isCustomTrackerMuon[im] ) continue;

      goodMuons.push_back(im);

    }

    // select good Z boson

    bool findMPair = false;
    TLorentzVector l4_Z(0,0,0,0);
    TLorentzVector* thisMu = NULL;
    TLorentzVector* thatMu = NULL;

    for(unsigned int i = 0; i < goodMuons.size(); i++){

      Int_t im = goodMuons[i];
      thisMu = (TLorentzVector*)muP4->At(im);

      for(unsigned int j = 0; j < i; j++){

	Int_t jm = goodMuons[j];
	thatMu = (TLorentzVector*)muP4->At(jm);

	Float_t pt1   = thisMu->Pt();
	Float_t pt2   = thatMu->Pt();
	Float_t ptMax = TMath::Max(pt1,pt2);
	Float_t mll   = (*thisMu+*thatMu).M();

	if( muCharge[im]*muCharge[jm] > 0 ) continue;
	if( mll < 60 || mll > 120 ) continue;
	if( ptMax < 50 ) continue;
	if( !( (isHighPtMuon[im] && isCustomTrackerMuon[jm]) || (isHighPtMuon[jm] && isCustomTrackerMuon[im]) ) ) continue;
	if( !findMPair ) l4_Z = (*thisMu+*thatMu);

	findMPair = true;
	break;

      }
    }

    if( !findMPair ) continue;

    nPass[1]++;

    // select good FATjet

    Int_t goodFATJetID = -1;
    Int_t goodsubJetID[2] = {-1};
    TLorentzVector* thisJet = NULL;

    for(Int_t ij = 0; ij < FATnJet; ij++){

      thisJet = (TLorentzVector*)FATjetP4->At(ij);

      if( thisJet->Pt() < 30 ) continue;
      if( fabs(thisJet->Eta()) > 2.5 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( FATnSubSDJet[ij] < 2 ) continue;

      for(Int_t is = 0; is < FATnSubSDJet[ij]; is++){

	goodsubJetID[is] = is;

	if( goodsubJetID[0] >= 0 && goodsubJetID[1] >= 0 ) 
	  break;

      }

      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue;
    nPass[2]++;

    if( goodsubJetID[0] < 0 || goodsubJetID[1] < 0 ) continue;
    nPass[3]++;

    h_FATjetPt        ->Fill(thisJet->Pt());
    h_FATjetEta       ->Fill(thisJet->Eta());
    h_FATjetCISVV2    ->Fill(FATjetCISVV2[goodFATJetID]);
    h_FATjetSDmass    ->Fill(FATjetSDmass[goodFATJetID]);
    h_FATjetPRmass    ->Fill(FATjetPRmass[goodFATJetID]);
    h_FATjetTau1      ->Fill(FATjetTau1[goodFATJetID]);
    h_FATjetTau2      ->Fill(FATjetTau2[goodFATJetID]);
    h_FATjetTau2dvTau1->Fill(FATjetTau2[goodFATJetID]/FATjetTau1[goodFATJetID]);

    TLorentzVector l4_subjet0(0,0,0,0);
    TLorentzVector l4_subjet1(0,0,0,0);

    l4_subjet0.SetPxPyPzE(FATsubjetSDPx[goodFATJetID][goodsubJetID[0]],
			  FATsubjetSDPy[goodFATJetID][goodsubJetID[0]],
			  FATsubjetSDPz[goodFATJetID][goodsubJetID[0]],
			  FATsubjetSDE[goodFATJetID][goodsubJetID[0]]);

    l4_subjet1.SetPxPyPzE(FATsubjetSDPx[goodFATJetID][goodsubJetID[1]],
                          FATsubjetSDPy[goodFATJetID][goodsubJetID[1]],
                          FATsubjetSDPz[goodFATJetID][goodsubJetID[1]],
                          FATsubjetSDE[goodFATJetID][goodsubJetID[1]]);

    h_FATsubjetPt   ->Fill(l4_subjet0.Pt());
    h_FATsubjetPt   ->Fill(l4_subjet1.Pt());
    h_FATsubjetEta  ->Fill(l4_subjet0.Eta());
    h_FATsubjetEta  ->Fill(l4_subjet1.Eta());
    h_FATsubjetSDCSV->Fill(FATsubjetSDCSV[goodFATJetID][goodsubJetID[0]]);
    h_FATsubjetSDCSV->Fill(FATsubjetSDCSV[goodFATJetID][goodsubJetID[1]]);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::cout << "\nnPass[0] = " << nPass[0] 
	    << "\nnPass[1] = " << nPass[1] 
	    << "\nnPass[2] = " << nPass[2] 
	    << "\nnPass[3] = " << nPass[3]
	    << std::endl;

  std::string h_name[12] = {"FATjetPt","FATjetEta","FATjetCISVV2","FATjetSDmass",
			    "FATjetPRmass","FATjetTau1","FATjetTau2","FATjetTau2dvTau1",
			    "FATsubjetPt","FATsubjetEta","FATsubjetSDCSV","eventWeight"};

  TFile* outFile = new TFile(Form("%s_jetmumuVariable.root",outputFile[num].c_str()), "recreate");
  
  h_FATjetPt        ->Write(h_name[0].data());
  h_FATjetEta       ->Write(h_name[1].data());
  h_FATjetCISVV2    ->Write(h_name[2].data());
  h_FATjetSDmass    ->Write(h_name[3].data());
  h_FATjetPRmass    ->Write(h_name[4].data());
  h_FATjetTau1      ->Write(h_name[5].data());
  h_FATjetTau2      ->Write(h_name[6].data());
  h_FATjetTau2dvTau1->Write(h_name[7].data());
  h_FATsubjetPt     ->Write(h_name[8].data());
  h_FATsubjetEta    ->Write(h_name[9].data());
  h_FATsubjetSDCSV  ->Write(h_name[10].data());
  h_eventWeight     ->Write(h_name[11].data());

  outFile->Write();
  
}
