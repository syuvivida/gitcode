#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <TTree.h>
#include <TH1.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
#include "../setNCUStyle.h"

void scaleGraph(TGraphAsymmErrors* g, double factor){

  int npoints = g->GetN();

  for(int i = 0; i != npoints; ++i) {

    double x = g->GetX()[i];
    double y = g->GetY()[i];
    double eyh = g->GetEYhigh()[i];
    double eyl = g->GetEYlow()[i];
    y = (y*factor);
    eyh = (eyh*factor);
    eyl = (eyl*factor);
    g->SetPoint(i,x,y);
    g->SetPointEYhigh(i, eyh);
    g->SetPointEYlow(i, eyl);

  }

}

double expo_interp(double s2, double s1,  double newM, double m2, double m1){
  
  if( m1 > m2 ){

    double tmps = s1;
    double tmpm = m1;
    s1 = s2;
    m1 = m2;
    s2 = tmps;
    m2 = tmpm;

  }

  double alpha = (log(s2)-log(s1))/(m2-m1);

  return s1*pow(exp(newM-m1),alpha);

}

double linear_interp(double s2, double s1, double mass, double m2, double m1){

  if (m1 > m2){
  
    double tmps = s1;
    double tmpm = m1;
    s1 = s2;
    m1 = m2;
    s2 = tmps;
    m2 = tmpm;
  
  }

  return s1+(s2-s1)*(mass-m1)/(m2-m1);

}

void plot_Asymptotic(string outputname){

  setNCUStyle();  
  gStyle->SetTitleSize(0.04,"XYZ");
  gStyle->SetLabelSize(0.03,"XYZ");

  ifstream xsect_file("CSZH.txt", ios::in);

  float mH, CS;
  vector<int> v_mhxs;
  vector<float> v_xs, v_toterrh, v_toterrl;

  if( xsect_file.is_open() ){

    while( !xsect_file.eof()/*xsect_file.good()*/ ){

      xsect_file >> mH >> CS;
      v_mhxs.push_back(mH);
      v_xs.push_back(CS);

      // unavailable theory errors for graviton

      float tot_err_p = 0.0;
      float tot_err_m = 0.0;

      v_toterrh.push_back(1.0+(tot_err_p));
      v_toterrl.push_back(1.0-(tot_err_m));

    }
  
    xsect_file.close();

  }

  else{

    cout << "Failed to open text file: " << xsect_file << endl;
    return;

  }

  /// END THEORY INPUT PART ///

  int nmZH = v_mhxs.size();
  vector<double> v_mh, v_median, v_68l, v_68h, v_95l, v_95h, v_obs;
  TFile *fFREQ[nmZH];
  TTree *t[nmZH];
 
  for(int n = 0; n < nmZH; n++){

    char limitfile[100];

    if( outputname.find("Shape2d") != std::string::npos )
      sprintf(limitfile,"higgsCombineShape_2d.Asymptotic.mZH%d.root",v_mhxs[n]);
    else if( outputname.find("Shape1d") != std::string::npos )
      sprintf(limitfile,"higgsCombineShape_1d.Asymptotic.mZH%d.root",v_mhxs[n]);
    else if( outputname.find("Counting") != std::string::npos )
      sprintf(limitfile,"higgsCombineCounting.Asymptotic.mZH%d.root",v_mhxs[n]);
    
    fFREQ[n] = new TFile(limitfile, "READ");
    t[n] = (TTree*)fFREQ[n]->Get("limit");
  
    double mh, limit;
    float quant;
    
    t[n]->SetBranchAddress("mh", &mh);
    t[n]->SetBranchAddress("limit", &limit);
    t[n]->SetBranchAddress("quantileExpected", &quant);
  
    for(int i = 0; i < t[n]->GetEntries(); i++){

      t[n]->GetEntry(i);

      /// Map: mh --> observed, 95low, 68low, expected, 68hi, 95hi, xsec
      
      if (quant > -1.01 && quant < -0.99)
        v_obs.push_back(limit);
       
      else if (quant > 0.02 && quant < 0.03)
	v_95l.push_back(limit);
      
      else if (quant > 0.15 && quant < 0.17)
	v_68l.push_back(limit);
      
      else if (quant > 0.49 && quant < 0.51){
	v_median.push_back(limit);
        v_mh.push_back(mh);
      }
      else if (quant > 0.83 && quant < 0.85)
	v_68h.push_back(limit);
      
      else if (quant > 0.965 && quant < 0.98)
	v_95h.push_back(limit);
      
      else
        cout << "Error! Quantile =  " << quant << endl;
      
    }

  } //file loop

  /// Here we multiply the limits in terms of signal strength by the cross-section.
  /// There are also some hooks to exclude sick mass points.

  double mass[nmZH], obs_lim_cls[nmZH];
  double medianD[nmZH];
  double up68err[nmZH], down68err[nmZH], up95err[nmZH], down95err[nmZH];
  double xs[nmZH];
  double xs10[nmZH];
  int nMassEff = 0;
  
  for(int im = 0; im < nmZH; im++){

    double fl_xs = double(v_xs.at(im));
    double fl_xs10 = 0;
    fl_xs = (fl_xs);
    fl_xs10 = (fl_xs10);

    mass[nMassEff] = v_mhxs[im];

    /// This is the part where we multiply the limits in terms of signal strength
    /// by the cross-section, in order to have limits in picobarns.

    obs_lim_cls[nMassEff] = v_obs.at(im) * fl_xs;      
    medianD[nMassEff] = v_median.at(im) * fl_xs;
    up68err[nMassEff] = (v_68h.at(im) - v_median.at(im)) * fl_xs;
    down68err[nMassEff] = (v_median.at(im) - v_68l.at(im)) * fl_xs;

    //scale factor 100 for making the xsect visible

    xs[nMassEff] = fl_xs; 
    xs10[nMassEff] = fl_xs10;
    up95err[nMassEff] = (v_95h.at(im) - v_median.at(im)) * fl_xs;
    down95err[nMassEff] = (v_median.at(im) - v_95l.at(im)) * fl_xs;
    
    cout << "fl_xs:" << fl_xs << " v_obs" << v_obs.at(im) << " obs_lim_cls: " << obs_lim_cls[nMassEff] << medianD[nMassEff] << " mass: " << mass[nMassEff] <<endl;
 
    nMassEff++;
    
  } //end loop over im (mass points)


  TGraphAsymmErrors *grobslim_cls = new TGraphAsymmErrors(nMassEff, mass, obs_lim_cls);
  grobslim_cls->SetName("LimitObservedCLs");
  TGraphAsymmErrors *grmedian_cls = new TGraphAsymmErrors(nMassEff, mass, medianD);
  grmedian_cls->SetName("LimitExpectedCLs");
  TGraphAsymmErrors *gr68_cls = new TGraphAsymmErrors(nMassEff, mass, medianD, 0, 0, down68err, up68err);
  gr68_cls->SetName("Limit68CLs");
  TGraphAsymmErrors *gr95_cls = new TGraphAsymmErrors(nMassEff, mass, medianD, 0, 0, down95err, up95err);
  gr95_cls->SetName("Limit95CLs");
  TGraph *grthSM = new TGraph(nMassEff,mass,xs);
  grthSM->SetName("SMXSection");
  TGraph *grthSM10 = new TGraph(nMassEff,mass,xs10);
  grthSM10->SetName("SMXSection_2nd");

  double fr_left = 0.0, fr_down = 1E-6, fr_right = 4500.0, fr_up = 1;

  TCanvas *cMCMC = new TCanvas("c_lim_Asymptotic", "canvas with limits for Asymptotic CLs", 630, 600);

  cMCMC->cd();
  cMCMC->SetGridx(1);
  cMCMC->SetGridy(1);

  // draw a frame to define the range

  TH1F *hr = cMCMC->DrawFrame(fr_left, fr_down, fr_right, fr_up, "");
  TString VV = "ZH";
  
  hr->SetXTitle("M_{ZH} [GeV]");
  hr->SetYTitle("#sigma_{95%} [pb]"); // #rightarrow 2l2q
  
  gr95_cls->SetFillColor(kYellow);
  gr95_cls->SetFillStyle(1001);
  gr95_cls->SetLineStyle(kDashed);
  gr95_cls->SetLineWidth(3);
  gr95_cls->GetXaxis()->SetTitle("M_{V'} [GeV]");
  gr95_cls->GetYaxis()->SetTitle("#sigma_{95%} #times BR(V' #rightarrow " + VV + ") [pb]"); // #rightarrow 2l2q
  gr95_cls->GetXaxis()->SetRangeUser(fr_left, fr_right);
  gr95_cls->Draw("3");

  gr68_cls->SetFillColor(kGreen);
  gr68_cls->SetFillStyle(1001);
  gr68_cls->SetLineStyle(kDashed);
  gr68_cls->SetLineWidth(3);
  gr68_cls->Draw("3same");

  grmedian_cls->GetXaxis()->SetTitle("M_{V'} [GeV]");
  grmedian_cls->GetYaxis()->SetTitle("#sigma_{95%} #times BR(V' #rightarrow " + VV + ") [pb]"); // #rightarrow 2l2q
  grmedian_cls->SetMarkerStyle(24);
  grmedian_cls->SetMarkerColor(kBlack);
  grmedian_cls->SetLineStyle(2);
  grmedian_cls->SetLineWidth(3);
  grmedian_cls->SetMinimum(0.0);
  grmedian_cls->SetMaximum(8.0);

  grobslim_cls->SetMarkerColor(kBlack);
  grobslim_cls->SetMarkerStyle(21);
  grobslim_cls->SetMarkerSize(1.0);
  grobslim_cls->SetLineStyle(1);
  grobslim_cls->SetLineWidth(3);

  grthSM->SetLineColor(kRed);
  grthSM->SetLineWidth(2);
  grthSM->SetLineStyle(kSolid);
  grthSM->SetFillColor(kRed);
  grthSM->SetFillStyle(3344);

  grthSM10->SetLineColor(kRed);
  grthSM10->SetLineWidth(2);
  grthSM10->SetLineStyle(1);
  grthSM10->SetLineStyle(kDashed);
  grthSM10->SetFillColor(kRed);
  grthSM10->SetFillStyle(3344);

  grthSM->Draw("L3");
  grmedian_cls->Draw("L");
  //grobslim_cls->Draw("LP");

  //draw grid on top of limits

  TH1D* postGrid = new TH1D("postGrid", "", 1, fr_left, fr_right);

  postGrid->GetYaxis()->SetRangeUser(fr_down, fr_up);
  postGrid->Draw("AXIGSAME");

  TLegend *leg = new TLegend(.20, .20, .70, .35);

  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  //leg->AddEntry(grobslim_cls, "CL_{S} Observed", "LP");
  leg->AddEntry(gr68_cls, "CL_{S}  Expected #pm 1#sigma", "LF");
  leg->AddEntry(gr95_cls, "CL_{S}  Expected #pm 2#sigma", "LF");
  leg->AddEntry(grthSM, "#sigma_{TH}", "L");
  leg->Draw();

  TLatex * latex = new TLatex();

  latex->SetNDC(kTRUE);
  latex->SetTextSize(0.035);
  latex->DrawLatex(0.15, 0.94, "CMS preliminary 2015");
  latex->DrawLatex(0.55, 0.94, "L = 831.7 pb^{-1} at #sqrt{s} = 13 TeV");

  gPad->RedrawAxis("");

  cMCMC->Update();
  //cMCMC->Print(Form("XZHllbb_%s_Asymptotic.pdf", outputname.data()));
  gPad->SetLogy();
  cMCMC->Print(Form("XZHllbb_%s_Asymptotic_log.pdf", outputname.data()));

} //end of main function
