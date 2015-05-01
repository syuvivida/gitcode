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
  
  TCanvas* c[4];
  TH1F* h_cosTh[2];
  TH1F* h_cosThStar[2];
  TFile* f[2];

  f[0] = TFile::Open("BulkGraviton_WW_WlepWhad_M2000.root");
  f[1] = TFile::Open("RSGraviton_WW_WlepWhad_M2000.root");

  std::string title[2] = {"Bulk Graviton", "RS Graviton"};

  int count = 0;

  for(int i=0; i<2; i++){

    c[count] = new TCanvas(Form("c%d",count),"",0,0,1024,768);
    c[count]->cd();

    h_cosTh[i] = (TH1F*)(f[i]->Get("h_cosTh"));
    h_cosTh[i]->SetTitle(title[i].data());
    h_cosTh[i]->SetXTitle("cos#theta_{1} in the leptonic W rest frame");
    h_cosTh[i]->Draw();

    count++;

    c[count] = new TCanvas(Form("c%d",count),"",0,0,1024,768);
    c[count]->cd();

    h_cosThStar[i] = (TH1F*)(f[i]->Get("h_cosThStar"));
    h_cosThStar[i]->SetTitle(title[i].data());
    h_cosThStar[i]->SetXTitle("cos#theta* in the RSG rest frame");
    h_cosThStar[i]->Draw();

    count++;

  }

  c[0]->Print("GravitonCosTh.pdf(");
  c[1]->Print("GravitonCosTh.pdf");
  c[2]->Print("GravitonCosTh.pdf");
  c[3]->Print("GravitonCosTh.pdf)"); 

}
