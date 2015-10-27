#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 0
fi

if [ `echo $1 | grep -c "ele"` -gt 0 ]; then
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleElectron-Run2015D-PromptReco-V4_20151019/\"\,\"SingleElectron-Run2015D-V4_20151019\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleElectron-Run2015D-PromptReco-V3_20151019/\"\,\"SingleElectron-Run2015D-V3_20151019\"\)
elif [ `echo $1 | grep -c "mu"` -gt 0 ]; then
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V4_20151019/\"\,\"SingleMuon-Run2015D-V4_20151019\"\)             
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V3_20151019/\"\,\"SingleMuon-Run2015D-V3_20151019\"\)
fi

root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-100to200_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-200to400_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-400to600_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-600toInf_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"TT_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WW_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"WW_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"WZ_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_ZZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CSW7412_20151006/\"\,\"ZZ_TuneCUETP8M1_13TeV\"\)

rm -f inputdir.txt
rm -f *.d *.so
mv *root output*

exit