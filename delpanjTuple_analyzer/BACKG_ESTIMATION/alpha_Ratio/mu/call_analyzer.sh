#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

DATAAA_DIR=$PWD/../../../../delpanjTuple_data/totalmuon
BACKGR_DIR=$PWD/../../../../delpanjTuple_background/DY

rm -i *root

root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,-1\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,0\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,1\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,-1\,1\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,0\,1\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-70To100.root\"\,2\,1\,1\)

root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,-1\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,0\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,1\,0\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,-1\,1\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,0\,1\)
root -q -b -l "$1".C++\(\"$BACKGR_DIR/delpanj_v4_DYJetsToLL_PtZ-100.root\"\,2\,1\,1\)

root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,-1\,0\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,0\,0\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,1\,0\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,-1\,1\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,0\,1\)
root -q -b -l "$1".C++\(\"$DATAAA_DIR/delpanj_v4_data_DoubleMu.root\"\,2\,1\,1\)

#root -q -b -l compareAlpMu.C++\(\)

echo -e "All jobs done!\n"

exit
