#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

SIGNAL_DIR=$PWD/../../delpanjTuple_signal/lljj

ls -l $SIGNAL_DIR/delpanj_v3_AbelianZPrime_ZH_lljj_M*root | tail -n+2 | awk -F "$SIGNAL_DIR/delpanj_v3_AbelianZPrime_ZH_lljj_M" '{print $2}' | awk -F ".root" '{ print "echo -e \"\n\nProcessing root file --> "$9"\"; root -q -b -l '"$1"'.C++\\(\\\"'"$SIGNAL_DIR"'/delpanj_v3_AbelianZPrime_ZH_lljj_M"$1".root\\\",\\\""$1"\\\"\\); echo -e \"\nDone!\n\""}' | bash

mv massEleptEff.txt massEleptEff_old.txt
sort -g massEleptEff_old.txt > massEleptEff.txt
rm -f massEleptEff_old.txt

rm -f *.d *.so
echo -e "All jobs done!\n"

exit
