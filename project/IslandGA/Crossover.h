#ifndef CROSSOVER_H
#define CROSSOVER_H

#define CROSSOVER_ARGUMENT_TYPE "crossover_type"
#define CROSSOVER_ARGUMENT_PROBABILITY "crossover_probability"

#include "Error.h"
#include "RandUtils.h"

#include <istream>

using namespace std;


enum ECrossoverType
{

};//enum ECrossoverType


template <class TGenotype>
class CCrossover
{
public:
	CCrossover();
	CCrossover(float fProbability);

	virtual ~CCrossover();

	virtual CError eConfigure(istream *psSettings);

	virtual bool bApplyCrossover(TGenotype *pcGenotype) { return RandUtils::bSatisfyProbability(f_probability); };
	virtual void vCrossover(TGenotype *pcParent0, TGenotype *pcParent1, TGenotype *pcChild0, TGenotype *pcChild1) = 0;

protected:
	float f_probability;
};//class CCrossover

#endif//CROSSOVER_H