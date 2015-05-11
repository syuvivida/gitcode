#include <vector>
#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>

Double_t fitFunc(Double_t* v, Double_t* par){
  
  Double_t x = v[0];
  return par[0]*TMath::Exp(par[1]*x + par[2]/x);
  
}

void combine(Int_t scaleMode){

  TFile *f_side = TFile::Open("SideZpMMu.root");
  TFile *f_signal = TFile::Open("SignalZpMMu.root");
  
  TH1D* h_target[2][3];  // [side][signal]
  
  h_target[0][0] = (TH1D*)(f_side->Get(Form("side_MX_%d_DYJetsToLL_PtZ-70To100",scaleMode)));
  h_target[0][1] = (TH1D*)(f_side->Get(Form("side_MX_%d_DYJetsToLL_PtZ-100",scaleMode)));
  h_target[0][2] = (TH1D*)(f_side->Get(Form("side_MX_%d_data_DoubleMu",scaleMode)));
  
  h_target[1][0] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_DYJetsToLL_PtZ-70To100",scaleMode)));
  h_target[1][1] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_DYJetsToLL_PtZ-100",scaleMode)));
  h_target[1][2] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_data_DoubleMu",scaleMode)));

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,
			      1250,1400,1600,1800,2000,2400};

  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  // fixing the discontinuous of bin content
  Int_t smallestBin = varBins[1] - varBins[0];
  vector<Int_t> binRatio;
  
  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 3; j++){
      for(Int_t nb = 0; nb < nvarBins; nb++){

	binRatio.push_back((varBins[nb+1]-varBins[nb])/smallestBin);
	h_target[i][j]->SetBinContent(nb+1, (h_target[i][j]->GetBinContent(nb+1)/binRatio[nb]));

      }
    }
  }

  // formula: scale = data_luminosity / bkg_luminosity
  const Double_t scale_dy70 = 19671.225/(11764538/63.5);
  const Double_t scale_dy100 = 19671.225/(12511326/39.4);

  Double_t scale[3] = {scale_dy70, scale_dy100, 1};
  std::string htitle[2][2] = {{"Bkg: Side-band Zp Mass","Bkg: Signal-band Zp Mass"},
			      {"Data: Side-band Zp Mass","Data: Signal-band Zp Mass"}};

  gStyle->SetOptFit(0);

  TF1* fitCurve[2][2]; // [bkg,data][side,signal]
  TH1D* h_combine[2][2]; // [bkg,data][side,signal]
  
  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 2; j++){

      fitCurve[i][j] = new TF1(Form("fitCurve%d%d",i,j), fitFunc, 680, 2400, 3);
      h_combine[i][j] = new TH1D(Form("h_combine%d%d",i,j), "", nvarBins, varBins);

      h_combine[i][j]->Sumw2();
      h_combine[i][j]->SetMarkerStyle(8);
      h_combine[i][j]->SetMarkerSize(0.7);
      h_combine[i][j]->SetMarkerColor(1);
      h_combine[i][j]->SetTitle(htitle[i][j].data());
      h_combine[i][j]->GetXaxis()->SetTitle("Mass");
      h_combine[i][j]->GetYaxis()->SetTitle("Event Number");
      
      if( i < 1 ){	
	h_combine[i][j]->Add(h_target[j][0], scale[0]);
	h_combine[i][j]->Add(h_target[j][1], scale[1]);
      }

      else if( i > 0 )
	h_combine[i][j]->Add(h_target[j][2], scale[2]);

      fitCurve[i][j]->SetParameters(10,-0.005,400);
      h_combine[i][j]->Fit(Form("fitCurve%d%d",i,j), "", "", 680, 2400);
      h_combine[i][j]->SetMinimum(0);
      h_combine[i][j]->Draw();

    }
  }

  TH1D* h_numbkgData = new TH1D("h_numbkgData", "", nvarBins, varBins);
  TH1D* h_alpha = new TH1D("h_alpha", "", nvarBins, varBins); // bkg only
  TF1* f_alphaFit = new TF1("f_alphaFit", "([0]*TMath::Exp([1]*x+[2]/x))/([3]*TMath::Exp([4]*x+[5]/x))", 680, 2400);
  
  h_alpha->Sumw2();
  h_alpha->SetMarkerColor(1);
  h_alpha->SetMarkerStyle(8);
  h_alpha->SetMarkerSize(0.8);
  h_alpha->Divide(h_combine[0][1], h_combine[0][0]); // signal/side
  
  f_alphaFit->SetParameter(0, fitCurve[0][1]->GetParameter(0));
  f_alphaFit->SetParameter(1, fitCurve[0][1]->GetParameter(1));
  f_alphaFit->SetParameter(2, fitCurve[0][1]->GetParameter(2));
  f_alphaFit->SetParameter(3, fitCurve[0][0]->GetParameter(0));
  f_alphaFit->SetParameter(4, fitCurve[0][0]->GetParameter(1));
  f_alphaFit->SetParameter(5, fitCurve[0][0]->GetParameter(2));
  f_alphaFit->SetMinimum(0);
  f_alphaFit->SetMaximum(1);
  f_alphaFit->SetTitle("Alpha ratio");
  f_alphaFit->GetXaxis()->SetTitle("Zprime mass");
  f_alphaFit->GetYaxis()->SetTitle("Alpha Ratio");
  f_alphaFit->Draw();
  h_alpha->Draw("same");

  Double_t alphaRatio, sidebandData, numbkgData;
  Double_t alphaRatioError, sidebandDataError, numbkgDataError;

  for(Int_t i = 1; i <= nvarBins; i++){

    alphaRatio = h_alpha->GetBinContent(i);
    sidebandData = h_combine[1][0]->GetBinContent(i);
    numbkgData = alphaRatio*sidebandData;
      
    alphaRatioError = h_alpha->GetBinError(i);
    sidebandDataError = h_combine[1][0]->GetBinError(i);

    if( alphaRatio == 0 || sidebandData == 0 ) continue;
    numbkgDataError = numbkgData*TMath::Sqrt(pow((alphaRatioError/alphaRatio),2)+pow((sidebandDataError/sidebandData),2));

    h_numbkgData->SetBinContent(i,numbkgData);
    h_numbkgData->SetBinError(i,numbkgDataError);

  }
    
  h_numbkgData->SetMarkerColor(1);
  h_numbkgData->SetMarkerStyle(8);
  h_numbkgData->SetMarkerSize(0.8);
  h_numbkgData->SetTitle("Number of Bkg of Data");
  h_numbkgData->GetXaxis()->SetTitle("Zprime mass");
  h_numbkgData->GetYaxis()->SetTitle("Number of Bkg of Data");
  h_numbkgData->SetMinimum(0);
  h_numbkgData->Draw();

  TFile* outFile = new TFile("temp.root","update");
  
  f_alphaFit->Write(Form("alphaFit%d",scaleMode));
  h_alpha->Write(Form("alphaRatio%d",scaleMode));
  h_numbkgData->Write(Form("numbkdData%d",scaleMode));
  
  outFile->Write();

}
