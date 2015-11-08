#include <string>
#include <vector>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystemDirectory.h>
#include "../setNCUStyle.h"

const Double_t varBins[] = {600,800,1000,1200,1400,1600,1800,2000,2500,3000,3500,4000,4500};
Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

Double_t dataLumi  = 831.7; //pb-1
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

TH1D* fixDiscdBin(TH1D* h){

  for( Int_t nb = 0; nb < nvarBins; nb++ ){
    Int_t binRatio = (varBins[nb+1]-varBins[nb])/(varBins[1]-varBins[0]);
    h->SetBinContent(nb+1,(h->GetBinContent(nb+1)/binRatio));
    h->SetBinError(nb+1,(h->GetBinError(nb+1)/binRatio));
  }

  return h;

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

  TH1D* DY100_temp = fixDiscdBin((TH1D*)(f_DY100->Get(Form("%s",hname.c_str()))));
  TH1D* DY200_temp = fixDiscdBin((TH1D*)(f_DY200->Get(Form("%s",hname.c_str()))));
  TH1D* DY400_temp = fixDiscdBin((TH1D*)(f_DY400->Get(Form("%s",hname.c_str()))));
  TH1D* DY600_temp = fixDiscdBin((TH1D*)(f_DY600->Get(Form("%s",hname.c_str()))));
  TH1D* TTbar_temp = fixDiscdBin((TH1D*)(f_TTbar->Get(Form("%s",hname.c_str()))));
  TH1D* WW_temp    = fixDiscdBin((TH1D*)(f_WW->Get(Form("%s",hname.c_str()))));
  TH1D* WZ_temp    = fixDiscdBin((TH1D*)(f_WZ->Get(Form("%s",hname.c_str()))));
  TH1D* ZZ_temp    = fixDiscdBin((TH1D*)(f_ZZ->Get(Form("%s",hname.c_str()))));

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

void alphaRplots(std::string outputFolder){

  setNCUStyle();
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

  TH1D* h_sideTotalBKG = addSamples(infiles,"ZprimeSide_pMC",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_signTotalBKG = addSamples(infiles,"ZprimeSign_pMC",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_sideDATA     = addSamples(infiles,"ZprimeSide_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_signDATA     = addSamples(infiles,"ZprimeSign_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_corrPRMassMC = addSamples(infiles,"corrPRMass_pMC",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);
  TH1D* h_corrPRMassDA = addSamples(infiles,"corrPRMass_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);

  h_corrPRMassMC->SetXTitle("pseudo-MC corrected pruned mass");
  h_corrPRMassDA->SetXTitle("pseudo-data corrected pruned mass");

  h_corrPRMassMC->SetYTitle("Event numbers");
  h_corrPRMassDA->SetYTitle("Event numbers");

  h_corrPRMassMC->SetLineColor(kBlack);
  h_corrPRMassDA->SetLineColor(kBlack);

  h_corrPRMassMC->SetLineWidth(2);
  h_corrPRMassDA->SetLineWidth(2);

  TH1D* h_alphaRatio   = new TH1D("h_alphaRatio", "", nvarBins, varBins);

  h_alphaRatio->Sumw2();
  h_alphaRatio->SetXTitle("ZH mass");
  h_alphaRatio->SetYTitle("Alpha ratio");
  h_alphaRatio->Divide(h_signTotalBKG,h_sideTotalBKG);
  h_alphaRatio->SetMinimum(0);
  h_alphaRatio->Draw();

  TH1D* h_numbkgDATA = (TH1D*)h_alphaRatio->Clone("h_numbkgDATA");

  h_numbkgDATA->Reset();
  h_numbkgDATA->SetXTitle("ZH mass");
  h_numbkgDATA->SetYTitle("Number of backgrounds in signal region");

  for( Int_t i = 1; i <= nvarBins; i++ ){

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

  h_numbkgDATA->Scale(h_signDATA->Integral()/h_numbkgDATA->Integral());
  h_numbkgDATA->SetMinimum(0);
  h_numbkgDATA->Draw();

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  c->cd();
  h_alphaRatio->Draw();
  c->Print("alphaRatio.pdf(");  

  c->cd();
  h_numbkgDATA->Draw();
  c->Print("alphaRatio.pdf");  

  c->cd();
  h_corrPRMassMC->Draw();
  c->Print("alphaRatio.pdf");

  c->cd();
  h_corrPRMassDA->Draw();
  c->Print("alphaRatio.pdf)");

}
