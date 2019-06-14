#include "Optimizer.h"

#include "BinaryCoding.h"
#include "GenerationUtils.h"
#include "StopConditionUtils.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)
{
	pc_problem = pcProblem;

	pc_stop_condition = nullptr;

	pc_log = pcLog;

	pc_empty_generation = GenerationUtils::pcGetEmptyGeneration(pcProblem);
	pc_best_individual = nullptr;

	b_own_params = true;
}//COptimizer<TGenotype, TFenotype>::COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::COptimizer(COptimizer<TGenotype, TFenotype> *pcOther)
{
	pc_problem = pcOther->pc_problem;

	pc_stop_condition = pcOther->pc_stop_condition;

	i_gene_pattern_min_length = pcOther->i_gene_pattern_min_length;
	i_linkage_frequency = pcOther->i_linkage_frequency;

	pc_log = pcOther->pc_log;

	pc_empty_generation = pcOther->pc_empty_generation;
	pc_best_individual = nullptr;

	b_own_params = false;
}//COptimizer<TGenotype, TFenotype>::COptimizer(COptimizer<TGenotype, TFenotype> *pcOther)

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype>::~COptimizer()
{
	v_clear_params();

	if (b_own_params)
	{
		delete pc_empty_generation;
	}//if (b_own_params)

	vResetBestIndividual();
}//COptimizer::~COptimizer()

template <class TGenotype, class TFenotype>
CError COptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error;

	v_clear_params();

	pc_stop_condition = StopConditionUtils::pcGetStopCondition<TGenotype, TFenotype>(pc_problem->pcGetEvaluation(), psSettings, &c_error);

	return c_error;
}//CError COptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	vResetBestIndividual();
}//void COptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vRun()
{
	time_t t_start_time = time(nullptr);
	uint32_t i_iteration_number = 0;

	vInitialize(t_start_time);

	while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual))
	{
		bRunIteration(i_iteration_number, t_start_time);
		i_iteration_number++;
	}//while (!pc_stop_condition->bStop(t_start_time, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE(), pc_best_individual))
}//void COptimizer<TGenotype, TFenotype>::vRun()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vResetBestIndividual()
{
	delete pc_best_individual;
	pc_best_individual = nullptr;
}//void COptimizer<TGenotype, TFenotype>::vResetBestIndividual()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy)
{
	vResetBestIndividual();
	pc_best_individual = bCopy ? new CIndividual<TGenotype, TFenotype>(pcBestIndividual) : pcBestIndividual;
}//void COptimizer<TGenotype, TFenotype>::vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy)

template <class TGenotype, class TFenotype>
bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual)
{
	bool b_updated = false;

	if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcIndividual, pc_best_individual))
	{
		delete pc_best_individual;
		pc_best_individual = new CIndividual<TGenotype, TFenotype>(pcIndividual);

		v_update_statistics_of_best(iIterationNumber, tStartTime);

		b_updated = true;
	}//if (!pc_best_individual || pc_problem->bIsBetterIndividual(pcIndividual, pc_best_individual))

	return b_updated;
}//bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual)

template <class TGenotype, class TFenotype>
bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype)
{
	bool b_updated = false;

	if (!pc_best_individual || pc_problem->bIsBetterFitnessValue(dCurrentBestFitnessValue, pc_best_individual->dGetFitnessValue()))
	{
		if (!pc_best_individual)
		{
			pc_best_individual = pc_create_individual(pc_empty_generation->pcGenerateEmpty());
		}//if (!pc_best_individual)

		fUpdateBestGenotype(pc_best_individual->pcGetGenotype());
		pc_best_individual->vSetFitnessValue(dCurrentBestFitnessValue);

		v_update_statistics_of_best(iIterationNumber, tStartTime);

		b_updated = true;
	}//if (!pc_best_individual || pc_problem->bIsBetterFitnessValue(dCurrentBestFitnessValue, pc_best_individual->dGetFitnessValue()))

	return b_updated;
}//bool COptimizer<TGenotype, TFenotype>::b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype)

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::v_clear_params()
{
	if (b_own_params)
	{
		delete pc_stop_condition;
		pc_stop_condition = nullptr;
	}//if (b_own_params)
}//void COptimizer<TGenotype, TFenotype>::v_clear_params()

template <class TGenotype, class TFenotype>
void COptimizer<TGenotype, TFenotype>::v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime)
{
	t_best_time = time(nullptr) - tStartTime;
	i_best_ffe = pc_problem->pcGetEvaluation()->iGetFFE();
}//void COptimizer<TGenotype, TFenotype>::v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime)

template class COptimizer<CBinaryCoding, CBinaryCoding>;