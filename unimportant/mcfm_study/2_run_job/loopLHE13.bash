#!/bin/bash

iteration=0
lastfile=20

while [ $iteration -lt $lastfile ]; 
do
  iteration=$(( iteration + 1 ))
  echo $iteration
  filename=(`head -n $iteration data_inputList | tail -1`)
  echo $filename
  bsub -q 2nw $PWD/runJob13.sh $filename
done
