#!/bin/sh

if [ -z $1 ]; then
    echo "Usage: $0 [1: electron 2: muon]"
    exit 0
fi

if [ $1 -eq 1 ]; then
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Ele/outputEle/barrel\"\,\"barrelEleVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Ele/outputEle/endcap\"\,\"endcapEleVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Zee/outputZee\"\,\"ZeeVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Jetee/outputJetee\"\,\"JeteeVariables\"\)
    root -q -b -l mZHlepplots.C++\(\"electron/forLimitCalc/outputmZHele\"\,\"mZHeleVariables\"\)
elif [ $1 -eq 2 ]; then
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Mu/outputMu/highpt\"\,\"highPtMuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Mu/outputMu/tracker\"\,\"trackerMuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Zmumu/outputZmumu\"\,\"ZmumuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Jetmumu/outputJetmumu\"\,\"JetmumuVariables\"\)
    root -q -b -l mZHlepplots.C++\(\"muon/forLimitCalc/outputmZHmu\"\,\"mZHmuVariables\"\)
else
    exit 1
fi

rm -f *.d *.so
mv *pdf /afs/cern.ch/user/h/htong/www

exit