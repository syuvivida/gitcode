#include <iostream>
#include <vector>
#include <TF1.h>
#include <TH1.h>
#include <TMatrixD.h>
#include <TMinuit.h>
#include <TStyle.h>
#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TFitResult.h>

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x)*0.5*(1+TMath::Erf((x-par[2])/par[3]));
  
}

void drawPrunedJM_noDiboson(){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);

  TFile *f = TFile::Open("prunedJetMMu_noDiboson.root");

  TH1D* h_MCWW = (TH1D*)(f->Get("prunedJetMMu_WW_pythia_filtered"));
  TH1D* h_MCWZ = (TH1D*)(f->Get("prunedJetMMu_WZ_pythia_filtered"));
  TH1D* h_MCZZ = (TH1D*)(f->Get("prunedJetMMu_ZZ_pythia_filtered"));
  TH1D* h_dataMu = (TH1D*)(f->Get("prunedJetMMu_data_DoubleMu"));

  Double_t lumi_MCWW = 9959752/56.0;
  Double_t lumi_MCWZ = 9910267/22.4;
  Double_t lumi_MCZZ = 9769891/7.6;
  Double_t lumi_dataMu = 19671.225;
    
  h_MCWW->Scale(lumi_dataMu/lumi_MCWW);
  h_MCWZ->Scale(lumi_dataMu/lumi_MCWZ);
  h_MCZZ->Scale(lumi_dataMu/lumi_MCZZ);

  TH1D* h_allDibosonMC = new TH1D("h_allDibosonMC", "", 100, 40, 240);
  h_allDibosonMC->Add(h_MCWW);
  h_allDibosonMC->Add(h_MCWZ); 
  h_allDibosonMC->Add(h_MCZZ);

  TH1D* h_dataMu_noDiboson = new TH1D("h_dataMu_noDiboson", "", 100, 40, 240);
  h_dataMu_noDiboson->SetTitle("Pruned jet mass distribution (data)");
  h_dataMu_noDiboson->GetXaxis()->SetTitle("Mass");
  h_dataMu_noDiboson->GetYaxis()->SetTitle("Event numbers");  
  
  Int_t NBIN = h_allDibosonMC->GetNbinsX();
  
  for(Int_t i = 1; i < NBIN; i++){
    
    Int_t binCenter = h_dataMu_noDiboson->GetBinCenter(i);
    Int_t binContent = h_dataMu->GetBinContent(i) - h_allDibosonMC->GetBinContent(i);

    if( binCenter > 110 && binCenter < 140 ) continue;
    h_dataMu_noDiboson->SetBinContent(i, binContent);

  }

  TF1* fitCurve = new TF1("fitCurve", fitFunc, 40, 240, 4);
  h_dataMu_noDiboson->Fit(fitCurve, "", "", 40, 240);

  Double_t nBkgSig = fitCurve->Integral(110,140)/h_dataMu->GetBinWidth(1);
  
  vector<Double_t> par;
  vector<Double_t> posPar;
  vector<Double_t> negPar;
  
  for(Int_t i = 0; i < 4; i++){

    par.push_back(fitCurve->GetParameters()[i]);
    posPar.push_back(par[i] + fitCurve->GetParErrors()[i]);
    negPar.push_back(par[i] - fitCurve->GetParErrors()[i]);
    
  }
  
  Double_t x;
  TF1* posFit[4];

  posFit[0] = new TF1("posFit1",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",posPar[0],par[1],par[2],par[3]),
		      40, 240);
  posFit[1] = new TF1("posFit2",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],posPar[1],par[2],par[3]),
		      40, 240);
  posFit[2] = new TF1("posFit3",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],par[1],posPar[2],par[3]),
		      40, 240);
  posFit[3] = new TF1("posFit4",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],par[1],par[2],posPar[3]),
		      40, 240);

  TF1* negFit[4]; 
  
  negFit[0] = new TF1("negFit1",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",negPar[0],par[1],par[2],par[3]),
		      40, 240);
  negFit[1] = new TF1("negFit2",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],negPar[1],par[2],par[3]),
		      40, 240);
  negFit[2] = new TF1("negFit3",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],par[1],negPar[2],par[3]),
		      40, 240);
  negFit[3] = new TF1("negFit4",
		      Form("%f*TMath::Exp(%f*x)*0.5*(1+TMath::Erf((x-(%f))/%f))",par[0],par[1],par[2],negPar[3]),
		      40, 240);
  
  TMatrixD posColM(4,1);
  TMatrixD negColM(4,1);
  TMatrixD posRowM(1,4);
  TMatrixD negRowM(1,4);

  for(Int_t i = 0; i < 4; i++){
    
    posColM(i,0) = fabs(nBkgSig - posFit[i]->Integral(110,140)/h_dataMu->GetBinWidth(1));
    negColM(i,0) = fabs(nBkgSig - negFit[i]->Integral(110,140)/h_dataMu->GetBinWidth(1));
    posRowM(0,i) = posColM(i,0);
    negRowM(0,i) = negColM(i,0);
    
  }

  gMinuit->mnmatu(1);
  
  TFitResultPtr fitptr = h_dataMu_noDiboson->Fit(fitCurve, "S");
  TFitResult fitresult = (*fitptr);
  TMatrixD corrM = fitresult.GetCorrelationMatrix();
  TMatrixD posTemp = posRowM*(corrM*posColM);
  TMatrixD negTemp = negRowM*(corrM*negColM);
  Double_t posSystUnc = TMath::Sqrt(posTemp(0,0));
  Double_t negSystUnc = TMath::Sqrt(negTemp(0,0));
  
  cout << "\nNumber of background events in signal region: "
       << nBkgSig << " +" << posSystUnc << " -" << negSystUnc << "\n" << endl;
  
  TCanvas* c = new TCanvas("c","",0,0,800,800);
  c->cd();
  h_dataMu_noDiboson->SetFillColor(kCyan);
  h_dataMu_noDiboson->Draw();
  c->Print("dataMu_noDiboson.pdf");
  
}
