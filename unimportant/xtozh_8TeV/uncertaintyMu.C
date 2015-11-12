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

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*x + par[1];
  
}

void uncertaintyMu(){

  gStyle->SetOptStat(0);

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  TFile *f_root  = TFile::Open("sideBandMu_step2.root");

  vector<TH1D*>h_target;

  for(Int_t i = 0; i < 8; i++){

    h_target.push_back((TH1D*)(f_root->Get(Form("h%d",i))));

  }

  /*
    htarget:
    0: DY
    1: ttbar
    2: diBosons
    3: DY with corr
    4: all
    5: all, DY with corr
    6: all, ttbar with corr
    7: all, dibosons with corr
  */

  // Statistical Uncertainty

  Double_t binContent[8][nvarBins] = {0};
  Double_t binError[8][nvarBins] = {0};

  for(Int_t i = 0; i < 8; i++){
    for(Int_t j = 0; j < nvarBins; j++){

      binContent[i][j] = h_target[i]->GetBinContent(j+1);
      binError[i][j] = h_target[i]->GetBinError(j+1);

    }
  }

  Double_t centerValue[4][nvarBins] = {0};

  for(Int_t j = 0; j < nvarBins; j++){

    centerValue[0][j] = binContent[0][j] / binContent[4][j];
    centerValue[1][j] = binContent[3][j] / binContent[5][j];
    centerValue[2][j] = binContent[0][j] / binContent[6][j];
    centerValue[3][j] = binContent[0][j] / binContent[7][j];

  }

  Double_t stattemp[nvarBins] = {0};
  Double_t statUnc[nvarBins] = {0};
  Double_t diff[2];

  for(Int_t j = 0; j < nvarBins; j++){
    for(Int_t i = 0; i < 3; i++){

      diff[0] = (binContent[4][j]-binContent[0][j]) / TMath::Power(binContent[4][j],2);
      diff[1] = -binContent[0][j] / TMath::Power(binContent[4][j],2);

      if( i < 1 ) stattemp[j] += TMath::Power(binError[i][j],2)*TMath::Power(diff[0],2);
      else stattemp[j] += TMath::Power(binError[i][j],2)*TMath::Power(diff[1],2);

    }

    statUnc[j] = TMath::Sqrt(stattemp[j]);

  }


  // Systematic Uncertainty

  Double_t uncertainty[3][nvarBins];
  Double_t vy[nvarBins] = {0};

  for(Int_t i = 0; i < 3; i++){
    for(Int_t j = 0; j < nvarBins; j++){

      vy[j] = centerValue[0][j];
      uncertainty[i][j] = centerValue[i+1][j] - vy[j];

    }
  }

  Double_t systemp[nvarBins] = {0};
  Double_t vx[nvarBins] = {0};
  Double_t ex[nvarBins] = {0};
  Double_t ey[nvarBins] = {0};

  for(Int_t j = 0; j < nvarBins; j++){
    for(Int_t i = 0; i < 3; i++){

      systemp[j] += TMath::Power(uncertainty[i][j],2);

    }

    vx[j] = h_target[0]->GetBinCenter(j+1);
    ex[j] = 0.5*(varBins[j+1] - varBins[j]);
    ey[j] = TMath::Sqrt(systemp[j]);

  }

  // Drawing

  TLine* line = new TLine(680,1,2400,1);
  line->SetLineWidth(2);
  line->SetLineColor(kBlue);

  TGraphErrors *g_stat = new TGraphErrors(nvarBins, vx, vy, ex, statUnc);
  g_stat->SetLineColor(kBlack);

  TGraphErrors *g_syst = new TGraphErrors(nvarBins, vx, vy, ex, ey);
  g_syst->SetMarkerStyle(8);
  g_syst->SetMarkerSize(0);
  g_syst->SetMarkerColor(kBlue);
  g_syst->SetFillColor(kGreen);
  g_syst->SetFillStyle(3001);

  TCanvas* c = new TCanvas("c", "", 0, 0, 800, 600);

  TF1* f_ratioFit = new TF1("f_ratioFit", fitFunc, 680, 2400, 2);

  TH1D* h_ = new TH1D("", "", nvarBins, varBins);

  c->cd();

  h_->GetYaxis()->SetTitle("Ratio");
  h_->Fit("f_ratioFit", "", "", 680, 2400);
  h_->SetMinimum(0);
  h_->SetMaximum(2);
  h_->Draw("p");

  line->Draw("same");
  h_->Draw("psame");
  g_stat->Draw("psame");
  g_syst->Draw("3psame");
  
  c->Print("bkgRatioMu.jpg");
  
}

/*
  We need to do scalling when adding MC background samples. By doing this, 
  we first scale DY backgrounds with absolute uncertainty above, to get a 
  new value on each mass bin. By substract this new value with central value, 
  we get an uncertainty of DY backgrounds. Next, we scale the ttbar background 
  with absolute uncertainty, and do the same thing, to get another uncertainty 
  of this background. Since these two kins of backgrounds are not correlatedwith 
  each other, we do the square root sum on the two uncertainties, and apply this 
  new uncertainty to the original center value. This is the way how to do this 
  kind of systematic uncerainty. 
*/
