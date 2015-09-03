#!/bin/bash

rm -f checkLog.txt

iteration=0
lastfile=`find ./check_13TeV_MC_production -type f | wc -l`

while [ $iteration -lt $lastfile ];
do
    iteration=$(( iteration + 1 ))
    echo " ::: Status: "$iteration " of " $lastfile " :::"
    file=`find ./check_13TeV_MC_production -type f | head -n $iteration $1 | tail -1`
    mass=`echo ${file//[!0-9]/} | cut -c4-`
    root -q -b -l checkPlot.C++\(\"$file\"\,$mass\) >> checkLog.txt
done

echo " ::: All jobs are done! The output is saved in checkLog.txt :::"

exit
