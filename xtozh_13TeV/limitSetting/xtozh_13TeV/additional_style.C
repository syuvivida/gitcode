
void additional_style()
{

  // For Legend
  TLegend* legend = new TLegend(0.5, 0.7, 0.9,0.88,NULL,"brNDC");
  legend->SetLineColor(1);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.05);



  // For Data where luminosity is stated
  TLatex *t2 = new TLatex(0.13, 0.94, "CMS Preliminary");
  t2->SetNDC(kTRUE); 
  t2->SetTextSize(0.045);
  t2->SetTextFont(62);
  t2->SetLineWidth(5);
  t2->Draw("");

  float lumi = 40.7;
  TString latexnamepre= "#sqrt{s} = 13 TeV, #scale[0.45]{#int}L = ";
  TString latexnamemiddle;
  latexnamemiddle.Form("%1.2f",lumi);
  TString latexnamepost = " pb^{-1}";
  TString latexname = latexnamepre+latexnamemiddle+latexnamepost;
  TLatex *t2a = new TLatex(0.72,0.95,latexname);
  t2a->SetTextSize(0.045);
  t2a->SetTextAlign(22);
  t2a->SetNDC(kTRUE);
  t2a->SetTextFont(42);
  t2a->Draw("");

  // for MC figures 
  TLatex *lar = new TLatex(0.13, 0.94, "CMS");
  lar->SetNDC(kTRUE); 
  lar->SetTextSize(0.050);
  lar->SetTextFont(62);
  lar->SetLineWidth(5);
  lar->Draw("");

  TLatex *lar2 = new TLatex(0.25, 0.94, "Simulation");
  lar2->SetNDC(kTRUE); 
  lar2->SetTextSize(0.050);
  lar2->SetTextFont(52);
  lar2->SetLineWidth(5);
  lar2->Draw("");

  TH1F* h1 = new TH1F("h1","",100,0,1000);
  // when you want to change the x-axis range
  float xmin=10;
  float xmax=100;
  h1->GetXaxis()->SetRangeUser(xmin,xmax);
  // set title offset so that the title is not covered by labels
  h1->GetXaxis()->SetTitleOffset(1.5);
  h1->GetYaxis()->SetTitleOffset(1.5);
}
