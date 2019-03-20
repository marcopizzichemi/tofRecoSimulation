# FibresCalo
Geant4 simulation of a fibres "SPACAL" calorimeter


## Instructions for first time setup

### Instructions for installation on a personal PC

To donwnload and compile the package (assuming you have a local working release of Geant4 and CLHEP installed), execute the following commands:
```
git clone https://github.com/marcopizzichemi/spacal.git
cd spacal/build
cmake ../
make -j
cd -
```

### Instructions for installation on lxplus (updated after decommissioning of LSF)

To donwnload and compile the package on lxplus, these commands need to be run at each login:
```
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
source /cvmfs/geant4.cern.ch/geant4/10.2.p03/x86_64-slc6-gcc49-opt/bin/geant4.sh
source /cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/setup.sh
```
The installation and compilation of the package then follows similarly:
```
git clone https://github.com/marcopizzichemi/spacal.git
cd spacal/build
cmake --verbose -DCMAKE_CXX_COMPILER=/cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/bin/g++ -DCMAKE_C_COMPILER=/cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/bin/gcc ../
make
cd -
```
N.B. The source commands reported above work, but they are not the cleanest procedure with the new EOS/Condor platform. One of the LCG environments prepared by CERN should be used. See http://lcginfo.cern.ch/ for more details. We should investigate and change the recipe accordingly.

## Instructions to run the code

To run the code in interactive mode (i.e. with visualization), simply execute
```
./build/FibresCalo template.cfg
```
This mode reads the visualization and beam parameters from file `vis.mac`

To produce multiple events and save the output in a root file, execute
```
./build/FibresCalo template.cfg outFileName
```
In this case, the beam parameters are read from file `gps.mac`

The file `template.cfg` contains a number of parameters to configure the calorimeter layout.
Most of them should be self-explanatory.


## For fix X11 problems  with macOS execute
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
