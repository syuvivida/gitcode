#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>

void myPlot(TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, Double_t*);

void stcmZHmu_onlyOneSubjet(){

  TFile *file[6];

  file[0] = TFile::Open("outputmZHmu/onlyOneSubjet/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmu_onlyOneSubjet.root");
  file[1] = TFile::Open("outputmZHmu/onlyOneSubjet/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmu_onlyOneSubjet.root");
  file[2] = TFile::Open("outputmZHmu/onlyOneSubjet/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmu_onlyOneSubjet.root");
  file[3] = TFile::Open("outputmZHmu/onlyOneSubjet/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_mZHmu_onlyOneSubjet.root");
  file[4] = TFile::Open("outputmZHmu/onlyOneSubjet/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8_0830_mZHmu_onlyOneSubjet.root");
  file[5] = TFile::Open("outputmZHmu/onlyOneSubjet/crab_SingleMuon-Run2015C-PromptReco-v1_mZHmu_onlyOneSubjet.root");

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

  TCanvas c("c","",0,0,1000,800);

  std::string h_name[] = {"mZprime","mZ","ptZ","FATjetPt","FATjetSDmass",
                          "FATjetPRmass","FATjetTau2dvTau1","cutFlow"};

  Int_t size = sizeof(h_name)/sizeof(h_name[0]);

  for(Int_t i = 0; i < size; i++){

    c.cd();

    myPlot(((TH1D*)(file[0]->Get(h_name[i].data()))),
	   ((TH1D*)(file[1]->Get(h_name[i].data()))),
	   ((TH1D*)(file[2]->Get(h_name[i].data()))),
	   ((TH1D*)(file[3]->Get(h_name[i].data()))),
	   ((TH1D*)(file[4]->Get(h_name[i].data()))),
	   ((TH1D*)(file[5]->Get(h_name[i].data()))),
	   scale);

    c.Draw();

    if( i == 0 ) c.Print("mZHmu_onlyOneSubjet.pdf(");
    else if( i == size-1 ) c.Print("mZHmu_onlyOneSubjet.pdf)");
    else c.Print("mZHmu_onlyOneSubjet.pdf");

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

  h_data->SetLineColor(kBlack);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.6);
  if( h_stack->GetMaximum() > h_data->GetMaximum() ){
    h_stack->Draw("histe");
    h_stack->GetHistogram()->GetYaxis()->SetTitle("Event Numbers");
    h_stack->GetHistogram()->GetXaxis()->SetTitle(h_data->GetXaxis()->GetTitle());
    h_data->Draw("same");
  }
  else if( h_data->GetMaximum() > h_stack->GetMaximum() ){
    h_data->Draw();
    h_stack->Draw("histesame");
    h_stack->GetHistogram()->GetYaxis()->SetTitle("Event Numbers");
    h_stack->GetHistogram()->GetXaxis()->SetTitle(h_data->GetXaxis()->GetTitle());
    h_data->Draw("same");
  }

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

  TLatex *lar = new TLatex(0.48, 0.92, "CMS,  #sqrt{s} = 13 TeV, L = 8.6 pb^{-1}");

  lar->SetNDC(kTRUE); 
  lar->SetTextSize(0.04);
  lar->SetLineWidth(5);
  lar->Draw();

}
