#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <TF1.h>
#include <TPad.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TVectorT.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>

const Int_t totalNEvent_dy70 = 11764538;
const Int_t totalNEvent_dy100 = 12511326;
const Double_t crossSection_dy70 = 63.5*1000;
const Double_t crossSection_dy100 = 39.4*1000;
// formula: scale = data_luminosity / bkg_luminosity
Double_t scale_dy70 = 1 / (totalNEvent_dy70 / crossSection_dy70);
Double_t scale_dy100 = 1 / (totalNEvent_dy100 / crossSection_dy100);

Double_t fitFunc(Double_t*, Double_t*);

void combineBkgEst(){

  TFile *f = TFile::Open("backgEstimate.root");

  TH1D* h_dy70side  = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70sign  = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side = (TH1D*)(f->Get("sideZpMass_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100sign = (TH1D*)(f->Get("signZpMass_DYJetsToLL_PtZ-100.root"));

  TH1D* h_dy70side_const  = (TH1D*)(f->Get("sideZpMass_constBin_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy70sign_const  = (TH1D*)(f->Get("signZpMass_constBin_DYJetsToLL_PtZ-70To100.root"));
  TH1D* h_dy100side_const = (TH1D*)(f->Get("sideZpMass_constBin_DYJetsToLL_PtZ-100.root"));
  TH1D* h_dy100sign_const = (TH1D*)(f->Get("signZpMass_constBin_DYJetsToLL_PtZ-100.root"));


  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;


  // fixing the discontinuous of bin content
  Int_t smallestBin = varBins[1] - varBins[0];
  vector<Int_t> binRatio;
  
  for(Int_t i = 0; i < nvarBins; i++){

    binRatio.push_back((varBins[i+1]-varBins[i])/smallestBin);

    h_dy70side->SetBinContent(i+1, (h_dy70side->GetBinContent(i+1)/binRatio[i]));
    h_dy70sign->SetBinContent(i+1, (h_dy70sign->GetBinContent(i+1)/binRatio[i]));
    h_dy100side->SetBinContent(i+1, (h_dy100side->GetBinContent(i+1)/binRatio[i]));
    h_dy100sign->SetBinContent(i+1, (h_dy100sign->GetBinContent(i+1)/binRatio[i]));

  }


  // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- //

  
  // variable-bin-width
  TF1* fitCurveSide = new TF1("fitCurveSide", fitFunc, 680, 2400, 3);
  TF1* fitCurveSign = new TF1("fitCurveSign", fitFunc, 680, 2400, 3);

  TH1D* h_combineBkgSide = new TH1D("h_combineBkgSide", "Side-band Zp Mass", nvarBins, varBins);
  TH1D* h_combineBkgSign = new TH1D("h_combineBkgSign", "Signal-band Zp Mass", nvarBins, varBins);
  TH1D* h_alpha = new TH1D("h_alpha", "Alpha ratio", nvarBins, varBins);

  gStyle->SetOptFit(1111);

  TCanvas* c0 = new TCanvas("c0", "", 0, 0, 1280, 800);
  c0->Divide(2,2);

  c0->cd(1);
  h_combineBkgSide->Sumw2();
  h_combineBkgSide->SetMarkerStyle(8);
  h_combineBkgSide->SetMarkerSize(0.7);
  h_combineBkgSide->SetMarkerColor(1);
  h_combineBkgSide->Add(h_dy70side, scale_dy70);
  h_combineBkgSide->Add(h_dy100side, scale_dy100);
  h_combineBkgSide->Fit("fitCurveSide", "", "", 680, 2400);
  h_combineBkgSide->GetXaxis()->SetTitle("Mass");
  h_combineBkgSide->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSide->Draw();

  c0->cd(2);
  h_combineBkgSign->Sumw2();
  h_combineBkgSign->SetMarkerStyle(8);
  h_combineBkgSign->SetMarkerSize(0.7);
  h_combineBkgSign->SetMarkerColor(1);
  h_combineBkgSign->Add(h_dy70sign, scale_dy70);
  h_combineBkgSign->Add(h_dy100sign, scale_dy100);
  h_combineBkgSign->Fit("fitCurveSign", "", "", 680, 2400);
  h_combineBkgSign->GetXaxis()->SetTitle("Mass");
  h_combineBkgSign->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSign->Draw();

  gStyle->SetOptStat(0);
  
  c0->cd(3);
  h_alpha->Sumw2();
  h_alpha->SetMarkerColor(1);
  h_alpha->SetMarkerStyle(8);
  h_alpha->SetMarkerSize(0.8);
  h_alpha->Divide(h_combineBkgSign, h_combineBkgSide);
  h_alpha->SetMinimum(0);
  h_alpha->SetMaximum(1);
  h_alpha->GetXaxis()->SetTitle("Zprime mass");
  h_alpha->GetYaxis()->SetTitle("Alpha Ratio");
  h_alpha->Draw();
  
  c0->cd(4);

  TF1* f_alphaFit = new TF1("f_alphaFit", "([0]*TMath::Exp([1]*x+[2]/x))/([3]*TMath::Exp([4]*x+[5]/x))", 680, 2400);
  f_alphaFit->SetParameter(0, fitCurveSign->GetParameter(0));
  f_alphaFit->SetParameter(1, fitCurveSign->GetParameter(1));
  f_alphaFit->SetParameter(2, fitCurveSign->GetParameter(2));
  f_alphaFit->SetParameter(3, fitCurveSide->GetParameter(0));
  f_alphaFit->SetParameter(4, fitCurveSide->GetParameter(1));
  f_alphaFit->SetParameter(5, fitCurveSide->GetParameter(2));
  f_alphaFit->SetMinimum(0);
  f_alphaFit->SetMaximum(1);
  f_alphaFit->SetTitle("Fit Alpha ratio");
  f_alphaFit->GetXaxis()->SetTitle("Zprime mass");
  f_alphaFit->GetYaxis()->SetTitle("Alpha Ratio");
  f_alphaFit->Draw();
  h_alpha->Draw("same");
  
  c0->Print("alphaRatio.gif");

  
  // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- //


  // fix-bin-width
  TH1D* h_combineBkgSide_const = new TH1D("h_combineBkgSide_const", "Side-band Zp Mass", 43, 680, 2400);
  TH1D* h_combineBkgSign_const = new TH1D("h_combineBkgSign_const", "Signal-band Zp Mass", 43, 680, 2400);
  TH1D* h_alpha_const = new TH1D("h_alpha_const", "Alpha ratio", 43, 680, 2400);

  TF1* fitCurveSide_const = new TF1("fitCurveSide_const", fitFunc, 680, 2400, 3);
  TF1* fitCurveSign_const = new TF1("fitCurveSign_const", fitFunc, 680, 2400, 3);

  TCanvas* c1 = new TCanvas("c1", "", 0, 0, 1280, 800);
  c1->Divide(2,2);

  c1->cd(1);
  h_combineBkgSide_const->Sumw2();
  h_combineBkgSide_const->Add(h_dy70side_const, scale_dy70);
  h_combineBkgSide_const->Add(h_dy100side_const, scale_dy100);
  h_combineBkgSide_const->SetMarkerStyle(8);
  h_combineBkgSide_const->SetMarkerSize(0.7);
  h_combineBkgSide_const->SetMarkerColor(1);
  h_combineBkgSide_const->Fit("fitCurveSide_const", "", "", 680, 2400);
  h_combineBkgSide_const->GetXaxis()->SetTitle("Mass");
  h_combineBkgSide_const->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSide_const->Draw();

  c1->cd(2);
  h_combineBkgSign_const->Sumw2();
  h_combineBkgSign_const->Add(h_dy70sign_const, scale_dy70);
  h_combineBkgSign_const->Add(h_dy100sign_const, scale_dy100);
  h_combineBkgSign_const->SetMarkerStyle(8);
  h_combineBkgSign_const->SetMarkerSize(0.7);
  h_combineBkgSign_const->SetMarkerColor(1);
  h_combineBkgSign_const->Fit("fitCurveSign_const", "", "", 680, 2400);
  h_combineBkgSign_const->GetXaxis()->SetTitle("Mass");
  h_combineBkgSign_const->GetYaxis()->SetTitle("Event Number");
  h_combineBkgSign_const->Draw();
  
  c1->cd(3);
  h_alpha_const->Sumw2();
  h_alpha_const->SetMarkerColor(1);
  h_alpha_const->SetMarkerStyle(8);
  h_alpha_const->SetMarkerSize(0.8);
  h_alpha_const->Divide(h_combineBkgSign_const, h_combineBkgSide_const);
  h_alpha_const->SetMinimum(0);
  h_alpha_const->SetMaximum(1);
  h_alpha_const->GetXaxis()->SetTitle("Zprime mass");
  h_alpha_const->GetYaxis()->SetTitle("Alpha Ratio");
  h_alpha_const->Draw();

  c1->cd(4);

  TF1* f_alphaFit_const = new TF1("f_alphaFit_const", "([0]*TMath::Exp([1]*x+[2]/x))/([3]*TMath::Exp([4]*x+[5]/x))", 680, 2400);
  f_alphaFit_const->SetParameter(0, fitCurveSign_const->GetParameter(0));
  f_alphaFit_const->SetParameter(1, fitCurveSign_const->GetParameter(1));
  f_alphaFit_const->SetParameter(2, fitCurveSign_const->GetParameter(2));
  f_alphaFit_const->SetParameter(3, fitCurveSide_const->GetParameter(0));
  f_alphaFit_const->SetParameter(4, fitCurveSide_const->GetParameter(1));
  f_alphaFit_const->SetParameter(5, fitCurveSide_const->GetParameter(2));
  f_alphaFit_const->SetMinimum(0);
  f_alphaFit_const->SetMaximum(1);
  f_alphaFit_const->SetTitle("Fit Alpha ratio");
  f_alphaFit_const->GetXaxis()->SetTitle("Zprime mass");
  f_alphaFit_const->GetYaxis()->SetTitle("Alpha Ratio");
  f_alphaFit_const->Draw();
  h_alpha_const->Draw("same");

  c1->Print("aplhaRatio_constBin.gif");


  // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- //


  // Calculate the Chi-square of alpha ratio 
  Double_t DifSqr = 0.;
  Double_t ErrSqr = 0.;
  Double_t ChiSqr = 0.;

  for(Int_t i = 1; i <= nvarBins; i++){

    Double_t DifSqr = pow((f_alphaFit_const->Eval(varBins[i-1]) - h_alpha->GetBinContent(i)), 2);
    Double_t ErrSqr = pow(h_alpha->GetBinError(i), 2);

    ChiSqr += DifSqr/ErrSqr;

  }

  cout << "\n------- Chi-Square = " << ChiSqr << " --------" << endl;
  cout << "------ Degree of freedom = " << nvarBins << " -------" << endl;
  cout << "--- Probability of fit = " << TMath::Prob(ChiSqr, nvarBins) << " ---\n" << endl;

  // This probability is the probability of getting Chi2 larger or equal to (chiSqr) when the ndf is (nvarBins).
  // Only when the error is Gaussian distributed (error on the ratio covers 68% of the distribution) will this
  // probability make sense. 


}

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x + par[2]/x);
  
}
