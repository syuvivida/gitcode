#include <iostream>
#include <TH1D.h>
#include <TLine.h>

void myRatio(TH1D* h_DY, TH1D* h_ttbar, TH1D* h_data){

  TH1D *h_bkg = (TH1D*)h_data->Clone("h_bkg");
  h_bkg->Reset();
  h_bkg->Sumw2();
  h_bkg->Add(h_DY, scale_DY);
  h_bkg->Add(h_ttbar, scale_ttbar);

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
