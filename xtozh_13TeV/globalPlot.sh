#!/bin/sh

if [ -z $1 ]; then
    echo "Usage: $0 [bool electron] [bool muon]"
    exit 0
fi

electron=$1
muon=$2

if [ $electron == true ]; then

    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Ele/outputEle/barrel\"\,\"barrelEleVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Ele/outputEle/endcap\"\,\"endcapEleVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Zee/outputZee\"\,\"ZeeVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/dataMC_Jetee/outputJetee\"\,\"JeteeVariables\"\)
    root -q -b -l dataMCplots.C++\(\"electron/forLimitCalc/outputmZHele\"\,\"mZHeleVariables\"\)

else

    electron=false

fi

if [ $muon == true ]; then

    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Mu/outputMu/highpt\"\,\"highPtMuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Mu/outputMu/tracker\"\,\"trackerMuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Zmumu/outputZmumu\"\,\"ZmumuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/dataMC_Jetmumu/outputJetmumu\"\,\"JetmumuVariables\"\)
    root -q -b -l dataMCplots.C++\(\"muon/forLimitCalc/outputmZHmu\"\,\"mZHmuVariables\"\)

else

    muon=false

fi

rm -f *.d *.so
mv *pdf /afs/cern.ch/user/h/htong/www

exit