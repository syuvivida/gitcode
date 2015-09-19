#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;

template <class T> 
string ConvertToString(T);

void calculation_2(std::string inputFile){
  
  string lhapdf_temp;
  int    nproc_temp;
  double scale_temp;
  double facscale_temp;
  string dynscale_temp;
  int    min34_temp;
  int    min56_temp;
  string namestring_temp;
  double xsecValue_temp;
  double xsecPDFErr_plus_temp;
  double xsecPDFErr_minus_temp;
  double xsecPDFErr_temp;
  double branching_ratio_temp;

  vector<string> lhapdf;
  vector<int>    nproc;
  vector<double> scale;
  vector<double> facscale;
  vector<string> dynscale;
  vector<int>    min34;
  vector<int>    min56;
  vector<string> namestring;
  vector<double> xsecValue;
  vector<double> xsecPDFErr_plus;
  vector<double> xsecPDFErr_minus;
  vector<double> xsecPDFErr;
  vector<double> branching_ratio;

  ifstream fin;
  fin.open(inputFile.data());

  while(!fin.eof()){
    fin >> lhapdf_temp;
    fin >> nproc_temp;
    fin >> scale_temp;
    fin >> facscale_temp;
    fin >> dynscale_temp;
    fin >> min34_temp;
    fin >> min56_temp;
    fin >> namestring_temp;
    fin >> xsecValue_temp;
    fin >> xsecPDFErr_plus_temp;
    fin >> xsecPDFErr_minus_temp;
    fin >> xsecPDFErr_temp;
    fin >> branching_ratio_temp;

    if(!fin.eof()){
      lhapdf.push_back(lhapdf_temp);
      nproc.push_back(nproc_temp);
      scale.push_back(scale_temp);
      facscale.push_back(facscale_temp);
      dynscale.push_back(dynscale_temp);
      min34.push_back(min34_temp);
      min56.push_back(min56_temp);
      namestring.push_back(namestring_temp);
      xsecValue.push_back(xsecValue_temp);
      xsecPDFErr_plus.push_back(xsecPDFErr_plus_temp);
      xsecPDFErr_minus.push_back(xsecPDFErr_minus_temp);
      xsecPDFErr.push_back(xsecPDFErr_temp);
      branching_ratio.push_back(branching_ratio_temp);
    }
  }
  
  fin.close();

  ofstream fout1;
  ofstream fout2;

  fout1.open("output.tex", ios::out | ios::app);
  fout2.open("comput.txt", ios::out | ios::app);

  const int size = lhapdf.size();
  Double_t denomin, cent_unc;
  Double_t errp, errm;

  for(int i=0; i<size; i++){

    if(lhapdf[i] == "CT10" && scale[i] == 1 && facscale[i] == 1){
      denomin = xsecValue[i];
      cent_unc = xsecValue[i] / denomin;
      errp = xsecPDFErr_plus[i];
      errm = xsecPDFErr_minus[i];

    }
  }

  Double_t diff, cross_section, BR;
  Double_t npr, cut34, cut56;
  Double_t min_cc, max_cc, ave_cc;
  Double_t min_pdf_unc, max_pdf_unc;
  Double_t sca_unc, fac_sca_unc, abs_sca_unc;
  Double_t temp1=0, temp2=0, temp3=10000;
  Double_t cc[6];

  string process, dyn_sca;

  for(int i=0; i<size; i++){
    
    if(lhapdf[i] == "CT10"){
      sca_unc = xsecValue[i] / denomin;
      diff = abs(sca_unc - cent_unc);

      if (diff > temp1)
	temp1 = diff;
      fac_sca_unc = temp1;

    }//end if

    if(lhapdf[i] == "CT10"){
      cc[0] = denomin + errp;
      cc[1] = denomin - errm;

    }else if(lhapdf[i] == "MSTW2"){
      cc[2] = xsecValue[i] + xsecPDFErr_plus[i];
      cc[3] = xsecValue[i] - xsecPDFErr_minus[i];

    }else if(lhapdf[i] == "NNPDF"){
      cc[4] = xsecValue[i] + xsecPDFErr[i];
      cc[5] = xsecValue[i] - xsecPDFErr[i];
 
    }//end if

    BR = branching_ratio[i];
    npr = nproc[i];
    cut34 = min34[i];
    cut56 = min56[i];
    process = namestring[i];
    dyn_sca = dynscale[i];

  }//end for


  for(int k=0; k<6; k++){
  
    if(cc[k] > temp2)
      temp2 = cc[k];
    max_cc = temp2;

    if(cc[k] < temp3)
      temp3 = cc[k];
    min_cc = temp3;

  }//end for

  ave_cc = 0.5 * (min_cc + max_cc);
  cross_section = ave_cc / BR / 1000;
  max_pdf_unc = max_cc - ave_cc;
  min_pdf_unc = ave_cc - min_cc;
  abs_sca_unc = ave_cc * fac_sca_unc;

  string processL;
  if(npr == 61)
    processL = "$W^{+}W^{-}$";
  else if(npr == 71)
    processL = "$W^{+}Z^{0}$";
  else if(npr == 76)
    processL = "$W^{-}Z^{0}$";
  else if(npr == 81)
    processL = "$Z^{0}/\\gamma^{*}Z^{0}/\\gamma^{*}$";
  else if(npr == 82)
    processL = "$Z^{0}/\\gamma^{*}Z^{0}$";
  else if(npr == 86)
    processL = "$Z^{0}Z^{0}$";
  else if(npr == 141)
    processL = "$t\\bar{t}$";

  string cut34s, cut56s;
  cut34s = ConvertToString(cut34);
  cut56s = ConvertToString(cut56);

  if(npr == 141){
    cut34s = "---";
    cut56s = "---";

  }

  string dyn_scaL;
  if(dyn_sca == "no")
    dyn_scaL = "no";
  else if(dyn_sca == "HT")
    dyn_scaL = "$\\mu_1$";
  else if(dyn_sca == "sq")
    dyn_scaL = "$\\mu_2$"; 


  fout1 << processL      << " & "
	<< cut34s         << " & "
	<< cut56s        << " & "
	<< dyn_scaL      << " & "
	<< setprecision(1) << setiosflags(ios::fixed)
	<< ave_cc        << " & "
	<< cross_section << " & "
	<< (min_pdf_unc)/BR/1000 << " & "
	<< (abs_sca_unc)/BR/1000 << " \\\\"
	<< endl;

  fout2 << ave_cc      << "\t\t"
	<< max_pdf_unc << "\t\t"
	<< min_pdf_unc << "\t\t"
	<< abs_sca_unc 
	<< endl;

  fout1.close();
  fout2.close();

}//end void

template <class T> 
string ConvertToString(T value){
  stringstream ss;
  ss << value;
  return ss.str();
}
