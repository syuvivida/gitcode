#include <vector>
#include <iostream>
#include <TH1.h>
#include <TFile.h>
#include <TMath.h>

void step2(Int_t scaleMode){

  TFile *f_side = TFile::Open("SideZpMMu.root");
  TFile *f_signal = TFile::Open("SignalZpMMu.root");
  
  TH1D* h_target[2][3];  // [side][signal]
  
  h_target[0][0] = (TH1D*)(f_side->Get(Form("side_MX_%d_DYJetsToLL_PtZ-70To100",scaleMode)));
  h_target[0][1] = (TH1D*)(f_side->Get(Form("side_MX_%d_DYJetsToLL_PtZ-100",scaleMode)));
  h_target[0][2] = (TH1D*)(f_side->Get(Form("side_MX_%d_data_DoubleMu",scaleMode)));
  h_target[1][0] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_DYJetsToLL_PtZ-70To100",scaleMode)));
  h_target[1][1] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_DYJetsToLL_PtZ-100",scaleMode)));
  h_target[1][2] = (TH1D*)(f_signal->Get(Form("signal_MX_%d_data_DoubleMu",scaleMode)));

  const Double_t varBins[] = {680,720,760,800,840,920,1000,1100,1250,1400,1600,1800,2000,2400};
  Int_t nvarBins = sizeof(varBins)/sizeof(varBins[0])-1;

  // fixing the discontinuous of bin content
  
  Int_t smallestBin = varBins[1] - varBins[0];
  vector<Int_t> binRatio;
  
  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 3; j++){
      for(Int_t nb = 0; nb < nvarBins; nb++){
	
	binRatio.push_back((varBins[nb+1]-varBins[nb])/smallestBin);
	h_target[i][j]->SetBinContent(nb+1, (h_target[i][j]->GetBinContent(nb+1)/binRatio[nb]));
	h_target[i][j]->SetBinError(nb+1, (h_target[i][j]->GetBinError(nb+1)/binRatio[nb]));
	
      }
    }
  }

  // formula: scale = data_luminosity / bkg_luminosity
  
  const Double_t scale_dy70 = 19671.225/(11764538/63.5);
  const Double_t scale_dy100 = 19671.225/(12511326/39.4);

  Double_t scale[3] = {scale_dy70, scale_dy100, 1};
  TH1D* h_combine[2][2]; // [bkg,data][side,signal]
  
  for(Int_t i = 0; i < 2; i++){
    for(Int_t j = 0; j < 2; j++){

      h_combine[i][j] = new TH1D(Form("h_combine%d%d",i,j), "", nvarBins, varBins);
      h_combine[i][j]->Sumw2();
      
      if( i < 1 ){	
	h_combine[i][j]->Add(h_target[j][0], scale[0]);
	h_combine[i][j]->Add(h_target[j][1], scale[1]);
      }

      else if( i > 0 )
	h_combine[i][j]->Add(h_target[j][2], scale[2]);

      h_combine[i][j]->SetMinimum(0);
      h_combine[i][j]->Draw();

    }
  }

  TH1D* h_alphaRatio = new TH1D("h_alphaRatio", "", nvarBins, varBins); // bkg only
  TH1D* h_numbkgData = (TH1D*)h_alphaRatio->Clone("h_numbkgData");
  
  h_alphaRatio->Sumw2();
  h_alphaRatio->Divide(h_combine[0][1], h_combine[0][0]); // signal/side
  h_alphaRatio->SetMinimum(0);
  h_alphaRatio->SetMaximum(1); 
  h_alphaRatio->Draw();

  Double_t alphaRatio, sidebandData, numbkgData;
  Double_t alphaRatioError, sidebandDataError, numbkgDataError;

  for(Int_t i = 1; i <= nvarBins; i++){

    alphaRatio = h_alphaRatio->GetBinContent(i);
    sidebandData = h_combine[1][0]->GetBinContent(i);
    numbkgData = alphaRatio*sidebandData;
      
    alphaRatioError = h_alphaRatio->GetBinError(i);
    sidebandDataError = h_combine[1][0]->GetBinError(i);

    if( alphaRatio == 0 || sidebandData == 0 ) continue;
    numbkgDataError = numbkgData*TMath::Sqrt(pow((alphaRatioError/alphaRatio),2)+pow((sidebandDataError/sidebandData),2));

    h_numbkgData->SetBinContent(i,numbkgData);
    h_numbkgData->SetBinError(i,numbkgDataError);

  }
  
  //Double_t signalEvents = 10000;
  
  h_numbkgData->Scale(signalEvents/h_numbkgData->Integral(1,nvarBins));
  h_numbkgData->SetMinimum(0);
  h_numbkgData->Draw();

  TFile* outFile = new TFile("step2.root", "update");

  h_combine[0][0]->Write(Form("h_combine00_%d", scaleMode));
  h_combine[0][1]->Write(Form("h_combine01_%d", scaleMode));
  h_combine[1][0]->Write(Form("h_combine10_%d", scaleMode));
  h_combine[1][1]->Write(Form("h_combine11_%d", scaleMode));
  h_alphaRatio->Write(Form("AlpRo_%d", scaleMode));
  h_numbkgData->Write(Form("NBkg_Dat_%d", scaleMode));
  
  outFile->Write();
  
}
