#!/bin/bash

if [ -z "$8" ];  then
    echo "Usage: $0 [PREFIX] [PROCESS] [MZP_INI] [MZP_FIN] [MZP_INT] [DECAY_WIDTH] [NEW_PREFIX] [NEW_PROCESS]"
    exit 1
fi

PREFIX=$1
PROCESS=$2
MZP_INI=$3
MZP_FIN=$4
MZP_INT=$5
DECAY_WIDTH=$6
NEW_PREFIX=$7
NEW_PROCESS=$8
CARD_DIR=cards
TARBALL=MG5_aMC_v2.1.1.tar.gz
WORKING_AREA=$PWD

chmod 755 *.sh

#**********************************************#
# Check if the files exist
#**********************************************#
echo "Check if the files exist ..."
if [ -e $CARD_DIR ]; then
    echo "The folder is exist!"
else
    echo "Create a folder ..."
    mkdir $CARD_DIR
fi

count=1

for (( i=${MZP_INI}; i<=${MZP_FIN}; i=i+${MZP_INT} ))
do

echo "==========================================="
echo "Processing loop: " $count
echo "==========================================="

#**********************************************#
# Generating cards and collecting cards
#**********************************************#
echo "Generating param_card.dat ..."
./param_card_template.sh ${PREFIX} $i ${DECAY_WIDTH}

echo "Generating proc_card_mg5.dat ..."
./proc_card_mg5_template.sh "${PROCESS}" ${PREFIX}

echo "Moving all cards into the folder ..."
mv *.dat ${CARD_DIR}

#**********************************************#
# Check the tarball and run the main script
#**********************************************#
echo "Check if the tarball of MG5_aMC_v2.1.1 exist ..."
if [ -f $TARBALL ]; then

    echo "The tarball is exist!"
    echo "Running the script of generation_LO"
    ./generation_LO.sh ${PREFIX} 

else

    echo "The tarball is not exist!"
    exit 1

fi

cd $WORKING_AREA

#**********************************************#
# Find and grep the data
#**********************************************#
echo "Moving and unzip the results tarball ..."
cp ${WORKING_AREA}/${PREFIX}/${PREFIX}_local/work/MG5_aMC_v2_1_1/${PREFIX}/Events/pilotrun/events.lhe.gz $WORKING_AREA
gzip -df events.lhe.gz

echo "Grepping the needed data and write them into a file ..."
NEW_DECAY_WIDTH=`grep -A2 "<init>" "events.lhe" | tail -1 | awk '{print $1}'`

#**********************************************#
# Export variables for call_script_2.sh
#**********************************************#
export CARD_DIR
export TARBALL
export WORKING_AREA
export NEW_PREFIX
export NEW_PROCESS
export NEW_DECAY_WIDTH
export i
export count

./call_script_2.sh

#loop counter:
(( count++ ))

done

echo "All job is done ..."

exit 0
