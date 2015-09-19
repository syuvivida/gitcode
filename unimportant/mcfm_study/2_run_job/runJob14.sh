#!/bin/bash

cd /afs/cern.ch/work/h/htong/MCFM-6.6/Bin
source /afs/cern.ch/work/h/htong/MCFM-6.6/Bin/setup_LHAPDF
./mcfm 100K_40E_14TeV/JOB $1
