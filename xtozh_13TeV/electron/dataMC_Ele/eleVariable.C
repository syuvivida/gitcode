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

// 25ns data: root -q -b eleVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/DoubleEG_Run2015C-PromptReco-v1/\"\,0\);
// 25ns DY: root -q -b eleVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0830/150830_215828/0000/\"\,1\);
// 25ns TTbar: root -q -b eleVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830/150831_085116/0000/\"\,2\);

void eleVariable(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  std::string outputFile[3] = {"DoubleEG_Run2015C-PromptReco-v1","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns",
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
  
  // Declare the histogram (barrel,endcap)

  TH1D* h_eleEtaseedAtVtx[2]; 
  TH1D* h_eledPhiAtVtx[2];
  TH1D* h_eleHoverE[2];  
  TH1D* h_eleSigmaIEtaIEtaFull5x5[2];
  TH1D* h_eleFull5x5E2x5dvE5x5[2];
  TH1D* h_eleFull5x5E1x5dvE5x5[2]; 
  TH1D* h_eleMissHits[2]; 
  TH1D* h_eleD0[2];   
  TH1D* h_eleMiniIso[2]; 
  TH1D* h_dilepMass[2]; // 0: loose; 1: tight
  TH1D* h_eventWeight[2];

  TProfile* pf_eleEtaseedAtVtx[2];
  TProfile* pf_eledPhiAtVtx[2];
  TProfile* pf_eleHoverE[2];
  TProfile* pf_eleSigmaIEtaIEtaFull5x5[2];
  TProfile* pf_eleFull5x5E2x5dvE5x5[2];
  TProfile* pf_eleFull5x5E1x5dvE5x5[2];
  TProfile* pf_eleMissHits[2];
  TProfile* pf_eleD0[2];
  TProfile* pf_eleMiniIso[2];

  for(Int_t i = 0; i < 2; i++){
    
    h_eleEtaseedAtVtx[i]         = new TH1D(Form("h_eleEtaseedAtVtx%d",i), "eleEtaseedAtVtx", 100, -0.02, 0.02);
    h_eledPhiAtVtx[i]            = new TH1D(Form("h_eledPhiAtVtx%d",i), "eledPhiAtVtx", 100, -0.03, 0.03);
    h_eleHoverE[i]               = new TH1D(Form("h_eleHoverE%d",i), "eleHoverE", 100, 0, 0.05);
    h_eleSigmaIEtaIEtaFull5x5[i] = new TH1D(Form("h_eleSigmaIEtaIEtaFull5x5%d",i), "eleSigmaIEtaIEtaFull5x5", 100, 0, 0.05);
    h_eleFull5x5E2x5dvE5x5[i]    = new TH1D(Form("h_eleFull5x5E2x5dvE5x5%d",i), "eleFull5x5E2x5dvE5x5", 100, 0, 1.5);
    h_eleFull5x5E1x5dvE5x5[i]    = new TH1D(Form("h_eleFull5x5E1x5dvE5x5%d",i), "eleFull5x5E1x5dvE5x5", 100, 0, 1.5);
    h_eleMissHits[i]             = new TH1D(Form("h_eleMissHits%d",i), "eleMissHits", 6, -0.5, 5.5);
    h_eleD0[i]                   = new TH1D(Form("h_eleD0%d",i), "eleD0", 100, -0.015, 0.015);  
    h_eleMiniIso[i]              = new TH1D(Form("h_eleMiniIso%d",i), "eleMiniIso", 100, 0, 0.15);
    h_dilepMass[i]               = new TH1D(Form("h_dilepMass%d",i), "dilepMass", 100, 50, 150);
    h_eventWeight[i]             = new TH1D(Form("h_eventWeight%d",i), "eventWeight", 100, -1, 1);

    h_eleEtaseedAtVtx[i]        ->Sumw2();
    h_eledPhiAtVtx[i]           ->Sumw2();
    h_eleHoverE[i]              ->Sumw2();
    h_eleSigmaIEtaIEtaFull5x5[i]->Sumw2();
    h_eleFull5x5E2x5dvE5x5[i]   ->Sumw2();
    h_eleFull5x5E1x5dvE5x5[i]   ->Sumw2();
    h_eleMissHits[i]            ->Sumw2(); 
    h_eleD0[i]                  ->Sumw2();
    h_eleMiniIso[i]             ->Sumw2();
    h_dilepMass[i]              ->Sumw2();

    h_eleEtaseedAtVtx[i]        ->GetXaxis()->SetTitle("eleEtaseedAtVtx"); 
    h_eledPhiAtVtx[i]           ->GetXaxis()->SetTitle("eledPhiAtVtx");   
    h_eleHoverE[i]              ->GetXaxis()->SetTitle("eleHoverE");    
    h_eleSigmaIEtaIEtaFull5x5[i]->GetXaxis()->SetTitle("eleSigmaIEtaIEtaFull5x5");  
    h_eleFull5x5E2x5dvE5x5[i]   ->GetXaxis()->SetTitle("eleFull5x5E2x5dvE5x5");
    h_eleFull5x5E1x5dvE5x5[i]   ->GetXaxis()->SetTitle("eleFull5x5E1x5dvE5x5");   
    h_eleMissHits[i]            ->GetXaxis()->SetTitle("eleMissHits"); 
    h_eleD0[i]                  ->GetXaxis()->SetTitle("eleD0");      
    h_eleMiniIso[i]             ->GetXaxis()->SetTitle("eleMiniIso"); 
    h_eventWeight[i]            ->GetXaxis()->SetTitle("eventWeight");

    pf_eleEtaseedAtVtx[i]         = new TProfile(Form("pf_eleEtaseedAtVtx%d",i), "eleEtaseedAtVtx profile", 10, 0.5, 20.5);
    pf_eledPhiAtVtx[i]            = new TProfile(Form("pf_eledPhiAtVtx%d",i), "eledPhiAtVtx profile", 10, 0.5, 20.5);
    pf_eleHoverE[i]               = new TProfile(Form("pf_eleHoverE%d",i), "eleHoverE profile", 10, 0.5, 20.5);
    pf_eleSigmaIEtaIEtaFull5x5[i] = new TProfile(Form("pf_eleSigmaIEtaIEtaFull5x5%d",i), "eleSigmaIEtaIEtaFull5x5 profile", 10, 0.5, 20.5);
    pf_eleFull5x5E2x5dvE5x5[i]    = new TProfile(Form("pf_eleFull5x5E2x5dvE5x5%d",i), "eleFull5x5E2x5dvE5x5 profile", 10, 0.5, 20.5);
    pf_eleFull5x5E1x5dvE5x5[i]    = new TProfile(Form("pf_eleFull5x5E1x5dvE5x5%d",i), "eleFull5x5E1x5dvE5x5 profile", 10, 0.5, 20.5);
    pf_eleMissHits[i]             = new TProfile(Form("pf_eleMissHits%d",i), "eleMissHits profile", 10, 0.5, 20.5);
    pf_eleD0[i]                   = new TProfile(Form("pf_eleD0%d",i), "eleD0 profile", 10, 0.5, 20.5);
    pf_eleMiniIso[i]              = new TProfile(Form("pf_eleMiniIso%d",i), "eleMiniIso profile", 10, 0.5, 20.5);

    pf_eleEtaseedAtVtx[i]        ->GetXaxis()->SetTitle("eleEtaseedAtVtx");
    pf_eledPhiAtVtx[i]           ->GetXaxis()->SetTitle("eledPhiAtVtx");
    pf_eleHoverE[i]              ->GetXaxis()->SetTitle("eleHoverE");
    pf_eleSigmaIEtaIEtaFull5x5[i]->GetXaxis()->SetTitle("eleSigmaIEtaIEtaFull5x5");
    pf_eleFull5x5E2x5dvE5x5[i]   ->GetXaxis()->SetTitle("eleFull5x5E2x5dvE5x5");
    pf_eleFull5x5E1x5dvE5x5[i]   ->GetXaxis()->SetTitle("eleFull5x5E1x5dvE5x5");
    pf_eleMissHits[i]            ->GetXaxis()->SetTitle("eleMissHits");
    pf_eleD0[i]                  ->GetXaxis()->SetTitle("eleD0");
    pf_eleMiniIso[i]             ->GetXaxis()->SetTitle("eleMiniIso");

  }

  h_dilepMass[0]->GetXaxis()->SetTitle("dilepMass_loose");
  h_dilepMass[1]->GetXaxis()->SetTitle("dilepMass_tight");

  // begin of event loop

  Int_t nPass = 0;

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t    nVtx                    = data.GetInt("nVtx");
    Int_t    nEle                    = data.GetInt("nEle");
    Int_t*   eleMissHits             = data.GetPtrInt("eleMissHits");
    Float_t  mcWeight                = data.GetFloat("mcWeight");
    Float_t* eleScEn                 = data.GetPtrFloat("eleScEn");
    Float_t* eleScEt                 = data.GetPtrFloat("eleScEt");
    Float_t* eleScEta                = data.GetPtrFloat("eleScEta");
    Float_t* eleEtaseedAtVtx         = data.GetPtrFloat("eleEtaseedAtVtx");	
    Float_t* eledPhiAtVtx            = data.GetPtrFloat("eledPhiAtVtx");
    Float_t* eleHoverE               = data.GetPtrFloat("eleHoverE");
    Float_t* eleSigmaIEtaIEtaFull5x5 = data.GetPtrFloat("eleSigmaIEtaIEtaFull5x5");
    Float_t* eleE1x5Full5x5          = data.GetPtrFloat("eleE1x5Full5x5");
    Float_t* eleE2x5Full5x5          = data.GetPtrFloat("eleE2x5Full5x5");
    Float_t* eleE5x5Full5x5          = data.GetPtrFloat("eleE5x5Full5x5");   
    Float_t* eleD0                   = data.GetPtrFloat("eleD0");
    Float_t* eleMiniIso              = data.GetPtrFloat("eleMiniIso");
    TClonesArray* eleP4              = (TClonesArray*) data.GetPtrTObject("eleP4");
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
    
    h_eventWeight[0]->Fill(0.,eventWeight);
    h_eventWeight[1]->Fill(0.,eventWeight);

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

    // choosing electron

    Int_t eleId[2] = {-1,-1};
    Float_t mll = -1;

    bool findEPair = false;
    bool findTightEPair = false;
    
    for(Int_t ie = 0; ie < nEle; ie++){

      if( !(fabs(eleScEta[ie]) < 1.442 || fabs(eleScEta[ie]) > 1.566) ) continue;
      if( fabs(eleScEta[ie]) > 2.5 ) continue;

      TLorentzVector* thisEle = (TLorentzVector*)eleP4->At(ie);
      
      for(Int_t je = 0; je < ie; je++){

	if( eleScEt[ie] <= 35 || eleScEt[je] <= 35 ) continue;
	if( !eleEcalDrivenSeed[ie] || !eleEcalDrivenSeed[je] ) continue;
	if( !(fabs(eleScEta[je]) < 1.442 || fabs(eleScEta[je]) > 1.566) ) continue;
	if( fabs(eleScEta[je]) > 2.5 ) continue;

	TLorentzVector* thatEle = (TLorentzVector*)eleP4->At(je);
	
	mll = (*thisEle+*thatEle).M();

	if( mll < 60 || mll > 120 ) continue;		
	if( !findEPair ) h_dilepMass[0]->Fill(mll,eventWeight); // loose

	findEPair = true;

        eleId[0] = ie;
	eleId[1] = je;

	if( !eleIsPassHEEPNoIso[ie] || !eleIsPassHEEPNoIso[je] ) continue;
	if( !findTightEPair ) h_dilepMass[1]->Fill(mll,eventWeight); // tight

	findTightEPair = true;

	break;
	
      }
    }

    if(	eleId[0] < 0 || eleId[1] < 0 ) continue;
    
    nPass++;

    // electron selections and cuts

    for(Int_t ie = 0; ie < 2; ie++){

      Float_t E = eleScEn[eleId[ie]];

      if( fabs(eleScEta[eleId[ie]]) < 1.4442 ){ // barrel selections and cuts

	for(Int_t flag = 0; flag <= 7; flag++){

	  if( fabs(eleEtaseedAtVtx[eleId[ie]])   >= 0.004     && flag != 0 ) continue;
	  if( fabs(eledPhiAtVtx[eleId[ie]])      >= 0.06      && flag != 1 ) continue;
	  if( eleHoverE[eleId[ie]]               >= 1/E+0.05  && flag != 2 ) continue;
	  if( eleMissHits[eleId[ie]]             >  1         && flag != 3 ) continue;
	  if( fabs(eleD0[eleId[ie]])             >= 0.02      && flag != 4 ) continue;
	  if( eleMiniIso[eleId[ie]]              >= 0.1       && flag != 5 ) continue;
	  if( (eleE2x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]] <= 0.94  &&
	       eleE1x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]] <= 0.83) && flag != 6 )
	    continue;
	  	    
	  switch(flag){

	  case 0: 
	    h_eleEtaseedAtVtx[0] ->Fill(eleEtaseedAtVtx[eleId[ie]],eventWeight);
	    pf_eleEtaseedAtVtx[0]->Fill(nVtx,eleEtaseedAtVtx[eleId[ie]],eventWeight);
	    break;

	  case 1:
	    h_eledPhiAtVtx[0] ->Fill(eledPhiAtVtx[eleId[ie]],eventWeight);
	    pf_eledPhiAtVtx[0]->Fill(nVtx,eledPhiAtVtx[eleId[ie]],eventWeight);
	    break;

	  case 2:
	    h_eleHoverE[0] ->Fill(eleHoverE[eleId[ie]],eventWeight); 
	    pf_eleHoverE[0]->Fill(nVtx,eleHoverE[eleId[ie]],eventWeight);
	    break;

	  case 3:
	    h_eleMissHits[0] ->Fill(eleMissHits[eleId[ie]],eventWeight);   
	    pf_eleMissHits[0]->Fill(nVtx,eleMissHits[eleId[ie]],eventWeight);
	    break;

	  case 4:
	    h_eleD0[0] ->Fill(eleD0[eleId[ie]],eventWeight);           
	    pf_eleD0[0]->Fill(nVtx,eleD0[eleId[ie]],eventWeight);
	    break;

	  case 5:
	    h_eleMiniIso[0] ->Fill(eleMiniIso[eleId[ie]],eventWeight);      
	    pf_eleMiniIso[0]->Fill(nVtx,eleMiniIso[eleId[ie]],eventWeight);
	    break;	 

	  case 6:
	    h_eleFull5x5E2x5dvE5x5[0] ->Fill(eleE2x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    h_eleFull5x5E1x5dvE5x5[0] ->Fill(eleE1x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    pf_eleFull5x5E2x5dvE5x5[0]->Fill(nVtx,eleE2x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
            pf_eleFull5x5E1x5dvE5x5[0]->Fill(nVtx,eleE1x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    break;

	  case 7: 
	    h_eleSigmaIEtaIEtaFull5x5[0] ->Fill(eleSigmaIEtaIEtaFull5x5[eleId[ie]],eventWeight); 
	    pf_eleSigmaIEtaIEtaFull5x5[0]->Fill(nVtx,eleSigmaIEtaIEtaFull5x5[eleId[ie]],eventWeight);
	    break;
	    
	  } // end of switch
    
	} // end of flag loop

      } // end of barrel


      if( fabs(eleScEta[eleId[ie]]) > 1.566 && fabs(eleScEta[eleId[ie]]) < 2.5 ){ // endcap selections and cuts

	for(Int_t flag = 0; flag <= 7; flag++){

	  if( fabs(eleEtaseedAtVtx[eleId[ie]])   >= 0.006    && flag != 0 ) continue;
	  if( fabs(eledPhiAtVtx[eleId[ie]])      >= 0.06     && flag != 1 ) continue;
	  if( eleHoverE[eleId[ie]]               >= 5/E+0.05 && flag != 2 ) continue;
	  if( eleSigmaIEtaIEtaFull5x5[eleId[ie]] >= 0.03     && flag != 3 ) continue;
	  if( eleMissHits[eleId[ie]]             >  1        && flag != 4 ) continue;
	  if( fabs(eleD0[eleId[ie]])             >= 0.05     && flag != 5 ) continue;
	  if( eleMiniIso[eleId[ie]]              >= 0.1      && flag != 6 ) continue;
	  	  	    
	  switch(flag){

	  case 0:
	    h_eleEtaseedAtVtx[1] ->Fill(eleEtaseedAtVtx[eleId[ie]],eventWeight); 
	    pf_eleEtaseedAtVtx[1]->Fill(nVtx,eleEtaseedAtVtx[eleId[ie]],eventWeight);
	    break;

	  case 1:
	    h_eledPhiAtVtx[1] ->Fill(eledPhiAtVtx[eleId[ie]],eventWeight);  
	    pf_eledPhiAtVtx[1]->Fill(nVtx,eledPhiAtVtx[eleId[ie]],eventWeight);
	    break;

	  case 2:
	    h_eleHoverE[1] ->Fill(eleHoverE[eleId[ie]],eventWeight);  
	    pf_eleHoverE[1]->Fill(nVtx,eleHoverE[eleId[ie]],eventWeight);
	    break;

	  case 3:
	    h_eleSigmaIEtaIEtaFull5x5[1] ->Fill(eleSigmaIEtaIEtaFull5x5[eleId[ie]],eventWeight);
	    pf_eleSigmaIEtaIEtaFull5x5[1]->Fill(nVtx,eleSigmaIEtaIEtaFull5x5[eleId[ie]],eventWeight);
	    break;

	  case 4:
	    h_eleMissHits[1] ->Fill(eleMissHits[eleId[ie]],eventWeight);
	    pf_eleMissHits[1]->Fill(nVtx,eleMissHits[eleId[ie]],eventWeight);
	    break;

	  case 5:
	    h_eleD0[1] ->Fill(eleD0[eleId[ie]],eventWeight);    
	    pf_eleD0[1]->Fill(nVtx,eleD0[eleId[ie]],eventWeight);
	    break;

	  case 6:
	    h_eleMiniIso[1] ->Fill(eleMiniIso[eleId[ie]],eventWeight);
	    pf_eleMiniIso[1]->Fill(nVtx,eleMiniIso[eleId[ie]],eventWeight);
	    break;	  

	  case 7:
	    h_eleFull5x5E2x5dvE5x5[1] ->Fill(eleE2x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    h_eleFull5x5E1x5dvE5x5[1] ->Fill(eleE1x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    pf_eleFull5x5E2x5dvE5x5[1]->Fill(nVtx,eleE2x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
            pf_eleFull5x5E1x5dvE5x5[1]->Fill(nVtx,eleE1x5Full5x5[eleId[ie]]/eleE5x5Full5x5[eleId[ie]],eventWeight);
	    break;
		    
	  } // end of switch
	
	} // end of flag loop

      } // end of endcap
	
    } // end of two electrons loop
      
  } // end of event loop

  fprintf(stderr, "Processed all events\n");
    
  std::cout << "pass events: " << nPass << std::endl;

  TFile* outFile0 = new TFile(Form("%s_dilepMass.root",outputFile[num].c_str()), "recreate");
  h_dilepMass[0]->Write("dilepMass_loose");
  h_dilepMass[1]->Write("dilepMass_tight");
  outFile0->Write();

  TFile* outFile[2];
  
  std::string h_name[10] = {"eleEtaseedAtVtx","eledPhiAtVtx","eleHoverE","eleSigmaIEtaIEtaFull5x5",
			    "eleFull5x5E2x5dvE5x5","eleFull5x5E1x5dvE5x5","eleMissHits","eleD0",
			    "eleMiniIso","eventWeight"};

  std::string pf_name[9] = {"pf_eleEtaseedAtVtx","pf_eledPhiAtVtx","pf_eleHoverE","pf_eleSigmaIEtaIEtaFull5x5",
                            "pf_eleFull5x5E2x5dvE5x5","pf_eleFull5x5E1x5dvE5x5","pf_eleMissHits","pf_eleD0",
                            "pf_eleMiniIso"};

  std::string region[2] = {"barrel","endcap"};

  for(Int_t i = 0; i < 2; i++){

    outFile[i] = new TFile(Form("%s_%s.root",outputFile[num].c_str(),region[i].c_str()), "recreate");
      
    h_eleEtaseedAtVtx[i]        ->Write(h_name[0].data());  
    h_eledPhiAtVtx[i]           ->Write(h_name[1].data());  
    h_eleHoverE[i]              ->Write(h_name[2].data());    
    h_eleSigmaIEtaIEtaFull5x5[i]->Write(h_name[3].data()); 
    h_eleFull5x5E2x5dvE5x5[i]   ->Write(h_name[4].data());   
    h_eleFull5x5E1x5dvE5x5[i]   ->Write(h_name[5].data());    
    h_eleMissHits[i]            ->Write(h_name[6].data());   
    h_eleD0[i]                  ->Write(h_name[7].data());      
    h_eleMiniIso[i]             ->Write(h_name[8].data());
    h_eventWeight[i]            ->Write(h_name[9].data());

    pf_eleEtaseedAtVtx[i]        ->Write(pf_name[0].data());
    pf_eledPhiAtVtx[i]           ->Write(pf_name[1].data());
    pf_eleHoverE[i]              ->Write(pf_name[2].data());
    pf_eleSigmaIEtaIEtaFull5x5[i]->Write(pf_name[3].data());
    pf_eleFull5x5E2x5dvE5x5[i]   ->Write(pf_name[4].data());
    pf_eleFull5x5E1x5dvE5x5[i]   ->Write(pf_name[5].data());
    pf_eleMissHits[i]            ->Write(pf_name[6].data());
    pf_eleD0[i]                  ->Write(pf_name[7].data());
    pf_eleMiniIso[i]             ->Write(pf_name[8].data());

    outFile[i]->Write();

  }
  
}