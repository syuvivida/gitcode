#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>

void finalAR(){

  TFile *f = TFile::Open("temp.root");

  TF1* h_alphaFit[3];
  TH1D* h_alphaRatio[3];
  TH1D* h_numbkgData[3];

  for(Int_t i = 0; i < 3; i++){
    
    h_alphaFit[i] = (TF1*)(f->Get(Form("alphaFit%d",i-1)));
    h_alphaRatio[i] = (TH1D*)(f->Get(Form("alphaRatio%d",i-1)));
    h_numbkgData[i] = (TH1D*)(f->Get(Form("numbkdData%d",i-1)));
    
  }

  TCanvas* c = new TCanvas("c","",0,0,1300,600);
  c->Divide(2,1);

  c->cd(1);
  h_alphaRatio[0]->SetXTitle("Zprime mass");
  h_alphaRatio[0]->SetYTitle("Ratio");
  h_alphaRatio[0]->SetMinimum(0);
  h_alphaRatio[0]->SetMaximum(1); 
  h_alphaRatio[0]->Draw();
  h_alphaRatio[1]->Draw("same");
  h_alphaRatio[2]->Draw("same");
  h_alphaFit[0]->Draw("same");
  h_alphaFit[1]->Draw("same");
  h_alphaFit[2]->Draw("same");

  c->cd(2);
  h_numbkgData[0]->Draw();
  h_numbkgData[1]->Draw("same");
  h_numbkgData[2]->Draw("same");

  c->Print("alphaRatioMu.jpg");
  
}
