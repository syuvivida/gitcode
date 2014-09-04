#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

DATAAA_DIR=$PWD/../../delpanjTuple_data
BACKGR_DIR=$PWD/../../delpanjTuple_background
SIGNAL_DIR=$PWD/../../delpanjTuple_signal/qqll

echo -e "Processing root file *** delpanj_v2_DYJetsToLL_PtZ-70To100.root ***\n"
root -q -b -l $1.C++\(\"$BACKGR_DIR/delpanj_v2_DYJetsToLL_PtZ-70To100.root\"\,\"delpanj_v2_DYJetsToLL_PtZ-70To100.root\"\)
echo -e "Done!\n"

echo -e "Processing root file *** delpanj_v2_DYJetsToLL_PtZ-100.root ***\n"
root -q -b -l $1.C++\(\"$BACKGR_DIR/delpanj_v2_DYJetsToLL_PtZ-100.root\"\,\"delpanj_v2_DYJetsToLL_PtZ-100.root\"\)
echo -e "Done!\n"

echo -e "Processing root file *** delpanj_v2_data_DoubleMu_A.root ***\n"
root -q -b -l $1.C++\(\"$DATAAA_DIR/delpanj_v2_data_DoubleMu_A.root\"\,\"delpanj_v2_data_DoubleMu_A.root\"\)
echo -e "Done!\n"

echo -e "Processing root file *** delpanj_v2_data_DoubleMu_A.root ***\n"
root -q -b -l $1.C++\(\"$SIGNAL_DIR/delpanj_v2_ZPrime_hZ_qqll_LHC8_M1000.root\"\,\"delpanj_v2_ZPrime_hZ_qqll_LHC8_M1000.root\"\)
echo -e "Done!\n"


rm -f *.d *.so
echo -e "All jobs done!\n"

exit
