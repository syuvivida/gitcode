#include <string>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>

void myProfile(TProfile*, TProfile*);
void myAsymm(TGraphAsymmErrors*, TGraphAsymmErrors*);

void drawMiniIso(){

  TFile *file[2];

  file[0] = TFile::Open("outputMiniIso/loose/ZprimeToZhToZlephbb_eleMiniIsoLoose.root");
  file[1] = TFile::Open("outputMiniIso/loose/DoubleEG_Run2015C-PromptReco-v1_eleMiniIsoLoose.root");
   
  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("c","",0,0,1000,800);

  std::string h_name[] = {"miniIsonVtx","corrIsonVtx","EffHEEPMini","EffHEEPCorr"};

  c->cd();    
  myProfile(((TProfile*)(file[0]->Get(h_name[0].data()))), 
	    ((TProfile*)(file[1]->Get(h_name[0].data()))));
  c->Print("eleMiniIsoLoose.pdf(");

  c->cd();
  myProfile(((TProfile*)(file[0]->Get(h_name[1].data()))),
	    ((TProfile*)(file[1]->Get(h_name[1].data()))));
  c->Print("eleMiniIsoLoose.pdf");

  TCanvas* d = new TCanvas("d","",0,0,1000,800);

  d->cd();
  myAsymm(((TGraphAsymmErrors*)(file[0]->Get(h_name[2].data()))),
	  ((TGraphAsymmErrors*)(file[1]->Get(h_name[2].data()))));
  d->Print("eleMiniIsoLoose.pdf");

  d->cd();
  myAsymm(((TGraphAsymmErrors*)(file[0]->Get(h_name[3].data()))),
          ((TGraphAsymmErrors*)(file[1]->Get(h_name[3].data()))));
  d->Print("eleMiniIsoLoose.pdf)");  

}

void myProfile(TProfile* h_sample, TProfile* h_data){

  h_data->SetLineColor(kRed);
  h_data->Draw();
  h_sample->Draw("same");

  TLegend *leg = new TLegend(0.5, 0.4, 0.8, 0.5);

  leg->SetFillStyle(0);
  leg->SetBorderSize(1);
  leg->AddEntry(h_sample, "ZprimeToZhToZlephbb", "le"); 
  leg->AddEntry(h_data, "DoubleEG_Run2015C-PromptReco-v1", "le");
  leg->Draw();

}

void myAsymm(TGraphAsymmErrors* h_sample, TGraphAsymmErrors* h_data){

  h_sample->SetLineColor(kBlue);
  h_sample->Draw("AP");
  h_data->SetLineColor(kRed);
  h_data->Draw("sameP");

  TLegend *leg = new TLegend(0.25, 0.25, 0.55, 0.35);

  leg->SetFillStyle(0);
  leg->SetBorderSize(1);
  leg->AddEntry(h_sample, "ZprimeToZhToZlephbb", "le");
  leg->AddEntry(h_data, "DoubleEG_Run2015C-PromptReco-v1", "le");
  leg->Draw();

}
