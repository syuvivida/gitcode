#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

SIGNAL_DIR=$PWD/../../delpanjTuple_signal/lljj

ls -l $SIGNAL_DIR | tail -n+2 | awk '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$SIGNAL_DIR"'/"$9"\\\",\\\""$9"\\\"\\); echo -e \"\nDone!\n\""}' | bash

rm -f *.d *.so
echo -e "All jobs done!\n"

exit
