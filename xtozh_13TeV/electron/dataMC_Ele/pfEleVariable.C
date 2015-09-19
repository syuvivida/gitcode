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

void pfEleVariable(){

  TFile *file[4];

  file[0] = TFile::Open("outputEle/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns_barrel.root");
  file[1] = TFile::Open("outputEle/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns_endcap.root");
  file[2] = TFile::Open("outputEle/DoubleEG_Run2015C-PromptReco-v1_barrel.root");
  file[3] = TFile::Open("outputEle/DoubleEG_Run2015C-PromptReco-v1_endcap.root");
   
  gStyle->SetOptStat(0);

  TCanvas c("c","",0,0,1000,800);
    
  std::string pf_name[] = {"pf_eleEtaseedAtVtx","pf_eledPhiAtVtx","pf_eleHoverE",
			   "pf_eleSigmaIEtaIEtaFull5x5","pf_eleFull5x5E2x5dvE5x5",
			   "pf_eleFull5x5E1x5dvE5x5","pf_eleMissHits","pf_eleD0",
			   "pf_eleMiniIso"};

  Int_t size = sizeof(pf_name)/sizeof(pf_name[0]);
  
  for(Int_t i = 0; i < size; i++){

    c.cd();
    
    myProfile(((TProfile*)(file[0]->Get(pf_name[i].data()))), 
	      ((TProfile*)(file[2]->Get(pf_name[i].data()))));

    c.Draw();
    
    if( i == 0 ) c.Print("pf_eleVariable_barrel.pdf(");
    else if( i == size-1 ) c.Print("pf_eleVariable_barrel.pdf)");
    else c.Print("pf_eleVariable_barrel.pdf");
    
  }

  for(Int_t i = 0; i < size; i++){
    
    c.cd();

    myProfile(((TProfile*)(file[1]->Get(pf_name[i].data()))), 
	      ((TProfile*)(file[3]->Get(pf_name[i].data()))));
     
    c.Draw();
   
    if( i == 0 ) c.Print("pf_eleVariable_endcap.pdf(");
    else if( i == size-1 ) c.Print("pf_eleVariable_endcap.pdf)");
    else c.Print("pf_eleVariable_endcap.pdf");
    
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
  leg->AddEntry(pf_DY, "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns", "lep"); 
  leg->AddEntry(pf_data, "DoubleEG_Run2015C-PromptReco-v1", "lep");
  leg->Draw();

}
