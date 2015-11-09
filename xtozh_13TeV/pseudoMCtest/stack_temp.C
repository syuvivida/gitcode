#include <string>
#include <vector>
#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
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

void stackSamples(std::vector<string>& infiles, std::string hname,
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

  TH1D* h_DY100 = fixDiscdBin((TH1D*)(f_DY100->Get(Form("%s",hname.c_str()))));
  TH1D* h_DY200 = fixDiscdBin((TH1D*)(f_DY200->Get(Form("%s",hname.c_str()))));
  TH1D* h_DY400 = fixDiscdBin((TH1D*)(f_DY400->Get(Form("%s",hname.c_str()))));
  TH1D* h_DY600 = fixDiscdBin((TH1D*)(f_DY600->Get(Form("%s",hname.c_str()))));
  TH1D* h_TTbar = fixDiscdBin((TH1D*)(f_TTbar->Get(Form("%s",hname.c_str()))));
  TH1D* h_WW    = fixDiscdBin((TH1D*)(f_WW->Get(Form("%s",hname.c_str()))));
  TH1D* h_WZ    = fixDiscdBin((TH1D*)(f_WZ->Get(Form("%s",hname.c_str()))));
  TH1D* h_ZZ    = fixDiscdBin((TH1D*)(f_ZZ->Get(Form("%s",hname.c_str()))));

  TH1D* h_DY = (TH1D*)h_DY100->Clone("h_DY");

  h_DY->Reset();
  h_DY->Add(h_DY100, scaleDY100);
  h_DY->Add(h_DY200, scaleDY200);
  h_DY->Add(h_DY400, scaleDY400);
  h_DY->Add(h_DY600, scaleDY600);
  h_DY->SetFillColor(kOrange-3);
  h_DY->SetLineColor(kBlack);

  h_TTbar->Scale(scaleTTbar);
  h_TTbar->SetFillColor(kGreen);
  h_TTbar->SetLineColor(kBlack);

  h_WW->Scale(scaleWW);
  h_WW->SetFillColor(kYellow);
  h_WW->SetLineColor(kBlack);

  h_WZ->Scale(scaleWZ);
  h_WZ->SetFillColor(kCyan);
  h_WZ->SetLineColor(kBlack);

  h_ZZ->Scale(scaleZZ);
  h_ZZ->SetFillColor(kPink);
  h_ZZ->SetLineColor(kBlack);

  THStack* h_stack = new THStack("h_stack", "");

  h_stack->Add(h_DY);
  h_stack->Add(h_TTbar);
  h_stack->Add(h_WW);
  h_stack->Add(h_WZ);
  h_stack->Add(h_ZZ);

  h_stack->Draw("histe");
  h_stack->GetHistogram()->GetYaxis()->SetTitle("Event numbers");
  h_stack->GetHistogram()->GetYaxis()->SetTitleSize(h_DY100->GetYaxis()->GetTitleSize());
  h_stack->GetHistogram()->GetYaxis()->SetLabelSize(h_DY100->GetYaxis()->GetLabelSize());
  h_stack->GetHistogram()->GetXaxis()->SetTitle("Corrected pruned mass");

  TLegend *leg = new TLegend(0.73, 0.60, 0.90, 0.87);
  
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
    
  leg->AddEntry(h_DY, "DY+Jets", "pf");
  leg->AddEntry(h_TTbar, "t#bar{t}", "pf");
  leg->AddEntry(h_WW, "WW", "pf");
  leg->AddEntry(h_WZ, "WZ", "pf");
  leg->AddEntry(h_ZZ, "ZZ", "pf");
  leg->Draw();

}

void stack_temp(std::string outputFolder){

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

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  c->cd();

  stackSamples(infiles,"corrPRmassAll_pDA",f_DY100,f_DY200,f_DY400,f_DY600,f_TTbar,f_WW,f_WZ,f_ZZ);

  c->Print("stackCorrPRmass.pdf");

}
