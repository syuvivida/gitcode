#include <string>
#include <fstream>
#include <iostream>
#include <TH1D.h>
#include <TFile.h>

void createText(string name, string sample, double crossSection, double luminosity){

  TFile *file = TFile::Open(name.data());
 
  TH1D* hp = (TH1D*)(file->Get(""));
  TH1D* ht = (TH1D*)(file->Get(""));  

  double nPass = hp->Integral()*crossSection*luminosity/ht->Integral();

  ofstream fout;
  fout.open("DRSubjet.txt", ios::out | ios::app);

  fout << sample << "\t" << nPass << endl;

  fout.close();
 
}
