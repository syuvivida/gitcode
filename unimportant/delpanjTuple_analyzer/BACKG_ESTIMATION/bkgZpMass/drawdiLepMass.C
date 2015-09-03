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

const Int_t totalNEvent_dy70 = 11764538;
const Int_t totalNEvent_dy100 = 12511326;
const Double_t crossSection_dy70 = 63.5;
const Double_t crossSection_dy100 = 39.4;
const Double_t dataLumi_totalDMu = 19671.225;
const Double_t dataLumi_totalDEle = 19712.225;
// formula: scale = data_luminosity / bkg_luminosity
const Double_t scale_mudy70 = dataLumi_totalDMu / (totalNEvent_dy70 / crossSection_dy70);
const Double_t scale_mudy100 = dataLumi_totalDMu / (totalNEvent_dy100 / crossSection_dy100);
const Double_t scale_eldy70 = dataLumi_totalDEle / (totalNEvent_dy70 / crossSection_dy70);
const Double_t scale_eldy100 = dataLumi_totalDEle / (totalNEvent_dy100 / crossSection_dy100);

void drawdiLepMass(){

  TFile *f_mu = TFile::Open("dileptonMassMu.root");
  TFile *f_el = TFile::Open("dileptonMassEl.root");

  TH1D* h_mudy70  = (TH1D*)(f_mu->Get("ZMass_DYJetsToLL_PtZ-70To100.root")); 
  TH1D* h_mudy100 = (TH1D*)(f_mu->Get("ZMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_mudata  = (TH1D*)(f_mu->Get("ZMass_data_DoubleMu.root"));
  TH1D* h_eldy70  = (TH1D*)(f_el->Get("ZMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_eldy100 = (TH1D*)(f_el->Get("ZMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_eldata  = (TH1D*)(f_el->Get("ZMass_data_DoubleEl.root"));

  TH1D* h_target[2][3] = {{h_mudy70,h_mudy100,h_mudata},
			  {h_eldy70,h_eldy100,h_eldata}};

  TH1D* h_combine[2];
  TCanvas* c = new TCanvas("c", "", 0, 0, 1300, 800);
  c->Divide(2,2);

  Double_t scale[2][2] = {{scale_mudy70,scale_mudy100},
			  {scale_eldy70,scale_eldy100}};

  gStyle->SetOptStat(0);

  for(Int_t i = 0; i < 2; i++){

    c->cd(i+1);

    TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.9);
    h_combine[i] = new TH1D(Form("h_combine%d",i), "", 100, 70, 110);

    for(Int_t j = 0; j < 3; j++){

      if( j < 2 ){

	h_combine[i]->Add(h_target[i][j], scale[i][j]);
	h_combine[i]->SetLineColor(1);
	h_combine[i]->SetFillColor(kCyan);

      } // if bkg
      
      if( j > 1 ){

	h_combine[i]->GetXaxis()->SetTitle("Mass");
	h_combine[i]->GetYaxis()->SetTitle("Event number");
	h_target[i][j]->Sumw2();
	h_target[i][j]->SetLineColor(1);
	h_target[i][j]->SetMarkerStyle(8);
	h_target[i][j]->SetMarkerSize(0.5);
	h_target[i][j]->Draw();
	h_combine[i]->Draw("same");

      } // if data

    } // for j

    leg->SetFillStyle(1001);
    leg->SetFillColor(10);
    leg->SetBorderSize(1);
    leg->AddEntry(h_combine[i], "DYJetsToLL", "f");
    leg->AddEntry(h_target[i][2], "Data", "lp");
    leg->Draw();

  }

  c->Print("dileptonMass.gif");

}
