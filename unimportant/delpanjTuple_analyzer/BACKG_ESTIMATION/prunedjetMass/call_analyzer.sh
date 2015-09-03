#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

DATAAA_DIR=$PWD/../../../delpanjTuple_data/totalmuon
BACKGR_DIR=$PWD/../../../delpanjTuple_background/minorBkg

#rm -i *root

root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_WW_pythia_filtered.root\"\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_WZ_pythia_filtered.root\"\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_ZZ_pythia_filtered.root\"\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\)

echo -e "All jobs done!\n"

exit
