#!/bin/sh

root -q -b -l dataMCplots.C++\(\"dataMC_Mu/outputMu/highpt\"\,\"highPtMuVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Mu/outputMu/tracker\"\,\"trackerMuVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Zmumu/outputZmumu\"\,\"ZmumuVariables\"\)
root -q -b -l dataMCplots.C++\(\"dataMC_Jetmumu/outputJetmumu\"\,\"JetmumuVariables\"\)

rm -f *.d *.so
mv *pdf /afs/cern.ch/user/h/htong/www

exit