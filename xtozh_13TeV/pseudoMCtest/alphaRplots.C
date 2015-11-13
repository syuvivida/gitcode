#include <string>
#include <vector>
#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TPad.h>
#include <TMath.h>
#include <TFile.h>
#include <TLine.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TMinuit.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMatrixD.h>
#include <TFitResult.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>
#include "../setNCUStyle.h"

const Double_t xmin = 500;
const Double_t xmax = 5000;
const Int_t nBins = (xmax-xmin)/100;

Double_t dataLumi  = 3000; //831.7; //pb-1
Double_t xSecDY100 = 139.4*1.23;
Double_t xSecDY200 = 42.75*1.23;
Double_t xSecDY400 = 5.497*1.23;
Double_t xSecDY600 = 2.21*1.23;
Double_t xSecTTbar = 831.76;
Double_t xSecWW    = 118.7;
Double_t xSecWZ    = 47.13;
Double_t xSecZZ    = 16.523;

TFile* getFile(std::string infiles, std::string hname, 
	       Double_t crossSection, Double_t* scale){

  TFile* f = TFile::Open(infiles.data());
  TH1D*  h = NULL;

  if( hname.find("pMC") != std::string::npos ) 
    h = (TH1D*)(f->Get("eventWeight_pMC"));
  else if( hname.find("pDA") != std::string::npos )
    h = (TH1D*)(f->Get("eventWeight_pDA"));

  *scale = dataLumi/(h->Integral()/crossSection);

  return f;

}

TH1D* addSamples(std::vector<string>& infiles, std::string hname,
		 TFile* f_DY100, TFile* f_DY200, TFile* f_DY400, TFile* f_DY600, 
		 TFile* f_TTbar, TFile* f_WW, TFile* f_WZ, TFile* f_ZZ){

  Double_t scaleDY100 = 0;
  Double_t scaleDY200 = 0;
  Double_t scaleDY400 = 0;
  Double_t scaleDY600 = 0;
  Double_t scaleTTbar = 0;
  Double_t scaleWW    = 0;
  Double_t scaleWZ    = 0;
  Double_t scaleZZ    = 0;

  for(unsigned int i = 0; i < infiles.size(); i++){

    if( infiles[i].find("HT-100") != std::string::npos )
      f_DY100 = getFile(infiles[i].data(), hname.data(), xSecDY100, &scaleDY100);

    if( infiles[i].find("HT-200") != std::string::npos )
      f_DY200 = getFile(infiles[i].data(), hname.data(), xSecDY200, &scaleDY200);

    if( infiles[i].find("HT-400") != std::string::npos )
      f_DY400 = getFile(infiles[i].data(), hname.data(), xSecDY400, &scaleDY400);

    if( infiles[i].find("HT-600") != std::string::npos )
      f_DY600 = getFile(infiles[i].data(), hname.data(), xSecDY600, &scaleDY600);

    if( infiles[i].find("TT_") != std::string::npos )
      f_TTbar = getFile(infiles[i].data(), hname.data(), xSecTTbar, &scaleTTbar);

    if( infiles[i].find("WW_") != std::string::npos )
      f_WW = getFile(infiles[i].data(), hname.data(), xSecWW, &scaleWW);

    if( infiles[i].find("WZ_") != std::string::npos )
      f_WZ = getFile(infiles[i].data(), hname.data(), xSecWZ, &scaleWZ);

    if( infiles[i].find("ZZ_") != std::string::npos )
      f_ZZ = getFile(infiles[i].data(), hname.data(), xSecZZ, &scaleZZ);

  }

  TH1D* DY100_temp = (TH1D*)(f_DY100->Get(Form("%s",hname.c_str())));
  TH1D* DY200_temp = (TH1D*)(f_DY200->Get(Form("%s",hname.c_str())));
  TH1D* DY400_temp = (TH1D*)(f_DY400->Get(Form("%s",hname.c_str())));
  TH1D* DY600_temp = (TH1D*)(f_DY600->Get(Form("%s",hname.c_str())));
  TH1D* TTbar_temp = (TH1D*)(f_TTbar->Get(Form("%s",hname.c_str())));
  TH1D* WW_temp    = (TH1D*)(f_WW->Get(Form("%s",hname.c_str())));
  TH1D* WZ_temp    = (TH1D*)(f_WZ->Get(Form("%s",hname.c_str())));
  TH1D* ZZ_temp    = (TH1D*)(f_ZZ->Get(Form("%s",hname.c_str())));

  TH1D* h_Total = (TH1D*)(f_DY100->Get(Form("%s",hname.c_str())))->Clone("h_Total");

  h_Total->Reset();
  h_Total->Add(DY100_temp,scaleDY100);
  h_Total->Add(DY200_temp,scaleDY200);
  h_Total->Add(DY400_temp,scaleDY400);
  h_Total->Add(DY600_temp,scaleDY600);
  h_Total->Add(TTbar_temp,scaleTTbar);
  h_Total->Add(WW_temp,scaleWW);
  h_Total->Add(WZ_temp,scaleWZ);
  h_Total->Add(ZZ_temp,scaleZZ);

  return h_Total;

}

void myRatio(TH1D* h_numer, TH1D* h_denom){

  TH1D* h_ratio = (TH1D*)h_numer->Clone("h_ratio");

  h_ratio->Reset();

  Int_t nbin = h_ratio->GetNbinsX();
  Double_t ratio[nbin];
  Double_t error[nbin];
  Double_t numer_nbincontent[nbin];
  Double_t denom_nbincontent[nbin];
  Double_t numer_binerror[nbin];
  Double_t denom_binerror[nbin];

  for( Int_t i = 1; i <= nbin; i++ ){

    numer_nbincontent[i] = h_numer->GetBinContent(i);
    denom_nbincontent[i] = h_denom->GetBinContent(i);
    numer_binerror[i] = h_numer->GetBinError(i);
    denom_binerror[i] = h_denom->GetBinError(i);

    if( denom_nbincontent[i] <= 0 || numer_nbincontent[i] <= 0 ) continue;
    if( denom_binerror[i] <= 0 || numer_binerror[i] <= 0 ) continue;

    ratio[i] = (Double_t)numer_nbincontent[i]/denom_nbincontent[i];
    error[i] = (ratio[i])*sqrt(pow(numer_binerror[i]/numer_nbincontent[i],2)+pow(denom_binerror[i]/denom_nbincontent[i],2));

    h_ratio->SetBinContent(i,ratio[i]);
    h_ratio->SetBinError(i,error[i]);

  }

  h_ratio->SetLineColor(kBlack);
  h_ratio->SetTitle("");
  h_ratio->GetYaxis()->SetTitle("Ratio");
  h_ratio->GetYaxis()->SetTitleOffset(0.3);
  h_ratio->GetXaxis()->SetLabelSize(0.1);
  h_ratio->GetXaxis()->SetTitleSize(0.125);
  h_ratio->GetYaxis()->SetLabelSize(0.1);
  h_ratio->GetYaxis()->SetTitleSize(0.1);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->GetYaxis()->SetRangeUser(0,2);
  h_ratio->Draw();

  Double_t x0 = h_denom->GetXaxis()->GetXmin();
  Double_t x1 = h_denom->GetXaxis()->GetXmax();
  Double_t y0 = 1.;
  Double_t y1 = 1.;

  TLine* one = new TLine(x0,y0,x1,y1);

  one->SetLineColor(2);
  one->SetLineStyle(1);
  one->SetLineWidth(2);
  one->Draw("same");

  h_ratio->Draw("same");

}

Double_t fitZpmass(Double_t* v, Double_t* p){

  Double_t x = v[0];
  return p[0]*TMath::Exp(p[1]*x + p[2]/x);

}

Double_t divFunc(Double_t* v, Double_t* p){

  Double_t x = v[0];
  return (p[0]*TMath::Exp(p[1]*x+p[2]/x))/(p[3]*TMath::Exp(p[4]*x+p[5]/x));

}

Double_t fitPRmass(Double_t* v, Double_t* p){

  Double_t x = v[0];
  return p[0]*TMath::Exp(p[1]*x)*0.5*(1+TMath::Erf((x-p[2])/p[3]));

}

TGraphAsymmErrors* fitUncertainty(TF1* f, TMatrixD corrMatrix){

  Double_t par[4] = {0};

  for( Int_t i = 0; i < 4; i++ )
    par[i] = f->GetParameter(i);

  TF1* posFit[4];
  TF1* negFit[4];

  for( Int_t i = 0; i < 4; i++ ){

    Double_t partemp[4] = {par[0],par[1],par[2],par[3]};

    posFit[i]  = new TF1(Form("posFit%d",i), fitPRmass, 40, 240, 4);
    partemp[i] = par[i] + f->GetParError(i);
    posFit[i]->SetParameters(partemp[0],partemp[1],partemp[2],partemp[3]);

  }

  for( Int_t i = 0; i < 4; i++ ){

    Double_t partemp[4] = {par[0],par[1],par[2],par[3]};

    negFit[i]  = new TF1(Form("negFit%d",i), fitPRmass, 40, 240, 4);
    partemp[i] = par[i] - f->GetParError(i);
    negFit[i]->SetParameters(partemp[0],partemp[1],partemp[2],partemp[3]);

  }

  TMatrixD posColM(4,1);
  TMatrixD negColM(4,1);
  TMatrixD posRowM(1,4);
  TMatrixD negRowM(1,4);

  Int_t    NBINS = 40;
  Double_t x     = 40.0;
  Double_t width = (240-x)/NBINS;

  Double_t funcX[NBINS];
  Double_t funcY[NBINS];
  Double_t posUnc[NBINS];
  Double_t negUnc[NBINS];

  for( Int_t n = 0; n < NBINS; n++){

    for(Int_t i = 0; i < 4; i++){
    
      posColM(i,0) = fabs(f->Eval(x) - posFit[i]->Eval(x));
      negColM(i,0) = fabs(f->Eval(x) - negFit[i]->Eval(x));
      posRowM(0,i) = posColM(i,0);
      negRowM(0,i) = negColM(i,0);
    
    }

    gMinuit->mnmatu(1);
  
    TMatrixD posTemp = posRowM*(corrMatrix*posColM);
    TMatrixD negTemp = negRowM*(corrMatrix*negColM);
    
    posUnc[n] = TMath::Sqrt(posTemp(0,0));
    negUnc[n] = TMath::Sqrt(negTemp(0,0));

    funcX[n] = x;
    funcY[n] = f->Eval(x);

    x += width;

  }

  TGraphAsymmErrors* g = new TGraphAsymmErrors(NBINS, funcX, funcY, 0, 0, negUnc, posUnc);

  return g;

}

void alphaRplots(std::string outputFolder){

  setNCUStyle();
  gStyle->SetOptFit(0);
  gStyle->SetMarkerSize(0);
  gStyle->SetTitleSize(0.04,"XYZ");
  gStyle->SetLabelSize(0.03,"XYZ");
  gStyle->SetHistLineWidth(2);

  std::vector<string> infiles;
 
  TSystemDirectory *base = new TSystemDirectory("root","root");
  base->SetDirectory(outputFolder.data());
  TList *listOfFiles = base->GetListOfFiles();
  TIter fileIt(listOfFiles);
  TFile *fileH = new TFile();
  Long64_t nfiles = 0;

  while( (fileH = (TFile*)fileIt()) ){
    
    std::string fileN = fileH->GetName();
    std::string baseString = "root";
    if( fileN.find(baseString) == std::string::npos ) continue;
    infiles.push_back(Form("%s/%s",outputFolder.data(),fileN.data()));
    nfiles++;
    
  }

  TFile *f_DY100 = NULL;
  TFile *f_DY200 = NULL;
  TFile *f_DY400 = NULL;
  TFile *f_DY600 = NULL;
  TFile *f_TTbar = NULL;
  TFile *f_WW    = NULL;
  TFile *f_WZ    = NULL;
  TFile *f_ZZ    = NULL;

  // Declare prefer histogram and add them together

  TH1D* h_sideTotalBKG  = addSamples(infiles,"ZprimeSide_pMC",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_signTotalBKG  = addSamples(infiles,"ZprimeSign_pMC",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_sideDATA      = addSamples(infiles,"ZprimeSide_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_signDATA      = addSamples(infiles,"ZprimeSign_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_corrPRmass    = addSamples(infiles,"corrPRmass_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_PRmassAll     = addSamples(infiles,"PRmassAll_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_corrPRmassAll = addSamples(infiles,"corrPRmassAll_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);

  h_sideTotalBKG->SetLineWidth(2);
  h_sideTotalBKG->SetLineColor(kBlack);
  h_sideTotalBKG->SetXTitle("Side band ZH mass in pseudo-MC");
  h_sideTotalBKG->SetYTitle("Event numbers");

  h_signTotalBKG->SetLineWidth(2);
  h_signTotalBKG->SetLineColor(kBlack);
  h_signTotalBKG->SetXTitle("Signal region ZH mass in pseudo-MC");
  h_signTotalBKG->SetYTitle("Event numbers");

  h_signDATA->SetLineWidth(2);
  h_signDATA->SetLineColor(kBlue);
  h_signDATA->SetXTitle("ZH mass");
  h_signDATA->SetYTitle("Event numbers");

  h_PRmassAll->SetLineWidth(2);
  h_PRmassAll->SetLineColor(kBlack);
  h_PRmassAll->SetXTitle("Uncorrected pruned mass in pseudo-data");
  h_PRmassAll->SetYTitle("Event numbers");

  h_corrPRmassAll->SetLineWidth(2);
  h_corrPRmassAll->SetLineColor(kBlack);
  h_corrPRmassAll->SetXTitle("Corrected pruned mass in pseudo-data");
  h_corrPRmassAll->SetYTitle("Event numbers");

  // Make the statistics error more like data

  TH1D* h_corrPRmassFixErr = (TH1D*)h_corrPRmass->Clone("h_corrPRmassFixErr");

  h_corrPRmassFixErr->Reset();
  h_corrPRmassFixErr->SetLineWidth(2);
  h_corrPRmassFixErr->SetLineColor(kBlack);
  h_corrPRmassFixErr->SetXTitle("Side band corrected pruned mass (error fix) in pseudo-data");
  h_corrPRmassFixErr->SetYTitle("Event numbers");

  for( Int_t i = 1; i <= h_corrPRmass->GetNbinsX(); i++ ){

    Double_t evperBin = h_corrPRmass->GetBinContent(i);

    h_corrPRmassFixErr->SetBinContent(i,evperBin);
    h_corrPRmassFixErr->SetBinError(i,TMath::Sqrt(evperBin));

  }

  // Calculate alpha ratio

  TH1D* h_alphaRatio = new TH1D("h_alphaRatio", "", nBins, xmin, xmax); 

  h_alphaRatio->Sumw2();
  h_alphaRatio->SetXTitle("ZH mass");
  h_alphaRatio->SetYTitle("Alpha ratio");
  h_alphaRatio->Divide(h_signTotalBKG,h_sideTotalBKG);
  h_alphaRatio->SetMinimum(0);

  // Calculate number of backgrounds in signal region

  TH1D* h_numbkgDATA = (TH1D*)h_alphaRatio->Clone("h_numbkgDATA");

  h_numbkgDATA->Reset();

  for( Int_t i = 1; i <= nBins; i++ ){

    Double_t alphaRatio      = h_alphaRatio->GetBinContent(i); 
    Double_t sideDATA        = h_sideDATA->GetBinContent(i);
    Double_t numbkgDATA      = alphaRatio*sideDATA;      
    Double_t alphaRatioError = h_alphaRatio->GetBinError(i);
    Double_t sideDATAError   = h_sideDATA->GetBinError(i);

    if( alphaRatio == 0 || sideDATA == 0 ) continue;

    Double_t numbkgDATAError = numbkgDATA*sqrt(pow((alphaRatioError/alphaRatio),2)+pow((sideDATAError/sideDATA),2));

    h_numbkgDATA->SetBinContent(i,numbkgDATA);
    h_numbkgDATA->SetBinError(i,numbkgDATAError);

  }

  // Fitting procedure

  TF1* f_fitPRmass = new TF1("f_fitPRmass", fitPRmass, 40, 240, 4);
  TF1* f_fitZpmass = new TF1("f_fitZpmass", fitZpmass, xmin, xmax, 3);
  TF1* f_fitAlphaR = new TF1("f_fitAlphaR", divFunc, xmin, xmax, 6);

  f_fitPRmass->SetLineWidth(2);
  f_fitZpmass->SetLineWidth(2);
  f_fitAlphaR->SetLineWidth(2);

  Double_t parFitPRm[4] = {1224,-0.107,139.6,107.4};//{342,-0.03,39,30};

  f_fitPRmass->SetParameters(parFitPRm[0],parFitPRm[1],parFitPRm[2],parFitPRm[3]);
  h_PRmassAll->Fit("f_fitPRmass", "", "", 40, 240);

  f_fitPRmass->SetParameters(parFitPRm[0],parFitPRm[1],parFitPRm[2],parFitPRm[3]);
  h_corrPRmassFixErr->Fit("f_fitPRmass", "", "", 40, 240);

  TFitResultPtr fitptr = h_corrPRmassFixErr->Fit(f_fitPRmass, "S");
  TFitResult fitresult = (*fitptr);
  TMatrixD corrMatrix  = fitresult.GetCorrelationMatrix();

  Double_t nBkgSig = f_fitPRmass->Integral(105,135)/h_corrPRmassFixErr->GetBinWidth(1);

  TGraphAsymmErrors* g_errorBands = fitUncertainty(f_fitPRmass, corrMatrix);

  //g_errorBands->SetFillStyle(3005);

  f_fitPRmass->SetParameters(parFitPRm[0],parFitPRm[1],parFitPRm[2],parFitPRm[3]);
  h_corrPRmassAll->Fit("f_fitPRmass", "", "", 40, 240);

  Double_t parAR[6] = {0};

  f_fitZpmass->SetParameters(0,0,0);
  h_signTotalBKG->Fit("f_fitZpmass", "", "", xmin, xmax);

  parAR[0] = f_fitZpmass->GetParameter(0);
  parAR[1] = f_fitZpmass->GetParameter(1);
  parAR[2] = f_fitZpmass->GetParameter(2);

  f_fitZpmass->SetParameters(0,0,0);
  h_sideTotalBKG->Fit("f_fitZpmass", "", "", xmin, xmax);

  parAR[3] = f_fitZpmass->GetParameter(0);
  parAR[4] = f_fitZpmass->GetParameter(1);
  parAR[5] = f_fitZpmass->GetParameter(2);

  f_fitAlphaR->SetParameters(parAR[0],parAR[1],parAR[2],parAR[3],parAR[4],parAR[5]);

  h_numbkgDATA->Scale(nBkgSig/h_numbkgDATA->Integral());
  h_numbkgDATA->SetMinimum(0);
  h_corrPRmassFixErr->SetMinimum(0);

  // Output results

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  TLegend* leg = new TLegend(0.30, 0.70, 0.85, 0.80);

  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.03);
  leg->AddEntry(h_signDATA, "signal region of pseudo-data", "le");
  leg->AddEntry(h_numbkgDATA, "backgrounds in signal region of pseudo-data", "le");

  TLatex* lar = new TLatex(0.50, 0.94, "CMS,  #sqrt{s} = 13 TeV, L = 3000 pb^{-1}");

  lar->SetNDC(kTRUE);
  lar->SetTextSize(0.04);
  lar->SetLineWidth(5);

  TLatex* eqn0 = new TLatex(0.50, 0.80, "#font[22]{#color[4]{f(x) = #frac{1}{2} p_{0} e^{p_{1}x} ( 1 + erf ( #frac{x - p_{2}}{p_{3}} ) )}}");
  TLatex* eqn1 = new TLatex(0.50, 0.80, "#font[22]{#color[4]{f(x) = p_{0} e^{p_{1}x + #frac{p_{2}}{x}}}}");

  eqn0->SetNDC(kTRUE);
  eqn0->SetTextSize(0.04);
  eqn1->SetNDC(kTRUE);
  eqn1->SetTextSize(0.04);

  c->cd();
  h_PRmassAll->Draw();
  lar->Draw();
  eqn0->Draw();
  c->Print("alphaRatio.pdf(");
  
  c->cd();
  h_corrPRmassAll->Draw();
  lar->Draw();
  eqn0->Draw();
  c->Print("alphaRatio.pdf");

  c->cd();
  h_corrPRmassFixErr->Draw();
  g_errorBands->Draw("ae2same");
  h_corrPRmassFixErr->Draw("same");
  lar->Draw();
  eqn0->Draw();
  c->Print("alphaRatio.pdf");
  
  c->cd();
  h_signTotalBKG->Draw();
  lar->Draw();
  eqn1->Draw();
  c->Print("alphaRatio.pdf");

  c->cd();
  h_sideTotalBKG->Draw();
  lar->Draw();
  eqn1->Draw();
  c->Print("alphaRatio.pdf");

  c->cd();
  h_alphaRatio->Draw();
  f_fitAlphaR->Draw("same");
  lar->Draw();
  c->Print("alphaRatio.pdf");  

  c->Divide(1,2);
  TPad* c_up = (TPad*) c->GetListOfPrimitives()->FindObject("c_1");
  TPad* c_dw = (TPad*) c->GetListOfPrimitives()->FindObject("c_2"); 
  Double_t up_height = 0.8;
  Double_t dw_correction = 1.455;
  Double_t dw_height = (1-up_height)*dw_correction;
  c_up->SetPad(0,1-up_height,1,1);
  c_dw->SetPad(0,0,1,dw_height);
  c_dw->SetBottomMargin(0.25);
  c_up->cd();
  h_signDATA->GetXaxis()->SetTitle("");
  h_signDATA->GetXaxis()->SetLabelOffset(999);
  h_signDATA->GetXaxis()->SetLabelSize(0);
  h_signDATA->Draw();
  h_numbkgDATA->Draw("same");
  leg->Draw();
  lar->Draw();
  c_up->RedrawAxis();
  c_dw->cd();
  myRatio(h_numbkgDATA,h_signDATA);
  c->Draw();
  c->Print("alphaRatio.pdf)");

}
