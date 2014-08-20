#include <string>
#include <iostream>
#include <TH2.h>
#include <TLine.h>
#include <TFile.h>
#include <TMath.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TCanvas.h>
#include "setTDRStyle.C"

using namespace std;

void overlay_histo(){

  setTDRStyle();
  gStyle->SetOptTitle(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFontSize(0.04); 
  gStyle->SetLegendFont(102);
  
  string r_name[] = { "genin_hZ_qqll_LHC8_M2000.root",
		      "blind_hZ_qqll_LHC8_M2000.root" };

  string h_name[] = { "h_IDeff_st_trkmuPt", 
		      "h_IDeff_st_gblmuPt",
		      "h_IDeff_nd_trkmuPt",
		      "h_IDeff_nd_gblmuPt" };

  /*
  string h_name[] = { "h_IDeff_st_elePt", 
		      "h_IDeff_nd_elePt" };
  */
  const Int_t size_r = sizeof(r_name)/sizeof(r_name[0]);
  const Int_t size_h = sizeof(h_name)/sizeof(h_name[0]);

  TFile* r[size_r];
  TH1F*  h[size_h];

  float x1NDC = 0.65;
  float y1NDC = 0.84;
  float x2NDC = 1;
  float y2NDC = 1;

  TLegend* leg[size_h];

  TCanvas* c = new TCanvas("c1","",0,0,800,600); 
  c->SetMargin(10,20,10,20);
  
  for(Int_t i=0; i<size_h; i++){ // loop over variables

    string title_name = "total_" + h_name[i];
    string print_name = "total_M2000" + h_name[i] + ".gif";
    string line_name;
    Double_t get_max;
    Double_t temp = 0.0;

    leg[i] = new TLegend(x1NDC,y1NDC,x2NDC,y2NDC);

    //_____________________________________________________________

    for(Int_t j=0; j<size_r; j++){ // loop over masses

      r[j] = new TFile(r_name[j].data());
      h[j] = (TH1F*)r[j]->FindObjectAny(h_name[i].data());

      get_max = h[j]->GetMaximum();
      if(get_max > temp) temp = get_max;

    }

    for(Int_t j=0; j<size_r; j++){ // loop over masses again

      r[j] = new TFile(r_name[j].data());
      h[j] = (TH1F*)r[j]->FindObjectAny(h_name[i].data());
      h[j]->GetXaxis()->SetTitleOffset(1.2);
      h[j]->GetYaxis()->SetTitleOffset(1.1);
      h[j]->SetLineWidth(1);
      h[j]->SetLineColor(j+3);
      h[j]->SetMaximum(temp+temp*0.05);
      h[j]->SetMarkerColor(j+3);
      h[j]->SetMarkerStyle(2);
      h[j]->SetMarkerSize(1); 

      if(j == 0)
	h[j]->Draw();
      else
	h[j]->Draw("same");

      line_name = r_name[j].substr(0,r_name[j].size()-5);

      leg[i]->SetHeader(title_name.data());
      leg[i]->SetFillColor(0);
      leg[i]->SetFillStyle(0);
      leg[i]->SetTextSize(0.026);
      leg[i]->SetBorderSize(1);
      leg[i]->AddEntry(h[j],line_name.data());
      leg[i]->Draw(); 

    }
    
    //_____________________________________________________________
  
    c->Print(print_name.data());
  }

}
