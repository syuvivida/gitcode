#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>

void myProfile(TProfile*, TProfile*);
void myAsymm(TGraphAsymmErrors*, TGraphAsymmErrors*);

void drawMiniIso(){

  TFile *file[2];

  file[0] = TFile::Open("outputMini/ZprimeToZhToZlephbb_eleMiniIso.root");
  file[1] = TFile::Open("outputMini/DoubleEG_Run2015C-PromptReco-v1_eleMiniIso.root");
   
  gStyle->SetOptStat(0);
  //  gStyle->SetOptTitle(0);                                                                  
  gStyle->SetFrameLineWidth(2);                                                          
  gStyle->SetLineWidth(1);

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  std::string h_name[] = {"miniIsonVtx","corrIsonVtx","EffHEEPMini","EffHEEPCorr"};

  c->cd();    
  myProfile(((TProfile*)(file[0]->Get(h_name[0].data()))), 
	    ((TProfile*)(file[1]->Get(h_name[0].data()))));
  c->Print("eleMiniIso.pdf(");

  c->cd();
  myProfile(((TProfile*)(file[0]->Get(h_name[1].data()))),
	    ((TProfile*)(file[1]->Get(h_name[1].data()))));
  c->Print("eleMiniIso.pdf");

  TCanvas* d = new TCanvas("d","",0,0,1000,800);

  d->cd();
  myAsymm(((TGraphAsymmErrors*)(file[0]->Get(h_name[2].data()))),
	  ((TGraphAsymmErrors*)(file[1]->Get(h_name[2].data()))));
  d->Print("eleMiniIso.pdf");

  d->cd();
  myAsymm(((TGraphAsymmErrors*)(file[0]->Get(h_name[3].data()))),
          ((TGraphAsymmErrors*)(file[1]->Get(h_name[3].data()))));
  d->Print("eleMiniIso.pdf)");  

}

void myProfile(TProfile* h_sample, TProfile* h_data){

  h_data->SetLineColor(kRed);
  h_data->Draw();
  h_sample->Draw("same");

  TLegend *leg = new TLegend(0.65, 0.72, 0.87, 0.87);

  leg->SetBorderSize(0);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(h_sample, "Z' #rightarrow ZH signal", "le");
  leg->AddEntry(h_data, "Data", "le");
  leg->Draw();

  TLatex *lar = new TLatex(0.50, 0.92, "CMS,  #sqrt{s} = 13 TeV, L = 8.6 pb^{-1}");

  lar->SetNDC(kTRUE);
  lar->SetTextSize(0.04);
  lar->SetLineWidth(5);
  lar->Draw();

}

void myAsymm(TGraphAsymmErrors* h_sample, TGraphAsymmErrors* h_data){

  h_sample->SetLineColor(kBlue);
  h_sample->Draw("AP");
  h_data->SetLineColor(kRed);
  h_data->Draw("sameP");

  TLegend *leg = new TLegend(0.25, 0.32, 0.47, 0.47);

  leg->SetBorderSize(0);                                                     
  leg->SetLineColor(1);                                                                  
  leg->SetLineStyle(1);                                                                 
  leg->SetLineWidth(1);                                                               
  leg->SetFillColor(0);                                                               
  leg->SetFillStyle(0);                                                                   
  leg->SetTextSize(0.04);
  leg->AddEntry(h_sample, "Z' #rightarrow ZH signal", "le");
  leg->AddEntry(h_data, "Data", "le");
  leg->Draw();

  TLatex *lar = new TLatex(0.50, 0.92, "CMS,  #sqrt{s} = 13 TeV, L = 8.6 pb^{-1}");

  lar->SetNDC(kTRUE); 
  lar->SetTextSize(0.04);
  lar->SetLineWidth(5);
  lar->Draw();

}
