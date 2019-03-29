#!/bin/bash 

#parse input, they are written in args.txt

GATECOMMAND=$1
FOLDER=$2
TIME=$3
CUT=$4
SEED=$5
OUTPUTFILE=$6

set --

source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/setup.sh
source /cvmfs/geant4.cern.ch/geant4/10.3/x86_64-slc6-gcc49-opt/bin/geant4.sh

$GATECOMMAND -a [folder,${FOLDER}][time,${TIME}][cut,${CUT}][seed,${SEED}][outname,${OUTPUTFILE}] ${FOLDER}/runSim.mac

#echo -e "Copying result to: $OUTPUTFILE";
#cp s.root ${OUTPUTFILE};
