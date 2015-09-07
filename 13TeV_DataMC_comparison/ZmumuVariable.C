B1;3803;0c#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include "untuplizer.h"

// 25ns: root -q -b ZmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/crab_SingleMuon-Run2015C-PromptReco-v1/150830_214159/0000\"\,0\)
// 25ns: root -q -b ZmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0830/150830_215828/0000\"\,1\)
// 25ns/50ns: root -q -b ZmumuVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803/150803_175618/0000\"\,2\)

void ZmumuVariable(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  std::string outputFile[3] = {"crab_SingleMuon-Run2015C-PromptReco-v1","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns",
			       "TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803"};

  TSystemDirectory *base = new TSystemDirectory("root","root");
  base->SetDirectory(inputFile.data());
  TList *listOfFiles = base->GetListOfFiles();
  TIter fileIt(listOfFiles);
  TFile *fileH = new TFile();
  int nfile = 0;
  while((fileH = (TFile*)fileIt())){
    std::string fileN = fileH->GetName();
    if( fileH->IsFolder() ) continue;
    if( fileN.find("NCUGlobalTuples") == std::string::npos ) continue;
    fileN = inputFile + "/" + fileN;
    cout << fileN.data() << endl;
    nfile++;
    infiles.push_back(fileN);
  }
  
  std::cout << "Opened " << nfile << " files" << std::endl;
  
  TreeReader data(infiles);
  
  // Declare the histogram
     
  TH1D* h_Zmass        = new TH1D("h_Zmass", "Zmass", 100, 50, 130);
  TH1D* h_Zpt          = new TH1D("h_Zpt", "Zpt", 100, 0, 130);
  TH1D* h_Zeta         = new TH1D("h_Zeta", "Zeta", 100, -10, 10);
  TH1D* h_leadMuPt     = new TH1D("h_leadMuPt", "leadMuPt", 100, 0, 130);
  TH1D* h_leadMuEta    = new TH1D("h_leadMuEta", "leadMuEta", 100, -5, 5);
  TH1D* h_subleadMuPt  = new TH1D("h_subleadMuPt", "subleadMuPt", 100, 0, 130);
  TH1D* h_subleadMuEta = new TH1D("h_subleadMuEta", "subleadMuEta", 100, -5, 5);
  TH1D* h_eventWeight  = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_Zmass       ->Sumw2();
  h_Zpt         ->Sumw2();
  h_Zeta        ->Sumw2();
  h_leadMuPt    ->Sumw2();
  h_leadMuEta   ->Sumw2();
  h_subleadMuPt ->Sumw2();
  h_subleadMuEta->Sumw2(); 
  
  h_Zmass       ->GetXaxis()->SetTitle("Zmass"); 
  h_Zpt         ->GetXaxis()->SetTitle("Zpt");   
  h_Zeta        ->GetXaxis()->SetTitle("Zeta");    
  h_leadMuPt    ->GetXaxis()->SetTitle("leadMuPt");  
  h_leadMuEta   ->GetXaxis()->SetTitle("leadMuEta");
  h_subleadMuPt ->GetXaxis()->SetTitle("subleadMuPt");   
  h_subleadMuEta->GetXaxis()->SetTitle("subleadMuEta"); 
  h_eventWeight ->GetXaxis()->SetTitle("eventWeight");
    
  // begin of event loop

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t    nVtx      = data.GetInt("nVtx");
    Int_t    nMu       = data.GetInt("nMu");
    Int_t*   muCharge  = data.GetPtrInt("muCharge");
    Float_t  mcWeight  = data.GetFloat("mcWeight");    
    Float_t* muMiniIso = data.GetPtrFloat("muMiniIso");
    TClonesArray* muP4 = (TClonesArray*) data.GetPtrTObject("muP4");
    vector<bool>& isHighPtMuon = *((vector<bool>*) data.GetPtr("isHighPtMuon"));
    vector<bool>& isCustomTrackerMuon = *((vector<bool>*) data.GetPtr("isCustomTrackerMuon"));

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

    // select good muons
      
    std::vector<Int_t> goodMuons;
  
    for(Int_t im = 0; im < nMu; im++){

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
	Float_t mll = (*thisMu+*thatMu).M();

	if( muCharge[im]*muCharge[jm] > 0 ) continue;
	if( mll < 60 || mll > 120 ) continue;
	if( !((isHighPtMuon[im] && isCustomTrackerMuon[jm]) ||
	      (isHighPtMuon[jm] && isCustomTrackerMuon[im])
	      )) continue;

	if( !findMPair ) l4_Z = (*thisMu+*thatMu);

	findMPair = true;
	break;

      }
    }

    if( !findMPair ) continue;

    h_Zmass->Fill(l4_Z.M());
    h_Zpt  ->Fill(l4_Z.Pt());
    h_Zeta ->Fill(l4_Z.Eta());

    if( thisMu->Pt() > thatMu->Pt() ){

      h_leadMuPt    ->Fill(thisMu->Pt());
      h_leadMuEta   ->Fill(thisMu->Eta());
      h_subleadMuPt ->Fill(thatMu->Pt());
      h_subleadMuEta->Fill(thatMu->Eta());

    }else{

      h_leadMuPt    ->Fill(thatMu->Pt());
      h_leadMuEta   ->Fill(thatMu->Eta());
      h_subleadMuPt ->Fill(thisMu->Pt());
      h_subleadMuEta->Fill(thisMu->Eta());

    }

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[8] = {"Zmass","Zpt","Zeta","leadMuPt","leadMuEta",
			   "subleadMuPt","subleadMuEta","eventWeight"};

  TFile* outFile = new TFile(Form("%s_ZmumuVariable.root",outputFile[num].c_str()), "recreate");
      
  h_Zmass       ->Write(h_name[0].data());  
  h_Zpt         ->Write(h_name[1].data());  
  h_Zeta        ->Write(h_name[2].data());    
  h_leadMuPt    ->Write(h_name[3].data()); 
  h_leadMuEta   ->Write(h_name[4].data());   
  h_subleadMuPt ->Write(h_name[5].data());
  h_subleadMuEta->Write(h_name[6].data());    
  h_eventWeight ->Write(h_name[7].data());
  
  outFile->Write();
  
}
