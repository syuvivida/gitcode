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

void stcMuVariable(){

  TFile *glof = TFile::Open("globalMuVariable.root");
  TFile *trcf = TFile::Open("trackerMuVariable.root");

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
  myPlot( ((TH1D*)(glof->Get("global_Hits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Hits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Hits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Hits_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Hits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Hits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Hits_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Hits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Hits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Hits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Hits_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Hits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Hits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Hits_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf(");
  
  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Hits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Hits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Hits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Hits_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Hits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Hits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Hits_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Hits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Hits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Hits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Hits_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Hits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Hits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Hits_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");
  
  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Matches_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Matches_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Matches_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Matches_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Matches_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Matches_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Matches_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Matches_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Matches_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Matches_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Matches_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Matches_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Matches_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Matches_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//
  
  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Matches_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Matches_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Matches_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Matches_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Matches_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Matches_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Matches_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Matches_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Matches_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Matches_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Matches_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Matches_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Matches_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Matches_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_PixelHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_PixelHits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_PixelHits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_PixelHits_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PixelHits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PixelHits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PixelHits_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_PixelHits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_PixelHits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_PixelHits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_PixelHits_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PixelHits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PixelHits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PixelHits_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_PixelHits_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_PixelHits_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_PixelHits_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_PixelHits_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PixelHits_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PixelHits_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PixelHits_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_PixelHits_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_PixelHits_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_PixelHits_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_PixelHits_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PixelHits_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PixelHits_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PixelHits_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_TrkLayers_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_TrkLayers_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_TrkLayers_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_TrkLayers_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkLayers_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkLayers_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkLayers_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_TrkLayers_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_TrkLayers_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_TrkLayers_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_TrkLayers_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkLayers_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkLayers_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkLayers_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_TrkLayers_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkLayers_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_TrkLayers_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkLayers_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_PtErrx_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_PtErrx_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_PtErrx_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_PtErrx_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PtErrx_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PtErrx_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_PtErrx_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_PtErrx_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_PtErrx_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_PtErrx_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_PtErrx_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PtErrx_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PtErrx_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_PtErrx_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_PtErrx_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_PtErrx_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_PtErrx_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_PtErrx_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PtErrx_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PtErrx_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_PtErrx_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_PtErrx_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_PtErrx_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_PtErrx_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_PtErrx_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PtErrx_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PtErrx_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_PtErrx_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Dxy_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Dxy_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Dxy_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dxy_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dxy_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dxy_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Dxy_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Dxy_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Dxy_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dxy_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dxy_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dxy_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Dxy_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Dxy_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dxy_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dxy_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dxy_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dxy_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Dxy_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Dxy_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dxy_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dxy_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dxy_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dxy_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dxy_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Dz_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Dz_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Dz_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dz_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dz_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Dz_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Dz_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Dz_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Dz_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dz_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dz_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Dz_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Dz_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Dz_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dz_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dz_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dz_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Dz_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Dz_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Dz_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dz_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dz_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dz_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dz_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Dz_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Pt_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Pt_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Pt_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Pt_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Pt_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Pt_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Pt_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Pt_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Pt_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Pt_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Pt_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Pt_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");
  
  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Pt_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Pt_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Pt_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Pt_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Pt_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Pt_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Pt_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Pt_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Pt_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Pt_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Pt_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Pt_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Pt_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//
  
  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Eta_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Eta_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Eta_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Eta_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Eta_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Eta_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Eta_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Eta_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Eta_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Eta_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Eta_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Eta_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Eta_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Eta_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Eta_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Eta_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Eta_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Eta_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Eta_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Eta_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Eta_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Eta_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Eta_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Eta_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(glof->Get("global_Phi_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_Phi_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_Phi_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_Phi_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Phi_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Phi_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_Phi_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_Phi_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_Phi_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_Phi_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_Phi_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Phi_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Phi_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_Phi_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy(0);
  myPlot( ((TH1D*)(trcf->Get("tracker_Phi_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Phi_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_Phi_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_Phi_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Phi_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Phi_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_Phi_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_Phi_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_Phi_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_Phi_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_Phi_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Phi_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Phi_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_Phi_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(glof->Get("global_CorrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_CorrTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_CorrTrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_CorrTrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_CorrTrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_CorrTrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_CorrTrkIso_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_CorrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_CorrTrkIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_CorrTrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_CorrTrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_CorrTrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_CorrTrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_CorrTrkIso_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(trcf->Get("tracker_CorrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_CorrTrkIso_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_CorrTrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_CorrTrkIso_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(glof->Get("global_TrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(glof->Get("global_TrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(glof->Get("global_TrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(glof->Get("global_TrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(glof->Get("global_TrkIso_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(glof->Get("global_TrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(glof->Get("global_TrkIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(glof->Get("global_TrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(glof->Get("global_TrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(glof->Get("global_TrkIso_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf");

  //---------------------------------------------------------------------//

  c_up->cd()->SetLogy();
  myPlot( ((TH1D*)(trcf->Get("tracker_TrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_TrkIso_DYJetsToLL_PtZ-100.root"))), 
	  ((TH1D*)(trcf->Get("tracker_TrkIso_TTTo2L2Nu2B.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkIso_WW_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkIso_WZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkIso_ZZ_pythia_filtered.root"))),
	  ((TH1D*)(trcf->Get("tracker_TrkIso_data_DoubleMu.root")))
	  );

  c_up->RedrawAxis();

  c_dw->cd();
  myRatio( ((TH1D*)(trcf->Get("tracker_TrkIso_DYJetsToLL_PtZ-70To100.root"))), 
	   ((TH1D*)(trcf->Get("tracker_TrkIso_DYJetsToLL_PtZ-100.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkIso_TTTo2L2Nu2B.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkIso_WW_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkIso_WZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkIso_ZZ_pythia_filtered.root"))),
	   ((TH1D*)(trcf->Get("tracker_TrkIso_data_DoubleMu.root")))
	   );

  c.Draw();
  c.Print("muVariable.pdf)");
  
  //---------------------------------------------------------------------//

}
