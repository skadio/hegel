#include <parameter.h>
#include <iostream.h>
  
static char* parameterNameArray[]= {"airlinecase",
				    "toplevel", "crewlevel", "pairinglevel",
				    "cost", "nbobjectiveparts", "objectivepart", 
				    "crewmembercost", "pairingcost", 
				    "pairingcostdummy", "pairingcostdummydefault", 
				    "crewmembercostdummy", "crewmembercostdummydefault",
				    "savecode",
				    "timelimit",
				    "timewindowfract", "timewindowmin", "focstepwidth", "unassignedfix",
				    "groupsize",
				    "chronologicalorder",
				    "hybridbvs", "bvselect", "initialhybridbvs", "initialbvselect", 
				    "bdselect", "bestblocked", "spc", "nrc", "trans", 
				    "swlimiton", "swlimit", 
				    "pcover", "pcfract", "pcabs", 
				    "initialpcover", "initialpcfract", "initialpcabs",
				    "diffstruct", 
				    "blocktol",
				    "domainreduction",
				    "indivrosters",
                                    "addvariety", 
				    "initialfaillimit", "initialrosters",
				    "exchangefile",
				    "verbose"};

void Parameter::parseError(int line)
{
  cout << "Error in Parameter File!\nLine " << line << endl << flush;
  exit (999);	  
}

void Parameter::init(void)
{
  topLevelPar=(char*)0;
  crewLevelPar=(char*)0;
  pairingLevelPar=(char*)0;
  timeLimitPar=10000;
  costPar=(char*)0;
  nbObjectivePartsCounter =0;
  nbObjectivePartsPar = 0;
  objectivePartsPar = (char**)0;
  crewmemberCostPar=(char*)0;
  pairingCostPar=(char*)0;
  pairingCostDummyPar=(char*)0;
  crewmemberCostDummyDefaultPar = 0;
  pairingCostDummyDefaultPar = 0;
  crewmemberCostDummyPar=(char*)0;
  saveCodePar=(char*)0;
  timeWindowFractPar=1;
  timeWindowMinPar=1;
  focStepWidthPar=1;
  unassignedFixPar=IlcTrue;
  groupSizePar=10;
  chronologicalOrderPar=IlcFalse;
  hybridBvsPar=0;
  initialHybridBvsPar=0;
  bvSelectPar=CHOOSE_FIRST;
  initialBvSelectPar=CHOOSE_LEAST_COVERED;
  bdSelectPar=IlcTrue;
  bestBlockedPar=IlcFalse;
  domainReductionPar=IlcFalse;
  spcPar=false;
  nrcPar=false;
  transPar=false;
  swLimitOnPar=false;
  pCoverPar=false;
  pcFractPar=1;
  pcAbsPar=0;
  initialPCoverPar=false;
  initialPcFractPar=1;
  initialPcAbsPar=0;
  diffStructPar=false;
  indivRostersPar=10;
  addVarietyPar=AUTOMATIC;
  exchangeFilePar=(char*)0;
  initialFailLimitPar=100;
  initialRostersPar=20;
  verbosePar=2;
}

void Parameter::readParam (void)
{
  ifstream in(paramfile, ios::in);
  char parameterName[maxParameterLength];
  char parameterValue[maxParameterLength];
  char lineString[maxLineLength];
  int line = 1;
  while (in.getline(lineString, maxLineLength, '\n'))
    {
      if ((lineString[0]=='%')||(lineString[0]=='#')) 
	{
	  line++;
	  continue;
	}
      int n = sscanf(lineString,"%s%s",parameterName,parameterValue);
      if (n!=2) break;
      bool found=false;
      Parameters p;
      for (p=firstpar; p<=lastpar; p++)
	{
	  if (strcmp(parameterNameArray[p],parameterName)) continue;
	  found = true;  
	  break;
	}
      if (!found)
	parseError(line);
      switch(p)
	{
	case airlinecase:
	  if (!strcmp("klm",parameterValue))
	    airlineCasePar=KLM;
	  else if (!strcmp("lx",parameterValue))
	    airlineCasePar=LX;
	  else if (!strcmp("oa",parameterValue))
	    airlineCasePar=OA;
	  else parseError(line);
	  break; 
	case toplevel:
	  topLevelPar=new char[strlen(parameterValue)+1];
	  strcpy(topLevelPar,parameterValue);
	  break;
	case crewlevel:
	  crewLevelPar=new char[strlen(parameterValue)+1];
	  strcpy(crewLevelPar,parameterValue);
	  break;
	case pairinglevel:
	  pairingLevelPar=new char[strlen(parameterValue)+1];
	  strcpy(pairingLevelPar,parameterValue);
	  break;
	case cost:
	  costPar=new char[strlen(parameterValue)+1];
	  strcpy(costPar,parameterValue);
	  break;
	case nbobjectiveparts:
	  if (nbObjectivePartsPar>0) 
	    parseError(line);
	  nbObjectivePartsPar=atol(parameterValue);
	  nbObjectivePartsCounter = 0;
	  objectivePartsPar = new char*[nbObjectivePartsPar];
	  break;
	case objectivepart:
	  if (nbObjectivePartsCounter>=nbObjectivePartsPar) 
	    parseError(line);
	  objectivePartsPar[nbObjectivePartsCounter]=new char[strlen(parameterValue)+1];
	  strcpy(objectivePartsPar[nbObjectivePartsCounter],parameterValue);
	  nbObjectivePartsCounter++;
	  break;
	  
	case crewmembercost:
	  crewmemberCostPar=new char[strlen(parameterValue)+1];
	  strcpy(crewmemberCostPar,parameterValue);
	  break;
	case pairingcost:
	  pairingCostPar=new char[strlen(parameterValue)+1];
	  strcpy(pairingCostPar,parameterValue);
	  break;
	case pairingcostdummy:
	  pairingCostDummyPar=new char[strlen(parameterValue)+1];
	  strcpy(pairingCostDummyPar,parameterValue);
	  break;
	case pairingcostdummydefault:
	  pairingCostDummyDefaultPar=atol(parameterValue);
	  break;
	case crewmembercostdummy:
	  crewmemberCostDummyPar=new char[strlen(parameterValue)+1];
	  strcpy(crewmemberCostDummyPar,parameterValue);
	  break;
	case crewmembercostdummydefault:
	  crewmemberCostDummyDefaultPar=atol(parameterValue);
	  break;
	case savecode:
	  saveCodePar=new char[strlen(parameterValue)+1];
	  strcpy(saveCodePar,parameterValue);
	  break;

	case timelimit:
	  timeLimitPar=atol(parameterValue);
	  break;

	case timewindowfract:
	  timeWindowFractPar=atol(parameterValue);
	  break;
	case timewindowmin:
	  timeWindowMinPar=atol(parameterValue);
	  break;
	case focstepwidth:
	  focStepWidthPar=atol(parameterValue);
	  break;
	case unassignedfix:
	  if (!strcmp("true",parameterValue))
	    unassignedFixPar=true;
	  else if (!strcmp("false",parameterValue))
	    unassignedFixPar=false;
	  else parseError(line);
	  break;
	case groupsize:
	  groupSizePar=atol(parameterValue);
	  break;
	case chronologicalorder:
	  if (!strcmp("true",parameterValue))
	    chronologicalOrderPar=true;
	  else if (!strcmp("false",parameterValue))
	    chronologicalOrderPar=false;
	  else parseError(line);
	  break;
	case hybridbvs:
	  hybridBvsPar=atol(parameterValue);
	  break;
	case initialhybridbvs:
	  initialHybridBvsPar=atol(parameterValue);
	  break;  
	case bvselect:
	  if (!strcmp("choose_first",parameterValue))
	    bvSelectPar=CHOOSE_FIRST;
	  else if (!strcmp("choose_heavy_load_first",parameterValue))
	    bvSelectPar=CHOOSE_HEAVY_LOAD_FIRST;
	  else if (!strcmp("choose_best",parameterValue))
	    bvSelectPar=CHOOSE_BEST;
	  else if (!strcmp("choose_first_on_path",parameterValue))
	    bvSelectPar=CHOOSE_FIRST_ON_PATH;
	  else if (!strcmp("choose_least_covered",parameterValue))
	    bvSelectPar=CHOOSE_LEAST_COVERED;
	  else if (!strcmp("choose_crewmember_least_covered",parameterValue))
	    bvSelectPar=CHOOSE_CREWMEMBER_LEAST_COVERED;
	  else parseError(line);
	  break;
	case initialbvselect:
	  if (!strcmp("choose_first",parameterValue))
	    initialBvSelectPar=CHOOSE_FIRST;
	  else if (!strcmp("choose_heavy_load_first",parameterValue))
	    initialBvSelectPar=CHOOSE_HEAVY_LOAD_FIRST;
	  else if (!strcmp("choose_best",parameterValue))
	    initialBvSelectPar=CHOOSE_BEST;
	  else if (!strcmp("choose_least_covered",parameterValue))
	    initialBvSelectPar=CHOOSE_LEAST_COVERED;
	  else if (!strcmp("choose_crewmember_least_covered",parameterValue))
	    initialBvSelectPar=CHOOSE_CREWMEMBER_LEAST_COVERED;
	  else parseError(line);
	  break;
	case bdselect:
	  if (!strcmp("true",parameterValue))
	    bdSelectPar=true;
	  else if (!strcmp("false",parameterValue))
	    bdSelectPar=false;
	  else parseError(line);
	  break;
	case bestblocked:
	  if (!strcmp("true",parameterValue))
	    bestBlockedPar=true;
	  else if (!strcmp("false",parameterValue))
	    bestBlockedPar=false;
	  else parseError(line);
	  break;
	case spc:
	  if (!strcmp("true",parameterValue))
	    spcPar=true;
	  else if (!strcmp("false",parameterValue))
	    spcPar=false;
	  else parseError(line);
	  break;
	case nrc:
	  if (!strcmp("true",parameterValue))
	    nrcPar=true;
	  else if (!strcmp("false",parameterValue))
	    nrcPar=false;
	  else parseError(line);
	  break;
	case trans:
	  if (!strcmp("true",parameterValue))
	    transPar=true;
	  else if (!strcmp("false",parameterValue))
	    transPar=false;
	  else parseError(line);
	  break;

	case swlimiton:
	  if (!strcmp("true",parameterValue))
	      swLimitOnPar=true;
	  else if (!strcmp("false",parameterValue))
	    swLimitOnPar=false;
	  else parseError(line);
	  break;
	case swlimit:
	  swLimitPar=atol(parameterValue);
	  break;

	case pcover:
	  if (!strcmp("true",parameterValue))
	    pCoverPar=true;
	  else if (!strcmp("false",parameterValue))
	    pCoverPar=false;
	  else parseError(line);
	  break;
	case initialpcover:
	  if (!strcmp("true",parameterValue))
	    initialPCoverPar=true;
	  else if (!strcmp("false",parameterValue))
	    initialPCoverPar=false;
	  else parseError(line);
	  break;
	case pcfract:
	  pcFractPar=atol(parameterValue);
	  break;
	case initialpcfract:
	  initialPcFractPar=atol(parameterValue);
	  break;	
	case pcabs:
	  pcAbsPar=atol(parameterValue);
	  break;
	case initialpcabs:
	  initialPcAbsPar=atol(parameterValue);
	  break;

	case diffstruct:
	  if (!strcmp("true",parameterValue))
	    diffStructPar=true;
	  else if (!strcmp("false",parameterValue))
	    diffStructPar=false;
	  else parseError(line);
	  break;

	case blocktol:
	  blockTolPar=atol(parameterValue);
	  break;

	case domainreduction:
	  if (!strcmp("true",parameterValue))
	    domainReductionPar=true;
	  else if (!strcmp("false",parameterValue))
	    domainReductionPar=false;
	  else parseError(line);
	  break;

	case indivrosters:
	  indivRostersPar=atol(parameterValue);
	  break;

	case addvariety:
	  if (!strcmp("automatic",parameterValue))
	    addVarietyPar=AUTOMATIC;
	  else if (!strcmp("external",parameterValue))
	    addVarietyPar=EXTERNAL;
	  else if (strcmp("none",parameterValue))
	    parseError(line);
	  break;
	case initialfaillimit:
	  if (addVarietyPar!=AUTOMATIC)
	    parseError(line);
	  initialFailLimitPar=atol(parameterValue);
	  break;
	case initialrosters:
	  if (addVarietyPar!=AUTOMATIC)
	    parseError(line);
	  initialRostersPar=atol(parameterValue);
	  break;
	case exchangefile:
	  if (addVarietyPar!=EXTERNAL)
	    parseError(line);
	  exchangeFilePar=new char[strlen(parameterValue)+1];
	  strcpy(exchangeFilePar,parameterValue);
	  break;
	case verbose:
	  verbosePar=atol(parameterValue);
	  break;
	}
      line++;     
    }
  if (nbObjectivePartsPar!=nbObjectivePartsCounter)
    {
      cout << "wrong number of objective parts!\n" << flush;
      parseError(line);
    }
  cout << "OptCRP Parameters read!\n" << flush;
  in.close();
}

void Parameter::setParameterFile(const char* pf)
{
  delete [] paramfile;
  paramfile = new char[strlen(pf)+1];
  strcpy(paramfile, pf);
  init();
  readParam();
}

Parameter::Parameter()
{
  paramfile = (char*)0;
  init();
}

Parameter::Parameter(const char *pf)
{
  paramfile = new char[strlen(pf)+1];
  strcpy(paramfile, pf);
  init();
  readParam();
}

Parameter::~Parameter()
{
  delete [] topLevelPar;
  delete [] crewLevelPar;
  delete [] pairingLevelPar;
  delete [] paramfile;
  delete [] costPar;
  for (int k=0; k<nbObjectivePartsCounter;k++)
    delete [] objectivePartsPar[k];
  delete [] objectivePartsPar;
  delete [] crewmemberCostPar;
  delete [] pairingCostPar;
  delete [] pairingCostDummyPar;
  delete [] crewmemberCostDummyPar;
  delete [] saveCodePar;
}




