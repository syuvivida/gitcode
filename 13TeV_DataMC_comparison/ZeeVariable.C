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

// 25ns: root -q -b ZeeVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/DoubleEG_Run2015C-PromptReco-v1\"\,0\)
// 25ns: root -q -b ZeeVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0830/150830_215828/0000\"\,1\)
// 25/50ns: root -q -b ZeeVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803/150803_175618/0000\"\,2\)

void ZeeVariable(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;
 
  std::string outputFile[3] = {"DoubleEG_Run2015C-PromptReco-v1","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns",
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
     
  TH1D* h_Zmass         = new TH1D("h_Zmass", "Zmass", 100, 50, 130);
  TH1D* h_Zpt           = new TH1D("h_Zpt", "Zpt", 100, 0, 130);
  TH1D* h_Zeta          = new TH1D("h_Zeta", "Zeta", 100, -10, 10);
  TH1D* h_leadElePt     = new TH1D("h_leadElePt", "leadElePt", 100, 0, 130);
  TH1D* h_leadEleEta    = new TH1D("h_leadEleEta", "leadEleEta", 100, -5, 5);
  TH1D* h_subleadElePt  = new TH1D("h_subleadElePt", "subleadElePt", 100, 0, 130);
  TH1D* h_subleadEleEta = new TH1D("h_subleadEleEta", "subleadEleEta", 100, -5, 5);
  TH1D* h_eventWeight   = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_Zmass        ->Sumw2();
  h_Zpt          ->Sumw2();
  h_Zeta         ->Sumw2();
  h_leadElePt    ->Sumw2();
  h_leadEleEta   ->Sumw2();
  h_subleadElePt ->Sumw2();
  h_subleadEleEta->Sumw2(); 
  
  h_Zmass        ->GetXaxis()->SetTitle("Zmass"); 
  h_Zpt          ->GetXaxis()->SetTitle("Zpt");   
  h_Zeta         ->GetXaxis()->SetTitle("Zeta");    
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
    Int_t    nEle       = data.GetInt("nEle");
    Int_t*   eleCharge  = data.GetPtrInt("eleCharge");
    Float_t  mcWeight   = data.GetFloat("mcWeight");
    Float_t* eleScEt    = data.GetPtrFloat("eleScEt");
    Float_t* eleScEta   = data.GetPtrFloat("eleScEta");
    Float_t* eleMiniIso = data.GetPtrFloat("eleMiniIso");
    TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");
    vector<bool>& eleEcalDrivenSeed  = *((vector<bool>*) data.GetPtr("eleEcalDrivenSeed"));
    vector<bool>& eleIsPassHEEPNoIso = *((vector<bool>*) data.GetPtr("eleIsPassHEEPNoIso"));

    if( nVtx < 1 ) continue;

    Double_t eventWeight = mcWeight;
    if( num == 1 ){
      if( eventWeight > 0 ) eventWeight = 1;
      else if( eventWeight < 0 ) eventWeight = -1;
    }
    else if( num == 0 || num == 2 )
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
      
      if( thisTrig.find("HLT_DoubleEle33") != std::string::npos && results==1 ){
	passTrigger = true;
	break;
      }
      
    }

    if( !passTrigger ) continue;

    // select good electrons
        
    std::vector<Int_t> goodElectrons;

    for(Int_t ie = 0; ie < nEle; ie++){

      if( !(fabs(eleScEta[ie]) < 1.442 || fabs(eleScEta[ie]) > 1.566) ) continue;
      if( fabs(eleScEta[ie]) > 2.5 ) continue;
      if( eleScEt[ie] <= 35 ) continue;
      if( !eleEcalDrivenSeed[ie] ) continue;
      if( !eleIsPassHEEPNoIso[ie] ) continue;
      if( eleMiniIso[ie] >= 0.1 ) continue;

      goodElectrons.push_back(ie);

    } // end of ele loop

    // select good Z boson

    bool findEPair = false;
    TLorentzVector l4_Z(0,0,0,0);
    TLorentzVector* thisEle = NULL;
    TLorentzVector* thatEle = NULL;

    for(unsigned int i = 0; i < goodElectrons.size(); i++){

      Int_t ie = goodElectrons[i];
      thisEle = (TLorentzVector*)eleP4->At(ie);

      for(unsigned int j = 0; j < i; j++){

	Int_t je = goodElectrons[j];
	thatEle = (TLorentzVector*)eleP4->At(je);
	Float_t mll = (*thisEle+*thatEle).M();

	if( eleCharge[ie]*eleCharge[je] > 0 ) continue;   
	if( mll < 60 || mll > 120 ) continue;
	if( !findEPair ) l4_Z = (*thisEle+*thatEle);

	findEPair = true;
	break;

      }
    }

    if( !findEPair ) continue;

    h_Zmass->Fill(l4_Z.M());
    h_Zpt  ->Fill(l4_Z.Pt());
    h_Zeta ->Fill(l4_Z.Eta());

    if( thisEle->Pt() > thatEle->Pt() ){

      h_leadElePt    ->Fill(thisEle->Pt());
      h_leadEleEta   ->Fill(thisEle->Eta());
      h_subleadElePt ->Fill(thatEle->Pt());
      h_subleadEleEta->Fill(thatEle->Eta());

    }else{

      h_leadElePt    ->Fill(thatEle->Pt());
      h_leadEleEta   ->Fill(thatEle->Eta());
      h_subleadElePt ->Fill(thisEle->Pt());
      h_subleadEleEta->Fill(thisEle->Eta());

    }

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[8] = {"Zmass","Zpt","Zeta","leadElePt","leadEleEta",
			   "subleadElePt","subleadEleEta","eventWeight"};

  TFile* outFile = new TFile(Form("%s_ZeeVariable.root",outputFile[num].c_str()), "recreate");
      
  h_Zmass        ->Write(h_name[0].data());  
  h_Zpt          ->Write(h_name[1].data());  
  h_Zeta         ->Write(h_name[2].data());    
  h_leadElePt    ->Write(h_name[3].data()); 
  h_leadEleEta   ->Write(h_name[4].data());   
  h_subleadElePt ->Write(h_name[5].data());
  h_subleadEleEta->Write(h_name[6].data());    
  h_eventWeight  ->Write(h_name[7].data());
  
  outFile->Write();
  
}
