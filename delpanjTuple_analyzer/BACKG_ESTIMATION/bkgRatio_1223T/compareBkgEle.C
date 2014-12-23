#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TPad.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TLine.h>
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
const Int_t totalNEvent_ttbar = 10783509;
const Int_t totalNEvent_wwptia = 9959752;
const Int_t totalNEvent_wzptia = 9910267;
const Int_t totalNEvent_zzptia = 9769891;
const Double_t crossSection_dy70 = 63.5;
const Double_t crossSection_dy100 = 39.4;
const Double_t crossSection_ttbar = 25.8;
const Double_t crossSection_wwptia = 56.0;
const Double_t crossSection_wzptia = 22.4;
const Double_t crossSection_zzptia = 7.6;
const Double_t dataLumi_totalDEle = 19712.225;
// formula: scale = data_luminosity / bkg_luminosity
const Double_t scale_dy70 = dataLumi_totalDEle / (totalNEvent_dy70 / crossSection_dy70);
const Double_t scale_dy100 = dataLumi_totalDEle / (totalNEvent_dy100 / crossSection_dy100);
const Double_t scale_ttbar = dataLumi_totalDEle / (totalNEvent_ttbar / crossSection_ttbar);
const Double_t scale_wwptia = dataLumi_totalDEle / (totalNEvent_wwptia / crossSection_wwptia);
const Double_t scale_wzptia = dataLumi_totalDEle / (totalNEvent_wzptia / crossSection_wzptia);
const Double_t scale_zzptia = dataLumi_totalDEle / (totalNEvent_zzptia / crossSection_zzptia);

Double_t fitFunc(Double_t*, Double_t*);

void compareBkgEle(){

  TFile *f = TFile::Open("sideSigZpMEle.root");

  TH1D* h_dy70side = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_ttbarside = (TH1D*)(f->Get("sideZpMass_TTTo2L2Nu2B.root"));
  TH1D* h_wwptiaside = (TH1D*)(f->Get("sideZpMass_WW_pythia_filtered.root"));
  TH1D* h_wzptiaside = (TH1D*)(f->Get("sideZpMass_WZ_pythia_filtered.root"));
  TH1D* h_zzptiaside = (TH1D*)(f->Get("sideZpMass_ZZ_pythia_filtered.root"));

  TH1D* h_target[6] = {h_dy70side, h_dy100side, h_ttbarside, h_wwptiaside, h_wzptiaside, h_zzptiaside};
  
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

  TH1D* h_dybkg = new TH1D("h_dybkg", "Side band: DY Bkg", nvarBins, varBins);
  TH1D* h_allbkg = new TH1D("h_allbkg", "Side band: All Bkg", nvarBins, varBins);
  TH1D* h_bkgRatio = new TH1D("h_bkgRatio", "DY bkg over all bkg", nvarBins, varBins);
  TCanvas* c = new TCanvas("c", "", 0, 0, 1360, 800);

  Double_t scale[6] = {scale_dy70, scale_dy100, scale_ttbar, scale_wwptia, scale_wzptia, scale_zzptia};

  c->Divide(2,2);

  c->cd(1);

  h_dybkg->Sumw2();
  h_dybkg->SetMarkerStyle(8);
  h_dybkg->SetMarkerSize(0.7);
  h_dybkg->SetMarkerColor(1);
  h_dybkg->GetXaxis()->SetTitle("Mass");
  h_dybkg->GetYaxis()->SetTitle("Event Number");

  for(Int_t i = 0; i < 2; i++){
    h_dybkg->Add(h_target[i], scale[i]);
  }

  h_dybkg->SetMinimum(0);
  h_dybkg->Draw();

  c->cd(2);

  h_allbkg->Sumw2();
  h_allbkg->SetMarkerStyle(8);
  h_allbkg->SetMarkerSize(0.7);
  h_allbkg->SetMarkerColor(1);
  h_allbkg->GetXaxis()->SetTitle("Mass");
  h_allbkg->GetYaxis()->SetTitle("Event Number");

  for(Int_t i = 0; i < 6; i++){
    h_allbkg->Add(h_target[i], scale[i]);
  }

  h_allbkg->SetMinimum(0);
  h_allbkg->Draw();

  c->cd(3);

  gStyle->SetOptFit(1111);

  TF1* f_ratioFit = new TF1("f_ratioFit", fitFunc, 680, 2400, 2);

  h_bkgRatio->Sumw2();
  h_bkgRatio->SetMarkerColor(1);
  h_bkgRatio->SetMarkerStyle(8);
  h_bkgRatio->SetMarkerSize(0.8);
  h_bkgRatio->GetXaxis()->SetTitle("Mass");
  h_bkgRatio->GetYaxis()->SetTitle("Ratio");
  h_bkgRatio->Divide(h_dybkg, h_allbkg);
  h_bkgRatio->Fit("f_ratioFit", "", "", 680, 2400);
  h_bkgRatio->SetMinimum(0);
  h_bkgRatio->SetMaximum(2);
  h_bkgRatio->Draw();

  TLine* line = new TLine(680,1,2400,1);
  line->SetLineWidth(2);
  line->SetLineColor(kBlue);
  line->Draw("same");
  h_bkgRatio->Draw("same");
	       
  c->Print("bkgRatioEle.gif");
  c->Print("bkgRatioEle.pdf");

}

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*x + par[1];
  
}
