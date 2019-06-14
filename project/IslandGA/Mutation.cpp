#include "Mutation.h"

#include "BinaryCoding.h"
#include "FloatCommandParam.h"

template <class TGenotype>
CMutation<TGenotype>::CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	v_init(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired);
}//CMutation<TGenotype>::CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

template <class TGenotype>
CMutation<TGenotype>::CMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	v_init(bPopulationRequired, bBestGenotypeRequired, bBestGenotypesRequired);
	f_probability = fProbability;
}//CMutation<TGenotype>::CMutation(float fProbability, bool bPopulationRequired, bBestGenotypeRequired, bool bBestGenotypesRequired)

template <class TGenotype>
CMutation<TGenotype>::~CMutation()
{
	v_clear_population();
	v_clear_best_genotypes();

	delete pc_best_genotype;
}//CMutation<TGenotype>::~CMutation()

template <class TGenotype>
CError CMutation<TGenotype>::eConfigure(istream * psSettings)
{
	CError c_error;

	CFloatCommandParam p_probability(MUTATION_ARGUMENT_PROBABILITY, DEF_PROBABILITY_MIN, DEF_PROBABILITY_MAX, DEF_PROBABILITY_MAX);
	f_probability = p_probability.fGetValue(psSettings, &c_error);

	return c_error;
}//CError CMutation<TGenotype>::eConfigure(istream * psSettings)

template <class TGenotype>
void CMutation<TGenotype>::vUpdatePopulation(uint32_t iSize, TGenotype **ppcPopulation)
{
	if (b_population_required)
	{
		v_clear_population();

		i_population_size = iSize;
		ppc_population = new TGenotype*[i_population_size];

		TGenotype *pc_genotype_copy;

		for (uint32_t i = 0; i < i_population_size; i++)
		{
			pc_genotype_copy = new TGenotype(*(ppcPopulation + i));
			*(ppc_population + i) = pc_genotype_copy;
		}//for (uint32_t i = 0; i < i_population_size; i++)
	}//if (b_population_required)
}//void CMutation<TGenotype>::(uint32_t iSize, TGenotype **ppcPopulation)

template <class TGenotype>
void CMutation<TGenotype>::vUpdateBestGenotype(TGenotype *pcBestGenotype)
{
	delete pc_best_genotype;
	pc_best_genotype = new TGenotype(pcBestGenotype);
}//void CMutation<TGenotype>::vUpdateBestGenotype(TGenotype *pcBestGenotype)

template <class TGenotype>
void CMutation<TGenotype>::vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes)
{
	if (b_best_genotypes_required)
	{
		v_clear_best_genotypes();

		TGenotype *pc_genotype_copy;

		for (uint8_t i = 0; i < iCount; i++)
		{
			pc_genotype_copy = new TGenotype(*(ppcBestGenotypes + i));
			v_best_genotypes.push_back(pc_genotype_copy);
		}//for (uint8_t i = 0; i < iCount; i++)
	}//if (b_best_genotypes_required)
}//void CMutation<TGenotype>::::vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes)

template <class TGenotype>
void CMutation<TGenotype>::v_clear_population()
{
	if (ppc_population)
	{
		for (uint32_t i = 0; i < i_population_size; i++)
		{
			delete *(ppc_population + i);
		}//for (uint32_t i = 0; i < i_population_size; i++)

		delete ppc_population;
	}//if (ppc_population)
}//void CMutation<TGenotype>::v_clear_population()

template <class TGenotype>
void CMutation<TGenotype>::v_clear_best_genotypes()
{
	for (uint32_t i = 0; i < (uint32_t)v_best_genotypes.size(); i++)
	{
		delete v_best_genotypes.at(i);
	}//for (uint32_t i = 0; i < (uint32_t)v_best_genotypes.size(); i++)

	v_best_genotypes.clear();
}//void CMutation<TGenotype>::v_clear_best_genotypes()

template <class TGenotype>
void CMutation<TGenotype>::v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)
{
	b_population_required = bPopulationRequired;
	b_best_genotype_required = bBestGenotypeRequired;
	b_best_genotypes_required = bBestGenotypesRequired;

	ppc_population = nullptr;
	pc_best_genotype = nullptr;
}//void CMutation<TGenotype>::v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired)

template class CMutation<CBinaryCoding>;