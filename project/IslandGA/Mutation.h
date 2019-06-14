#ifndef MUTATION_H
#define MUTATION_H

#define MUTATION_ARGUMENT_TYPE "mutation_type"
#define MUTATION_ARGUMENT_PROBABILITY "mutation_probability"

#include "Error.h"
#include "RandUtils.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace std;


enum EMutationType
{

};//enum EMutationType


template <class TGenotype>
class CMutation
{
public:
	CMutation(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	CMutation(float fProbability, bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
	
	virtual ~CMutation();

	virtual CError eConfigure(istream *psSettings);

	virtual bool bMutate(TGenotype *pcGenotype, uint32_t iIndex) = 0;

	virtual void vUpdatePopulation(uint32_t iSize, TGenotype **ppcPopulation);
	virtual void vUpdateBestGenotype(TGenotype *pcBestGenotype);
	virtual void vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes);

	bool bPopulationRequired() { return b_population_required; };
	bool bBestGenotypeRequired() { return b_best_genotype_required; };
	bool bBestGenotypesRequired() { return b_best_genotypes_required; };

protected:
	virtual bool b_apply_mutation(TGenotype *pcGenotype) { return RandUtils::bSatisfyProbability(f_probability); };

	void v_clear_population();
	void v_clear_best_genotypes();

	float f_probability;

	bool b_population_required;
	bool b_best_genotype_required;
	bool b_best_genotypes_required;

	uint32_t i_population_size;
	TGenotype **ppc_population;

	TGenotype *pc_best_genotype;

	vector<TGenotype*> v_best_genotypes;

private:
	void v_init(bool bPopulationRequired, bool bBestGenotypeRequired, bool bBestGenotypesRequired);
};//class CMutation

#endif//MUTATION_H