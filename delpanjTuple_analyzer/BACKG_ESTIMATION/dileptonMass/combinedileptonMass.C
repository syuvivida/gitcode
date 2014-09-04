#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TVectorT.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>

const Int_t totalNEvent1 = 11765438;
const Int_t totalNEvent2 = 12511326;
const Double_t crossSection1 = 63.5*1000;
const Double_t crossSection2 = 39.4*1000;
// scale = ? fb^-1 / luminosity
Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100

void myPlot(TH1D*, TH1D*, TH1D*);

void combinediLepMass(){

  TFile *muf = TFile::Open("dileptonMassMu.root");
  TFile *elf = TFile::Open("dileptonMassEl.root");

  TCanvas* c = new TCanvas("c", "", 0, 0, 1920, 1080);
  c->Divide(2,1);

  c->cd(1);
  myPlot( ((TH1D*)(muf->Get("ZMass_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(muf->Get("ZMass_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(muf->Get("ZMass_data_DoubleMu_A.root")))
	  );

  c->cd(2);
  myPlot( ((TH1D*)(elf->Get("ZMass_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(elf->Get("ZMass_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(elf->Get("ZMass_data_DoubleEl_A.root")))
	  );

  c->Print("dileptonMass.gif");


}


void myPlot(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_data){

  h_data->Sumw2();

  TH1D* h_combine = new TH1D("h_combine", "", 100, 70, 110);
  h_combine->Sumw2();
  h_combine->GetXaxis()->SetTitle("Mass");
  h_combine->GetYaxis()->SetTitle("Event Number");
  h_combine->Add(h_dy70, scale1);
  h_combine->Add(h_dy100, scale2);

  gStyle->SetOptStat(0);

  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->Draw();
  h_combine->SetLineColor(1);
  h_combine->SetFillColor(kPink+6);
  h_combine->Draw("histesame");
  h_data->Draw("same");
  
  TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.9);
  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_data, "Data", "lp"); 
  leg->AddEntry(h_combine, "DYJetsToLL", "f"); 
  leg->Draw();


}
