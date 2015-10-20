#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V320151011_4WithTaus/151012_141630/0000/\"\,\"SingleMuon-Run2015D-V320151011_4WithTaus\"\)             
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V420151011_4WithTaus/151012_141725/0000/\"\,\"SingleMuon-Run2015D-V420151011_4WithTaus\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-100to200_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-200to400_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-400to600_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-600toInf_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/151007_220812/\"\,\"TT_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WW_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/151007_220812/0000/\"\,\"WW_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/151007_221020/0000/\"\,\"WZ_TuneCUETP8M1_13TeV\"\)
root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_ZZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CSW7412_20151006/0000/\"\,\"ZZ_TuneCUETP8M1_13TeV\"\)

rm -f *.d *.so
mv *root output*

exit