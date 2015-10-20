#!/bin/sh

root -q -b -l dataMCplots.C++\(\"dataMC_Ele/outputEle/barrel\"\,\"barrelEleVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Ele/outputEle/endcap\"\,\"endcapEleVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Zee/outputZee\"\,\"ZeeVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Jetee/outputJetee\"\,\"JeteeVariables\"\)

rm -f *.d *.so
mv *pdf /afs/cern.ch/user/h/htong/www

exit