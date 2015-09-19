#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void separate_1(string filename){
  
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
  
  ifstream fin;
  fin.open(filename.data());
  
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
    }
  }
  
  fin.close();
  
  int count = 1;
  for(int i=0; i<lhapdf.size(); i++){     
    if(lhapdf[i+1] == lhapdf[i])
      count++;
    else
      break;
  }
  cout << "count = " << count << endl;

  ofstream fout;
  const int num = count;
   
  int processId[num]={61,71,71,76,76,81,81,82,86,141,141};
  int massId[num]={0,12,40,12,40,12,40,0,1,0,0};
  int dynamId[num]={0,0,0,0,0,0,0,0,0,1,2}; //0=no,1=HT,2=sq

  //Branching ratio of particles from PDG
  double W_e_nu  = 0.10750;
  double W_mu_nu = 0.10570;
  double Z_e_e   = 0.03363;
  double Z_mu_mu = 0.03366;
  double Z_nu_nu = 0.20000;
  double t_W_b   = 0.09400;

  const int size = lhapdf.size();
  int dynamics[size];

  for(int i=0; i<size; i++){

    if(dynscale[i] == "no")
      dynamics[i]=0;
    else if(dynscale[i] == "HT")
      dynamics[i]=1;
    else if(dynscale[i] == "sq")
      dynamics[i]=2;


    for(int ip=0; ip<num; ip++){
      if(nproc[i] == processId[ip] && min56[i] == massId[ip]
	 && dynamics[i] == dynamId[ip]){

	fout.open(Form("results_p%d_c%d_%d.txt",
		       processId[ip], massId[ip], dynamId[ip]), 
		  ios::out || ios::app);

	fout << lhapdf[i] << "\t";
	fout << nproc[i] << "\t";
	fout << scale[i] << "\t";
	fout << facscale[i] << "\t";
	fout << dynscale[i] << "\t";
	fout << min34[i] << "\t";
	fout << min56[i] << "\t";
	fout << namestring[i] << "\t";
	fout << xsecValue[i] << "\t\t";
	fout << xsecPDFErr_plus[i] << "\t\t";
	fout << xsecPDFErr_minus[i] << "\t\t";
	fout << xsecPDFErr[i] << "\t\t"; 

	if(processId[ip] == 61)
	  fout << W_e_nu * W_e_nu << "\n";

	else if(processId[ip] == 71 || processId[ip] == 76)
	  fout << W_mu_nu * Z_e_e << "\n";

	else if(processId[ip] == 81 || processId[ip] == 86)
	  fout << Z_e_e * Z_mu_mu * 2 << "\n";

	else if(processId[ip] == 82)
	  fout << Z_e_e * Z_nu_nu * 2 << "\n";

	else if(processId[ip] == 141)
	  fout << t_W_b * t_W_b << "\n";

	fout.close();

      } //end of if statement
    } // end of ip loop
  } // end of i loop
}//end of void
