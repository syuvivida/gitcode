#include <string>
#include <iostream>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>

/*
  Why we don't need any scale?
  Because the y-axis of profile is just the average mean of the distribution of each ID variable. 
  For the same physics process, the cross section and luminosity only affect the total integrated area, but not the mean.
*/

void myProfile(TProfile*, TProfile*);

void pfMuVariable(){

  TFile *file[4];

  file[0] = TFile::Open("outputMu/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_highptMuon.root");
  file[1] = TFile::Open("outputMu/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_customizeTrackerMuon.root");
  file[2] = TFile::Open("outputMu/crab_SingleMuon_Run2015B-PromptReco-v1_0825_highptMuon.root");
  file[3] = TFile::Open("outputMu/crab_SingleMuon_Run2015B-PromptReco-v1_0825_customizeTrackerMuon.root");
   
  gStyle->SetOptStat(0);

  TCanvas c("c","",0,0,1000,800);
    
  std::string pf_name[] = {"pf_muMatches","pf_muTrkLayers","pf_muPixelHits","pf_muTrkPtErrdvTrkPt",
			   "pf_mudxy","pf_mudz","pf_muMiniIso","pf_muHits"};

  Int_t size = sizeof(pf_name)/sizeof(pf_name[0]);
  
  for(Int_t i = 0; i < size; i++){

    c.cd();
    
    myProfile(((TProfile*)(file[0]->Get(pf_name[i].data()))), 
	      ((TProfile*)(file[2]->Get(pf_name[i].data()))));

    c.Draw();
    
    if( i == 0 ) c.Print("pf_muVariable_highptMuon.pdf(");
    else if( i == size-1 ) c.Print("pf_muVariable_highptMuon.pdf)");
    else c.Print("pf_muVariable_highptMuon.pdf");
    
  }

  for(Int_t i = 0; i < size; i++){
    
    c.cd();

    myProfile(((TProfile*)(file[1]->Get(pf_name[i].data()))), 
	      ((TProfile*)(file[3]->Get(pf_name[i].data()))));
     
    c.Draw();
   
    if( i == 0 ) c.Print("pf_muVariable_customizeTrackerMuon.pdf(");
    else if( i == size-1 ) c.Print("pf_muVariable_customizeTrackerMuon.pdf)");
    else c.Print("pf_muVariable_customizeTrackerMuon.pdf");
    
  }
  
}


void myProfile(TProfile* pf_DY, TProfile* pf_data){

  pf_DY->SetLineColor(kBlack); 
  pf_DY->SetMarkerColor(kBlack);
  pf_DY->SetMarkerStyle(8);
  pf_DY->SetMarkerSize(0.6);
  pf_DY->Draw();
  
  pf_data->SetLineColor(kRed);
  pf_data->SetMarkerColor(kRed);
  pf_data->SetMarkerStyle(8);
  pf_data->SetMarkerSize(0.6);
  pf_data->Draw("same");

  TLegend *leg = new TLegend(0.4, 0.8, 0.9, 0.9);

  leg->SetFillStyle(0);
  leg->SetBorderSize(1);
  leg->AddEntry(pf_DY, "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", "lep"); 
  leg->AddEntry(pf_data, "crab_SingleMuon_Run2015B-PromptReco-v1_0825", "lep");
  leg->Draw();

}
