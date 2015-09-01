#include <string>
#include <iostream>
#include <TPad.h>
#include <TH1D.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "myPlot.C"
#include "myRatio.C"

void myPlot(TH1D*, TH1D*, TH1D*);
void myRatio(TH1D*, TH1D*, TH1D*);

void stcEleVariable(){

  TFile *file[6];

  file[0] = TFile::Open("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns_barrel.root");
  file[1] = TFile::Open("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_25ns_endcap.root");
  file[2] = TFile::Open("TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803_barrel.root");
  file[3] = TFile::Open("TT_TuneCUETP8M1_13TeV-powheg-pythia8_0803_endcap.root");
  file[4] = TFile::Open("SingleElectron_Run2015C-PromptReco-v1_barrel.root");
  file[5] = TFile::Open("SingleElectron_Run2015C-PromptReco-v1_endcap.root");

  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadGridX(kTRUE);

  Double_t up_height     = 0.8;
  Double_t dw_correction = 1.3;
  Double_t dw_height     = (1-up_height)*dw_correction;

  TCanvas c("c","",0,0,1000,800);
  c.Divide(1,2);

  TPad* c_up = (TPad*) c.GetListOfPrimitives()->FindObject("c_1");
  TPad* c_dw = (TPad*) c.GetListOfPrimitives()->FindObject("c_2"); 

  c_up->SetPad(0,1-up_height,1,1);
  c_dw->SetPad(0,0,1,dw_height);
  c_dw->SetBottomMargin(0.25);
  
  std::string h_name[9] = {"eleEtaseedAtVtx","eledPhiAtVtx","eleHoverE",
			   "eleSigmaIEtaIEtaFull5x5","eleFull5x5E2x5dvE5x5",
			   "eleFull5x5E1x5dvE5x5","eleMissHits","eleD0","eleMiniIso"};
  
  for(Int_t i = 0; i < 9; i++){

    c_up->cd();
    myPlot(((TH1D*)(file[0]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[2]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[4]->Get(h_name[i].data())))
	   );

    c_up->RedrawAxis();
    
    c_dw->cd();
    myRatio(((TH1D*)(file[0]->Get(h_name[i].data()))), 
	    ((TH1D*)(file[2]->Get(h_name[i].data()))),
	    ((TH1D*)(file[4]->Get(h_name[i].data())))
	    );

    c.Draw();
    
    if( i == 0 ) c.Print("eleVariable_barrel.pdf(");
    else if( i == 8 ) c.Print("eleVariable_barrel.pdf)");
    else c.Print("eleVariable_barrel.pdf");
    
  }

  for(Int_t i = 0; i < 9; i++){
    
    c_up->cd();
    myPlot(((TH1D*)(file[1]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[3]->Get(h_name[i].data()))), 
	   ((TH1D*)(file[5]->Get(h_name[i].data())))
	   );

    c_up->RedrawAxis();
    
    c_dw->cd();
    myRatio(((TH1D*)(file[1]->Get(h_name[i].data()))), 
	    ((TH1D*)(file[3]->Get(h_name[i].data()))),
	    ((TH1D*)(file[5]->Get(h_name[i].data())))
	    );
    
    c.Draw();
   
    if( i == 0 ) c.Print("eleVariable_endcap.pdf(");
    else if( i == 8 ) c.Print("eleVariable_endcap.pdf)");
    else c.Print("eleVariable_endcap.pdf");
    
  }

}
