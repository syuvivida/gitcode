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

// 25ns: root -q -b muVariable.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/crab_SingleMuon-Run2015C-PromptReco-v1/150830_214159/0000\"\,0\)
// 25ns: root -q -b muVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns/crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0830/150830_215828/0000\"\,1\)
// 25ns/50ns: root -q -b muVariable.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803/150803_175618/0000\"\,2\)

void muVariable(std::string inputFile, int num){

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
  
  // Declare the histogram (hightptMuon, customizeTrackerMuon)

  TH1D* h_muHits[2]; 
  TH1D* h_muMatches[2];
  TH1D* h_muTrkLayers[2];  
  TH1D* h_muPixelHits[2];
  TH1D* h_muTrkPtErrdvTrkPt[2];
  TH1D* h_mudxy[2]; 
  TH1D* h_mudz[2];   
  TH1D* h_muMiniIso[2]; 
  TH1D* h_eventWeight[2];

  TProfile* pf_muHits[2];
  TProfile* pf_muMatches[2];
  TProfile* pf_muTrkLayers[2];
  TProfile* pf_muPixelHits[2];
  TProfile* pf_muTrkPtErrdvTrkPt[2];
  TProfile* pf_mudxy[2];
  TProfile* pf_mudz[2];
  TProfile* pf_muMiniIso[2];

  for(Int_t i = 0; i < 2; i++){
    
    h_muHits[i]      = new TH1D(Form("h_muHits%d",i), "muHits", 60, -0.5, 59.5);
    h_muMatches[i]   = new TH1D(Form("h_muMatches%d",i), "muMatches", 7, -0.5, 6.5);
    h_muTrkLayers[i] = new TH1D(Form("h_muTrkLayers%d",i), "muTrkLayers", 18, -0.5, 17.5);
    h_muPixelHits[i] = new TH1D(Form("h_muPixelHits%d",i), "muPixelHits", 8, -0.5, 7.5);
    h_muTrkPtErrdvTrkPt[i] = new TH1D(Form("h_muTrkPtErrdvTrkPt%d",i), "muTrkPtErrdvTrkPt", 100, 0, 0.4);
    h_mudxy[i]       = new TH1D(Form("h_mudxy%d",i), "mudxy", 100, -0.1, 0.1);
    h_mudz[i]        = new TH1D(Form("h_mudz%d",i), "mudz", 100, -1, 1);  
    h_muMiniIso[i]   = new TH1D(Form("h_muMiniIso%d",i), "muMiniIso", 100, 0, 0.15);
    h_eventWeight[i] = new TH1D(Form("h_eventWeight%d",i), "eventWeight", 20, -1, 1);

    h_muHits[i]     ->Sumw2();
    h_muMatches[i]  ->Sumw2();
    h_muTrkLayers[i]->Sumw2();
    h_muPixelHits[i]->Sumw2();
    h_muTrkPtErrdvTrkPt[i]->Sumw2();
    h_mudxy[i]      ->Sumw2();
    h_mudz[i]       ->Sumw2();
    h_muMiniIso[i]  ->Sumw2();

    h_muHits[i]     ->GetXaxis()->SetTitle("muHits");
    h_muMatches[i]  ->GetXaxis()->SetTitle("muMatches");
    h_muTrkLayers[i]->GetXaxis()->SetTitle("muTrkLayers");
    h_muPixelHits[i]->GetXaxis()->SetTitle("muPixelHits");
    h_muTrkPtErrdvTrkPt[i]->GetXaxis()->SetTitle("muTrkPtErrdvTrkPt");
    h_mudxy[i]      ->GetXaxis()->SetTitle("mudxy");
    h_mudz[i]       ->GetXaxis()->SetTitle("mudz");
    h_muMiniIso[i]  ->GetXaxis()->SetTitle("muMiniIso");
    h_eventWeight[i]->GetXaxis()->SetTitle("eventWeight");

    pf_muHits[i]      = new TProfile(Form("pf_muHits%d",i), "muHits profile", 25, 0.5, 25.5);
    pf_muMatches[i]   = new TProfile(Form("pf_muMatches%d",i), "muMatches profile", 25, 0.5, 25.5);
    pf_muTrkLayers[i] = new TProfile(Form("pf_muTrkLayers%d",i), "muTrkLayers profile", 25, 0.5, 25.5);
    pf_muPixelHits[i] = new TProfile(Form("pf_muPixelHits%d",i), "muPixelHits profile", 25, 0.5, 25.5);
    pf_muTrkPtErrdvTrkPt[i] = new TProfile(Form("pf_muTrkPtErrdvTrkPt%d",i), "muTrkPtErrdvTrkPt profile", 25, 0.5, 25.5);
    pf_mudxy[i]       = new TProfile(Form("pf_mudxy%d",i), "mudxy profile", 25, 0.5, 25.5);
    pf_mudz[i]        = new TProfile(Form("pf_mudz%d",i), "mudz profile", 25, 0.5, 25.5);
    pf_muMiniIso[i]   = new TProfile(Form("pf_muMiniIso%d",i), "muMiniIso profile", 25, 0.5, 25.5);

    pf_muHits[i]     ->GetXaxis()->SetTitle("muHits");
    pf_muMatches[i]  ->GetXaxis()->SetTitle("muMatches");
    pf_muTrkLayers[i]->GetXaxis()->SetTitle("muTrkLayers");
    pf_muPixelHits[i]->GetXaxis()->SetTitle("muPixelHits");
    pf_muTrkPtErrdvTrkPt[i]->GetXaxis()->SetTitle("muTrkPtErrdvTrkPt");
    pf_mudxy[i]      ->GetXaxis()->SetTitle("mudxy");
    pf_mudz[i]       ->GetXaxis()->SetTitle("mudz");
    pf_muMiniIso[i]  ->GetXaxis()->SetTitle("muMiniIso");

  }

  // begin of event loop

  Int_t nPass = 0;

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t    nVtx        = data.GetInt("nVtx");
    Int_t    nMu         = data.GetInt("nMu");
    Int_t*   muHits      = data.GetPtrInt("muHits");
    Int_t*   muMatches   = data.GetPtrInt("muMatches");
    Int_t*   muTrkLayers = data.GetPtrInt("muTrkLayers");
    Int_t*   muPixelHits = data.GetPtrInt("muPixelHits");
    Float_t  mcWeight    = data.GetFloat("mcWeight");    
    Float_t* muTrkPtErr  = data.GetPtrFloat("muTrkPtErr");	
    Float_t* muTrkPt     = data.GetPtrFloat("muTrkPt");
    Float_t* mudxy       = data.GetPtrFloat("mudxy");
    Float_t* mudz        = data.GetPtrFloat("mudz");
    Float_t* muMiniIso   = data.GetPtrFloat("muMiniIso");
    TClonesArray* muP4   = (TClonesArray*) data.GetPtrTObject("muP4");
    vector<bool>& isGlobalMuon  = *((vector<bool>*) data.GetPtr("isGlobalMuon"));
    vector<bool>& isTrackerMuon = *((vector<bool>*) data.GetPtr("isTrackerMuon"));

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

    // choosing muon

    Int_t muId[2] = {-1,-1};
    Float_t mll = -1;

    for(Int_t ie = 0; ie < nMu; ie++){

      TLorentzVector* thisMu = (TLorentzVector*)muP4->At(ie);

      if( !isGlobalMuon[ie] || !isTrackerMuon[ie] ) continue;
      if( thisMu->Pt() <= 50 ) continue;
      if( fabs(thisMu->Eta()) >= 2.1 ) continue;    

      for(Int_t je = 0; je < ie; je++){

	TLorentzVector* thatMu = (TLorentzVector*)muP4->At(je);
	
	if( !isGlobalMuon[je] || !isTrackerMuon[je] ) continue;
	if( thatMu->Pt() <= 50 ) continue;
	if( fabs(thatMu->Eta()) >= 2.1 ) continue;
		
	mll = (*thisMu+*thatMu).M();

	if( mll < 60 || mll > 120 ) continue;		

        muId[0] = ie;
	muId[1] = je;

	break;
	
      }
    }

    if(	muId[0] < 0 || muId[1] < 0 ) continue;
    
    nPass++;

    // muon selections and cuts

    for(Int_t ie = 0; ie < 2; ie++){

      if( isGlobalMuon[muId[ie]] ){ // highptMuon selections and cuts

	for(Int_t flag = 0; flag <= 7; flag++){

	  if( muHits[muId[ie]]      <  1   && flag != 0 ) continue;
	  if( muMatches[muId[ie]]   <  2   && flag != 1 ) continue;
	  if( muTrkLayers[muId[ie]] <  6   && flag != 2 ) continue;
	  if( muPixelHits[muId[ie]] <  1   && flag != 3 ) continue;
	  if( muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]] > 0.3 && flag != 4 ) continue;
	  if( fabs(mudxy[muId[ie]]) >  0.2 && flag != 5 ) continue;
	  if( fabs(mudz[muId[ie]])  >  0.5 && flag != 6 ) continue;
	  if( muMiniIso[muId[ie]]   >= 0.1 && flag != 7 ) continue;
	  	  	    
	  switch(flag){

	  case 0: 
	    h_muHits[0] ->Fill(muHits[muId[ie]],eventWeight);
	    pf_muHits[0]->Fill(nVtx,muHits[muId[ie]],eventWeight);
	    break;

	  case 1:
	    h_muMatches[0] ->Fill(muMatches[muId[ie]],eventWeight); 
	    pf_muMatches[0]->Fill(nVtx,muMatches[muId[ie]],eventWeight);
	    break;

	  case 2: 
	    h_muTrkLayers[0] ->Fill(muTrkLayers[muId[ie]],eventWeight);
	    pf_muTrkLayers[0]->Fill(nVtx,muTrkLayers[muId[ie]],eventWeight);
	    break;

	  case 3:
	    h_muPixelHits[0] ->Fill(muPixelHits[muId[ie]],eventWeight);
	    pf_muPixelHits[0]->Fill(nVtx,muPixelHits[muId[ie]],eventWeight);
	    break;

	  case 4: 
	    h_muTrkPtErrdvTrkPt[0] ->Fill(muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]],eventWeight); 
	    pf_muTrkPtErrdvTrkPt[0]->Fill(nVtx,muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]],eventWeight);
	    break;

	  case 5: 
	    h_mudxy[0] ->Fill(mudxy[muId[ie]],eventWeight);   
	    pf_mudxy[0]->Fill(nVtx,mudxy[muId[ie]],eventWeight);
	    break;	 

	  case 6: 
	    h_mudz[0] ->Fill(mudz[muId[ie]],eventWeight);
	    pf_mudz[0]->Fill(nVtx,mudz[muId[ie]],eventWeight);
	    break;

	  case 7: 
	    h_muMiniIso[0] ->Fill(muMiniIso[muId[ie]],eventWeight); 
	    pf_muMiniIso[0]->Fill(nVtx,muMiniIso[muId[ie]],eventWeight);
	    break;
	    
	  } // end of switch
    
	} // end of flag loop

      } // end of barrel


      if( isTrackerMuon[muId[ie]] ){ // customizeTrackerMuon selections and cuts

	for(Int_t flag = 0; flag <= 7; flag++){

	  if( muMatches[muId[ie]]   <  2   && flag != 0 ) continue;
          if( muTrkLayers[muId[ie]] <  6   && flag != 1 ) continue;
          if( muPixelHits[muId[ie]] <  1   && flag != 2 ) continue;
          if( muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]] > 0.3 && flag != 3 ) continue;
          if( fabs(mudxy[muId[ie]]) >  0.2 && flag != 4 ) continue;
          if( fabs(mudz[muId[ie]])  >  0.5 && flag != 5 ) continue;
          if( muMiniIso[muId[ie]]   >= 0.1 && flag != 6 ) continue;

          switch(flag){

	  case 0:
            h_muMatches[1] ->Fill(muMatches[muId[ie]],eventWeight);
            pf_muMatches[1]->Fill(nVtx,muMatches[muId[ie]],eventWeight);
            break;

          case 1:
            h_muTrkLayers[1] ->Fill(muTrkLayers[muId[ie]],eventWeight);
            pf_muTrkLayers[1]->Fill(nVtx,muTrkLayers[muId[ie]],eventWeight);
            break;

          case 2:
            h_muPixelHits[1] ->Fill(muPixelHits[muId[ie]],eventWeight);
            pf_muPixelHits[1]->Fill(nVtx,muPixelHits[muId[ie]],eventWeight);
            break;

          case 3:
            h_muTrkPtErrdvTrkPt[1] ->Fill(muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]],eventWeight);
            pf_muTrkPtErrdvTrkPt[1]->Fill(nVtx,muTrkPtErr[muId[ie]]/muTrkPt[muId[ie]],eventWeight);
            break;

          case 4:
            h_mudxy[1] ->Fill(mudxy[muId[ie]],eventWeight);
            pf_mudxy[1]->Fill(nVtx,mudxy[muId[ie]],eventWeight);
            break;

          case 5:
            h_mudz[1] ->Fill(mudz[muId[ie]],eventWeight);
            pf_mudz[1]->Fill(nVtx,mudz[muId[ie]],eventWeight);
            break;

          case 6:
            h_muMiniIso[1] ->Fill(muMiniIso[muId[ie]],eventWeight);
            pf_muMiniIso[1]->Fill(nVtx,muMiniIso[muId[ie]],eventWeight);
            break;

	  case 7:
            h_muHits[1] ->Fill(muHits[muId[ie]],eventWeight);
            pf_muHits[1]->Fill(nVtx,muHits[muId[ie]],eventWeight);
            break;
    
	  } // end of switch
	
	} // end of flag loop

      } // end of endcap
	
    } // end of two muctrons loop
      
  } // end of event loop

  fprintf(stderr, "Processed all events\n");
    
  std::cout << "pass events: " << nPass << std::endl;

  TFile* outFile[2];
  
  std::string h_name[9] = {"muMatches","muTrkLayers","muPixelHits","muTrkPtErrdvTrkPt",
			   "mudxy","mudz","muMiniIso","muHits","eventWeight"};

  std::string pf_name[8] = {"pf_muMatches","pf_muTrkLayers","pf_muPixelHits","pf_muTrkPtErrdvTrkPt",
			    "pf_mudxy","pf_mudz","pf_muMiniIso","pf_muHits"};

  std::string region[2] = {"highptMuon","customizeTrackerMuon"};

  for(Int_t i = 0; i < 2; i++){

    outFile[i] = new TFile(Form("%s_%s.root",outputFile[num].c_str(),region[i].c_str()), "recreate");
      
    h_muMatches[i]        ->Write(h_name[0].data());
    h_muTrkLayers[i]      ->Write(h_name[1].data());
    h_muPixelHits[i]      ->Write(h_name[2].data());
    h_muTrkPtErrdvTrkPt[i]->Write(h_name[3].data());
    h_mudxy[i]            ->Write(h_name[4].data());
    h_mudz[i]             ->Write(h_name[5].data());
    h_muMiniIso[i]        ->Write(h_name[6].data());
    h_muHits[i]           ->Write(h_name[7].data());
    h_eventWeight[i]      ->Write(h_name[8].data());

    pf_muMatches[i]        ->Write(pf_name[0].data());
    pf_muTrkLayers[i]      ->Write(pf_name[1].data());
    pf_muPixelHits[i]      ->Write(pf_name[2].data());
    pf_muTrkPtErrdvTrkPt[i]->Write(pf_name[3].data());
    pf_mudxy[i]            ->Write(pf_name[4].data());
    pf_mudz[i]             ->Write(pf_name[5].data());
    pf_muMiniIso[i]        ->Write(pf_name[6].data());
    pf_muHits[i]           ->Write(pf_name[7].data());

    outFile[i]->Write();

  }
  
}
