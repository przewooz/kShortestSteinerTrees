#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "spin-glass.h"
//#include "stack.h"

#define WOLFF_PROBABILITY 0.1

//#define DEBUG

//=================================================================
/*
int generateRandomSpinGlassInstance(SpinGlassParams *spinGlassParams)
{
  int i,j,num,power,pos;

  // create ID for the instance

  sprintf(spinGlassParams->ID,"Ising Spin Glass Randomly Generated Instance with Standard Neighbors, n=%u, d=%u",spinGlassParams->n,spinGlassParams->d);

  // allocate memort for and create the standard neighborhood

  spinGlassParams->N = pow(spinGlassParams->n,spinGlassParams->d);

  spinGlassParams->numNeighbors = (int*) calloc(spinGlassParams->N,sizeof(int));
  spinGlassParams->neighbors    = (int**) calloc(spinGlassParams->N,sizeof(int*));

  for (i=0; i<spinGlassParams->N; i++)
    {
      spinGlassParams->numNeighbors[i] = spinGlassParams->d;

      if (spinGlassParams->numNeighbors[i]>0)
 spinGlassParams->neighbors[i] = (int*) calloc(spinGlassParams->numNeighbors[i],sizeof(int));

      num = 0;

      for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
 {
   power = pow(spinGlassParams->n,j);
   pos   = (i%(spinGlassParams->n*power))/power;

   spinGlassParams->neighbors[i][num] = (i+power);
   if (pos==spinGlassParams->n-1)
     spinGlassParams->neighbors[i][num] -= pow(spinGlassParams->n,j+1);

   //printf("i = %u, pos(%u) = %u => %u\n",i,j,pos,spinGlassParams->neighbors[i][num]);

   num++;
 };

      spinGlassParams->numNeighbors[i] = num;
    }

  // allocate memory for and generate interactions

  spinGlassParams->interactions = (double**) calloc(spinGlassParams->N,sizeof(double*));

  for (i=0; i<spinGlassParams->N; i++)
    {

      if (spinGlassParams->numNeighbors[i]>0)
 spinGlassParams->interactions[i] = (double*) calloc(spinGlassParams->numNeighbors[i],sizeof(double));

      for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
 spinGlassParams->interactions[i][j] = intRand(2)*2-1;
    }

  // the optimal values isn't known

  spinGlassParams->optimalValue = -1;;

  // get back

  return 0;
};
*/
//=================================================================
int freeSpinGlassInstance(SpinGlassParams *spinGlassParams) {
    int i;

    // free the memory used by arrays

    for (i=0; i<spinGlassParams->N; i++) {
        if (spinGlassParams->numNeighbors[i]>0) {
            free(spinGlassParams->neighbors[i]);
            free(spinGlassParams->interactions[i]);
        }
        if (spinGlassParams->numFriends[i]>0) {
            free(spinGlassParams->friends[i]);
            free(spinGlassParams->friendInteractions[i]);
        };
    };

    free(spinGlassParams->numNeighbors);
    free(spinGlassParams->neighbors);
    free(spinGlassParams->interactions);
    free(spinGlassParams->friends);
    free(spinGlassParams->friendInteractions);

    // get back

    return 0;
};
//=================================================================

int printSpinGlassInstance(FILE *output, SpinGlassParams *spinGlassParams) {
    int i,j,N;

    // initialize the variables

    N = spinGlassParams->N;

    // print out size and dimension

    fprintf(output,"ID\n");
    fprintf(output,"%s\n\n",spinGlassParams->ID);
    fprintf(output,"Size\n");
    fprintf(output,"%u\n\n",spinGlassParams->N);

    // print out info about neighbors and interactions

    fprintf(output,"Neighbors and Interactions (num, neighbors, interactions)\n");
    for (i=0; i<N; i++) {
        fprintf(output,"%3u ",spinGlassParams->numNeighbors[i]);

        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {

            for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
                fprintf(output,"%4i ",spinGlassParams->neighbors[i][j]);
            for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
                fprintf(output,"%10f",spinGlassParams->interactions[i][j]);
            fprintf(output,"\n");
        };
    };

    fprintf(output,"\nThe Optimum\n");
    fprintf(output,"%f\n",spinGlassParams->optimalValue);

    fprintf(output,"\nThe End.");

    // get back

    return 0;
};

//=================================================================

int printSpinGlassInstance2(FILE *output, SpinGlassParams *spinGlassParams) {
    int i,j,N;

    // initialize the variables

    N = spinGlassParams->N;

    // print out size and dimension

    fprintf(output,"type: pm\n");
    fprintf(output,"size: %u\n",(int) sqrt((double)spinGlassParams->N));
    fprintf(output,"name: My randomly generated instance of size %u x %u\n",(int) sqrt((double)spinGlassParams->N),(int) sqrt((double)spinGlassParams->N));

    // print out info about neighbors and interactions

    for (i=0; i<N; i++) {
        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {
            fprintf(output,"%2u %2u %f\n",i+1,spinGlassParams->neighbors[i][j]+1,spinGlassParams->interactions[i][j]);
        };
    };

    // get back

    return 0;
};

//=================================================================

int loadSpinGlassInstance(FILE *input, SpinGlassParams *spinGlassParams) {
    int i,j;
    char aux[300];
#ifdef DEBUG
    //printf("Going to load spin glass instance\n");
#endif

    spinGlassParams->n = spinGlassParams->d = -1;

    fgets(aux,300,input);
    fgets(spinGlassParams->ID,120,input);
    spinGlassParams->ID[strlen(spinGlassParams->ID)-1]=0;

    fgets(aux,300,input);
    fgets(aux,300,input);

    fscanf(input,"%u",&(spinGlassParams->N));

    fgets(aux,300,input);
    fgets(aux,300,input);
    fgets(aux,300,input);

    spinGlassParams->numNeighbors = (int*) calloc(spinGlassParams->N,sizeof(int));
    spinGlassParams->neighbors    = (int**) calloc(spinGlassParams->N,sizeof(int*));
    spinGlassParams->interactions = (double**) calloc(spinGlassParams->N,sizeof(double*));
    spinGlassParams->numFriends   = (int*) calloc(spinGlassParams->N,sizeof(int));
    spinGlassParams->friends      = (int**) calloc(spinGlassParams->N,sizeof(int*));
    spinGlassParams->friendInteractions = (double**) calloc(spinGlassParams->N,sizeof(double*));

    for (i=0; i<spinGlassParams->N; i++) {
        fscanf(input,"%u",&(spinGlassParams->numNeighbors[i]));

        if (spinGlassParams->numNeighbors[i]>0)
            spinGlassParams->neighbors[i] = (int*) calloc(spinGlassParams->numNeighbors[i],sizeof(int));

        for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
            fscanf(input,"%u",&(spinGlassParams->neighbors[i][j]));

        if (spinGlassParams->numNeighbors[i]>0)
            spinGlassParams->interactions[i] = (double*) calloc(spinGlassParams->numNeighbors[i],sizeof(double));

        for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
            fscanf(input,"%lf",&(spinGlassParams->interactions[i][j]));

        fgets(aux,300,input);
    };

    fgets(aux,300,input);
    fgets(aux,300,input);
    fscanf(input,"%lf",&(spinGlassParams->optimalValue));

#ifdef DEBUG
    //printf("\n\n\n\n#####################\noptimum:%lf\n####################\n\n\n\n\n",spinGlassParams->optimalValue);
#endif

    // compute the friends (double interactions)


    for (i=0; i<spinGlassParams->N; i++)
        spinGlassParams->numFriends[i]=0;

    for (i=0; i<spinGlassParams->N; i++)
        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {
            spinGlassParams->numFriends[spinGlassParams->neighbors[i][j]]++;
            spinGlassParams->numFriends[i]++;
        };

    for (i=0; i<spinGlassParams->N; i++) {
        if (spinGlassParams->numFriends[i]>0) {
            spinGlassParams->friends[i] = new int[spinGlassParams->numFriends[i]];
            spinGlassParams->friendInteractions[i] = new double[spinGlassParams->numFriends[i]];
        } else {
            spinGlassParams->friends[i] = NULL;
            spinGlassParams->friendInteractions[i]=NULL;
        };
    };

    for (i=0; i<spinGlassParams->N; i++)
        spinGlassParams->numFriends[i]=0;

    for (i=0; i<spinGlassParams->N; i++)
        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {
            spinGlassParams->friends[spinGlassParams->neighbors[i][j]][spinGlassParams->numFriends[spinGlassParams->neighbors[i][j]]]=i;
            spinGlassParams->friendInteractions[spinGlassParams->neighbors[i][j]][spinGlassParams->numFriends[spinGlassParams->neighbors[i][j]]]=spinGlassParams->interactions[i][j];
            spinGlassParams->numFriends[spinGlassParams->neighbors[i][j]]++;

            spinGlassParams->friends[i][spinGlassParams->numFriends[i]]=spinGlassParams->neighbors[i][j];
            spinGlassParams->friendInteractions[i][spinGlassParams->numFriends[i]]=spinGlassParams->interactions[i][j];
            spinGlassParams->numFriends[i]++;
        };

#ifdef DEBUG
    //printf("\n\n------------------------------------------\nFriends\n\n");
    for (i=0; i<spinGlassParams->N; i++) {
        //printf("%2u friends of %2u: ",spinGlassParams->numFriends[i],i);
        for (j=0; j<spinGlassParams->numFriends[i]; j++)
            //printf("%u (%5.2f) ",spinGlassParams->friends[i][j],spinGlassParams->friendInteractions[i][j]);
        //printf("\n");
    };
    getchar();
#endif

    // get back

    return 0;
};

//=================================================================

double spinGlassValue(int *x, SpinGlassParams *spinGlassParams) {
    int i;
    register int j;

    int     N;
    int    *numNeighbors;
    int   **neighbors;
    double **interactions;
    int    *s;
    int    si;
    int    *neighbor;
    double  *interaction;
    double  f;
    int nN;

    N            = spinGlassParams->N;
    numNeighbors = spinGlassParams->numNeighbors;
    neighbors    = spinGlassParams->neighbors;
    interactions = spinGlassParams->interactions;
    s            = x;

    f = 0;

    for (i=0; i<N; i++) {
        si          = *s;
        nN          = *numNeighbors;

        if (nN>0) {
            neighbor    = *neighbors;
            interaction = *interactions;

            for (j=0; j<nN; j++) {
                f += si*x[*neighbor]*(*interaction);
                neighbor++;
                interaction++;
            }
        }

        s++;
        numNeighbors++;
        neighbors++;
        interactions++;
    };

    return f;
};

//=================================================================

inline void swap(int &x, int &y) {
    int tmp;
    tmp = x;
    x   = y;
    y   = tmp;
};

inline void demote(double *x, int *idx, int initI, int n) {
    int i=initI;
    int idxChild1,idxChild2,idxI;

    int child1=i*2+1;
    int child2=child1+1;
    idxI=idx[i];

    while (child1<n) {
        idxChild1=idx[child1];
        if (child2<n) {
            idxChild2=idx[child2];

            if (x[idxChild2]>x[idxChild1]) {
                child1=child2;
                idxChild1=idxChild2;
            };
        };

        if (x[idxI]<x[idxChild1]) {
            swap(idx[child1],idx[i]);
//    int tmp;
//    tmp=idx[i];
//    idx[i]=idx[child1];
//    idx[child1]=tmp;

            i=child1;
            idxI=idx[i];//idxI=idxChild1;
            child1=i*2+1;
            child2=child1+1;
        } else
            child1=n;
    };
};
/*
double spinGlassValueDHCOptimal(int *x, SpinGlassParams *spinGlassParams, long &numFlips)
{
  int i;
  int j;

  int     N;
  int    *neighbor;
  double  *interaction;
  double  f;
  int nN;

  double *improvement = new double[spinGlassParams->N];
  double maxImprovement=-1;
  int nmi=0;

  N = spinGlassParams->N;

  int *idx=new int[N];

  f = 0;

  for (i=0; i<N; i++)
    {
      improvement[i]=0;
      idx[i]=i;
    };

  for (i=0; i<N; i++)
    {
      nN          = spinGlassParams->numNeighbors[i];

      if (nN>0)
 {
   neighbor    = spinGlassParams->neighbors[i];
   interaction = spinGlassParams->interactions[i];

   for (j=0; j<nN; j++)
     {
       double d=x[i]*x[*neighbor]*(*interaction);
       improvement[i]-=2*d;
       improvement[*neighbor]-=2*d;
       f += d;
       neighbor++;
       interaction++;
     }
 }
    };

  for (i=0; i<N; i++)
    improvement[i]=drand();    // remove this!!!!

  for (i=N/2-1; i>=0; i--)
    demote(improvement,idx,i,N);

  for (i=0; i<N; i++)
    {
      //printf("%u - %2f\n",i,improvement[idx[i]]);
      int k=i<<1+1;
      if (k<N)
 {
   if (improvement[idx[i]]<improvement[idx[k]])
     //printf("==============> bad heap!\n");
 };
      k++;
      if (k<N)
 {
   if (improvement[idx[i]]<improvement[idx[k]])
     //printf("==============> bad heap!\n");
 };
    };
  getchar();

  do {

    maxImprovement=0;

    for (i=0; i<N; i++)
      {
 if (improvement[i]>maxImprovement)
   {
     maxImprovement=improvement[i];
     nmi=1;
   }
 else
   if (improvement[i]==maxImprovement)
     {
       nmi++;
     }
      };

    if (maxImprovement>0)
      {
 int which=intRand(nmi);
 int flip=-1;

 for (i=0; i<N; i++)
   if (improvement[i]==maxImprovement)
     if (which==0)
              flip=i;
     else
              which--;

 x[flip]=-x[flip];
 f+=improvement[flip];

 // update improvements (does this work or what)

 int numFriends = spinGlassParams->numFriends[flip];
 for (i=0; i<numFriends; i++)
   {
     double c;
     c=spinGlassParams->friendInteractions[flip][i]*x[flip]*x[spinGlassParams->friends[flip][i]];

     improvement[flip]-=4*c;
     improvement[spinGlassParams->friends[flip][i]]-=4*c;
   };

 numFlips++;
      };

  } while (maxImprovement>0);

  delete[] improvement;

  if ((spinGlassParams->optimalValue>0)&&
      (f-spinGlassParams->optimalValue>0.00001))
    {
      //printf("\n\nWARNING: Found state lot better than expected\n");
    };

  delete[] idx;

  return f;
};

*/
//=================================================================

double spinGlassValueDHC(int *x, SpinGlassParams *spinGlassParams, long &numFlips) {
    int i;
    register int j;

    int     N;
    int    *neighbor;
    double  *interaction;
    double  f;
    int nN;

    double *improvement = new double[spinGlassParams->N];
    double maxImprovement=-1;
    int nmi=0;

    N = spinGlassParams->N;

    f = 0;

    for (i=0; i<N; i++)
        improvement[i]=0;

    for (i=0; i<N; i++) {
        nN          = spinGlassParams->numNeighbors[i];

        if (nN>0) {
            neighbor    = spinGlassParams->neighbors[i];
            interaction = spinGlassParams->interactions[i];

            for (j=0; j<nN; j++) {
                double d=x[i]*x[*neighbor]*(*interaction);
                improvement[i]-=2*d;
                improvement[*neighbor]-=2*d;
                f += d;
                neighbor++;
                interaction++;
            }
        }
    };

    do {

        maxImprovement=0;

        for (i=0; i<N; i++) {
            if (improvement[i]>maxImprovement) {
                maxImprovement=improvement[i];
                nmi=1;
            } else if (improvement[i]==maxImprovement) {
                nmi++;
            }
        };

        if (maxImprovement>0) {
            int which=myRand.uniformInt(0, nmi-1);
            int flip=-1;

            for (i=0; i<N; i++) {
                if (improvement[i]==maxImprovement) {
                    if (which==0)
                        flip=i;
                    else
                        which--;
                }
            }

            x[flip]=-x[flip];
            f+=improvement[flip];

            // update improvements (does this work or what)

            int numFriends = spinGlassParams->numFriends[flip];
            for (i=0; i<numFriends; i++) {
                double c;
                c=spinGlassParams->friendInteractions[flip][i]*x[flip]*x[spinGlassParams->friends[flip][i]];

                improvement[flip]-=4*c;
                improvement[spinGlassParams->friends[flip][i]]-=4*c;
            };

            numFlips++;
        };

    } while (maxImprovement>0);

    delete improvement;

    if ((spinGlassParams->optimalValue>0)&&
            (f-spinGlassParams->optimalValue>0.00001)) {
        //printf("\n\nWARNING: Found state lot better than expected\n");
    };

    return f;
};

//=================================================================
/*
double spinGlassValueDHC_OLD(int *x, SpinGlassParams *spinGlassParams)
{
  int i;
  register int j;

  int     N;
  int    *neighbor;
  double  *interaction;
  double  f;
  int nN;

  double *improvement = new double[spinGlassParams->N];
  double maxImprovement=-1;
  int nmi=0;

  N = spinGlassParams->N;

  f = 0;

  do {

    for (i=0; i<N; i++)
      improvement[i]=0;

    maxImprovement=0;
    f=0;

    for (i=0; i<N; i++)
      {
 nN          = spinGlassParams->numNeighbors[i];

 if (nN>0)
   {
     neighbor    = spinGlassParams->neighbors[i];
     interaction = spinGlassParams->interactions[i];

     for (j=0; j<nN; j++)
       {
  double d=x[i]*x[*neighbor]*(*interaction);
  improvement[i]-=d;
  improvement[*neighbor]-=d;
  f += d;
  neighbor++;
  interaction++;
       }
   }
      };

    for (i=0; i<N; i++)
      {
 if (improvement[i]>maxImprovement)
   {
     maxImprovement=improvement[i];
     nmi=1;
   }
 else
   if (improvement[i]==maxImprovement)
     {
       nmi++;
     }
      };

    if (maxImprovement>0)
      {
 int which=intRand(nmi);
 int flip=-1;

 for (i=0; i<N; i++)
   if (improvement[i]==maxImprovement)
     if (which==0)
              flip=i;
     else
              which--;

 x[flip]=-x[flip];
 f+=improvement[i];
      };

  } while (maxImprovement>0);

  delete improvement;

  return f;
};
*/
//=================================================================
/*
double spinGlassValueDHCRandomized(int *x, SpinGlassParams *spinGlassParams)
{
  int i;
  register int j;

  int     N;
  int    *neighbor;
  double  *interaction;
  double  f;
  int nN;

  double *improvement = new double[spinGlassParams->N];
  double maxImprovement=-1;
  int nmi=0;
  ///  char *mark;

  N            = spinGlassParams->N;
  ///  mark = new char[N];

  f = 0;

  if (drand()<0.25)
    {
      do {

 for (i=0; i<N; i++)
   improvement[i]=0;
 maxImprovement=0;
 f=0;

 for (i=0; i<N; i++)
   {
     nN          = spinGlassParams->numNeighbors[i];

     if (nN>0)
       {
  neighbor    = spinGlassParams->neighbors[i];
  interaction = spinGlassParams->interactions[i];

  for (j=0; j<nN; j++)
    {
      double d=x[i]*x[*neighbor]*(*interaction);
      improvement[i]-=d;
      improvement[*neighbor]-=d;
      f += d;
      neighbor++;
      interaction++;
    }
       }
   };

 for (i=0; i<N; i++)
   {
     if (improvement[i]>maxImprovement)
       {
  maxImprovement=improvement[i];
  nmi=1;
       }
     else
       if (improvement[i]==maxImprovement)
  {
    nmi++;
  }
   };

      if (maxImprovement>0)
 {
   int which=intRand(nmi);
   int flip=-1;

   for (i=0; i<N; i++)
     if (improvement[i]==maxImprovement)
       if (which==0)
  flip=i;
       else
  which--;

   x[flip]=-x[flip];
   f+=improvement[i];
 }

      } while (maxImprovement>0);
    }
  else
    f=spinGlassValueWithWolff(x,spinGlassParams);
//     {
//    for (i=0; i<N; i++)
//     {
//       int newStart;
//       int from  = -1;
//       int start = intRand(N);

//       for (j=0; (j<N)&&(spinGlassParams->numNeighbors[start]>0); j++)
//  {
//    int neighbor;
//    do
//      {
//        neighbor = intRand(spinGlassParams->numNeighbors[start]);
//        newStart = spinGlassParams->neighbors[start][neighbor];
//      } while (newStart==from);

//    x[newStart] =
//      (spinGlassParams->interactions[start][neighbor]>0)?
//      x[start]:(-x[start]);

//    from=start;
//    start=newStart;
//  };

//       f= spinGlassValue(x,spinGlassParams);
//     };
//    };

  delete improvement;

  return f;
};
*/
//=================================================================
/*
double spinGlassValueWithWolff(int *x, SpinGlassParams *spinGlassParams)
{
  int i;
  register int j;

  int     N;
  int    *numNeighbors;
  int   **neighbors;
  double **interactions;
  int    *s;
  int    si;
  int    *neighbor;
  double  *interaction;
  double  f;
  int nN;

  N            = spinGlassParams->N;
  numNeighbors = spinGlassParams->numNeighbors;
  neighbors    = spinGlassParams->neighbors;
  interactions = spinGlassParams->interactions;
  s            = x;

  f = 0;

  if (drand()<WOLFF_PROBABILITY)
    {
      IntStack *stack = new IntStack(N);

      i=intRand(N);
      stack->push(i);
      int value = x[i];
      int invValue = -value;
      x[i]=invValue;

      while (!stack->empty())
 {
   i=stack->pop();
   for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
     {
       int neighbor = spinGlassParams->neighbors[i][j];
       if (x[neighbor]==value)
  {
    x[neighbor]=invValue;
    stack->push(neighbor);
  };
     };
 };

      delete stack;
    };

  for (i=0; i<N; i++)
    {
      si          = *s;
      nN          = *numNeighbors;

      if (nN>0)
 {
   neighbor    = *neighbors;
   interaction = *interactions;

   for (j=0; j<nN; j++)
     {
       f += si*x[*neighbor]*(*interaction);
       neighbor++;
       interaction++;
     }
 }

      s++;
      numNeighbors++;
      neighbors++;
      interactions++;
    };

  return f;
};
*/
//==========================================================
/*
int loadHeinzInteractions(FILE *input, SpinGlassParams *spinGlassParams)
{
  int i,j,num,power,pos;
  char c;

  spinGlassParams->N = pow(spinGlassParams->n,spinGlassParams->d);

  spinGlassParams->numNeighbors = (int*) calloc(spinGlassParams->N,sizeof(int));
  spinGlassParams->neighbors    = (int**) calloc(spinGlassParams->N,sizeof(int*));
  spinGlassParams->interactions = (double**) calloc(spinGlassParams->N,sizeof(double*));

  for (i=0; i<spinGlassParams->N; i++)
    {
      spinGlassParams->numNeighbors[i]=2;

      spinGlassParams->neighbors[i] = (int*) calloc(spinGlassParams->numNeighbors[i],sizeof(int));
      spinGlassParams->interactions[i] = (double*) calloc(spinGlassParams->numNeighbors[i],sizeof(double));

      num = 0;

      for (j=0; j<spinGlassParams->numNeighbors[i]; j++)
 {
   power = pow(spinGlassParams->n,j);
   pos   = (i%(spinGlassParams->n*power))/power;

   if (pos+1<spinGlassParams->n)
     spinGlassParams->neighbors[i][num++] = i+power;
 };

      do {c=fgetc(input); //printf("%c",c);} while ((c<'0')||(c>'3'));

      if (c=='3')
 {
   spinGlassParams->interactions[i][0] = +1;
   spinGlassParams->interactions[i][1] = +1;
 }
      else
 if (c=='2')
   {
     spinGlassParams->interactions[i][0] = -1;
     spinGlassParams->interactions[i][1] = +1;
   }
 else
   if (c=='1')
     {
       spinGlassParams->interactions[i][0] = +1;
       spinGlassParams->interactions[i][1] = -1;
     }
   else
     {
       spinGlassParams->interactions[i][0] = -1;
       spinGlassParams->interactions[i][1] = -1;
     };


      spinGlassParams->numNeighbors[i] = num;
    };

  // get back

  return 0;
};
*/
//====================================================================

int isOptimalSpinGlass(int *x, SpinGlassParams *spinGlassParams) {
    double f;

    if (spinGlassParams->optimalValue<0)
        return 0;

    f=spinGlassValue(x,spinGlassParams);

    return (f>=spinGlassParams->optimalValue-1E-12);
};

//====================================================================

int isOptimalSpinGlassDHC(int *x, SpinGlassParams *spinGlassParams) {
    double f;
    long dummy;

    if (spinGlassParams->optimalValue<0)
        return 0;

    f=spinGlassValueDHC(x,spinGlassParams,dummy);

    return (f>=spinGlassParams->optimalValue-1E-8);
};

//====================================================================

int loadSimonsSpinGlassInstance(FILE *input, SpinGlassParams *spinGlassParams) {
    int i,j,k,l;
    double dummy;
    int dimension=2;

    if (fscanf(input,"%u",&spinGlassParams->n) == 0) return loadSpinGlassInstance(input,spinGlassParams);
    fscanf(input,"%u",&spinGlassParams->N);
    spinGlassParams->d = dimension;
    sprintf(spinGlassParams->ID,"Spin glass %uD - %u nodes",dimension,spinGlassParams->N);

    spinGlassParams->numNeighbors = (int*) calloc(spinGlassParams->N,sizeof(int));

    spinGlassParams->neighbors    = (int**) calloc(spinGlassParams->N,sizeof(int*));

    spinGlassParams->interactions = (double**) calloc(spinGlassParams->N,sizeof(double*));

    for (i=0; i<spinGlassParams->N; i++) {

        spinGlassParams->numNeighbors[i] = (1 << dimension)/2;
        spinGlassParams->neighbors[i] = (int*) calloc(spinGlassParams->numNeighbors[i],sizeof(int));
        spinGlassParams->interactions[i] = (double*) calloc(spinGlassParams->numNeighbors[i],sizeof(double));

        for (j=0; j<(1 << dimension); j++) {
            if (j < spinGlassParams->numNeighbors[i]) {
                fscanf(input,"%lf",&(spinGlassParams->interactions[i][j]));
                spinGlassParams->interactions[i][j] = (spinGlassParams->interactions[i][j] > 0.5) ? +1 : -1;
            } else
                fscanf(input,"%lf",&dummy);
        }

        spinGlassParams->neighbors[i][0] = (i + spinGlassParams->N - spinGlassParams->n) % spinGlassParams->N;
        spinGlassParams->neighbors[i][1] = spinGlassParams->n * (i / spinGlassParams->n) + (i+1) % spinGlassParams->n;
    };

    spinGlassParams->numFriends = (int*) calloc(spinGlassParams->N,sizeof(int));

    spinGlassParams->friends    = (int**) calloc(spinGlassParams->N,sizeof(int*));

    spinGlassParams->friendInteractions = (double**) calloc(spinGlassParams->N,sizeof(double*));

    for (i=0; i<spinGlassParams->N; i++) spinGlassParams->numFriends[i] = 0;

    for (i=0; i<spinGlassParams->N; i++)
        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {
            spinGlassParams->numFriends[i] ++;
            spinGlassParams->numFriends[spinGlassParams->neighbors[i][j]] ++;
        }

    for (i=0; i<spinGlassParams->N; i++) {
        spinGlassParams->friends[i] = (int*) calloc(spinGlassParams->numFriends[i],sizeof(int));
        spinGlassParams->friendInteractions[i] = (double*) calloc(spinGlassParams->numFriends[i],sizeof(double));
    }

    for (i=0; i<spinGlassParams->N; i++) spinGlassParams->numFriends[i] = 0;

    for (i=0; i<spinGlassParams->N; i++)
        for (j=0; j<spinGlassParams->numNeighbors[i]; j++) {
            l = spinGlassParams->neighbors[i][j];
            k = spinGlassParams->numFriends[i] ++;
            spinGlassParams->friends[i][k] = l;
            spinGlassParams->friendInteractions[i][k] = spinGlassParams->interactions[i][j];

            k = spinGlassParams->numFriends[l] ++;
            spinGlassParams->friends[l][k] = i;
            spinGlassParams->friendInteractions[l][k] = spinGlassParams->interactions[i][j];
        }


    fscanf(input,"%lf",&dummy);
    fscanf(input,"%lf",&(spinGlassParams->optimalValue));
    spinGlassParams->optimalValue = -spinGlassParams->optimalValue;

#ifdef DEBUG
    //printf("\n\n\n\n#####################\noptimum:%f\n####################\n\n\n\n\n",spinGlassParams->optimalValue);
#endif

    // get back

    return 0;

};

//=================================================================
/*
double spinGlassValueMCMC(int *x, SpinGlassParams *spinGlassParams, double t, int steps)
{
  int i;
  int N=spinGlassParams->N;

  int where;
  int numFriends;
  int *friends;
  double *friendInteractions;
  double fchange;
  double pAccept;

  for (int step=0; step<steps; step++)
    {
      // decide where to flip

      where=intRand(N);
      numFriends=spinGlassParams->numFriends[where];
      friends=spinGlassParams->friends[where];
      friendInteractions=spinGlassParams->friendInteractions[where];

      fchange=0;
      for (i=0; i<numFriends; i++)
 fchange-=2*x[where]*friendInteractions[i]*x[friends[i]];

      pAccept = (fchange>0)? 1:exp(fchange/t);

      if (drand()<pAccept)
 x[where]=-x[where];
    };

  double f=spinGlassValue(x,spinGlassParams);

  return f;
};
//==========================================================
*/
