#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TSystemDirectory.h>

void checkPlot(std::string fileName, Int_t Msample){

  Double_t MW = 80.3;
  Double_t MZ = 91.2;
  Double_t MH = 125.7;

  TFile *f = TFile::Open(fileName.data());

  cout << "\n ::: Checking file => " << fileName << " :::" << endl;

  TH1D* h[19];
  h[0] = (TH1D*)(f->Get("h_Xm"));
  h[1] = (TH1D*)(f->Get("h_Bm0")); 
  h[2] = (TH1D*)(f->Get("h_Bm1")); 
  h[3] = (TH1D*)(f->Get("h_Xpt"));
  h[4] = (TH1D*)(f->Get("h_Bpt0"));
  h[5] = (TH1D*)(f->Get("h_Dpt0_0"));
  h[6] = (TH1D*)(f->Get("h_Dpt0_1"));
  h[7] = (TH1D*)(f->Get("h_Bpt1"));
  h[8] = (TH1D*)(f->Get("h_Dpt1_0"));
  h[9] = (TH1D*)(f->Get("h_Dpt1_1"));
  h[10] = (TH1D*)(f->Get("h_D_dR0"));
  h[11] = (TH1D*)(f->Get("h_D_dR1"));
  h[12] = (TH1D*)(f->Get("h_Xy"));
  h[13] = (TH1D*)(f->Get("h_By0"));
  h[14] = (TH1D*)(f->Get("h_Dy0_0")); 
  h[15] = (TH1D*)(f->Get("h_Dy0_1"));
  h[16] = (TH1D*)(f->Get("h_By1"));
  h[17] = (TH1D*)(f->Get("h_Dy1_0")); 
  h[18] = (TH1D*)(f->Get("h_Dy1_1"));

  Bool_t status = true;
  Int_t falseCount = 0;

  if( TMath::Nint(h[0]->GetMean(1)) != Msample ){ status = false; falseCount++; }
  else status = true;
  cout << " ::: status => " << status << " ::: exotic mass => " << TMath::Nint(h[0]->GetMean(1)) << " :::" << endl;
  
  Int_t pID[2] = {0};
  Double_t Mboson[2] = {0};

  for(Int_t i = 1; i <= 2; i++){

    if( h[i]->GetMean(1) > 0.7*MZ && h[i]->GetMean(1) < 1.3*MZ ){
      pID[i-1] = 23;
      Mboson[i-1] = h[i]->GetMean(1);
      status = true;
    }else if( h[i]->GetMean(1) > 0.7*MW && h[i]->GetMean(1) < 1.3*MW ){
      pID[i-1] = 24;
      Mboson[i-1] = h[i]->GetMean(1);
      status = true;
    }else if( h[i]->GetMean(1) > 0.7*MH && h[i]->GetMean(1) < 1.3*MH ){
      pID[i-1] = 25;
      Mboson[i-1] = h[i]->GetMean(1);
      status = true;
    }
    else{ status = false; falseCount++; }
    cout << " ::: status => " << status << " ::: pID of boson " << i-1 << " => " << pID[i-1] << " ::: mass => " << Mboson[i-1] << " :::" << endl;

  }

  if( h[3]->GetMean(1) > 1e-6 ){ status = false; falseCount++; }
  else status = true;
  cout << " ::: status => " << status << " ::: exotic Pt => " << h[3]->GetMean(1) << " :::" << endl;

  Double_t ptLow[6] = {0};
  std::string pName[6] = {"boson 0","daughter 00","daughter 01","boson 1","daughter 10","daughter 10"};

  for(Int_t i = 4; i <= 9; i++){

    ptLow[i-4] = h[i]->GetXaxis()->GetBinUpEdge(h[i]->FindLastBinAbove(0,1));
    if( ptLow[i-4] < 0.7*Msample/2 || ptLow[i-4] > 1.3*Msample/2 ){ status = false; falseCount++; }
    else status = true;
    cout << " ::: status => " << status << " ::: cutoff Pt of " << pName[i-4] << " => " << ptLow[i-4] << " :::" << endl;

  }

  Double_t dRpeak[2] = {0};
  dRpeak[0] = h[10]->GetXaxis()->GetBinCenter(h[10]->GetMaximumBin());
  dRpeak[1] = h[11]->GetXaxis()->GetBinCenter(h[11]->GetMaximumBin());

  for(Int_t i = 0; i <= 1; i++){

    Double_t theoretical = 4*Mboson[i]/Msample;
    if( dRpeak[i] < 0.7*theoretical || dRpeak[i] > 1.3*theoretical ){ status = false; falseCount++; }
    else status = true;
    cout << " ::: status => " << status << " ::: deltaR peak of daughter of boson " << i << " => " << dRpeak[i] << " ::: theoretical => " << theoretical << " :::" << endl;

  }

  for(Int_t i = 12; i <= 18; i++){

    Double_t totalLeft = h[i]->Integral(h[i]->GetXaxis()->GetFirst(),(h[i]->GetXaxis()->GetFirst()+h[i]->GetXaxis()->GetLast())/2);  
    Double_t totalRight = h[i]->Integral((h[i]->GetXaxis()->GetFirst()+h[i]->GetXaxis()->GetLast())/2,h[i]->GetXaxis()->GetLast());
    Double_t fraction = fabs(totalLeft-totalRight)/(totalLeft+totalRight);

    if( fraction > 0.04 ){ status = false; falseCount++; }
    else status = true;
    cout << " ::: status => " << status << " ::: asymmetry of rapidity => " << fraction << " :::" << endl;

  }

  if( falseCount > 0 ) cout << " ::: There might have one or more problems in this file! :::" << endl;
  else cout << " ::: There is no problem in this file! :::" << endl;
  cout << " ::: Check completed! :::\n" << endl;

}
