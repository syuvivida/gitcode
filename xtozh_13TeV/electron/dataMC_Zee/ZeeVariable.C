#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "../untuplizer.h"
#include "../isPassZee.h"

void ZeeVariable(std::string inputFile, std::string outputFile){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;
 
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
     
  TH1D* h_Zmass         = new TH1D("h_Zmass", "Zmass", 100, 50, 130);
  TH1D* h_Zpt           = new TH1D("h_Zpt", "Zpt", 100, 0, 130);
  TH1D* h_Zeta          = new TH1D("h_Zeta", "Zeta", 100, -10, 10);
  TH1D* h_ZRapidity     = new TH1D("h_ZRapidity", "ZRapidity", 100, -10, 10);
  TH1D* h_leadElePt     = new TH1D("h_leadElePt", "leadElePt", 100, 0, 130);
  TH1D* h_leadEleEta    = new TH1D("h_leadEleEta", "leadEleEta", 100, -5, 5);
  TH1D* h_subleadElePt  = new TH1D("h_subleadElePt", "subleadElePt", 100, 0, 130);
  TH1D* h_subleadEleEta = new TH1D("h_subleadEleEta", "subleadEleEta", 100, -5, 5);
  TH1D* h_eventWeight   = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_Zmass        ->Sumw2();
  h_Zpt          ->Sumw2();
  h_Zeta         ->Sumw2();
  h_ZRapidity    ->Sumw2();
  h_leadElePt    ->Sumw2();
  h_leadEleEta   ->Sumw2();
  h_subleadElePt ->Sumw2();
  h_subleadEleEta->Sumw2(); 
  
  h_Zmass        ->GetXaxis()->SetTitle("Zmass"); 
  h_Zpt          ->GetXaxis()->SetTitle("Zpt");   
  h_Zeta         ->GetXaxis()->SetTitle("Zeta");    
  h_ZRapidity    ->GetXaxis()->SetTitle("ZRapidity");
  h_leadElePt    ->GetXaxis()->SetTitle("leadElePt");  
  h_leadEleEta   ->GetXaxis()->SetTitle("leadEleEta");
  h_subleadElePt ->GetXaxis()->SetTitle("subleadElePt");   
  h_subleadEleEta->GetXaxis()->SetTitle("subleadEleEta"); 
  h_eventWeight  ->GetXaxis()->SetTitle("eventWeight");
    
  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t    nVtx       = data.GetInt("nVtx");
    Float_t  mcWeight   = data.GetFloat("mcWeight");
    TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");

    if( nVtx < 1 ) continue;

    Double_t eventWeight = mcWeight;
    if( inputFile.find("DYJets") != std::string::npos ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else
      eventWeight = 1;
    
    h_eventWeight->Fill(0.,eventWeight);
    
    // data trigger cut (electron channel)

    std::string* trigName = data.GetPtrString("hlt_trigName");
    vector<bool> &trigResult = *((vector<bool>*) data.GetPtr("hlt_trigResult"));
    const Int_t nsize = data.GetPtrStringSize();    
    bool passTrigger = false;
    
    for(Int_t it = 0; it < nsize; it++){
    
      std::string thisTrig = trigName[it];
      bool results = trigResult[it];
      
      if( thisTrig.find("HLT_Ele105") != std::string::npos && results==1 ){
	passTrigger = true;
	break;
      }
      
    }

    if( !passTrigger ) continue;

    // select good electrons

    vector<Int_t> goodEleID;
    if( !isPassZee(data, goodEleID) ) continue;

    TLorentzVector* thisEle = (TLorentzVector*)eleP4->At(goodEleID[0]);
    TLorentzVector* thatEle = (TLorentzVector*)eleP4->At(goodEleID[1]);

    TLorentzVector l4_Z = (*thisEle+*thatEle);

    h_Zmass    ->Fill(l4_Z.M(),eventWeight);
    h_Zpt      ->Fill(l4_Z.Pt(),eventWeight);
    h_Zeta     ->Fill(l4_Z.Eta(),eventWeight);
    h_ZRapidity->Fill(l4_Z.Rapidity(),eventWeight);

    if( thisEle->Pt() > thatEle->Pt() ){

      h_leadElePt    ->Fill(thisEle->Pt(),eventWeight);
      h_leadEleEta   ->Fill(thisEle->Eta(),eventWeight);
      h_subleadElePt ->Fill(thatEle->Pt(),eventWeight);
      h_subleadEleEta->Fill(thatEle->Eta(),eventWeight);

    }else{

      h_leadElePt    ->Fill(thatEle->Pt(),eventWeight);
      h_leadEleEta   ->Fill(thatEle->Eta(),eventWeight);
      h_subleadElePt ->Fill(thisEle->Pt(),eventWeight);
      h_subleadEleEta->Fill(thisEle->Eta(),eventWeight);

    }

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[9] = {"Zmass","Zpt","Zeta","ZRapidity","leadElePt","leadEleEta",
			   "subleadElePt","subleadEleEta","eventWeight"};

  TFile* outFile = new TFile(Form("%s_ZeeVariable.root",outputFile.c_str()), "recreate");
      
  h_Zmass        ->Write(h_name[0].data());  
  h_Zpt          ->Write(h_name[1].data());  
  h_Zeta         ->Write(h_name[2].data());    
  h_ZRapidity    ->Write(h_name[3].data());
  h_leadElePt    ->Write(h_name[4].data()); 
  h_leadEleEta   ->Write(h_name[5].data());   
  h_subleadElePt ->Write(h_name[6].data());
  h_subleadEleEta->Write(h_name[7].data());    
  h_eventWeight  ->Write(h_name[8].data());
  
  outFile->Write();
  
}
