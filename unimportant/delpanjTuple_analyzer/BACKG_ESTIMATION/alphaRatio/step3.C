#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x + par[2]/x);
  
}

void step3(){

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("step2.root");

  TH1D* h_alphaRatio[3];
  TH1D* h_numbkgData[3];
  TH1D* h_combine[2][2]; // [bkg,data][side,signal]
  TF1* fitCurve[2][2]; // [bkg,data][side,signal]
  TF1* f_alphaFit;

  for(Int_t mode = 0; mode < 3; mode++){
    
    h_alphaRatio[mode] = (TH1D*)(f->Get(Form("AlpRo_%d",mode-1)));
    h_numbkgData[mode] = (TH1D*)(f->Get(Form("NBkg_Dat_%d",mode-1)));

  }

  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 2; j++){
      
      fitCurve[i][j] = new TF1(Form("fitCurve%d%d",i,j), fitFunc, 680, 2400, 3);
      fitCurve[i][j]->SetParameters(10,-0.005,400);
      h_combine[i][j] = (TH1D*)(f->Get(Form("h_combine%d%d_0",i,j)));
      h_combine[i][j]->Fit(Form("fitCurve%d%d",i,j), "", "", 680, 2400);

    }
  }

  f_alphaFit = new TF1("f_alphaFit","([0]*TMath::Exp([1]*x+[2]/x))/([3]*TMath::Exp([4]*x+[5]/x))",680,2400);
  f_alphaFit->SetParameter(0, fitCurve[0][1]->GetParameter(0));
  f_alphaFit->SetParameter(1, fitCurve[0][1]->GetParameter(1));
  f_alphaFit->SetParameter(2, fitCurve[0][1]->GetParameter(2));
  f_alphaFit->SetParameter(3, fitCurve[0][0]->GetParameter(0));
  f_alphaFit->SetParameter(4, fitCurve[0][0]->GetParameter(1));
  f_alphaFit->SetParameter(5, fitCurve[0][0]->GetParameter(2));
  f_alphaFit->SetMinimum(0);
  f_alphaFit->SetMaximum(1);
    
  TCanvas* c[2];

  c[0] = new TCanvas("c0","",0,0,800,800);
  c[1] = new TCanvas("c1","",0,0,800,800);

  c[0]->cd();
  h_alphaRatio[1]->SetTitle("Alpha ratio");
  h_alphaRatio[1]->GetXaxis()->SetTitle("X mass");
  h_alphaRatio[1]->GetYaxis()->SetTitle("Ratio"); 
  h_alphaRatio[1]->SetLineColor(kBlack);
  h_alphaRatio[1]->Draw();
  h_alphaRatio[0]->SetLineWidth(2);  
  h_alphaRatio[0]->SetLineColor(kGreen+1);
  h_alphaRatio[0]->Draw("histcsame");
  h_alphaRatio[2]->SetLineWidth(2);  
  h_alphaRatio[2]->SetLineColor(kCyan+1);
  h_alphaRatio[2]->Draw("histcsame");
  f_alphaFit->Draw("same");
  h_alphaRatio[1]->Draw("same");

  TLegend* leg1 = new TLegend(0.6, 0.7, 0.9, 0.9);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(10);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_alphaRatio[0],"scale=-1");
  leg1->AddEntry(h_alphaRatio[1],"scale=0 with stat. unc.","le");
  leg1->AddEntry(h_alphaRatio[2],"scale=1");
  leg1->AddEntry(f_alphaFit, "Fitting of scale=0");
  leg1->Draw();
    
  c[1]->cd();
  h_numbkgData[1]->SetTitle("Number of backgrounds in signal region");
  h_numbkgData[1]->GetXaxis()->SetTitle("X mass");
  h_numbkgData[1]->GetYaxis()->SetTitle("Numbers");
  h_numbkgData[1]->SetLineColor(kBlack);
  h_numbkgData[1]->Draw();
  h_numbkgData[0]->SetLineWidth(2);  
  h_numbkgData[0]->SetLineColor(kGreen+1);
  h_numbkgData[0]->Draw("histcsame");
  h_numbkgData[2]->SetLineWidth(2); 
  h_numbkgData[2]->SetLineColor(kCyan+1);
  h_numbkgData[2]->Draw("histcsame");
  h_numbkgData[1]->Draw("same");

  TLegend* leg2 = new TLegend(0.6, 0.7, 0.9, 0.9);
  leg2->SetFillStyle(1001);
  leg2->SetFillColor(10);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_numbkgData[0],"scale=-1");
  leg2->AddEntry(h_numbkgData[1],"scale=0 with stat. unc.","le");
  leg2->AddEntry(h_numbkgData[2],"scale=1");
  leg2->Draw();

  c[0]->Print("alphaRatio.pdf");
  c[1]->Print("numbkgData.pdf");
    
}
