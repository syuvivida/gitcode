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

void drawBkgZpMass(){

  gStyle->SetOptStat(0);

  TFile *f = TFile::Open("backgZpMass.root");

  TH1D* h_bkg[4];

  h_bkg[0] = (TH1D*)(f->Get("ZpMass_TTTo2L2Nu2B.root"));
  h_bkg[1] = (TH1D*)(f->Get("ZpMass_WW_pythia_filtered.root"));
  h_bkg[2] = (TH1D*)(f->Get("ZpMass_WZ_pythia_filtered.root"));
  h_bkg[3] = (TH1D*)(f->Get("ZpMass_ZZ_pythia_filtered.root"));
  /*
    h_bkg[] = (TH1D*)(f->Get("ZpMass_WWJetsTo2L2Nu_TuneZ2star_8TeV_filtered.root"));
    h_bkg[] = (TH1D*)(f->Get("ZpMass_WZJetsTo2L2Q_TuneZ2star_8TeV_filtered.root"));
    h_bkg[] = (TH1D*)(f->Get("ZpMass_WZJetsTo3LNu_TuneZ2_8TeV_filtered.root"));
    h_bkg[] = (TH1D*)(f->Get("ZpMass_ZZJetsTo2L2Nu_TuneZ2star_8TeV_filtered.root"));
    h_bkg[] = (TH1D*)(f->Get("ZpMass_ZZJetsTo2L2Q_TuneZ2star_8TeV_filtered.root"));
  */

  TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
  TCanvas* c = new TCanvas("c", "", 0, 0, 1000, 800);
  c->cd(1);

  Double_t vmax = 1.2;

  std::string title[4] = {"ttto2l2nu2b","wwpythia","wzpythia","zzpythia"};
  /*"wwjetsto2l2nu","wzjetsto2l2q","wzjetsto3lnu","zzjetsto2l2nu","zzjetsto2l2q",*/

  Color_t color[4] = {kRed,kOrange+7,kGreen+1,kCyan};

  for(Int_t i = 0; i < 4; i++){

    h_bkg[i]->Scale(1/h_bkg[i]->Integral());
    h_bkg[i]->SetMaximum(vmax);
    h_bkg[i]->SetLineColor(1);
    h_bkg[i]->SetFillStyle(3244);
    h_bkg[i]->SetFillColor(color[i]);

    if( i < 1 ) h_bkg[i]->Draw();
    if( i > 0 ) h_bkg[i]->Draw("same");

    leg->SetFillStyle(1001);
    leg->SetFillColor(10);
    leg->SetBorderSize(1);
    leg->AddEntry(h_bkg[i], title[i].data(), "f");
    leg->Draw();

  }

  c->Print("BkgZpMass.gif");
  //c->Print("BkgZpMass.pdf");

}
