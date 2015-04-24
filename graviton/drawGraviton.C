#include <iostream>
#include <cstdlib>
#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TStyle.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include "untuplizer.h"

void drawGraviton(){

  gStyle->SetOptStat(0);
  
  TCanvas* c[2];
  TH1F* h_cosTh[2];
  TFile* f[2];

  f[0] = TFile::Open("BulkGraviton_WW_WlepWhad_M2000.root");
  f[1] = TFile::Open("RSGraviton_WW_WlepWhad_M2000.root");

  std::string title[2] = {"Bulk Graviton", "RS Graviton"};

  for(int i=0; i<2; i++){

    c[i] = new TCanvas(Form("c%d",i),"",0,0,1024,768);
    c[i]->cd();

    h_cosTh[i] = (TH1F*)(f[i]->Get("h_cosTh"));
    h_cosTh[i]->SetTitle(title[i].data());
    h_cosTh[i]->SetXTitle("cos#theta_{1} in the leptonic W rest frame");
    h_cosTh[i]->Draw();

  }

  c[0]->Print("BulkGravitonCosTh.pdf");
  c[1]->Print("RSGravitonCosTh.pdf");  

}
