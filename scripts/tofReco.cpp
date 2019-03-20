// small program to analyze the data coming from the GATE simulation and
// produce a ttree with summarized data. the output of the GATE simulation
// is analyzed running on the hits, they are grouped for events (i.e. back-to-back
// gamma emissions), then for each the total energy deposited in the scanner computed
// as well as the amount of energy in each ot the two materials

// compile with
// g++ -o ../bin/tofReco tofReco.cpp `root-config --cflags --libs`

// run as
// ./bin/tofReco input.root output.root
// or just
// ./bin/tofReco
// to get usage instructions

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>    // std::sort
#include <getopt.h>
#include "TROOT.h"
#include "TRandom1.h"
#include "TSystem.h"
#include "TChain.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TH1.h"
#include "TAxis.h"
#include "TDirectory.h"
#include "TList.h"
#include "Rtypes.h"
#include "TChainElement.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH2.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TRandom.h"

#define PI 3.141592653


struct enDep
{
  Float_t  localPosX;
  Float_t  localPosY;
  Float_t  localPosZ;
  Int_t    primaryID;
  Int_t    parentID;
  Int_t    trackID;
  Int_t    eventID;
  Int_t    gantryID;
  Int_t    rsectorID;
  Int_t    moduleID;
  Int_t    submoduleID;
  Int_t    crystalID;
  Int_t    layerID;
  Int_t    sourceID;
  Double_t time;
  Float_t  edep;
};

//function to compare deposition event struct vectors using the field time
// to be used by std::sort
bool compareByTime(const enDep &a,const enDep  &b)
{
  return a.time < b.time;
}

void usage()
{
  std::cout << "\t\t" << "[-i|--input] <input file>  [-o|--output] <output file> " << std::endl
  << "\t\t" << "[ <input file>      = name of input file, from GATE sim ] " << std::endl
  << "\t\t" << "[ <output file> ]   = name of output file where ttree will be stored " << std::endl
  << "\t\t" << std::endl;
}


int main(int argc, char** argv)
{

  // --------------------------------//
  // check if enough input           //
  //---------------------------------//
  if(argc < 2)
  {
    std::cout << argv[0] << std::endl;
    usage();
    return 1;
  }
  // --------------------------------//


  // --------------------------------//
  // Parse input args                //
  // --------------------------------//
  std::string inputName = "";
  std::string outputName = "";
  static struct option longOptions[] =
  {
			{ "input", required_argument, 0, 0 },
			{ "output", required_argument, 0, 0 },
			{ NULL, 0, 0, 0 }
	};
  while(1) {
		int optionIndex = 0;
		int c = getopt_long(argc, argv, "i:o:", longOptions, &optionIndex);

		if (c == -1) {
			break;
		}

		if (c == 'i'){
			inputName = (char *)optarg;
    }
		else if (c == 'o'){
      outputName = (char *)optarg;
    }
		else if (c == 0 && optionIndex == 0){
      inputName = (char *)optarg;
    }
    else if (c == 0 && optionIndex == 1){
      outputName = (char *)optarg;
    }
		else
    {
			std::cout	<< "Usage: " << argv[0] << std::endl;
      usage();
			return 1;
		}
	}
  // --------------------------------//

  // --------------------------------//
  // check                           //
  // --------------------------------//
  if(inputName.empty())
  {
    std::cout << "ERROR! You need to provide an input file name!" << std::endl;
    usage();
    return 1;
  }
  if(outputName.empty())
  {
    std::cout << "ERROR! You need to provide an output file name!" << std::endl;
    usage();
    return 1;
  }


  // --------------------------------//
  // Feedback to user                //
  // --------------------------------//
  std::cout << "Input file           = " << inputName << std::endl;
  std::cout << "Output file          = " << outputName << std::endl;
  TFile *inputFile = TFile::Open(inputName.c_str());
  // --------------------------------//


  // --------------------------------//
  // Read the Hits tree              //
  // --------------------------------//
  TTree *Hits;
  Hits = (TTree*) inputFile->Get("Hits");
  //variables
  Int_t    HITSPDGEncoding;
  Int_t    HITStrackID;
  Int_t    HITSparentID;
  Double_t HITStime;
  Float_t  HITSedep;
  Float_t  HITSstepLength;
  Float_t  HITSposX;
  Float_t  HITSposY;
  Float_t  HITSposZ;
  Float_t  HITSlocalPosX;
  Float_t  HITSlocalPosY;
  Float_t  HITSlocalPosZ;
  Int_t    HITSgantryID;
  Int_t    HITSrsectorID;
  Int_t    HITSmoduleID;
  Int_t    HITSsubmoduleID;
  Int_t    HITScrystalID;
  Int_t    HITSlayerID;
  Int_t    HITSphotonID;
  Int_t    HITSnPhantomCompton;
  Int_t    HITSnCrystalCompton;
  Int_t    HITSnPhantomRayleigh;
  Int_t    HITSnCrystalRayleigh;
  Int_t    HITSprimaryID;
  Float_t  HITSsourcePosX;
  Float_t  HITSsourcePosY;
  Float_t  HITSsourcePosZ;
  Int_t    HITSsourceID;
  Int_t    HITSeventID;
  Int_t    HITSrunID;
  Float_t  HITSaxialPos;
  Float_t  HITSrotationAngle;
  //set branch address
  Hits->SetBranchAddress("PDGEncoding"     ,&HITSPDGEncoding         );
  Hits->SetBranchAddress("trackID"         ,&HITStrackID             );
  Hits->SetBranchAddress("parentID"        ,&HITSparentID            );
  Hits->SetBranchAddress("time"            ,&HITStime                );
  Hits->SetBranchAddress("edep"            ,&HITSedep                );
  Hits->SetBranchAddress("stepLength"      ,&HITSstepLength          );
  Hits->SetBranchAddress("posX"            ,&HITSposX                );
  Hits->SetBranchAddress("posY"            ,&HITSposY                );
  Hits->SetBranchAddress("posZ"            ,&HITSposZ                );
  Hits->SetBranchAddress("localPosX"       ,&HITSlocalPosX           );
  Hits->SetBranchAddress("localPosY"       ,&HITSlocalPosY           );
  Hits->SetBranchAddress("localPosZ"       ,&HITSlocalPosZ           );
  Hits->SetBranchAddress("gantryID"        ,&HITSgantryID            );
  Hits->SetBranchAddress("rsectorID"       ,&HITSrsectorID           );
  Hits->SetBranchAddress("moduleID"        ,&HITSmoduleID            );
  Hits->SetBranchAddress("submoduleID"     ,&HITSsubmoduleID         );
  Hits->SetBranchAddress("crystalID"       ,&HITScrystalID           );
  Hits->SetBranchAddress("layerID"         ,&HITSlayerID             );
  Hits->SetBranchAddress("photonID"        ,&HITSphotonID            );
  Hits->SetBranchAddress("nPhantomCompton" ,&HITSnPhantomCompton     );
  Hits->SetBranchAddress("nCrystalCompton" ,&HITSnCrystalCompton     );
  Hits->SetBranchAddress("nPhantomRayleigh",&HITSnPhantomRayleigh    );
  Hits->SetBranchAddress("nCrystalRayleigh",&HITSnCrystalRayleigh    );
  Hits->SetBranchAddress("primaryID"       ,&HITSprimaryID           );
  Hits->SetBranchAddress("sourcePosX"      ,&HITSsourcePosX          );
  Hits->SetBranchAddress("sourcePosY"      ,&HITSsourcePosY          );
  Hits->SetBranchAddress("sourcePosZ"      ,&HITSsourcePosZ          );
  Hits->SetBranchAddress("sourceID"        ,&HITSsourceID            );
  Hits->SetBranchAddress("eventID"         ,&HITSeventID             );
  Hits->SetBranchAddress("runID"           ,&HITSrunID               );
  Hits->SetBranchAddress("axialPos"        ,&HITSaxialPos            );
  Hits->SetBranchAddress("rotationAngle"   ,&HITSrotationAngle       );
  // count entries
  Int_t HITSnentries = (Int_t)(Hits->GetEntries());
  std::cout << "Entries in Hits tree = " << HITSnentries << std::endl;
  // --------------------------------//


  // --------------------------------//
  // Prepare output tree             //
  // --------------------------------//
  // variables
  float g1_totalEnDep;
  Int_t g1_firstMatType;
  float g1_enDepMat0;
  float g1_enDepMat1;
  float g2_totalEnDep;
  Int_t g2_firstMatType;
  float g2_enDepMat0;
  float g2_enDepMat1;
  //file
  TFile *outFile = new TFile(outputName.c_str(),"RECREATE");
  // declare ttree
  TTree *tree = new TTree("tree","tree of data");
  // branch ttree
  // each event in the ttree will be made of 8 leaves
  // each event summarizes what happens to a primary gamma and all its secondaries
  // for each primary (g1 and g2):
  // gN_totalEnDep    -> sum of all energy deposited in the scanner by anything whose primaryId == N
  // gN_firstMatType  -> type of material where first energy deposition happened, for anything whose primaryId == N
  // gN_enDepMat0     -> total energy deposition in material 0, for anything whose primaryId == N
  // gN_enDepMat1     -> total energy deposition in material 1, for anything whose primaryId == N
  tree->Branch("g1_totalEnDep",&g1_totalEnDep,"g1_totalEnDep/F"); // total energy deposition by the first primary, anywhere in the scanner
  tree->Branch("g1_firstMatType",&g1_firstMatType,"g1_firstMatType/I");  // type of material where first energy deposition took place
  tree->Branch("g1_enDepMat0",&g1_enDepMat0,"g1_enDepMat0/F"); // total energy deposition by the first primary, in material 0
  tree->Branch("g1_enDepMat1",&g1_enDepMat1,"g1_enDepMat1/F"); // total energy deposition by the first primary, in material 1
  tree->Branch("g2_totalEnDep",&g2_totalEnDep,"g2_totalEnDep/F"); // total energy deposition by the second primary, anywhere in the scanner
  tree->Branch("g2_firstMatType",&g2_firstMatType,"g2_firstMatType/I");  // type of material where second energy deposition took place
  tree->Branch("g2_enDepMat0",&g2_enDepMat0,"g2_enDepMat0/F"); // total energy deposition by the second primary, in material 0
  tree->Branch("g2_enDepMat1",&g2_enDepMat1,"g2_enDepMat1/F"); // total energy deposition by the second primary, in material 1
  // --------------------------------//


  // --------------------------------//
  // Main loop                       //
  // --------------------------------//
  // The loop will run on Hits. It has to be noted that a "Hit" in GATE is any step in which
  // an energy deposition is recorded. It is NOT sorted by event (where one event in the
  // simulation is the emission of 2 back-to-back gammas), it is just a list of all the energy
  // depositions. It does anyway contain ad eventID, of course, and that's what we use to
  // group together all the energy depositions belonging to the same back-to-back gamma event
  //
  // prepare a structure of enDep elements (each element is a struct),
  // to group everything that belongs to the same eventID
  std::vector<enDep> event;
  // prepare a check for the event IDs
  Int_t eventsCheck = -1;
  // prepare some counter
  Int_t eventCounter = 0;
  int statuscounter = 0;
  Int_t coincidenceCounter = 0;
  // run on all hits
  // a event vector is ready and empty now. In the loop, all the hits with the same eventID
  // will be pushed to the same event vector, and when a new eventID is found, the vector
  // will be analyzed, the output ttree will be filled with the results of the analysis
  // and the vector will be cleared, to start accumulate in an empty vector all the edep
  // of the next eventID
  std::cout << std::endl;
  for (Int_t i = 0 ; i < HITSnentries ; i++)
  {
    // get the i-th entry
    Hits->GetEntry(i);

    // now, check if the current ID (eventsCheck) is different from the eventID of
    // this Hit (HITSeventID) and start the fun. To be clear, these will be the first
    // iterations

    // i == 0
    // eventsCheck == -1, and HITSeventID >= 0 (at least 0, it could be higher
    // if there was not en depo in the first event of the simulation)
    // so eventsCheck != HITSeventID by contruction for i == 0, and the code enters the loop
    // on the other hand, the event vector is empty, so the only things done in the following
    // if statement for i == 0 is to increase the event counter (i.e. count the first event)
    // and set the current ID to HITSeventID. Then the rest is skipped and a enDep struct is
    // created after the if statement, it is filled with the data from this entry
    // and it is pushed to the event vector

    // i == 1
    // now eventsCheck == N (N is the HITSeventID for i == 0), if HITSeventID for i == 1
    // is still N, the if statement will be skipped, and all that happens is just creating
    // a new enDep struct for this entry, fill it, push it to event vector. Otherwise, if
    // this entry has a different HITSeventID, the code enters the statement, analyzes the
    // event vector, that now has only data from the PREVIOUS eventID (see the internal
    // part of the statement to see how), fills the tree and clears the event vector.
    // so then after the if statement, it can start filling an empty event vector, and
    // go on like this until all the entries are analyzed

    if(eventsCheck != HITSeventID)
    {
      //increase counter
      eventCounter++;
      //set check to this eventID
      eventsCheck = HITSeventID;

      // at this point, since the eventID of this entry is different from the ones
      // of the energy depositions stored in the current event vector, this vector is
      // analyzed (if not empty), tree is filled with a new entry, then vector is
      // cleared to be ready for a new eventID
      if(event.size() != 0)
      {
        //summarize the event
        std::vector<enDep> fromPrimary1;
        std::vector<enDep> fromPrimary2;

        float en1 = 0;
        float en2 = 0;

        // std::sort(event.begin(), event.end(), compareByTime);
        // run on entire event
        // --> sum total energy due to primary 1 and to primary 2
        // --> split in two subevents, one with everything coming from primary 1 and one
        // with everything coming from primary 2
        for(int eEvent = 0; eEvent < event.size(); eEvent++)
        {
          if(event[eEvent].primaryID == 1)
          {
            en1 += event[eEvent].edep;
            fromPrimary1.push_back(event[eEvent]);
          }
          if(event[eEvent].primaryID == 2)
          {
            en2 += event[eEvent].edep;
            fromPrimary2.push_back(event[eEvent]);
          }
        }

        // initialize variables
        g1_totalEnDep   = 0;
        g1_firstMatType = 0;
        g1_enDepMat0    = 0;
        g1_enDepMat1    = 0;
        g2_totalEnDep   = 0;
        g2_firstMatType = 0;
        g2_enDepMat0    = 0;
        g2_enDepMat1    = 0;

        // filter only events with at least one endep
        if((fromPrimary1.size() > 0) && (fromPrimary2.size() > 0))
        {
          g1_totalEnDep = en1; // set total en dep from primary 1
          g2_totalEnDep = en2; // set total en dep from primary 2

          // just count the ones with more than 0.4 in both, but save all of them
          if((en1 > 0.4) && (en2 > 0.4))
          {
            coincidenceCounter++;
          }

          // PRIMARY 1
          g1_firstMatType = fromPrimary1[0].layerID;  // type of first energy depo
          float sumPr1_t0 = 0;
          float sumPr1_t1 = 0;
          for(int pEvent = 0; pEvent < fromPrimary1.size(); pEvent++)
          {
            if(fromPrimary1[pEvent].layerID == 0)
            {
              sumPr1_t0 += fromPrimary1[pEvent].edep;
            }
            if(fromPrimary1[pEvent].layerID == 1)
            {
              sumPr1_t1 += fromPrimary1[pEvent].edep;
            }
          }
          g1_enDepMat0 = sumPr1_t0;
          g1_enDepMat1 = sumPr1_t1;

          // PRIMARY 2
          g2_firstMatType = fromPrimary2[0].layerID;  // type of first energy depo
          float sumPr2_t0 = 0;
          float sumPr2_t1 = 0;
          for(int pEvent = 0; pEvent < fromPrimary2.size(); pEvent++)
          {
            if(fromPrimary2[pEvent].layerID == 0)
            {
              sumPr2_t0 += fromPrimary2[pEvent].edep;
            }
            if(fromPrimary2[pEvent].layerID == 1)
            {
              sumPr2_t1 += fromPrimary2[pEvent].edep;
            }
          }
          g2_enDepMat0 = sumPr2_t0;
          g2_enDepMat1 = sumPr2_t1;
          tree->Fill();
        }
        // clear struct, to start a new one
        event.clear();
      }
    }

    enDep hitDeposition;       //create a new endep struct
    hitDeposition.localPosX    = HITSlocalPosX;
    hitDeposition.localPosY    = HITSlocalPosY;
    hitDeposition.localPosZ    = HITSlocalPosZ;
    hitDeposition.eventID      = HITSeventID;
    hitDeposition.parentID     = HITSparentID;
    hitDeposition.primaryID    = HITSprimaryID;
    hitDeposition.trackID      = HITStrackID;
    hitDeposition.gantryID     = HITSgantryID;
    hitDeposition.rsectorID    = HITSrsectorID;
    hitDeposition.moduleID     = HITSmoduleID;
    hitDeposition.submoduleID  = HITSsubmoduleID;
    hitDeposition.crystalID    = HITScrystalID;
    hitDeposition.layerID      = HITSlayerID;
    hitDeposition.sourceID     = HITSsourceID;
    hitDeposition.time         = HITStime;
    hitDeposition.edep         = HITSedep;

    event.push_back(hitDeposition); //push it to the vector

    //LOOP COUNTER
    // counterAnalysis++;
    int perc = ((100*( (float) i))/( (float) HITSnentries));
    if( (perc % 5) == 0 )
    {
      std::cout << "\r";
      std::cout << perc << "% done... ";
    }
  }
  std::cout << std::endl;
  // std::cout << "\r";
  std::cout << "Done!" << std::endl;
  std::cout << std::endl;
  // --------------------------------//

  // --------------------------------//
  // some preliminary analisys       //
  // --------------------------------//
  // TH2F *energyScatterNoCut = new TH2F("energyScatterNoCut","Scatter plot of energy deposited in both materials, without any cut in energy",60,0,0.6,60,0,0.6);
  // TH2F *energyScatter400Cut = new TH2F("energyScatter400Cut","Scatter plot of energy deposited in both materials, when at least 400 KeV are deposited from both primaries",60,0,0.6,60,0,0.6);
  // tree->Draw("");

  // --------------------------------//
  // summarize and save              //
  // --------------------------------//
  std::cout << "Number of hits                              = " << HITSnentries << std::endl;
  std::cout << "Number of events                            = " << eventCounter << std::endl;
  std::cout << "Number of output tree entries               = " << tree->GetEntries() << std::endl;
  std::cout << "Events with > 400 Kev from both primaries   = " << coincidenceCounter << std::endl;
  outFile->cd();
  tree->Write();
  outFile->Close();
  // --------------------------------//

  return 0;
}
