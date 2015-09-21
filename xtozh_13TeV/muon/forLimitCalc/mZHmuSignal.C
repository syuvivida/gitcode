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

/* 
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162742/0000/\"\,0\);
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162821/0000/\"\,1\);
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162858/0000/\"\,2\);
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/DYJetsHTBins25nsSamples/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0803/150812_162937/0000/\"\,3\);
root -q -b mZHmuSignal.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15/TT_TuneCUETP8M1_13TeV-powheg-pythia8/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830/150831_085116/\"\,4\);
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

void mZHmuSignal(std::string inputFile, int num){

  // read the ntuples (in pcncu)

  std::vector<string> infiles;

  std::string outputFile[19] = {"DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
				"DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
				"DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
				"DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
				"crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830",
				"ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph",
				"ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph",
				"crab_SingleMuon-Run2015C-PromptReco-v1"};

  if( inputFile.find(".root") != std::string::npos ){ 
    
    infiles.push_back(inputFile.data());
  
  }else{

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
  }
  
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
    TClonesArray*  FATjetP4            = (TClonesArray*) data.GetPtrTObject("FATjetP4");
    vector<bool>&  FATjetPassIDLoose   = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));
    vector<float>* FATsubjetSDCSV      = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);

    if( nVtx < 1 ) continue;
  
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
    
    Float_t mZll = (*thisMu+*thatMu+*thisJet).M();

    h_mZprime->Fill(mZll,eventWeight);

  } // end of event loop

  fprintf(stderr, "Processed all events\n");

  std::string h_name[2] = {"mZprime","eventWeight"};

  TFile* outFile = new TFile(Form("%s_mZHmuSignal.root",outputFile[num].c_str()), "recreate");

  h_mZprime         ->Write(h_name[0].data());
  h_eventWeight     ->Write(h_name[1].data());

  outFile->Write();
  
}
