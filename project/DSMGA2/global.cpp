/***************************************************************************
 *   Copyright (C) 2011 by Tian-Li Yu,,,                                   *
 *   tianliyu@cc.ee.ntu.edu.tw                                             *
 ***************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cfloat>
#include "myrand.h"
#include "statistics.h"
#include "doublelinkedlistarray.h"
#include "zkey.h"
#include "chromosome.h"
#include "sat.h"

#include "../IslandGA/BinaryCoding.h"
#include "../IslandGA/Evaluation.h"

int maxMemory = 0;

bool GHC = true;
bool SELECTION = true;
bool CACHE = false;

bool SHOW_HC = false;
bool SHOW_GRAPH = false;
bool SHOW_CLIQUE = false;
bool SHOW_LINKAGE = false;
bool SHOW_SPLIT = false;
bool SHOW_LINKAGE_BRIEF = false;
bool SHOW_POPULATION = false;
bool SHORT_HAND = true;
bool SHOW_SELECTION_INDEX = false;
bool SHOW_REPLACEMENT = false;
bool SHOW_MAPPING = false;
bool SHOW_COMPRESSION = false;
bool SHOW_BUSINESSMEN = false;
bool SHOW_BISECTION = true;

char outputFilename[100];
Chromosome::Function Chromosome::function;
int Chromosome::nfe;
int Chromosome::lsnfe;
int Chromosome::hitnfe;
bool Chromosome::hit;
unordered_map<unsigned long, double> Chromosome::cache;
CEvaluation<CBinaryCoding>* Chromosome::injectedEvaluation;

ZKey zKey;
MyRand myRand;
BitwiseDistance myBD;
SpinGlassParams mySpinGlassParams;
NKWAProblem nkwa;
SATinstance mySAT;



CError  eLoadZKeyFile(CString  sDirectory)
{
	CError  c_err;
	c_err = zKey.eLoadZKey(sDirectory);
	return(c_err);
}//void  vLoadZKeyFile(CString  sDirectory)

void outputErrMsg(const char *errMsg) {
    //printf("%s\n", errMsg);
    exit(1);
}

int pow2(int x) {
    return (1 << x);
}

