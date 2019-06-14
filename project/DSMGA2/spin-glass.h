#ifndef _spinglass_h_
#define _spinglass_h_

#include <stdio.h>

struct SpinGlassParams {

    char ID[120];

    int N;
    int n;
    int d;

    int    *numNeighbors;
    int   **neighbors;
    double **interactions;

    int *numFriends;
    int **friends;
    double **friendInteractions;

    double optimalValue;
};

//int   generateRandomSpinGlassInstance(SpinGlassParams *spinGlassParams);
int   printSpinGlassInstance(FILE *output, SpinGlassParams *spinGlassParams);
int   loadSpinGlassInstance(FILE *input, SpinGlassParams *spinGlassParams);
int   freeSpinGlassInstance(SpinGlassParams *spinGlassParams);
double spinGlassValue(int *x, SpinGlassParams *spinGlassParams);
double spinGlassValueDHC(int *x, SpinGlassParams *spinGlassParams, long &numFlips);
double spinGlassValueDHCOptimal(int *x, SpinGlassParams *spinGlassParams, long &numFlips);
//double spinGlassValueWithWolff(int *x, SpinGlassParams *spinGlassParams);
//double spinGlassValueDHCRandomized(int *x, SpinGlassParams *spinGlassParams);
//int   loadHeinzInteractions(FILE *input, SpinGlassParams *spinGlassParams);
int   isOptimalSpinGlass(int *x, SpinGlassParams *spinGlassParams);
int   isOptimalSpinGlassDHC(int *x, SpinGlassParams *spinGlassParams);
//long getSfitnessCalls();

int printSpinGlassInstance2(FILE *output, SpinGlassParams *spinGlassParams);
int loadSimonsSpinGlassInstance(FILE *input, SpinGlassParams *spinGlassParams);

//double spinGlassValueMCMC(int *x, SpinGlassParams *spinGlassParams, double t, int steps);
#endif
