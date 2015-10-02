#include <string>
#include <iostream>
#include <TPad.h>
#include <TH1D.h>
#include <TFile.h>
#include <TLine.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>

void myPlot(TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, Double_t*);
void myRatio(TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, Double_t*);

void stcjeteeVariable(){

  TFile *file[6];

  file[0] = TFile::Open("outputJetee/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_jeteeVariable.root");
  file[1] = TFile::Open("outputJetee/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_jeteeVariable.root");
  file[2] = TFile::Open("outputJetee/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_jeteeVariable.root");
  file[3] = TFile::Open("outputJetee/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_jeteeVariable.root");
  file[4] = TFile::Open("outputJetee/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830_jeteeVariable.root");
  file[5] = TFile::Open("outputJetee/DoubleEG_Run2015C-PromptReco-v1_jeteeVariable.root");

  // i=4 is ttbar 
  TH1D*    h_eventWeight[5];
  Int_t    nEvent[5]  = {0};
  Double_t scale[5]   = {0};
  Double_t dataLumi   = 8.6;  //mu //pb-1                                                                              
  Double_t crossSection[5] = {139.4,42.75,5.497,2.21,831.76};  //pb 

  for(Int_t i = 0; i < 5 ; i++){

    h_eventWeight[i] = (TH1D*)(file[i]->Get("eventWeight"));
    nEvent[i] = h_eventWeight[i]->Integral();
    scale[i] = dataLumi/(nEvent[i]/crossSection[i]);

  }
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);                                                                  
  gStyle->SetFrameLineWidth(2);                                                          
  gStyle->SetLineWidth(1);
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
  
  std::string h_name[] = {"FATjetPt","FATjetEta","FATjetCISVV2","FATjetSDmass",
			  "FATjetPRmass","FATjetTau1","FATjetTau2","FATjetTau2dvTau1",
			  "FATsubjetPt","FATsubjetEta","FATsubjetSDCSV"};

  Int_t size = sizeof(h_name)/sizeof(h_name[0]);
  
  for(Int_t i = 0; i < size; i++){

    c_up->cd()->SetLogy(0);
    
    myPlot(((TH1D*)(file[0]->Get(h_name[i].data()))),
	   ((TH1D*)(file[1]->Get(h_name[i].data()))),
	   ((TH1D*)(file[2]->Get(h_name[i].data()))),
	   ((TH1D*)(file[3]->Get(h_name[i].data()))),
	   ((TH1D*)(file[4]->Get(h_name[i].data()))),
	   ((TH1D*)(file[5]->Get(h_name[i].data()))),
	   scale);

    c_up->RedrawAxis();
    
    c_dw->cd();

    myRatio(((TH1D*)(file[0]->Get(h_name[i].data()))),
	    ((TH1D*)(file[1]->Get(h_name[i].data()))),
	    ((TH1D*)(file[2]->Get(h_name[i].data()))),
	    ((TH1D*)(file[3]->Get(h_name[i].data()))),
	    ((TH1D*)(file[4]->Get(h_name[i].data()))),
	    ((TH1D*)(file[5]->Get(h_name[i].data()))),
	    scale);

    c.Draw();
    
    if( i == 0 ) c.Print("jeteeVariable.pdf(");
    else if( i == size-1 ) c.Print("jeteeVariable.pdf)");
    else c.Print("jeteeVariable.pdf");
    
  }

}


void myPlot(TH1D* h_DY100, TH1D* h_DY200, TH1D* h_DY400,
            TH1D* h_DY600, TH1D* h_TTbar, TH1D* h_data,
	    Double_t* scale){

  h_data->Sumw2();

  TH1D* h_DY = (TH1D*)h_DY100->Clone("h_DY");

  h_DY->Reset(); 
  h_DY->Add(h_DY100,scale[0]);
  h_DY->Add(h_DY200,scale[1]);
  h_DY->Add(h_DY400,scale[2]);
  h_DY->Add(h_DY600,scale[3]);
  h_DY->SetFillColor(kOrange-3);
  h_DY->SetLineColor(kBlack);

  h_TTbar->Scale(scale[4]);
  h_TTbar->SetFillColor(kGreen);
  h_TTbar->SetLineColor(kBlack);
   
  THStack *h_stack = new THStack("h_stack", "");

  h_stack->Add(h_DY);
  h_stack->Add(h_TTbar);

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

  TLegend *leg = new TLegend(0.72, 0.72, 0.87, 0.87);

  leg->SetBorderSize(0);                                                     
  leg->SetLineColor(1);                                                                  
  leg->SetLineStyle(1);                                                                 
  leg->SetLineWidth(1);                                                               
  leg->SetFillColor(0);                                                               
  leg->SetFillStyle(0);                                                                   
  leg->SetTextSize(0.04);
  leg->AddEntry(h_DY, "DY+Jets", "lpf"); 
  leg->AddEntry(h_TTbar, "t#bar{t}", "lpf");
  leg->AddEntry(h_data, "Data", "lp");
  leg->Draw();

  TLatex *lar = new TLatex(0.55, 0.92, "CMS, #sqrt{s} = 13 TeV, L = 8.6 pb^{-1}");

  lar->SetNDC(kTRUE); 
  lar->SetTextSize(0.04);
  lar->SetLineWidth(5);
  lar->Draw();

}


void myRatio(TH1D* h_DY100, TH1D* h_DY200, TH1D* h_DY400,
	     TH1D* h_DY600, TH1D* h_TTbar, TH1D* h_data,
	     Double_t* scale){

  TH1D *h_bkg = (TH1D*)h_data->Clone("h_bkg");
  h_bkg->Reset();
  h_bkg->Sumw2();
  h_bkg->Add(h_DY100,scale[0]);
  h_bkg->Add(h_DY200,scale[1]);
  h_bkg->Add(h_DY400,scale[2]);
  h_bkg->Add(h_DY600,scale[3]);
  h_bkg->Add(h_TTbar,scale[4]);

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
