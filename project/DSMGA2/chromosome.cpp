/***************************************************************************
 *   Copyright (C) 2015 by TEIL                                            *
 ***************************************************************************/

#include <cstdint>
#include <cstdio>
#include <cstring>
#include "spin-glass.h"
#include "chromosome.h"
#include "nk-wa.h"
#include "sat.h"

#include <atlstr.h>

#define TRAP_K 5

Chromosome::Chromosome () {
    length = 0;
    index = 0;
    lengthLong = 0;
    gene = NULL;
    evaluated = false;
}

Chromosome::Chromosome (int n_length) {
    gene = NULL;
    init (n_length);
}


Chromosome::~Chromosome () {
    if (gene != NULL) delete []gene;
}

void Chromosome::init (int _length) {
    length = _length;
    lengthLong = quotientLong(length)+1;

    if (gene != NULL)
        delete []gene;

    gene = new unsigned long [lengthLong];
    gene[lengthLong-1] = 0;

    evaluated = false;
}

void Chromosome::init0 (int _length) {
    length = _length;
    lengthLong = quotientLong(length)+1;

    if (gene != NULL)
        delete []gene;

    gene = new unsigned long [lengthLong];

    for (int i=0; i<lengthLong; ++i)
        gene[i] = 0;

    key = 0;
    evaluated = false;
}

void Chromosome::initR (int _length) {
    length = _length;
    lengthLong = quotientLong(length)+1;

    if (gene != NULL)
        delete []gene;

    gene = new unsigned long [lengthLong];
    gene[lengthLong-1] = 0;

    key = 0;
    for (int i=0; i<length; ++i) {

        int val = myRand.flip();
        setValF(i, val);
        if (val == 1)
            key ^= zKey[i];
    }

    evaluated = false;
}

double Chromosome::getFitness () {
    if (evaluated)
        return fitness;
    else {
        fitness = evaluate();
        if (!hit && fitness > getMaxFitness()) {
            hit = true;
            hitnfe = nfe+lsnfe;
        }
        return fitness;
    }
}

double Chromosome::getUnitation() {
	double unitation = 0;

	for (int i = 0; i < length; ++i) {
		if (getVal(i) == 1) {
			unitation++;
		}
	}

	return unitation / (double)length;
}

bool Chromosome::isEvaluated () const {
    return evaluated;
}

bool Chromosome::hasSeen() const {

    unordered_map<unsigned long, double>::iterator it = cache.find(key);
    if (it != cache.end())
        return true;
    return false;
}

double Chromosome::evaluate () {


    if (CACHE)
        if (hasSeen()) {
            evaluated = true;
            return cache[key];
        }

    ++nfe;
    evaluated = true;
    double accum = 0.0;

    switch (function) {
        case ONEMAX:
            accum = oneMax();
            break;
        case MKTRAP:
            accum = mkTrap(1, 0.8);
            break;
        case CYCTRAP:
            accum = cycTrap(1, 0.8);
            break;
        case FTRAP:
            accum = fTrap();
            break;
        case SPINGLASS:
            accum = spinGlass();
            break;
        case NK:
            accum = nkFitness();
            break;
        case SAT:
            accum = satFitness();
            break;
		case INJECTED:
			accum = injectedFitness();
			break;
        default:
            accum = mkTrap(1, 0.8);
            break;
    }

    if (CACHE)
        cache[key]=accum;

    return accum;

}



double
Chromosome::spinGlass () const {

    int *x = new int[length];
    double result;

    for (int i=0; i<length; i++)
        if (getVal(i) == 1)
            x[i] = 1;
        else
            x[i] = -1;

    result = spinGlassValue(x, &mySpinGlassParams);

    delete []x;

    return result;
}

double Chromosome::nkFitness() const {
    char *x = new char[length];

    for ( int i = 0; i < length; ++i) {
        x[i] = (char) getVal(i);
    }

    double result = evaluateNKProblem(x, &nkwa);
    //double result = evaluateNKWAProblem(x, &nkwa);
    delete []x;
    return result;
}

// OneMax
double Chromosome::oneMax () const {

    double result = 0;

    for (int i = 0; i < length; ++i)
        result += getVal(i);

    return result;
}

bool Chromosome::operator== (const Chromosome& c) const {
    if (length != c.length)
        return false;

    for (int i=0; i<lengthLong; i++)
        if (gene[i] != c.gene[i])
            return false;

    return true;
}

Chromosome& Chromosome::operator= (const Chromosome& c) {

    if (length != c.length) {
        length = c.length;
        init (length);
    }

    evaluated = c.evaluated;
    fitness = c.fitness;
    lengthLong = c.lengthLong;
    key = c.key;

    memcpy(gene, c.gene, sizeof(long) * lengthLong);

    return *this;
}

double Chromosome::trap (int unitary, double fHigh, double fLow, int trapK) const {
    if (unitary > trapK)
        return 0;

    if (unitary == trapK)
        return fHigh;
    else
        return fLow - unitary * fLow / (trapK-1);
}


double Chromosome::fTrap() const {

    double result = 0.0;

    for (int i=0; i<length/6; ++i) {
        int u=0;
        for (int j=0; j<6; ++j)
            u += getVal(i*6+j);

        if (u==0)
            result += 1.0;
        else if (u==1)
            result += 0.0;
        else if (u==2)
            result += 0.4;
        else if (u==3)
            result += 0.8;
        else if (u==4)
            result += 0.4;
        else if (u==5)
            result += 0.0;
        else // u == 6
            result += 1.0;
    }

    return result;
}

double Chromosome::cycTrap(double fHigh, double fLow) const {
    int i, j;
    int u;
    int TRAP_M = length / (TRAP_K-1);
    if (length % (TRAP_K-1) != 0)
        outputErrMsg ("TRAP_k doesn't divide length for Cyclic Setting");
    double result = 0;
    for (i = 0; i < TRAP_M; i++) {
        u = 0;
        int idx = i * TRAP_K - i;
        for (j = 0; j < TRAP_K; j++) {
            int pos = idx + j;
            if (pos == length)
                pos = 0;
            else if (pos > length)
                outputErrMsg ("CYCLIC BUG");
            //
            u += getVal(pos);
        }
        result += trap (u, fHigh, fLow, TRAP_K);
    }
    return result;
}



double Chromosome::mkTrap (double fHigh, double fLow) const {
    int i, j;
    int u;

    int TRAP_M = length / TRAP_K;

    if (length % TRAP_K != 0)
        outputErrMsg ("TRAP_K doesn't divide length");

    double result = 0;

    for (i = 0; i < TRAP_M; i++) {
        u = 0;
        for (j = 0; j < TRAP_K; j++)
            u += getVal(i * TRAP_K + j);

        result += trap (u, fHigh, fLow, TRAP_K);
    }

    return result;
}

void Chromosome::printOut (CLog *log) const {
    int i;

	CString s_chromosome;

	for (i = 0; i < length; i++)
		s_chromosome.AppendFormat("%d", getVal(i));

	log->vPrintLine(s_chromosome);

	//for (i = 0; i < length; i++)
        //printf ("%d", getVal(i));
}

void Chromosome::shortPrintOut () const {
    int i, j;
    int u;

    int TRAP_M = length / TRAP_K;

    for (i = 0; i < TRAP_M; i++) {
        u = 0;
        for (j = 0; j < TRAP_K; j++)
            u += getVal(i * TRAP_K + j);
		
        //if (u == TRAP_K)
            //:://printf ("1");
        //else if (u == 0)
            //:://printf ("0");
        //else
            //:://printf ("*");

    }
}

int Chromosome::getLength () const {
    return length;
}

int Chromosome::getIndex () const {
    return index;
}

double Chromosome::getMaxFitness () const {

    double maxF;

    switch (function) {
        case ONEMAX:
            maxF = length;
            break;
        case MKTRAP:
            maxF = length/TRAP_K;
            break;
        case FTRAP:
            maxF = length/6;
            break;
        case CYCTRAP:
            maxF =  length/(TRAP_K - 1);
            break;
        case SPINGLASS:
            maxF = mySpinGlassParams.optimalValue;
            break;
        case NK:
            maxF = nkwa.maxF;
            break;
        case SAT:
            maxF = 0;
            break;
		case INJECTED:
			maxF = injectedEvaluation->dGetMaxValue();
			break;
        default:
            // Never converge
            maxF = INF;
    }

    return maxF - EPSILON;

}

void Chromosome::setEvaluated(bool evaluated) {
	this->evaluated = evaluated;
}

// contribute to lsnfe
bool Chromosome::tryFlipping(int index) {
    int oldNFE = nfe;

    double oldF = getFitness();
    flip(index);

    if (getFitness() <= oldF) {
        flip(index);
        evaluated = true;
        fitness = oldF;

        lsnfe += nfe - oldNFE;
        nfe = oldNFE;

        return false;
    } else {

        lsnfe += nfe - oldNFE;
        nfe = oldNFE;

        return true;
    }


}

bool Chromosome::GHC() {

    int* order = new int [length];
    myRand.uniformArray(order, length, 0, length-1);

    bool flag = false;
    for (int i=0; i<length; ++i) {
        if (tryFlipping(order[i])) flag = true;
    }

    delete []order;
    return flag;

}

double Chromosome::satFitness() const {
    int *x = new int[length];

    for ( int i = 0; i < length; ++i) {
        x[i] = getVal(i);
    }

    double result = evaluateSAT(x, &mySAT);
    delete []x;
    return result;
}

double Chromosome::injectedFitness() const {
	CBinaryCoding individual((uint16_t)length);

	for (int i = 0; i < length; ++i) {
		individual.piGetBits()[i] = (int32_t)getVal(i);
	}

	double result = injectedEvaluation->dEvaluate(&individual);

	return result;
}