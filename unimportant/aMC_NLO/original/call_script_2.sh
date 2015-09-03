#!/bin/bash

echo "================================="
echo "Loop: " $count
echo "Process: " $NEW_PROCESS
echo "================================="

#**********************************************#
# Generating cards and collecting cards
#**********************************************#
echo "Generating param_card.dat ..."
./param_card_template.sh ${NEW_PREFIX} $i ${NEW_DECAY_WIDTH}

echo "Generating proc_card_mg5.dat ..."
./proc_card_mg5_template.sh "${NEW_PROCESS}" ${NEW_PREFIX}

echo "Moving all cards into the folder ..."
mv *.dat ${CARD_DIR}

#**********************************************#
# Run the main script
#**********************************************#
echo "Running the script of generation_NLO"
./generation_NLO.sh ${NEW_PREFIX}

cd $WORKING_AREA

#**********************************************#
# Find and grep the data, output the data
#**********************************************#
echo "Moving and unzip the results tarball ..."
cp ${WORKING_AREA}/${NEW_PREFIX}/${NEW_PREFIX}_local/work/MG5_aMC_v2_1_1/${NEW_PREFIX}/Events/pilotrun/events.lhe.gz $WORKING_AREA
gzip -df events.lhe.gz

echo "Grepping the needed data and write them into a file ..."
CROSS_SECTION=`grep -A2 "<init>" "events.lhe" | tail -1 | awk '{print $1}'`

cat >> "Results.txt" <<EOF
Loop counter: ${count}
Process: ${NEW_PROCESS}
Z mass: $i GeV
Cross section: ${CROSS_SECTION} 

EOF

exit 0
