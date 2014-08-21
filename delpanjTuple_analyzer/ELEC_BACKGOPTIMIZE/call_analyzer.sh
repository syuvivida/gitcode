#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

SIGNAL_DIR=$PWD/../../delpanjTuple_signal/qqll
BACKGR_DIR=$PWD/../../delpanjTuple_background

SIG_NTUPLES=`ls $SIGNAL_DIR`
BKG_NTUPLES=`ls $BACKGR_DIR`

for FILE_1 in $BKG_NTUPLES
do
    echo -e "Processing root file *** $FILE_1 ***\n"
    root -q -b -l $1.C++\(\"$BACKGR_DIR/$FILE_1\"\,\"$FILE_1\"\,1000\)
    echo -e "Done!\n"
done


for FILE_1 in $BKG_NTUPLES
do
    echo -e "Processing root file *** $FILE_1 ***\n"
    root -q -b -l $1.C++\(\"$BACKGR_DIR/$FILE_1\"\,\"$FILE_1\"\,1500\)
    echo -e "Done!\n"
done

for FILE_1 in $BKG_NTUPLES
do
    echo -e "Processing root file *** $FILE_1 ***\n"
    root -q -b -l $1.C++\(\"$BACKGR_DIR/$FILE_1\"\,\"$FILE_1\"\,2000\)
    echo -e "Done!\n"
done

for FILE_1 in $SIG_NTUPLES
do
    echo -e "Processing root file *** $FILE_1 ***\n"
    root -q -b -l $1.C++\(\"$SIGNAL_DIR/$FILE_1\"\,\"$FILE_1\"\,0\)
    echo -e "Done!\n"
done

rm -f *.d *.so
echo -e "All jobs done!\n"

exit
