#!/bin/bash

python MakeDataCards.py nEventZH.txt ./dataCards

rm -f DataCard_MXXXGeV.txt

cd /afs/cern.ch/work/h/htong/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/src

massPoints=(800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000)

for ((i=0; i<${#massPoints[@]}; i++)); do

    dataCard=DataCard_M${massPoints[$i]}GeV_MonoHbb_13TeV.txt
    rootFile=higgsCombineCounting.Asymptotic.mZH${massPoints[$i]}.root

    echo -e "\n****** Using data card: " $dataCard " ******\n"

    combine -M Asymptotic /afs/cern.ch/work/h/htong/xtozh_13TeV/limitSetting/dataCards/$dataCard
    mv higgsCombineTest.Asymptotic.mH120.root /afs/cern.ch/work/h/htong/xtozh_13TeV/limitSetting/$rootFile

done

cd /afs/cern.ch/work/h/htong/xtozh_13TeV/limitSetting/
root -q -b -l plot_Asymptotic.C++\(\"Counting\"\)
mv *pdf /afs/cern.ch/user/h/htong/www
rm -f *.d *.so

exit