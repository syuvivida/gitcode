#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TChain.h>
#include <THStack.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TVectorT.h>
#include <TProfile.h>
#include <TLorentzVector.h>
#include <TSystemDirectory.h>
#include <TGraphAsymmErrors.h>

const Int_t totalNEvent1 = 11765438;
const Int_t totalNEvent2 = 12511326;
const Double_t crossSection1 = 63.5*1000;
const Double_t crossSection2 = 39.4*1000;
// scale = ? fb^-1 / luminosity
Double_t scale1 = 0.876 / (totalNEvent1 / crossSection1); // DYJetsToLL_PtZ-70To100
Double_t scale2 = 0.876 / (totalNEvent2 / crossSection2); // DYJetsToLL_PtZ100


void myPlot(TH1D*, TH1D*, TH1D*, TH1D*);

void stcEleVariable(){

  TFile *barf = TFile::Open("barrelEleVariable.root");
  TFile *endf = TFile::Open("endcapEleVariable.root");

  gStyle->SetOptStat(0);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c1 = new TCanvas("c1", "", 0, 0, 1920, 1080);
  c1->Divide(2,2);

  c1->cd(1);
  myPlot( ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c1->cd(2);
  myPlot( ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c1->cd(3);
  myPlot( ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c1->cd(4);
  myPlot( ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c2 = new TCanvas("c2", "", 0, 0, 1920, 1080);
  c2->Divide(2,2);

  c2->cd(1)->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_SigIhIh_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_SigIhIh_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c2->cd(2)->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_SigIhIh_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_SigIhIh_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c2->cd(3)->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_HoE_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_HoE_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c2->cd(4)->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_HoE_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_HoE_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c3 = new TCanvas("c3", "", 0, 0, 1920, 1080);
  c3->Divide(2,2);
  
  c3->cd(1);
  myPlot( ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dxy_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dxy_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c3->cd(2);
  myPlot( ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dxy_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dxy_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c3->cd(3);
  myPlot( ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dz_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dz_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c3->cd(4);
  myPlot( ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dz_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dz_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c4 = new TCanvas("c4", "", 0, 0, 1920, 1080);
  c4->Divide(2,2);

  c4->cd(1)->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_EoverP_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_EoverP_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c4->cd(2)->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_EoverP_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_EoverP_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c4->cd(3);
  myPlot( ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_PassConv_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_PassConv_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c4->cd(4);
  myPlot( ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_PassConv_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_PassConv_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c5 = new TCanvas("c5", "", 0, 0, 1920, 1080);
  c5->Divide(2,2);

  c5->cd(1);
  myPlot( ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_MissingHits_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_MissingHits_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c5->cd(2);
  myPlot( ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_MissingHits_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_MissingHits_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c5->cd(3);
  myPlot( ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Pt_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_Pt_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c5->cd(4);
  myPlot( ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Pt_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_Pt_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c6 = new TCanvas("c6", "", 0, 0, 1920, 1080);
  c6->Divide(2,2);

  c6->cd(1);
  myPlot( ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Eta_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_Eta_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c6->cd(2);
  myPlot( ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Eta_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_Eta_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c6->cd(3)->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c6->cd(4)->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  TCanvas* c7 = new TCanvas("c7", "", 0, 0, 1920, 1080);
  c7->Divide(2,2);

  c7->cd(1);
  myPlot( ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserCalIso_data_DoubleEl_A.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserCalIso_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );

  c7->cd(2);
  myPlot( ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserCalIso_data_DoubleEl_A.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserCalIso_ZPrime_hZ_qqll_LHC8_M1000.root")))
	  );


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


  c1->Print("eleVariable1.gif");
  c2->Print("eleVariable2.gif");
  c3->Print("eleVariable3.gif");
  c4->Print("eleVariable4.gif");
  c5->Print("eleVariable5.gif");
  c6->Print("eleVariable6.gif");
  c7->Print("eleVariable7.gif");


}


void myPlot(TH1D* h_dy70, TH1D* h_dy100, TH1D* h_data, TH1D* h_sign){

  h_data->Sumw2();

  h_dy70->Scale(scale1);
  h_dy70->SetFillColor(kCyan-4);
  h_dy70->SetLineColor(kBlack);

  h_dy100->Scale(scale2);
  h_dy100->SetFillColor(kAzure+9);
  h_dy100->SetLineColor(kBlack);

  THStack *h_stack = new THStack("h_stack", "");
  h_stack->Add(h_dy70);
  h_stack->Add(h_dy100);

  h_data->SetLineColor(1);
  h_data->SetMarkerStyle(8);
  h_data->SetMarkerSize(0.5);
  h_data->Draw("e1"); 
  h_stack->Draw("same");
  h_data->Draw("e1same");

  /*
  h_sign->SetFillColor(kOrange+1);
  h_sign->SetFillStyle(3353);
  h_sign->SetLineColor(kBlack);
  h_sign->Draw("same");
  */

  TLegend *leg = new TLegend(0.65, 0.65, 0.9, 0.9);

  leg->SetFillStyle(1001);
  leg->SetFillColor(10);
  leg->SetBorderSize(1);
  leg->AddEntry(h_dy70, "DYJetsToLL_PtZ-70To100", "f"); 
  leg->AddEntry(h_dy100,"DYJetsToLL_PtZ-100", "f"); 
  //leg->AddEntry(h_sign, "hZ_qqll_LHC8_M1000", "f"); 
  leg->AddEntry(h_data, "Data", "lp");
  leg->Draw();

}
