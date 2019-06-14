/*
 * dsmga2.cpp
 *
 *  Created on: May 2, 2011
 *      Author: tianliyu
 */


#include <list>
#include <vector>
#include <algorithm>
#include <iterator>

#include <iostream>
#include "chromosome.h"
#include "dsmga2.h"
#include "fastcounting.h"
#include "statistics.h"

#include <atlstr.h>

using namespace std;


DSMGA2::DSMGA2 (int n_ell, int n_nInitial, int n_maxGen, int n_maxFe, int fffff, time_t n_maxTime, CLog* n_logger) {


    previousFitnessMean = -INF;
    ell = n_ell;
    nCurrent = (n_nInitial/2)*2;  // has to be even

    Chromosome::function = (Chromosome::Function)fffff;
    Chromosome::nfe = 0;
    Chromosome::lsnfe = 0;
    Chromosome::hitnfe = 0;
    Chromosome::hit = false;

    selectionPressure = 2;
    maxGen = n_maxGen;
    maxFe = n_maxFe;
	maxTime = n_maxTime;

    graph.init(ell);

    maxIndex=1;

    bestIndex = -1;
    masks = new list<int>[ell];
    selectionIndex = new int[nCurrent];
    orderN = new int[nCurrent];
    orderELL = new int[ell];
    population = new Chromosome[nCurrent];
    fastCounting = new FastCounting[ell];

    for (int i = 0; i < ell; i++)
        fastCounting[i].init(nCurrent);


    pHash.clear();
    for (int i=0; i<nCurrent; ++i) {
        population[i].initR(ell);
        double f = population[i].getFitness();
        pHash[population[i].getKey()] = f;
    }

    if (GHC) {
        for (int i=0; i < nCurrent; i++)
            population[i].GHC();
    }

	bestFitness = -INF;
	bestSolution = new int[n_ell];

	logger = n_logger;
}


DSMGA2::~DSMGA2 () {
    delete []masks;
    delete []orderN;
    delete []orderELL;
    delete []selectionIndex;
    delete []population;
    delete []fastCounting;

	delete bestSolution;
}



bool DSMGA2::isSteadyState () {

    if (stFitness.getNumber () <= 0)
        return false;

    if (previousFitnessMean < stFitness.getMean ()) {
        previousFitnessMean = stFitness.getMean () + 1e-6;
        return false;
    }

    return true;
}



int DSMGA2::doIt (bool output) {
    ////cout << "doIt" << endl;
    generation = 0;

	startTime = time(nullptr);

	int iteration = 0;

    while (!shouldTerminate ()) {
        oneRun (iteration, output);
		iteration++;
    }
    return generation;
}


void DSMGA2::oneRun (int iteration, bool output) {
    //cout << "oneRun" << endl;
    
    if (CACHE)
        Chromosome::cache.clear();
    ////cout << "mixing" <<endl;

	//if (iteration < 1000) {
	//	logger->vPrintEmptyLine();

	//	CString s_message;
	//	s_message.Format("population in %d iteration", iteration);

	//	logger->vPrintLine(s_message);

	//	for (int i = 0; i<nCurrent; ++i) {
	//		population[i].printOut(logger);
	//		//cout << endl;
	//	}

	//	logger->vPrintEmptyLine();
	//}

    mixing();
    ////cout<<"nCurrent"<<nCurrent<<endl;
    //for (int i=0;i<nCurrent; ++i) {
    //    //cout <<population[i].getIndex();
    //    //cout << endl;
    //}
    //cout <<"======="<<endl;

    double max = -INF;
    stFitness.reset ();

	double unitation;

    for (int i = 0; i < nCurrent; ++i) {
        double fitness = population[i].getFitness();
        if (fitness > max) {
            max = fitness;
            bestIndex = i;
			unitation = population[i].getUnitation();
        }
        stFitness.record (fitness, unitation);

        if (SHOW_POPULATION) {
            if (SHORT_HAND)
                population[i].printOut(logger);
            else
                population[i].shortPrintOut();
            //cout << endl;
        }

		if (max > bestFitness) {
			bestFitness = max;
			bestTime = time(nullptr) - startTime;
			bestFFE = Chromosome::lsnfe + Chromosome::nfe;

			for (int i = 0; i < ell; i++) {
				*(bestSolution + i) = population[bestIndex].getVal(i);
			}
		}
    }

    if (output)
        showStatistics ();
    //cout << "_maxIndex"<<maxIndex;
    ++generation;
}


bool DSMGA2::shouldTerminate () {
    bool
    termination = false;

	time_t elapsedTime = time(nullptr) - startTime;

    if (maxFe != -1) {
        if (Chromosome::nfe > maxFe)
            termination = true;
    }

    if (maxGen != -1) {
        if (generation > maxGen)
            termination = true;
    }

	if (maxTime != -1) {
		if (elapsedTime >= maxTime) {
			termination = true;
		}
	}

    if (population[0].getMaxFitness() <= stFitness.getMax() )
        termination = true;


    //if (stFitness.getMax() - 1e-10 <= stFitness.getMean() )
    //    termination = true;

	if (generation > 0 && logger) {
		CString log_message;
		log_message.AppendFormat("best fitness: %f; best unitation: %f; ffe: %u; time: %u", stFitness.getMax(), stFitness.getUnitation(), 
			Chromosome::lsnfe + Chromosome::nfe, (unsigned int)elapsedTime);

		logger->vPrintLine(log_message, true);
	}

    return termination;

}


bool DSMGA2::foundOptima () {
    return (stFitness.getMax() > population[0].getMaxFitness());
}


void DSMGA2::showStatistics () {

    //printf ("Gen:%d  Fitness:(Max/Mean/Min):%f/%f/%f ",
            //generation, stFitness.getMax (), stFitness.getMean (),
            //stFitness.getMin ());
    //printf ("best chromosome:");
    //population[bestIndex].printOut();
    //printf ("\n");


    fflush(NULL);
}



void DSMGA2::buildFastCounting() {

    if (SELECTION) {
        for (int i = 0; i < nCurrent; i++)
            for (int j = 0; j < ell; j++) {
                fastCounting[j].setVal(i, population[selectionIndex[i]].getVal(j));
            }

    } else {
        for (int i = 0; i < nCurrent; i++)
            for (int j = 0; j < ell; j++) {
                fastCounting[j].setVal(i, population[i].getVal(j));
            }
    }

}

int DSMGA2::countOne(int x) const {

    int n = 0;

    for (int i=0; i<fastCounting[0].lengthLong; ++i) {
        unsigned long val = 0;

        val = fastCounting[x].gene[i];

        n += myBD.countOne(val);
    }

    return n;
}


int DSMGA2::countXOR(int x, int y) const {

    int n = 0;

    for (int i=0; i<fastCounting[0].lengthLong; ++i) {
        unsigned long val = 0;


        val = fastCounting[x].gene[i];

        val ^= fastCounting[y].gene[i];

        n += myBD.countOne(val);
    }

    return n;
}


void DSMGA2::restrictedMixing(Chromosome& ch) {
    //cout << "_rm";
    int r = myRand.uniformInt(0, ell-1);
    
    list<int> mask = masks[r];

    size_t size = findSize(ch, mask);
    if (size > (size_t)ell/2)
        size = ell/2;

    // prune mask to exactly size
    while (mask.size() > size)
        mask.pop_back();


    bool taken = restrictedMixing(ch, mask);

    EQ = true;
    if (taken) {

        genOrderN();

        for (int i=0; i<nCurrent; ++i) {
            //population[orderN[i]].getIndex()==maxIndex || population[orderN[i]].getIndex()==maxIndex-1
            if(population[orderN[i]].getIndex()==maxIndex || population[orderN[i]].getIndex()==maxIndex-1) {
                if (EQ)
                    backMixingE(ch, mask, population[orderN[i]]);
                else
                    backMixing(ch, mask, population[orderN[i]]);
            } 
        }
    }
    for (int i=0; i<nCurrent; ++i) {
        int max=maxIndex;
        if (population[orderN[i]].getIndex() > max)
            max = population[orderN[i]].getIndex();
        maxIndex = max;
    }

}

void DSMGA2::backMixing(Chromosome& source, list<int>& mask, Chromosome& des) {
    //cout <<"_bm";
    Chromosome trial(ell);
    trial = des;
    for (list<int>::iterator it = mask.begin(); it != mask.end(); ++it)
        trial.setVal(*it, source.getVal(*it));

    if (trial.getFitness() > des.getFitness()) {
        pHash.erase(des.getKey());
        pHash[trial.getKey()] = trial.getFitness();
        //int i=des.getIndex();
        //cout <<des.index;
        des.index++;
        des = trial;
        return;
    }

}

void DSMGA2::backMixingE(Chromosome& source, list<int>& mask, Chromosome& des) {
    //cout <<"_bme";
    Chromosome trial(ell);
    trial = des;
    for (list<int>::iterator it = mask.begin(); it != mask.end(); ++it)
        trial.setVal(*it, source.getVal(*it));

    if (trial.getFitness() > des.getFitness()) {
        pHash.erase(des.getKey());
        pHash[trial.getKey()] = trial.getFitness();

        EQ = false;
        //des.getIndex()++;
        des = trial;
        return;
    }

    if (trial.getFitness() >= des.getFitness()) {
        pHash.erase(des.getKey());
        pHash[trial.getKey()] = trial.getFitness();

        des = trial;
        return;
    }

}

bool DSMGA2::restrictedMixing(Chromosome& ch, list<int>& mask) {
    //cout << "_rmbool"<<endl;
    bool taken = false;
    size_t lastUB = 0;

    for (size_t ub = 1; ub <= mask.size(); ++ub) {

        size_t size = 1;
        Chromosome trial(ell);
        trial = ch;

        for (list<int>::iterator it = mask.begin(); it != mask.end(); ++it) {

            trial.flip(*it);

            ++size;
            if (size > ub) break;
        }

        if (isInP(trial)) continue;


        if (trial.getFitness() >= ch.getFitness()) {
            pHash.erase(ch.getKey());
            pHash[trial.getKey()] = trial.getFitness();

            taken = true;
            ch = trial;
        }

        if (taken) {
            lastUB = ub;
            break;
        }
    }

    if (lastUB != 0) {
        while (mask.size() > lastUB)
            mask.pop_back();
    }

    return taken;

}

size_t DSMGA2::findSize(Chromosome& ch, list<int>& mask) const {
    //cout <<"_Findsize";
    DLLA candidate(nCurrent);
    for (int i=0; i<nCurrent; ++i)
        candidate.insert(i);

    size_t size = 0;
    for (list<int>::iterator it = mask.begin(); it != mask.end(); ++it) {

        int allele = ch.getVal(*it);

        for (DLLA::iterator it2 = candidate.begin(); it2 != candidate.end(); ++it2) {
            if (population[*it2].getVal(*it) == allele)
                candidate.erase(*it2);

            if (candidate.isEmpty())
                break;
        }

        if (candidate.isEmpty())
            break;

        ++size;
    }

    return size;


}

size_t DSMGA2::findSize(Chromosome& ch, list<int>& mask, Chromosome& ch2) const {
    //cout <<"_findsize2";
    size_t size = 0;
    for (list<int>::iterator it = mask.begin(); it != mask.end(); ++it) {
        if (ch.getVal(*it) == ch2.getVal(*it)) break;
        ++size;
    }
    return size;
}

void DSMGA2::mixing() {
    //cout << "_mixing";
    if (SELECTION)
        selection();

    //* really learn model
    buildFastCounting();
    buildGraph();

    for (int i=0; i<ell; ++i)
        findClique(i, masks[i]);

    int repeat = (ell>50)? ell/50: 1;

    for (int k=0; k<repeat; ++k) {

        genOrderN();
        for (int i=0; i<nCurrent; ++i) {
            //population[orderN[i]].getIndex()==maxIndex
            if(1) {
                restrictedMixing(population[orderN[i]]);
                if (Chromosome::hit) break;
            }
            
        }
        if (Chromosome::hit) break;
    }


}

inline bool DSMGA2::isInP(const Chromosome& ch) const {

    unordered_map<unsigned long, double>::const_iterator it = pHash.find(ch.getKey());
    return (it != pHash.end());
}

inline void DSMGA2::genOrderN() {
    myRand.uniformArray(orderN, nCurrent, 0, nCurrent-1);
}

inline void DSMGA2::genOrderELL() {
    myRand.uniformArray(orderELL, ell, 0, ell-1);
}

void DSMGA2::buildGraph() {
    //cout << "_buildGraph";
    int *one = new int [ell];
    for (int i=0; i<ell; ++i) {
        one[i] = countOne(i);
    }

    for (int i=0; i<ell; ++i) {

        for (int j=i+1; j<ell; ++j) {

            int n00, n01, n10, n11;
            int nX =  countXOR(i, j);

            n11 = (one[i]+one[j]-nX)/2;
            n10 = one[i] - n11;
            n01 = one[j] - n11;
            n00 = nCurrent - n01 - n10 - n11;

            double p00 = (double)n00/(double)nCurrent;
            double p01 = (double)n01/(double)nCurrent;
            double p10 = (double)n10/(double)nCurrent;
            double p11 = (double)n11/(double)nCurrent;

            double linkage;
            linkage = computeMI(p00,p01,p10,p11);
            graph.write(i,j,linkage);
        }
    }

    if (SHOW_GRAPH) {
        //for (int i=0; i<ell; ++i) {
            //for (int j=0; j<ell; ++j)
                //printf("%3f ", graph(i,j));
            //printf("\n");
       //}
    }

    delete []one;

}

// from 1 to ell, pick by max edge
void DSMGA2::findClique(int startNode, list<int>& result) {
    //cout << "_findClique";

    result.clear();

    DLLA rest(ell);
    genOrderELL();
    for (int i=0; i<ell; ++i) {
        if (orderELL[i]==startNode)
            result.push_back(orderELL[i]);
        else
            rest.insert(orderELL[i]);
    }

    double *connection = new double[ell];

    for (DLLA::iterator iter = rest.begin(); iter != rest.end(); ++iter)
        connection[*iter] = graph(startNode, *iter);

    while (!rest.isEmpty()) {

        double max = -INF;
        int index = -1;
        for (DLLA::iterator iter = rest.begin(); iter != rest.end(); ++iter) {
            if (max < connection[*iter]) {
                max = connection[*iter];
                index = *iter;
            }
        }

        rest.erase(index);
        result.push_back(index);

        for (DLLA::iterator iter = rest.begin(); iter != rest.end(); ++iter)
            connection[*iter] += graph(index, *iter);
    }


    delete []connection;

}

double DSMGA2::computeMI(double a00, double a01, double a10, double a11) const {

    double p0 = a00 + a01;
    double q0 = a00 + a10;
    double p1 = 1-p0;
    double q1 = 1-q0;

    double join = 0.0;
    if (a00 > 1e-10)
        join += a00*log(a00);
    if (a01 > 1e-10)
        join += a01*log(a01);
    if (a10 > 1e-10)
        join += a10*log(a10);
    if (a11 > 1e-10)
        join += a11*log(a11);

    double p = 0.0;
    if (p0 > 1e-10)
        p += p0*log(p0);
    if (p1 > 1e-10)
        p += p1*log(p1);


    double q = 0.0;
    if (q0 > 1e-10)
        q += q0*log(q0);
    if (q1 > 1e-10)
        q += q1*log(q1);

    return -p-q+join;

}


void DSMGA2::selection () {
    tournamentSelection ();
}


// tournamentSelection without replacement
void DSMGA2::tournamentSelection () {
    int i, j;

    int *randArray = new int[selectionPressure * nCurrent];

    for (i = 0; i < selectionPressure; i++)
        myRand.uniformArray (randArray + (i * nCurrent), nCurrent, 0, nCurrent - 1);

    for (i = 0; i < nCurrent; i++) {

        int winner = 0;
        double winnerFitness = -INF;

        for (j = 0; j < selectionPressure; j++) {
            int challenger = randArray[selectionPressure * i + j];
            double challengerFitness = population[challenger].getFitness();

            if (challengerFitness > winnerFitness) {
                winner = challenger;
                winnerFitness = challengerFitness;
            }

        }
        selectionIndex[i] = winner;
    }

	delete randArray;
}

void DSMGA2::getResult(int * bestFFE, time_t * bestTime, int * bestSolution) {
	*bestFFE = DSMGA2::bestFFE;
	*bestTime = DSMGA2::bestTime;

	for (int i = 0; i < ell; i++) {
		*(bestSolution + i) = *(DSMGA2::bestSolution + i);
	}
}