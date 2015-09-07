#include <string>
#include <iostream>
#include <TPad.h>
#include <TH1D.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>

void myPlot(TH1D*, TH1D*, TH1D*, Double_t, Double_t);
void myRatio(TH1D*, TH1D*, TH1D*, Double_t, Double_t);

void stcZeeVariable(){

  TFile *file[3];

  file[0] = TFile::Open("outputZee/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns_ZeeVariable.root");
  file[1] = TFile::Open("outputZee/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830_ZeeVariable.root");
  file[2] = TFile::Open("outputZee/DoubleEG_Run2015C-PromptReco-v1_ZeeVariable.root");

  TH1D* h_eventWeightDY    = (TH1D*)(file[0]->Get("eventWeight"));
  TH1D* h_eventWeightTTbar = (TH1D*)(file[1]->Get("eventWeight"));  

  Int_t nEventDY      = h_eventWeightDY->Integral();
  Int_t nEventTTbar   = h_eventWeightTTbar->Integral();
  Double_t xSecDY     = 6025.2; //pb
  Double_t xSecTTbar  = 831.76; //pb
  Double_t dataLumi   = 8.1;    //ele //pb-1
  Double_t scaleDY    = dataLumi/(nEventDY/xSecDY);
  Double_t scaleTTbar = dataLumi/(nEventTTbar/xSecTTbar);

  std::cout << "\nnEventDY: " << nEventDY
	    << "\nnEventTTbar: " << nEventTTbar
	    << "\nscaleDY: " << scaleDY
	    << "\nscaleTTbar: " << scaleTTbar
	    << "\n" << std::endl;    
  
  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadGridX(kTRUE);

  Double_t up_height     = 0.8;
  Double_t dw_correction = 1.35;
  Double_t dw_height     = (1-up_height)*dw_correction;

  TCanvas c("c","",0,0,1000,800);
  c.Divide(1,2);

  TPad* c_up = (TPad*) c.GetListOfPrimitives()->FindObject("c_1");
  TPad* c_dw = (TPad*) c.GetListOfPrimitives()->FindObject("c_2"); 

  c_up->SetPad(0,1-up_height,1,1);
  c_dw->SetPad(0,0,1,dw_height);
  c_dw->SetBottomMargin(0.25);
  
  std::string h_name[] = {"Zmass","Zpt","Zeta","ZRapidity","leadElePt","leadEleEta",
			  "subleadElePt","subleadEleEta"};

  Int_t size = sizeof(h_name)/sizeof(h_name[0]);
  
  for(Int_t i = 0; i < size; i++){

    c_up->cd()->SetLogy(0);
    
    myPlot(((TH1D*)(file[0]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[1]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[2]->Get(h_name[i].data()))),
	   scaleDY, scaleTTbar);

    c_up->RedrawAxis();
    
    c_dw->cd();
    myRatio(((TH1D*)(file[0]->Get(h_name[i].data()))), 
	    ((TH1D*)(file[1]->Get(h_name[i].data()))),
	    ((TH1D*)(file[2]->Get(h_name[i].data()))),
	    scaleDY, scaleTTbar);

    c.Draw();
    
    if( i == 0 ) c.Print("ZeeVariable.pdf(");
    else if( i == size-1 ) c.Print("ZeeVariable.pdf)");
    else c.Print("ZeeVariable.pdf");
    
  }

}


void myPlot(TH1D* h_DY, TH1D* h_ttbar, TH1D* h_data, Double_t scaleDY, Double_t scaleTTbar){

  h_data->Sumw2();

  h_DY->Scale(scaleDY);
  h_DY->SetFillColor(kOrange-3);
  h_DY->SetLineColor(kBlack);
  
  h_ttbar->Scale(scaleTTbar);
  h_ttbar->SetFillColor(kGreen);
  h_ttbar->SetLineColor(kBlack);
   
  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_DY);
  h_stack->Add(h_ttbar);

  h_data->SetLineColor(kBlue);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.6);
  h_data->GetXaxis()->SetTitle("");
  h_data->GetXaxis()->SetLabelOffset(999);
  h_data->GetXaxis()->SetLabelSize(0);
  h_stack->Draw("histe");
  h_stack->GetHistogram()->GetYaxis()->SetTitle("Event Numbers");
  h_stack->GetHistogram()->GetXaxis()->SetTickLength(0);
  h_stack->GetHistogram()->GetXaxis()->SetLabelOffset(999);
  h_data->Draw("e1same");

  TLegend *leg = new TLegend(0.5, 0.8, 0.9, 0.9);

  leg->SetFillStyle(0);
  leg->SetBorderSize(1);
  leg->AddEntry(h_DY, "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns", "lpf"); 
  leg->AddEntry(h_ttbar, "crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830", "lpf");
  leg->AddEntry(h_data, "DoubleEG_Run2015C-PromptReco-v1", "lp");
  leg->Draw();

}


void myRatio(TH1D* h_DY, TH1D* h_ttbar, TH1D* h_data, Double_t scaleDY, Double_t scaleTTbar){

  TH1D *h_bkg = (TH1D*)h_data->Clone("h_bkg");
  h_bkg->Reset();
  h_bkg->Sumw2();
  h_bkg->Add(h_DY, scaleDY);
  h_bkg->Add(h_ttbar, scaleTTbar);

  TH1D* h_ratio = (TH1D*)h_data->Clone("h_ratio");
  h_ratio->Reset();
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
  h_ratio->GetYaxis()->SetTitle("data/MC");
  h_ratio->GetYaxis()->SetTitleOffset(0.3);
  h_ratio->GetXaxis()->SetLabelSize(font_size_dw);
  h_ratio->GetXaxis()->SetTitleSize(font_size_dw);
  h_ratio->GetYaxis()->SetLabelSize(font_size_dw);
  h_ratio->GetYaxis()->SetTitleSize(font_size_dw);
  h_ratio->GetYaxis()->SetNdivisions(505);
  h_ratio->GetYaxis()->SetRangeUser(0,2);
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

  h_ratio->Draw("same");

}
