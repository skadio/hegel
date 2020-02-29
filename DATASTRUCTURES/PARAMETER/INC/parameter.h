#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream.h>
#include <indiv_roster_maker.h>

const int maxParameterLength = 40;
const int maxLineLength = 256;

enum Parameters {airlinecase,
		 toplevel, crewlevel, pairinglevel,
		 cost, nbobjectiveparts, objectivepart, 
		 crewmembercost, pairingcost, 
		 pairingcostdummy, pairingcostdummydefault, 
		 crewmembercostdummy, crewmembercostdummydefault, 
		 savecode,
		 timelimit,
		 timewindowfract, timewindowmin, focstepwidth, unassignedfix,
		 groupsize,
		 chronologicalorder,
		 hybridbvs, bvselect, initialhybridbvs, initialbvselect,
		 bdselect, bestblocked, spc, nrc, trans, 
		 swlimiton, swlimit, 
		 pcover, pcfract, pcabs, 
		 initialpcover, initialpcfract, initialpcabs,
		 diffstruct, 
		 blocktol, 
		 domainreduction,
		 indivrosters,
		 addvariety, initialfaillimit, initialrosters, exchangefile, verbose};

enum AirlineCase {KLM, LX, OA};

enum VarietyParameter {NONE, AUTOMATIC, EXTERNAL};

const Parameters firstpar=airlinecase;
const Parameters lastpar=verbose;

class Parameter
{
  int nbObjectivePartsCounter;
  void parseError(int line);
  void init(void);
  void readParam(void);
  char *paramfile;
public:
  int verbosePar;
  long timeLimitPar, timeWindowFractPar, timeWindowMinPar, focStepWidthPar, groupSizePar,
    pcFractPar, pcAbsPar, initialPcFractPar,
    initialPcAbsPar,
    swLimitPar, blockTolPar, indivRostersPar, initialFailLimitPar, initialRostersPar, nbObjectivePartsPar, 
    pairingCostDummyDefaultPar, crewmemberCostDummyDefaultPar, hybridBvsPar, initialHybridBvsPar;
  char *costPar, *crewmemberCostPar, *pairingCostPar, *pairingCostDummyPar, *crewmemberCostDummyPar, *saveCodePar,
    *topLevelPar, *crewLevelPar, *pairingLevelPar, *exchangeFilePar;
  char **objectivePartsPar;
  VarietyParameter addVarietyPar;
  bool chronologicalOrderPar,spcPar, nrcPar, transPar, swLimitOnPar, pCoverPar, initialPCoverPar, 
       diffStructPar, bdSelectPar, bestBlockedPar, domainReductionPar, unassignedFixPar;
  VarSelectionStrategy bvSelectPar, initialBvSelectPar;
  AirlineCase airlineCasePar;

  void setParameterFile(const char*);
  Parameter();
  Parameter(const char *pf);
  ~Parameter();
};

#endif
