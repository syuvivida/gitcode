#include <TH1.h>
#include <TKey.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <string>
#include <iostream>

void outputHisto(string inputFile, char *name){
  
  gSystem->mkdir(name);
  TFile::Open(inputFile.data());
  TCanvas *c = new TCanvas("c", "", 1024, 768); 
  TDirectory *current_sourcedir = gDirectory;
  TIter nextkey(current_sourcedir->GetListOfKeys());
  TKey *key;

  while((key = (TKey*)nextkey())){
    TObject *obj = key->ReadObj();
    if (obj->IsA()->InheritsFrom("TH1")){
      TH1 *h = (TH1*)obj;   
      h->Draw();
      c->Print(Form("%s/%s.gif", name, obj->GetName()));
      c->Print(Form("%s/%s.pdf", name, obj->GetName()));
      c->Print(Form("%s/%s.C",   name, obj->GetName()));

    }
  }
}
