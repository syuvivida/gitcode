#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: $0 [macro name without .C]"
    exit 1
fi

##---------------- Run the analyzer macro ----------------##

# check!
SIGNAL_DIR=$PWD/../../delpanjTuple_signal/qqll
BACKGR_DIR=$PWD/../../delpanjTuple_background

# check!
NTUPLES=`ls $SIGNAL_DIR`
#NTUPLES=`ls $BACKGR_DIR`

for FILE_1 in $NTUPLES
do

    echo -e "Processing root file *** $FILE_1 ***\n"
    # check!
    root -q -b -l $1.C++\(\"$SIGNAL_DIR/$FILE_1\"\,\"$FILE_1\"\)
    echo -e "Done!\n"

done

#FOLDER=$1

#echo "Check if the folder exist..."
#if [ -d $FOLDER ] 
#then
#    echo "The folder exist!"
#else
#    mkdir $FOLDER
#fi

#mv -f *.root $FOLDER

##---------------- Run the output_histo macro ----------------##

#DATA_DIR=$PWD/$FOLDER

#cd $DATA_DIR

#ROOT=`ls $DATA_DIR`

#for FILE_2 in $ROOT
#do

#    NEWDIR=`echo ${FILE_2:0:-5}`
#    echo -e "Processing root file *** $FILE_2 ***\n"
#    root -q -b -l ../../HEADER/output_histo.C++\(\"$DATA_DIR/$FILE_2\"\,\"$NEWDIR\"\)
#    #mv -f $NEWDIR $DATA_DIR
#    echo -e "Done!\n"

#done

#echo -e "Remove unnecessary files..."
rm -f *.d *.so
echo -e "All jobs done!\n"

exit
