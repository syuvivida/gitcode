#include <string>
#include <iostream>
#include <fstream>
#include <TH1D.h>
#include <TFile.h>

void stcmZHmuSignal(){

  TFile *file[18];

  file[0] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[1] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[2] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[3] = TFile::Open("outputmZHmu/signal/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmuSignal.root");
  file[4] = TFile::Open("outputmZHmu/signal/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830_mZHmuSignal.root");
  file[5] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph_mZHmuSignal.root");
  file[6] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph_mZHmuSignal.root");
  file[7] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph_mZHmuSignal.root");
  file[8] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph_mZHmuSignal.root");
  file[9] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph_mZHmuSignal.root");
  file[10] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph_mZHmuSignal.root");
  file[11] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph_mZHmuSignal.root");
  file[12] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph_mZHmuSignal.root");
  file[13] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph_mZHmuSignal.root");
  file[14] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph_mZHmuSignal.root");
  file[15] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph_mZHmuSignal.root");
  file[16] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph_mZHmuSignal.root");
  file[17] = TFile::Open("outputmZHmu/signal/ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph_mZHmuSignal.root");

  // i=4 is ttbar                                                                                                                                                                                           

  TH1D*    h_eventWeight[5];
  Int_t    nEvent[5]  = {0};
  Double_t scale[5]   = {0};
  Double_t signalLumi = 1.0;  //pb-1
  Double_t crossSection[5] = {139.4,42.75,5.497,2.21,831.76};  //pb                                                                                                                                         

  for(Int_t i = 0; i < 5 ; i++){

    h_eventWeight[i] = (TH1D*)(file[i]->Get("eventWeight"));
    nEvent[i] = h_eventWeight[i]->Integral();
    scale[i] = signalLumi/(nEvent[i]/crossSection[i]);

  }

  TH1D* h_mZprime[18];

  for(Int_t i = 0; i < 18; i++)
    h_mZprime[i] = (TH1D*)(file[i]->Get("mZprime"));

  TH1D* h_mZprimeDY = (TH1D*)h_mZprime[0]->Clone("h_mZprimeDY");

  for(Int_t i = 0; i < 4 ; i++)
    h_mZprimeDY->Add(h_mZprime[i],scale[i]);

  h_mZprime[4]->Scale(scale[4]);

  TFile* outFile = new TFile("forRaman_mZHmuSignal.root","recreate");

  std::string name[18] = {"","","",
			  "mZprime_DYHT-100toInf",
			  "mZprime_TTbar",
			  "mZprime_signalM-600",
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
			  "mZprime_signalM-4500"};

  h_mZprimeDY->Write(name[3].data());

  for(Int_t i = 4; i < 18; i++)
    h_mZprime[i]->Write(name[i].data());

  outFile->Write();

  fstream ftext;
  ftext.open("forRaman_nEvent.txt", ios::out);

  ftext << name[3] << "  " << h_mZprimeDY->Integral() << "\n";

  for(Int_t i = 4; i < 18; i++)
    ftext << name[i] << "  " << h_mZprime[i]->Integral() << "\n";

}
