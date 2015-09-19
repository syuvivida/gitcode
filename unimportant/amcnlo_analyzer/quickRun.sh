#!/bin/bash

for i in flattp*root
do
    root -q -b -l amcNLOanalyzer.C++\(\"$i\"\)
done

root -q -b -l drawFlattp.C++\(\)

exit
