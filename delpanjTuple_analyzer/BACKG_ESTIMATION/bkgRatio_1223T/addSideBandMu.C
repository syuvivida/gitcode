#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TSystemDirectory.h>

void addSideBandMu(){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  const Double_t dataLumi_totalDMu = 19671.225;

  FILE  *f_table = fopen("inputFile.txt", "r");
  TFile *f_root  = TFile::Open("sideSigZpMMu.root");
  vector<TH1D*> h_target;
  vector<Double_t> scale;
  vector<Double_t> absUnc;
  vector<Double_t> adjScale;

  Int_t flag = 1;
  Char_t tmp[100];  

  while( flag != -1 ){

    flag = fscanf(f_table, "%s", tmp); std::string fileName = tmp;
    flag = fscanf(f_table, "%s", tmp); Int_t eventNum = atof(tmp);
    flag = fscanf(f_table, "%s", tmp); Double_t myCrossX = atof(tmp);
    flag = fscanf(f_table, "%s", tmp); Double_t incCrossX = atof(tmp);
    flag = fscanf(f_table, "%s", tmp); Double_t error = atof(tmp);

    if( flag != -1 ){

      h_target.push_back((TH1D*)(f_root->Get(fileName.data())));
      scale.push_back(dataLumi_totalDMu/(eventNum/myCrossX)); // scale = data_luminosity / bkg_luminosity (totEvent/crossX)
      absUnc.push_back((1/incCrossX)*error); // for scale itself, myCrossX is replaced by 1

    }
  }
  
  for(unsigned int i = 0; i < scale.size(); i++){

    adjScale.push_back(scale[i] + (scale[i]*absUnc[i]));

  }

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  // fixing the discontinuous of bin content
  Int_t smallestBin = varBins[1] - varBins[0];
  vector<Int_t> binRatio;
  
  for(Int_t i = 0; i < 2; i++){
    for(Int_t nb = 0; nb < nvarBins; nb++){

      binRatio.push_back((varBins[nb+1]-varBins[nb])/smallestBin);
      h_target[i]->SetBinContent(nb+1, (h_target[i]->GetBinContent(nb+1)/binRatio[nb]));

    }
  }

  std::string hTitle[] = { "DY Bkg",
			   "t#bar{t} Bkg",
			   "di-bosons Bkg",
			   "DY Bkg with correction",
			   "t#bar{t} Bkg with correction",
			   "di-bosons Bkg with correction",
			   "All Bkg",
			   "All Bkg with DY Bkg correction",
			   "All Bkg with t#bar{t} Bkg correction",
			   "All Bkg with di-bosons Bkg correction" };
  
  TH1D* h_output[10];
  unsigned int id = 0;

  while( id < sizeof(h_output)/sizeof(h_output[0]) ){
    
    h_output[id] = new TH1D(Form("h_output%d",id), hTitle[id].data(), nvarBins, varBins);
    id++;

  }
  
  // adding backgrounds with scales
  h_output[0]->Add(h_target[0],scale[0]); // DY
  h_output[0]->Add(h_target[1],scale[1]);
  h_output[1]->Add(h_target[2],scale[2]); // ttbar
  h_output[2]->Add(h_target[3],scale[3]); // dibosons
  h_output[2]->Add(h_target[4],scale[4]);
  h_output[2]->Add(h_target[5],scale[5]);
  h_output[3]->Add(h_target[0],adjScale[0]); // DY mod
  h_output[3]->Add(h_target[1],adjScale[1]);
  h_output[4]->Add(h_target[2],adjScale[2]); // ttbar mod
  h_output[5]->Add(h_target[3],adjScale[3]); // dibosons mod
  h_output[5]->Add(h_target[4],adjScale[4]);
  h_output[5]->Add(h_target[5],adjScale[5]);
  h_output[6]->Add(h_output[0]); // all
  h_output[6]->Add(h_output[1]);
  h_output[6]->Add(h_output[2]);
  h_output[7]->Add(h_output[3]); // all*Dy
  h_output[7]->Add(h_output[1]);
  h_output[7]->Add(h_output[2]);
  h_output[8]->Add(h_output[4]); // all*Ttbar
  h_output[8]->Add(h_output[0]);
  h_output[8]->Add(h_output[2]);
  h_output[9]->Add(h_output[5]); // all*Dibosons
  h_output[9]->Add(h_output[0]);
  h_output[9]->Add(h_output[1]);

  // output file
  TFile* outFile = new TFile("sideBandMu_step2.root", "recreate");

  h_output[0]->Write("h0");
  h_output[1]->Write("h1");
  h_output[2]->Write("h2");
  h_output[3]->Write("h3");
  h_output[6]->Write("h4");
  h_output[7]->Write("h5");
  h_output[8]->Write("h6");
  h_output[9]->Write("h7");

  outFile->Write();

}
