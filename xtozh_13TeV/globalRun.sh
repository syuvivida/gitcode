#!/bin/bash

if [ -z $6 ]; then

    echo "Usage: $0 [macro without .C] [bool data] [bool DY] [bool diboson] [bool ttbar] [bool signal]"
    exit 0

fi

data=$2
DY=$3
diboson=$4
ttbar=$5
signal=$6

if [ $data == true ]; then

    if [ `echo $1 | grep -c "ele"` -gt 0 ]; then

	root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleElectron-Run2015D-PromptReco-V4_20151019/\"\,\"SingleElectron-Run2015D-V4_20151019\"\)
	root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleElectron-Run2015D-PromptReco-V3_20151019/\"\,\"SingleElectron-Run2015D-V3_20151019\"\)

    elif [ `echo $1 | grep -c "mu"` -gt 0 ]; then

	root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V4_20151019/\"\,\"SingleMuon-Run2015D-V4_20151019\"\)             
	root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/Run2015D/crab_SingleMuon-Run2015D-PromptReco-V3_20151019/\"\,\"SingleMuon-Run2015D-V3_20151019\"\)

    fi

else

    data=false

fi

if [ $DY == true ]; then

    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-100to200_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-200to400_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-400to600_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/DYJetsLLHT/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/\"\,\"DYJetsToLL_M-50_HT-600toInf_13TeV\"\)

else

    DY=false

fi

if [ $diboson == true ]; then

    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WW_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"WW_TuneCUETP8M1_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_WZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"WZ_TuneCUETP8M1_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_ZZ_TuneCUETP8M1_13TeV-pythia8MC25ns_eleIDjet_CSW7412_20151006/\"\,\"ZZ_TuneCUETP8M1_13TeV\"\)

else

    diboson=false

fi

if [ $ttbar == true ]; then

    root -q -b -l $1.C++\(\"/data7/khurana/NCUGlobalTuples/SPRING15_2015_10_12/crab_TT_TuneCUETP8M1_13TeV-powheg-pythia8MC25ns_eleIDjet_CMSSW7412_20151006/\"\,\"TT_TuneCUETP8M1_13TeV\"\)

else

    ttbar=false

fi

if [ $signal == true ]; then

    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-600_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-800_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-1000_13TeV\"\) 
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-1200_13TeV\"\) 
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-1400_13TeV\"\)     
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-1600_13TeV\"\) 
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-1800_13TeV\"\)  
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-2000_13TeV\"\) 
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-2500_13TeV\"\)  
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-3000_13TeV\"\)  
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-3500_13TeV\"\)    
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-4000_13TeV\"\)
    root -q -b -l $1.C++\(\"/data7/syu/13TeV_signalMC/ZprimeToZhToZlephbb/ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph.root\"\,\"ZprimeToZhToZlephbb_M-4500_13TeV\"\)

else

    signal=false

fi

rm -f inputdir.txt
rm -f *.d *.so
mv *root output*

exit