///***************************************************************************
// *   Copyright (C) 2015 by Tian-Li Yu                                      *
// *   tianliyu@ntu.edu.tw                                                   *
// ***************************************************************************/
//
//
//#include <math.h>
//#include <iostream>
//#include <cstdlib>
//
//#include "statistics.h"
//#include "dsmga2.h"
//#include "global.h"
//#include "chromosome.h"
//
//using namespace std;
//
//
//int
//main (int argc, char *argv[]) {
//
//
//    if (argc != 10) {
//        //printf ("DSMGA2 ell nInitial function maxGen maxFe repeat display rand_seed maxTime problemConfigFilePath\n");
//        //printf ("function: \n");
//        //printf ("     ONEMAX:  0\n");
//        //printf ("     MK    :  1\n");
//        //printf ("     FTRAP :  2\n");
//        //printf ("     CYC   :  3\n");
//        //printf ("     NK    :  4\n");
//        //printf ("     SPIN  :  5\n");
//        //printf ("     SAT   :  6\n");
//		//printf ("     INJECTED   :  7\n");
//
//        return -1;
//    }
//
//    int ell = atoi (argv[1]); // problem size
//    int nInitial = atoi (argv[2]); // initial population size
//    int fffff = atoi (argv[3]); // function
//    int maxGen = atoi (argv[4]); // max generation
//    int maxFe = atoi (argv[5]); // max fe
//    int repeat = atoi (argv[6]); // how many time to repeat
//    int display = atoi (argv[7]); // display each generation or not
//    int rand_seed = atoi (argv[8]);  // rand seed
//	int maxTime = atoi(argv[9]);
//
//    if (fffff == 4) {
//
//        char filename[200];
//        sprintf(filename, "./NK_Instance/pnk%d_%d_%d_%d", ell, 4, 4, 0);
//
//        //printf("Loading: %s\n", filename);
//        FILE *fp = fopen(filename, "r");
//        loadNKWAProblem(fp, &nkwa);
//        fclose(fp);
//    }
//
//    if (rand_seed != -1)  // time
//        myRand.seed((unsigned long)rand_seed);
//
//    int i;
//
//    Statistics stGen, stFE, stLSFE;
//    int usedGen;
//
//    int failNum = 0;
//
//
//    for (i = 0; i < repeat; i++) {
//
//        DSMGA2 ga (ell, nInitial, maxGen, maxFe, fffff, maxTime, nullptr);
//
//        if (display == 1)
//            usedGen = ga.doIt (true);
//        else
//            usedGen = ga.doIt (false);
//
//
//        if (!ga.foundOptima()) {
//            failNum++;
//            //printf ("-");
//        } else {
//            stFE.record (Chromosome::hitnfe, 0);
//            stLSFE.record (Chromosome::lsnfe, 0);
//            stGen.record (usedGen, 0);
//            //printf ("+");
//        }
//
//        fflush (NULL);
//    }
//
//    //cout << endl;
//    //printf ("\n");
//    //printf ("%f  %f  %f %d\n", stGen.getMean (), stFE.getMean(), stLSFE.getMean(), failNum);
////    freeNKWAProblem(&nkwa);
//
//    return EXIT_SUCCESS;
//}
