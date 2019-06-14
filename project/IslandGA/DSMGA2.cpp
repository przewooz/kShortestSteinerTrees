#include "DSMGA2.h"

#include "BinaryEvaluation.h"
#include "Optimizer.h"
#include "UIntCommandParam.h"

#include "../DSMGA2/chromosome.h"
#include "../DSMGA2/dsmga2.h"

#include <atlstr.h>
#include <list>

CDSMGA2::CDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)
	: COptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog)
{
	pc_dsmga2 = nullptr;
	Chromosome::injectedEvaluation = pcProblem->pcGetEvaluation();
}//CDSMGA2::CDSMGA2(CDSMGA2Params *pcParams, CLog *pcLog)

CDSMGA2::CDSMGA2(CDSMGA2 *pcOther)
	: COptimizer<CBinaryCoding, CBinaryCoding>(pcOther)
{
	pc_dsmga2 = nullptr;
	i_population_size = pcOther->i_population_size;
}//CDSMGA2::CDSMGA2(CDSMGA2 *pcOther)

CDSMGA2::~CDSMGA2()
{
	delete pc_dsmga2;
}//CDSMGA2::~CDSMGA2()

CError CDSMGA2::eConfigure(istream *psSettings)
{
	CError c_error = COptimizer<CBinaryCoding, CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_population_size(POPULATION_OPTIMIZER_ARGUMENT_POPULATION_SIZE);
		i_population_size = p_population_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CDSMGA2::eConfigure(istream *psSettings)

void CDSMGA2::vInitialize(time_t tStartTime)
{
	COptimizer<CBinaryCoding, CBinaryCoding>::vInitialize(tStartTime);

	delete pc_dsmga2;
	pc_dsmga2 = new DSMGA2(pc_problem->pcGetEvaluation()->iGetNumberOfElements(), i_population_size, -1, -1, 7, -1, pc_log);
}//void CDSMGA2::vInitialize(time_t tStartTime)

bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	pc_dsmga2->oneRun((int)iIterationNumber, false);

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime);

	CString s_log_message;
	s_log_message.Format("best fitness: %f; best unitation: %f; ffe: %u; time: %u", pc_best_individual->dGetFitnessValue(), pc_best_individual->pcGetGenotype()->dGetUnitation(),
		pc_problem->pcGetEvaluation()->iGetFFE(), (uint32_t)(time(nullptr) - tStartTime));

	pc_log->vPrintEmptyLine();
	pc_log->vPrintLine(s_log_message, true);
	pc_log->vPrintEmptyLine();

#ifdef EVALUATION_BINARY_STEINER_TREE_SET_DYNAMIC
	CBinarySteinerTreeSetDynamicEvaluation *pc_steiner_tree_set;

	if (pc_steiner_tree_set = dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))
	{
		pc_steiner_tree_set->vChange();

		pc_dsmga2->bestFitness = -INF;

		Chromosome::hit = false;

		Chromosome *pc_chromosome;

		for (int i = 0; i < pc_dsmga2->nCurrent; i++)
		{
			pc_chromosome = (pc_dsmga2->population + i);
			pc_chromosome->setEvaluated(false);
			//pc_dsmga2->pHash[pc_chromosome->getKey()] = pc_chromosome->getFitness();
		}//for (int i = 0; i < pc_dsmga2->nCurrent; i++)
	}//if (pc_steiner_tree_set = dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))
#endif//EVALUATION_BINARY_STEINER_TREE_SET_DYNAMIC

	return b_updated;
}//bool CDSMGA2::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)

bool CDSMGA2::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)
{
	double d_best_individual_candidate_fitness = pc_dsmga2->bestFitness;

	CBinarySteinerTreeSetDynamicEvaluation *pc_steiner_tree_set;

	if (pc_steiner_tree_set = dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))
	{
		d_best_individual_candidate_fitness = -pc_steiner_tree_set->dGetWorstTree();

		if (!pc_steiner_tree_set->bIsFull())
		{
			d_best_individual_candidate_fitness = -DBL_MAX;
		}//if (!pc_steiner_tree_set->bIsFull())
	}//if (pc_steiner_tree_set = dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))

	bool b_updated = b_update_best_individual(iIterationNumber, tStartTime, d_best_individual_candidate_fitness, [&](CBinaryCoding *pcBestGenotype)
	{
		for (uint16_t i = 0; i < pc_problem->pcGetEvaluation()->iGetNumberOfElements(); i++)
		{
			*(pcBestGenotype->piGetBits() + i) = (int32_t)*(pc_dsmga2->bestSolution + i);
		}//for (uint16_t i = 0; i < pc_problem->pcGetEvaluation()->iGetNumberOfElements(); i++)
	});

	return b_updated;
}//bool CDSMGA2::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime)