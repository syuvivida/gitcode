#include <string>
#include <iostream>
#include <fstream>
#include <TH1D.h>
#include <TFile.h>

void stcmZHmuSignal(){

  TFile *file[17];

  file[0] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[1] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[2] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[3] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[4] = TFile::Open("outputmZHmu/signal/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830_mZHmuSignal.root");
  file[5] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph_mZHmuSignal.root");
  file[6] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph_mZHmuSignal.root");
  file[7] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph_mZHmuSignal.root");
  file[8] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph_mZHmuSignal.root");
  file[9] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph_mZHmuSignal.root");
  file[10] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph_mZHmuSignal.root");
  file[11] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph_mZHmuSignal.root");
  file[12] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph_mZHmuSignal.root");
  file[13] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph_mZHmuSignal.root");
  file[14] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph_mZHmuSignal.root");
  file[15] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph_mZHmuSignal.root");
  file[16] = TFile::Open("outputmZHmu/signal/crab_SingleMuon-Run2015C-PromptReco-v1_mZHmuSignal.root");

  // i=4 is ttbar  
                                                                                     
  TH1D* h_mZprime[16];
  TH1D* h_eventWeight[16];
  Int_t nEvent[16] = {0};
  Double_t scale[16] = {0};
  Double_t signalLumi = 1000.0;  //pb-1
  Double_t crossSection[16] = { 139.4,       //HT100
				42.75,       //HT200
				5.497,       //HT400
				2.21,        //HT600
				831.76,      //TTbar
				0.0282665,   //M800
				0.0153743,   //M1000
				0.00790857,  //M1200
				0.00421385,  //M1400
				0.00233319,  //M1600
				0.00133522,  //M1800
				0.000785119, //M2000
				0.000227178, //M2500
				7.1426e-5,   //M3000
				2.35715e-5,  //M3500
				7.97489e-6 };//M4000 //pb

  for(Int_t i = 0; i < 16 ; i++){

    h_eventWeight[i] = (TH1D*)(file[i]->Get("eventWeight"));
    nEvent[i] = h_eventWeight[i]->Integral();
    scale[i] = signalLumi/(nEvent[i]/crossSection[i]);
    h_mZprime[i] = (TH1D*)(file[i]->Get("mZprime"));

  }

  TH1D* h_mZprimeDY = (TH1D*)h_mZprime[0]->Clone("h_mZprimeDY");
  h_mZprimeDY->Reset();

  for(Int_t i = 0; i < 4 ; i++)
    h_mZprimeDY->Add(h_mZprime[i],scale[i]);

  h_mZprime[4]->Scale(scale[4]);

  TFile* outFile = new TFile("forRaman_mZHmuSignal.root","recreate");

  std::string name[17] = {"","","",
			  "mZprime_DYHT-100toInf",
			  "mZprime_TTbar",
			  "mZprime_signalM-800",
			  "mZprime_signalM-1000",
			  "mZprime_signalM-1200",
			  "mZprime_signalM-1400",
			  "mZprime_signalM-1600",
			  "mZprime_signalM-1800",
			  "mZprime_signalM-2000",
			  "mZprime_signalM-2500",
			  "mZprime_signalM-3000",
			  "mZprime_signalM-3500",
			  "mZprime_signalM-4000",
			  "mZprime_Data"};
			 
  h_mZprimeDY->Write(name[3].data());

  for(Int_t i = 4; i < 16; i++)
    h_mZprime[i]->Write(name[i].data());

  TH1D* h_mZprimeData = (TH1D*)(file[16]->Get("mZprime"));
  h_mZprimeData->Write(name[16].data());

  outFile->Write();

  fstream ftext;
  ftext.open("forRaman_nEvent.txt", ios::out);

  ftext << name[3] << "  " << h_mZprimeDY->Integral() << "\n";

  for(Int_t i = 4; i < 16; i++)
    ftext << name[i] << "  " << h_mZprime[i]->Integral() << "\n";

  ftext << name[16] << "  " << h_mZprimeData->Integral() << "\n";

}
