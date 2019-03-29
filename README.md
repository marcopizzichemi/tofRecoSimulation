# tofRecoSimulation
GATE simulation of full ring for hybrid detector, plus some script to analyze data


## Instructions for first time setup

### Instructions for installation on a personal PC

You need to have GATE up and running, which means also ROOT and Geant4.
To download (and compile the small script) execute the following commands:

```
git clone https://github.com/marcopizzichemi/tofRecoSimulation.git
cd tofRecoSimulation
./deploy
```
This will create two executables in bin/ folder

### Instructions for installation on lxplus (updated after decommissioning of LSF)

Again, you need a GATE version up and running. One combination that was found to work on lxplus is

```
GATE 8.1
Geant 4.10.03
ROOT 6.06.08
g++ 4.9.1
```
This is an extremely delicate combination...

To setup everything and compile GATE from source, do

```
wget http://www.opengatecollaboration.org/sites/default/files/gate_v8.1.p01.tar.gz
tar xvzf gate_v8.1.p01.tar.gz
cd gate_v8.1.p01/
mkdir build
cd build/
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/setup.sh
source /cvmfs/geant4.cern.ch/geant4/10.3/x86_64-slc6-gcc49-opt/bin/geant4.sh
/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.8.1/Linux-x86_64/bin/cmake --verbose -DCMAKE_CXX_COMPILER=/cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/bin/g++ -DCMAKE_C_COMPILER=/cvmfs/sft.cern.ch/lcg/external/gcc/4.9.1/x86_64-slc6/bin/gcc ../
make

```

Then download and compile the simulation:
```
git clone https://github.com/marcopizzichemi/tofRecoSimulation.git
cd tofRecoSimulation
./deploy lxplus
```
Just specify lxplus to deploy, so the proper source commands are executed

## Run

### Run simulation

To run a single instance of simulation, once GATE is installed, do

```
time Gate -a [folder,/path/to/Gate/macros/folder][time,3][cut,10][seed,123456789][outname,output] /path/to/Gate/macros/folder/runSim.mac
```
where
```
/path/to/Gate/macros/folder  -> path to the folder where runSim.mac is
time                         -> simulation time (setTimeStop in GATE), in seconds
cut                          -> production cut for gammas, electrons and positrons, in microns
seed                         -> seed for the random number generator

```

The simulation CPU time depends heavily on the combination of time and cut. For a typical (time = 3, cut = 100), CPU time is around 30 minutes. The production cut anyway has to be smaller than the relevant dimensions of the hybrid system, otherwise the results will be uncorrect.

The parameters in input allow to change the GATE simulation without changing the macro (particularly useful for submitting many jobs to condor, see later). The output of the simulation will have a name sim{cut}um.root.

### Run simple analysis after simulation

To count how many fast, less fast and slow coincidences are produced, do

```
./path/to/bin/fastPercentage --folder ./ --input output.root --output results.txt
```
This will work on GATE Hits, so only real coincidences are taken into account. Results are written on terminal and in results.txt file.

### Only visualization

To run just a visual interface, do

```
Gate --qt -a [folder,/path/to/Gate/macros/folder][time,3][cut,10][seed,123456789][outname,output] /path/to/Gate/macros/folder/runVis.mac
```

in this case, time, cut, seed and outname do not really matter.


### Condor

The scripts needed for submitting in condor on lxplus are in the condor/ folder. You need to
```
Create a folder somewhere
copy the jobs.sub and run_script.su file there
Create 3 subdirs, err, log, out
properly set the variables in args.txt.
```

Each line in args.txt is a condor job. You need to specify in each line

```
the GATE executable file, with full path
the GATE macros folder
the simulation time (GATE time) in seconds
the production cut, in microns
the random seed
the final output file, i.e. the full path + file name where the output file will be written
```

Of course you need to change seed in every line, in general, and also the final output name, otherwise it will be overwritten. After this, you just need to run

```
condor_submit jobs.sub
```
