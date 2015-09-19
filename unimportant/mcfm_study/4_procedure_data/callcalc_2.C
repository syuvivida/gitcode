#include <string>
#include <iostream>
#include <ctime>
#include "calculation_2.C"
using namespace std;

void callcalc_2(){

  const int num = 11;
  int processId[num]={61,71,71,76,76,81,81,82,86,141,141};
  int massId[num]={0,12,40,12,40,12,40,0,1,0,0};
  int dynamId[num]={0,0,0,0,0,0,0,0,0,1,2};

  for(int ip=0; ip<num; ip++){
    calculation_2(Form("results_p%d_c%d_%d.txt",
		 processId[ip], massId[ip], dynamId[ip]));
  }
}
