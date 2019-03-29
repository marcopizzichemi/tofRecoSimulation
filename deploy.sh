#|/bin/bash
if [ -z "$1" ]
  then
    echo "No argument supplied, so compiling with machine compiler..."
    set --
    g++ -o ./bin/tofReco ./scripts/tofReco.cpp `root-config --cflags --libs`
    g++ -o ./bin/fastPercentage fastPercentage.cpp `root-config --cflags --libs`
else
  if [ $1 = "lxplus" ]; then
    set --
    echo "Sourcing compiler, ROOT and Geant4 for lxplus..."
    source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
    source /cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/setup.sh
    source /cvmfs/geant4.cern.ch/geant4/10.3/x86_64-slc6-gcc49-opt/bin/geant4.sh
    g++ -o ./bin/tofReco ./scripts/tofReco.cpp `root-config --cflags --libs`
    g++ -o ./bin/fastPercentage fastPercentage.cpp `root-config --cflags --libs`
  else
    echo "Invalid argument $1"
  fi
fi
