#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

BACKGR_DIR=$PWD/../../../delpanjTuple_background/minorBkg

rm -i *root

ls -l $BACKGR_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$BACKGR_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

root -q -b -l drawBkgZpMass.C++\(\)

rm -f *d *so
echo -e "All jobs done!\n"

exit
