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
#include "../isPassZmumu.h"

// DYHT: root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162742/0000/\"\,0\);
// DYHT: root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162821/0000/\"\,1\);
// DYHT: root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162858/0000/\"\,2\);
// DYHT: root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162937/0000/\"\,3\);
// ttbar:  root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830/150831_085116/0000/\"\,4\);
// data:   root -q -b mZHmu.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015C/crab_SingleMuon-Run2015C-PromptReco-v1/150830_214159/0000/\"\,5\); 

void mZHmu(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  std::string outputFile[6] = {"DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
			       "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
			       "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
			       "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
			       "crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830",
			       "crab_SingleMuon-Run2015C-PromptReco-v1"};
			  
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
     
  TH1D* h_mZprime          = new TH1D("h_mZprime", "mZprime", 100, 400, 5000);
  TH1D* h_mZ               = new TH1D("h_mZ", "mZ", 100, 50, 150);
  TH1D* h_ptZ              = new TH1D("h_ptZ", "ptZ", 100, 0, 20);
  TH1D* h_FATjetPt         = new TH1D("h_FATjetPt", "FATjetPt", 100, 100, 1000);
  TH1D* h_FATjetSDmass     = new TH1D("h_FATjetSDmass", "FATjetSDmass", 100, 50, 200);
  TH1D* h_FATjetPRmass     = new TH1D("h_FATjetPRmass", "FATjetPRmass", 100, 50, 200);
  TH1D* h_FATjetTau2dvTau1 = new TH1D("h_FATjetTau2dvTau1", "FATjetTau2dvTau1", 100, 0, 1);
  TH1D* h_cutFlow          = new TH1D("h_cutFlow", "cutFlow", 5, 0, 5);
  TH1D* h_eventWeight      = new TH1D("h_eventWeight", "eventWeight", 100, -1, 1);

  h_mZprime         ->Sumw2();
  h_mZ              ->Sumw2();
  h_ptZ             ->Sumw2();
  h_FATjetPt        ->Sumw2();   
  h_FATjetSDmass    ->Sumw2();
  h_FATjetPRmass    ->Sumw2();
  h_FATjetTau2dvTau1->Sumw2();
  h_cutFlow         ->Sumw2();  

  h_mZprime         ->GetXaxis()->SetTitle("mZprime");
  h_mZ              ->GetXaxis()->SetTitle("mZ");
  h_ptZ             ->GetXaxis()->SetTitle("ptZ");
  h_FATjetPt        ->GetXaxis()->SetTitle("FATjetPt");
  h_FATjetSDmass    ->GetXaxis()->SetTitle("FATjetSDmass");
  h_FATjetPRmass    ->GetXaxis()->SetTitle("FATjetPRmass");
  h_FATjetTau2dvTau1->GetXaxis()->SetTitle("FATjetTau2dvTau1");
  h_cutFlow         ->GetXaxis()->SetTitle("cutFlow");  
  h_eventWeight     ->GetXaxis()->SetTitle("eventWeight");  
    
  // begin of event loop

  Int_t nPass[10] = {0};

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){

    if ( ev % 100000 == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    data.GetEntry(ev);

    Int_t          nVtx                = data.GetInt("nVtx");
    Float_t        mcWeight            = data.GetFloat("mcWeight");    
    TClonesArray*  muP4                = (TClonesArray*) data.GetPtrTObject("muP4");
    Int_t          FATnJet             = data.GetInt("FATnJet");    
    Int_t*         FATnSubSDJet        = data.GetPtrInt("FATnSubSDJet");
    Float_t*       FATjetCISVV2        = data.GetPtrFloat("FATjetCISVV2");
    Float_t*       FATjetSDmass        = data.GetPtrFloat("FATjetSDmass");
    Float_t*       FATjetPRmass        = data.GetPtrFloat("FATjetPRmass");
    Float_t*       FATjetTau1          = data.GetPtrFloat("FATjetTau1");
    Float_t*       FATjetTau2          = data.GetPtrFloat("FATjetTau2");
    TClonesArray*  FATjetP4            = (TClonesArray*) data.GetPtrTObject("FATjetP4");
    vector<bool>&  FATjetPassIDLoose   = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));
    vector<float>* FATsubjetSDCSV      = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);

    if( nVtx < 1 ) continue;
    nPass[0]++;

    Double_t eventWeight = mcWeight;
    if( num==0 || num==1 || num==2 || num==3 ){
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
    nPass[1]++;

    // select good muons
      
    vector<Int_t> goodMuID;
    if( !isPassZmumu(data, goodMuID) ) continue;
    nPass[2]++;

    TLorentzVector* thisMu = (TLorentzVector*)muP4->At(goodMuID[0]);
    TLorentzVector* thatMu = (TLorentzVector*)muP4->At(goodMuID[1]);
 
    Float_t mll  = (*thisMu+*thatMu).M();
    Float_t ptll = (*thisMu+*thatMu).Pt();

    h_mZ ->Fill(mll,eventWeight);
    h_ptZ->Fill(ptll,eventWeight);

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

      for(Int_t is = 0; is < FATnSubSDJet[ij]; is++)
	if( FATsubjetSDCSV[ij][is] < 0.605 ) continue;
      
      goodFATJetID = ij;
      break;

    }

    if( goodFATJetID < 0 ) continue; 
    nPass[3]++;
    
    h_FATjetPt        ->Fill(thisJet->Pt(),eventWeight);
    h_FATjetSDmass    ->Fill(FATjetSDmass[goodFATJetID],eventWeight);
    h_FATjetPRmass    ->Fill(FATjetPRmass[goodFATJetID],eventWeight);
    h_FATjetTau2dvTau1->Fill(FATjetTau2[goodFATJetID]/FATjetTau1[goodFATJetID],eventWeight);

    Float_t mZll = (*thisMu+*thatMu+*thisJet).M();

    h_mZprime->Fill(mZll,eventWeight);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string cutName[4] = {"TotalEvents","Vertex","MuPair","FATjet"};

  for(Int_t i = 1; i <= 4; i++){

    if( i==1 ) h_cutFlow->SetBinContent(i,((Int_t)data.GetEntriesFast()));
    else h_cutFlow->SetBinContent(i,nPass[i-2]);
    h_cutFlow->GetXaxis()->SetBinLabel(i,cutName[i-1].data()); 

  }

  std::string h_name[9] = {"mZprime","mZ","ptZ","FATjetPt","FATjetSDmass",
			   "FATjetPRmass","FATjetTau2dvTau1","cutFlow","eventWeight"};

  TFile* outFile = new TFile(Form("%s_mZHmu.root",outputFile[num].c_str()), "recreate");

  h_mZprime         ->Write(h_name[0].data());
  h_mZ              ->Write(h_name[1].data());
  h_ptZ             ->Write(h_name[2].data());
  h_FATjetPt        ->Write(h_name[3].data());
  h_FATjetSDmass    ->Write(h_name[4].data());
  h_FATjetPRmass    ->Write(h_name[5].data());
  h_FATjetTau2dvTau1->Write(h_name[6].data());
  h_cutFlow         ->Write(h_name[7].data());
  h_eventWeight     ->Write(h_name[8].data());

  outFile->Write();
  
}
