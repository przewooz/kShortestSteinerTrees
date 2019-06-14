#ifndef CROSSOVER_UTILS_H
#define CROSSOVER_UTILS_H

#include "BinaryCoding.h"
#include "Crossover.h"
#include "Error.h"

#include <istream>

using namespace std;

namespace CrossoverUtils
{
	template <class TGenotype>
	CCrossover<TGenotype> *pcGetCrossover(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace CrossoverUtils

#endif//CROSSOVER_UTILS_H