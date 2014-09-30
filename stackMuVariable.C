#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TLine.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TChain.h>
#include <THStack.h>
#include <TLegend.h>
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
void myRatio(TH1D*, TH1D*, TH1D*);

void stcMuVariable(){

  TFile *f = TFile::Open("muVariable.root");

  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadGridX(kTRUE);
 
  // define the size
  Double_t up_height     = 0.8;
  Double_t dw_correction = 1.4;
  Double_t dw_height     = (1 - up_height) * dw_correction;


  TCanvas c1("c1","",0,0,1920,1080);
  c1.Divide(1,2);

  TPad* c1_up = (TPad*) c1.GetListOfPrimitives()->FindObject("c1_1");
  TPad* c1_dw = (TPad*) c1.GetListOfPrimitives()->FindObject("c1_2");
 
  c1_up->SetPad(0, 1-up_height, 1, 1);
  c1_dw->SetPad(0, 0, 1, dw_height);
  c1_dw->SetTopMargin(0.25);
  
  c1_up->cd();
  myPlot( ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(f->Get("nVtx_data_DoubleMu_A.root")))
	  );


  c1_dw->cd();
  myRatio( ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(f->Get("nVtx_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(f->Get("nVtx_data_DoubleMu_A.root")))
	   );


  TCanvas c2("c2","",0,0,1920,1080);
  c2.Divide(1,2);

  TPad* c2_up = (TPad*) c2.GetListOfPrimitives()->FindObject("c2_1");
  TPad* c2_dw = (TPad*) c2.GetListOfPrimitives()->FindObject("c2_2"); 

  c2_up->cd()->SetLogy();
  myPlot( ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(f->Get("corrTrkIso_data_DoubleMu_A.root")))
	  );

  c2_up->SetPad(0, 1-up_height, 1, 1);
  c2_dw->SetPad(0, 0, 1, dw_height);
  c2_dw->SetTopMargin(0.25);

  c2_dw->cd();
  myRatio( ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(f->Get("corrTrkIso_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(f->Get("corrTrkIso_data_DoubleMu_A.root")))
	   );
  

  c1.Print("muVariable1.png");
  c2.Print("muVariable2.png");

}


void myPlot(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_data){

  h_data->Sumw2();

  h_dy70->Scale(scale1);
  h_dy70->SetFillColor(kCyan-4);
  h_dy70->SetLineColor(kBlack);

  h_dy100->Scale(scale2);
  h_dy100->SetFillColor(kAzure+9);
  h_dy100->SetLineColor(kBlack);

  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_dy70);
  h_stack->Add(h_dy100);

  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->Draw("e1"); 
  h_stack->Draw("same");
  h_data->Draw("e1same");

  TLegend *leg = new TLegend(0.65, 0.75, 0.9, 0.9);

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg->AddEntry(h_dy100,"DYJetsToLL_PtZ-100", "f"); 
  leg->AddEntry(h_data, "Data", "lp");
  leg->Draw();

}


void myRatio(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_data){

  TH1D *h_bkg = (TH1D*)h_data->Clone("h_bkg");
  h_bkg->Sumw2();
  h_bkg->Add(h_dy70,scale1);
  h_bkg->Add(h_dy100,scale2);

  TH1D* h_ratio = (TH1D*)h_data->Clone("h_ratio");
  h_ratio->Sumw2();

  Int_t nbin = h_ratio->GetNbinsX();
  Double_t ratio[nbin];
  Double_t error[nbin];
  Double_t numer_nbincontent[nbin];
  Double_t denom_nbincontent[nbin];
  Double_t numer_binerror[nbin];
  Double_t denom_binerror[nbin];
 
  for(Int_t i=1; i<=nbin; i++){

    numer_nbincontent[i] = h_data->GetBinContent(i);
    denom_nbincontent[i] = h_bkg->GetBinContent(i);
    numer_binerror[i] = h_data->GetBinError(i);
    denom_binerror[i] = h_bkg->GetBinError(i);

    if( denom_nbincontent[i] <= 0 || numer_nbincontent[i] <= 0 ) continue; 
    if( denom_binerror[i] <= 0 || numer_binerror[i] <= 0 ) continue;

    ratio[i] = (Double_t)numer_nbincontent[i]/denom_nbincontent[i];
    error[i] = (ratio[i])*sqrt(pow(numer_binerror[i]/numer_nbincontent[i],2)+pow(denom_binerror[i]/denom_nbincontent[i],2));

    h_ratio->SetBinContent(i,ratio[i]);
    h_ratio->SetBinError(i,error[i]);

  }
  
  Double_t font_size_dw = 0.1; 

  h_ratio->SetMarkerStyle(8);
  h_ratio->SetMarkerSize(0.6);
  h_ratio->SetTitle("");
  h_ratio->GetYaxis()->SetRangeUser(0,2);
  h_ratio->SetYTitle("data/MC");
  h_ratio->GetXaxis()->SetLabelSize(font_size_dw);
  h_ratio->GetXaxis()->SetTitleSize(font_size_dw);
  h_ratio->GetYaxis()->SetLabelSize(font_size_dw);
  h_ratio->GetYaxis()->SetTitleSize(font_size_dw);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->Draw();

  Double_t x0 = h_bkg->GetXaxis()->GetXmin();
  Double_t x1 = h_bkg->GetXaxis()->GetXmax();
  Double_t y0 = 1.; 
  Double_t y1 = 1.;
 
  TLine* one = new TLine(x0,y0,x1,y1);
  one->SetLineColor(2);
  one->SetLineStyle(1);
  one->SetLineWidth(2);
  one->Draw("same");
  
}
