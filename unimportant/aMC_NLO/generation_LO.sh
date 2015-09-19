#!/bin/bash

##########################################################################################
#GENERAL INSTRUCTIONS:                                                                   #
#You should take care of having the following ingredients in order to have this recipe   #
#working: run card and proc card (in a "cards" folder), MadGraph release, this script    #
#all in the same folder!                                                                 #
#Important: Param card is not mandatory for this script                                  #
##########################################################################################

##########################################################################################
#DISCLAIMER:                                                                             #
#This script has been tested in CMSSW_6_2_0_patch1 and there is no guarantee it should   #
#work in another releases.                                                               #
#To try in another releases you should adapt it taking into account to put the correct   #
#parameters as the architechture, the release names and compatibility issues as decribed #
#in the comments in this script                                                          #
#Additionally, this script depends on the well behaviour of the lxplus machines, so if   #
#one job fails the full set of jobs will fail and then you have to try again             #
#Any issues should be addressed to: cms-madgraph-support-team_at_cernSPAMNOT.ch          #
##########################################################################################

##########################################################################################
#For runnning, the following command should be used                                      #
#bash create_gridpack_template.sh NAME_OF_PRODCUTION QUEUE_SELECTION                     #
#Or you can make this script as an executable and the launch it with                     #
#chmod +x create_gridpack_template.sh                                                    #
#./create_gridpack_template.sh NAME_OF_PRODCUTION QUEUE_SELECTION                        #
#by NAME_OF_PRODUCTION you should use the names of run and proc card                     #
#for example if the cards are bb_100_250_proc_card_mg5.dat and bb_100_250_run_card.dat   #
#NAME_OF_PRODUCTION should be bb_100_250                                                 #
#for QUEUE_SELECTION is commonly used 1nd, but you can take another choice from bqueues  #
##########################################################################################

#set -o verbose

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running

#First you need to set couple of settings:
# name of the run
name=${1}

#________________________________________
# to be set for user spesific
# Release to be used to define the environment and the compiler needed

#For correct running you should place at least the run and proc card in a folder 
#under the name "cards" in the same folder where you are going to run the script

export PRODHOME=`pwd`          # the folder where the script works, I guess
AFS_GEN_FOLDER=${PRODHOME}/${name}  # where to search for datacards, that have to follow a naming code: 
                                    #   ${name}_proc_card_mg5.dat
                                    #   ${name}_run_card.dat
CARDSDIR=${PRODHOME}/cards     # where to find the madgraph tarred distribution
MGDIR=${PRODHOME}/             # madgraph distribution, that has been downloaded
MG=MG5_aMC_v2.1.1.tar.gz
MGSOURCE=${PRODHOME}/${MG}
MGBASEDIR=MG5_aMC_v2_1_1

if [ ! -d ${AFS_GEN_FOLDER} ];then
    mkdir ${AFS_GEN_FOLDER}
fi
cd $AFS_GEN_FOLDER

########################
#Locating the proc card#
########################
if [ ! -e $CARDSDIR/${name}_proc_card_mg5.dat ]; then
    echo $CARDSDIR/${name}_proc_card_mg5.dat " does not exist!"
    exit 1;
else
    cp $CARDSDIR/${name}_proc_card_mg5.dat ${name}_proc_card_mg5.dat
fi

#######################
#Locating the run card#
#######################
if [ ! -e $CARDSDIR/${name}_run_card.dat ]; then
    echo $CARDSDIR/${name}_run_card.dat " does not exist!"
    #exit 1;
else
    cp $CARDSDIR/${name}_run_card.dat     ${name}_run_card.dat
fi	

#########################
#Locating the param card#
#########################
if [ ! -e $CARDSDIR/${name}_param_card.dat ]; then
    echo $CARDSDIR/${name}_param_card.dat " does not exist!"
    #exit 1;
else
    cp $CARDSDIR/${name}_param_card.dat   ${name}_param_card.dat
fi	

############################
#Create a workplace to work#
############################
mkdir ${name}_local; cd ${name}_local ; mkdir -p work ; cd work
WORKDIR=`pwd`

# force the f77 compiler to be the CMSSW defined ones
ln -s `which gfortran` f77
ln -s `which gfortran` g77
export PATH=`pwd`:${PATH}

#############################################
#Copy, Unzip and Delete the MadGraph tarball#
#############################################
#Here you have to put the correct name of the folder will be created when the MG tarball is untared, 
#in this specific case "MG5v1.4.8" but if you use another release this may be different

cp ${MGSOURCE} .; tar xzf ${MG}; cd $MGBASEDIR

#disable non-batch-suitable behaviour
echo "auto_update = 0" >> ./input/mg5_configuration.txt
echo "automatic_html_opening = False" >> ./input/mg5_configuration.txt

#set lhapdf path correctly
echo "lhapdf = ${LHAPATH}/../../../bin/lhapdf-config" >> ./input/mg5_configuration.txt
echo "run_mode = 2" >> ./input/mg5_configuration.txt
echo `pwd`
cp ${AFS_GEN_FOLDER}/${name}_run_card.dat Template/LO/Cards/run_card.dat

#________________________________________
# set the run cards with the appropriate initial seed

echo "running process card"
./bin/mg5_aMC ${AFS_GEN_FOLDER}/${name}_proc_card_mg5.dat
cd $name

###################################################################################
#Run the production stage - here you can select for running on multicore or not...#
###################################################################################
export PATH=`pwd`/bin:${PATH}

cp ${AFS_GEN_FOLDER}/${name}_param_card.dat Cards/param_card.dat
rm Cards/shower_card.dat

echo "==============================================================================="
cat Cards/proc_card_mg5.dat

echo "==============================================================================="
cat Cards/run_card.dat

echo "==============================================================================="
cat Cards/param_card.dat

echo "==============================================================================="
pwd
echo "==============================================================================="

./bin/generate_events -f pilotrun

ls -ltrh

#set to single core mode
if [ -e ./Cards/amcatnlo_configuration.txt ]; then
    echo "mg5_path = ../" >> ./Cards/amcatnlo_configuration.txt
    echo "run_mode = 0" >> ./Cards/amcatnlo_configuration.txt
fi

echo "End of job"

exit 0
