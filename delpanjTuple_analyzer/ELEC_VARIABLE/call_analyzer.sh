#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

DATAAA_DIR=$PWD/../../delpanjTuple_data
BACKGR_DIR=$PWD/../../delpanjTuple_background

ls -l $BACKGR_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$BACKGR_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

echo -e "\n\nProcessing root file --> delpanj_v3_data_DoubleEl_A.root"
root -q -b -l $1.C++\(\"$DATAAA_DIR/delpanj_v3_data_DoubleEl_A.root\"\,\"delpanj_v3_data_DoubleEl_A.root\"\)
echo -e "\nDone!\n"

rm -f *.d *.so
echo -e "All jobs done!\n"

exit
