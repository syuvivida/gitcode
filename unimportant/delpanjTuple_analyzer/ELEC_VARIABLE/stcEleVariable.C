#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <TPad.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TLine.h>
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
#include "../HEADER/myPlot.C"
#include "../HEADER/myRatio.C"

void myPlot(TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*);
void myRatio(TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*, TH1D*);

void stcEleVariable(){

  TFile *barf = TFile::Open("barrelEleVariable.root");
  TFile *endf = TFile::Open("endcapEleVariable.root");

  gStyle->SetOptStat(0);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadGridX(kTRUE);

  Double_t up_height     = 0.8;
  Double_t dw_correction = 1.3;
  Double_t dw_height     = (1 - up_height) * dw_correction;

  TCanvas c("c","",0,0,1000,800);
  c.Divide(1,2);

  TPad* c_up = (TPad*) c.GetListOfPrimitives()->FindObject("c_1");
  TPad* c_dw = (TPad*) c.GetListOfPrimitives()->FindObject("c_2");

  c_up->SetPad(0, 1-up_height, 1, 1);
  c_dw->SetPad(0, 0, 1, dw_height);
  c_dw->SetBottomMargin(0.25);

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelEtaIn_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelEtaIn_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf(");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelEtaIn_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelEtaIn_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(barf->Get("barrel_DelPhiIn_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_DelPhiIn_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_DelPhiIn_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(endf->Get("endcap_DelPhiIn_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_DelPhiIn_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_DelPhiIn_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_SigIhIh_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_SigIhIh_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_SigIhIh_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_SigIhIh_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_SigIhIh_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(barf->Get("barrel_SigIhIh_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_SigIhIh_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_SigIhIh_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_SigIhIh_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_SigIhIh_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_SigIhIh_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_SigIhIh_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_SigIhIh_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_SigIhIh_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_SigIhIh_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_SigIhIh_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_SigIhIh_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_SigIhIh_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_SigIhIh_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_SigIhIh_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_SigIhIh_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_HoE_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_HoE_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_HoE_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_HoE_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_HoE_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_HoE_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_HoE_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_HoE_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_HoE_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_HoE_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_HoE_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(endf->Get("endcap_HoE_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_HoE_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_HoE_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_HoE_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_HoE_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_HoE_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_HoE_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_HoE_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_HoE_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_HoE_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_HoE_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dxy_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_Dxy_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dxy_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dxy_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dxy_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Dxy_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_Dxy_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_Dxy_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dxy_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dxy_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dxy_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(endf->Get("endcap_Dxy_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_Dxy_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dxy_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dxy_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dxy_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Dxy_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Dxy_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_Dxy_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dxy_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dxy_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dxy_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(barf->Get("barrel_Dz_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_Dz_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dz_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dz_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Dz_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Dz_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Dz_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_Dz_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dz_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dz_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Dz_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(endf->Get("endcap_Dz_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_Dz_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dz_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dz_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Dz_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Dz_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Dz_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_Dz_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dz_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dz_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Dz_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_EoverP_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_EoverP_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_EoverP_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_EoverP_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_EoverP_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_EoverP_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_EoverP_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_EoverP_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_EoverP_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_EoverP_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_EoverP_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_EoverP_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_EoverP_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_EoverP_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_EoverP_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_EoverP_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_EoverP_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(endf->Get("endcap_EoverP_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_EoverP_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_EoverP_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_EoverP_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_EoverP_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_PassConv_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_PassConv_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_PassConv_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_PassConv_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_PassConv_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_PassConv_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(barf->Get("barrel_PassConv_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_PassConv_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_PassConv_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_PassConv_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_PassConv_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_PassConv_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_PassConv_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_PassConv_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_PassConv_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_PassConv_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_PassConv_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(endf->Get("endcap_PassConv_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_PassConv_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_PassConv_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_PassConv_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_PassConv_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(barf->Get("barrel_MissingHits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_MissingHits_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_MissingHits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_MissingHits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_MissingHits_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_MissingHits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_MissingHits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_MissingHits_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_MissingHits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_MissingHits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_MissingHits_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_MissingHits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_MissingHits_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_MissingHits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_MissingHits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_MissingHits_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_MissingHits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(endf->Get("endcap_MissingHits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_MissingHits_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_MissingHits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_MissingHits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_MissingHits_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Pt_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_Pt_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Pt_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Pt_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Pt_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Pt_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Pt_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_Pt_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Pt_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Pt_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Pt_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-100.root"))),
	  ((TH1D*)(endf->Get("endcap_Pt_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_Pt_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Pt_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Pt_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Pt_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Pt_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Pt_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_Pt_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Pt_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Pt_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Pt_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_Eta_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_Eta_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Eta_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Eta_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_Eta_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_Eta_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_Eta_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_Eta_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Eta_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Eta_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_Eta_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_Eta_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_Eta_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Eta_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Eta_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_Eta_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Eta_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_Eta_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_Eta_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Eta_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Eta_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_Eta_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserTrkIso_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserTrkIso_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserTrkIso_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserTrkIso_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(barf->Get("barrel_UserCalIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(barf->Get("barrel_UserCalIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserCalIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserCalIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(barf->Get("barrel_UserCalIso_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(barf->Get("barrel_UserCalIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(barf->Get("barrel_UserCalIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(barf->Get("barrel_UserCalIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserCalIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserCalIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(barf->Get("barrel_UserCalIso_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-100.root"))),
 	  ((TH1D*)(endf->Get("endcap_UserCalIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(endf->Get("endcap_UserCalIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserCalIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserCalIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(endf->Get("endcap_UserCalIso_data_DoubleEl.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(endf->Get("endcap_UserCalIso_DYJetsToLL_PtZ-100.root"))), 
	   ((TH1D*)(endf->Get("endcap_UserCalIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(endf->Get("endcap_UserCalIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserCalIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserCalIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(endf->Get("endcap_UserCalIso_data_DoubleEl.root")))
	   );

  c.Draw();
  c.Print("eleVariable.pdf)");

  //---------------------------------------------------------------------//

}
