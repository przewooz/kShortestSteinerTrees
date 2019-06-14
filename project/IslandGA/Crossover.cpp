#include "Crossover.h"

#include "BinaryCoding.h"
#include "FloatCommandParam.h"

template <class TGenotype>
CCrossover<TGenotype>::CCrossover()
{

}//CCrossover<TGenotype>::CCrossover()

template <class TGenotype>
CCrossover<TGenotype>::CCrossover(float fProbability)
{
	f_probability = fProbability;
}//CCrossover<TGenotype>::CCrossover(float fProbability)

template <class TGenotype>
CCrossover<TGenotype>::~CCrossover()
{

}//CCrossover<TGenotype>::~CCrossover()

template <class TGenotype>
CError CCrossover<TGenotype>::eConfigure(istream *psSettings)
{
	CError c_error;

	CFloatCommandParam p_probability(CROSSOVER_ARGUMENT_PROBABILITY, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX, DEF_PROBABILITY_MAX);
	f_probability = p_probability.fGetValue(psSettings, &c_error);

	return c_error;
}//CError CCrossover<TGenotype>::eConfigure(istream *psSettings)

template class CCrossover<CBinaryCoding>;