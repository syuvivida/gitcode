#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x + par[2]/x);
  
}

void step3(){

  gStyle->SetOptFit(0);

  TFile *f = TFile::Open("step2.root");

  TH1D* h_alphaRatio[3];
  TH1D* h_numbkgData[3];
  TH1D* h_combine[2][2]; // [bkg,data][side,signal]
  TF1* fitCurve[2][2]; // [bkg,data][side,signal]
  TF1* f_alphaFit[3];

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,1250,1400,1600,1800,2000,2400};
  const Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  Double_t alp_binContent[3][nvarBins] = {0};
  Double_t alp_vx[nvarBins] = {0};
  Double_t alp_vy[nvarBins] = {0};
  Double_t alp_ex[nvarBins] = {0};
  Double_t alp_eyl[nvarBins] = {0};
  Double_t alp_eyh[nvarBins] = {0};
  
  Double_t nbd_binContent[3][nvarBins] = {0};
  Double_t nbd_vx[nvarBins] = {0};
  Double_t nbd_vy[nvarBins] = {0};
  Double_t nbd_ex[nvarBins] = {0};
  Double_t nbd_eyl[nvarBins] = {0};
  Double_t nbd_eyh[nvarBins] = {0};
  
  for(Int_t mode = 0; mode < 3; mode++){
    
    h_alphaRatio[mode] = (TH1D*)(f->Get(Form("AlpRo_%d",mode-1)));
    h_numbkgData[mode] = (TH1D*)(f->Get(Form("NBkg_Dat_%d",mode-1)));

    for(Int_t i = 0; i < nvarBins; i++){
      
      alp_binContent[mode][i] = h_alphaRatio[mode]->GetBinContent(i+1);
      nbd_binContent[mode][i] = h_numbkgData[mode]->GetBinContent(i+1);
      
    }

  }

  for(Int_t i = 0; i < nvarBins; i++){

    alp_vx[i] = h_alphaRatio[1]->GetBinCenter(i+1);
    alp_vy[i] = alp_binContent[1][i];
    alp_ex[i] = 0.5*(varBins[i+1] - varBins[i]);
    alp_eyl[i] = alp_vy[i] - alp_binContent[0][i];
    alp_eyh[i] = alp_binContent[2][i] - alp_vy[i];
    std::cout <<  alp_eyl[i] << "   " << alp_eyh[i] << std::endl;

    nbd_vx[i] = h_numbkgData[1]->GetBinCenter(i+1);
    nbd_vy[i] = nbd_binContent[1][i];
    nbd_ex[i] = 0.5*(varBins[i+1] - varBins[i]);
    nbd_eyl[i] = nbd_vy[i] - nbd_binContent[0][i];
    nbd_eyh[i] = nbd_binContent[2][i] - nbd_vy[i];
    std::cout <<  nbd_eyl[i] << "   " << nbd_eyh[i] << std::endl;
	 
  }


  TCanvas* c = new TCanvas("c","",0,0,1300,600);
  c->Divide(2,2);

  c->cd(1);
  
  TGraphAsymmErrors *alp_syst = new TGraphAsymmErrors(nvarBins, alp_vx, alp_vy, alp_ex, alp_ex, alp_eyl, alp_eyh);
  alp_syst->SetMinimum(0);
  alp_syst->SetMaximum(1);
  alp_syst->SetFillColor(kCyan);
  alp_syst->Draw("ae4");

  c->cd(2);
    
  TGraphAsymmErrors *nbd_syst = new TGraphAsymmErrors(nvarBins, nbd_vx, nbd_vy, nbd_ex, nbd_ex, nbd_eyl, nbd_eyh);
  nbd_syst->SetMinimum(0);
  nbd_syst->SetFillColor(kCyan);
  nbd_syst->Draw("ae4");



  
  // std::string htitle[2][2] = {{"Bkg: Side-band Zp Mass","Bkg: Signal-band Zp Mass"},
  //		      {"Data: Side-band Zp Mass","Data: Signal-band Zp Mass"}};
  

  /*
    h_combine[0][0] = (TH1D*)(f->Get("h_combine00_-1"));
    h_combine[0][1] = (TH1D*)(f->Get("h_combine00_0"));

    h_combine[0][1]->SetLineColor(kRed);
    h_combine[0][0]->Draw(); 
    h_combine[0][1]->Draw("same");

  */
    /*
    
    for(Int_t i = 0; i < 2; i++){
      for(Int_t j = 0; j < 2; j++){

	fitCurve[i][j] = new TF1(Form("fitCurve%d%d",i,j), fitFunc, 680, 2400, 3);
	fitCurve[i][j]->SetParameters(10,-0.005,400);
	h_combine[i][j] = (TH1D*)(f->Get(Form("h_combine%d%d_%d",i,j,mode-1)));
	h_combine[i][j]->Fit(Form("fitCurve%d%d",i,j), "", "", 680, 2400);


      }
    }

    f_alphaFit[mode] = new TF1(Form("f_alphaFit%d",mode),"([0]*TMath::Exp([1]*x+[2]/x))/([3]*TMath::Exp([4]*x+[5]/x))",680,2400);
    f_alphaFit[mode]->SetParameter(0, fitCurve[0][1]->GetParameter(0));
    f_alphaFit[mode]->SetParameter(1, fitCurve[0][1]->GetParameter(1));
    f_alphaFit[mode]->SetParameter(2, fitCurve[0][1]->GetParameter(2));
    f_alphaFit[mode]->SetParameter(3, fitCurve[0][0]->GetParameter(0));
    f_alphaFit[mode]->SetParameter(4, fitCurve[0][0]->GetParameter(1));
    f_alphaFit[mode]->SetParameter(5, fitCurve[0][0]->GetParameter(2));
    f_alphaFit[mode]->SetMinimum(0);
    f_alphaFit[mode]->SetMaximum(1);
    */
    /*
    if( mode < 1 ){
      //c->cd(1); f_alphaFit[mode]->Draw();
      c->cd(2); h_alphaRatio[mode]->Draw();
      c->cd(3); h_numbkgData[mode]->Draw();
    }
    
    else{
      //c->cd(1); f_alphaFit[mode]->Draw("same");
      c->cd(2); h_alphaRatio[mode]->Draw("same");
      c->cd(3); h_numbkgData[mode]->Draw("same");
    }   
    */
}
