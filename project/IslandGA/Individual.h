#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Crossover.h"
#include "Evaluation.h"
#include "Generation.h"
#include "Mutation.h"
#include "Transformation.h"

#include <cstdint>

using namespace std;

template <class TGenotype, class TFenotype> class CProblem;

template <class TFenotype> class CBinaryGenePatternRandomMigrationReplacement;
template <class TFenotype> class CBinaryGenePatternBestMigrationReplacement;

template <class TGenotype, class TFenotype>
class CIndividual
{
friend class CBinaryGenePatternRandomMigrationReplacement<TFenotype>;
friend class CBinaryGenePatternBestMigrationReplacement<TFenotype>;

public:
	CIndividual(TGenotype *pcGenotype, CEvaluation<TFenotype> *pcEvaluation, CCrossover<TGenotype> *pcCrossover,
		CMutation<TGenotype> *pcMutation, CTransformation<TGenotype, TFenotype> *pcTransformation);
	CIndividual(TGenotype *pcGenotype, CProblem<TGenotype, TFenotype> *pcProblem);
	CIndividual(CIndividual<TGenotype, TFenotype> *pcOther);
	
	~CIndividual();

	void vEvaluate();
	void vCrossover(CIndividual<TGenotype, TFenotype> *pcParent0, CIndividual<TGenotype, TFenotype> *pcChild0, 
		CIndividual<TGenotype, TFenotype> *pcChild1)
	{
		pc_crossover->vCrossover(this->pc_genotype, pcParent0->pc_genotype, pcChild0->pc_genotype, pcChild1 ? pcChild1->pc_genotype : nullptr);
	};
	void vMutate(uint32_t iIndex);

	TGenotype *pcGetGenotype() { return pc_genotype; };
	TFenotype *pcGetFenotype() { return pc_fenotype; };

	double dGetFitnessValue() { return d_fitness_value; };
	void vSetFitnessValue(double dFitnessValue, bool bIsEvaluated = true);

	bool bIsEvaluated() { return b_is_evaluated; };
	void vIsEvaluated(bool bIsEvaluated) { b_is_evaluated = bIsEvaluated; };

private:
	void v_transform_genotype();

	double d_fitness_value;

	bool b_is_evaluated;

	TGenotype *pc_genotype;
	TFenotype *pc_fenotype;

	CEvaluation<TFenotype> *pc_evaluation;
	CCrossover<TGenotype> *pc_crossover;
	CMutation<TGenotype> *pc_mutation;

	CTransformation<TGenotype, TFenotype> *pc_transformation;
};//class CIndividual

#endif//INDIVIDUAL_H