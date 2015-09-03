#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

DATAAA_DIR=$PWD/../../delpanjTuple_data/totalmuon
BACKG1_DIR=$PWD/../../delpanjTuple_background/DY
BACKG2_DIR=$PWD/../../delpanjTuple_background/minorBkg

rm -i *root

ls -l $BACKG1_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$BACKG1_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

ls -l $BACKG2_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$BACKG2_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

ls -l $DATAAA_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$DATAAA_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

root -q -b -l stcMuVariable.C++\(\)

rm -f *.d *.so
echo -e "All jobs done!\n"

exit
