#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#define OPTIMIZER_ARGUMENT_TYPE "optimizer_type"
#define OPTIMIZER_ARGUMENT_TYPE_P3 "p3"
#define OPTIMIZER_ARGUMENT_TYPE_LTGA "ltga"
#define OPTIMIZER_ARGUMENT_TYPE_DSMGA2 "dsmga2"

#define OPTIMIZER_ARGUMENT_CONFIG_FILE_PATH "optimizer_config_file_path"

#define POPULATION_OPTIMIZER_ARGUMENT_POPULATION_SIZE "population_size"

#include "Error.h"
#include "Generation.h"
#include "Individual.h"
#include "Log.h"
#include "Problem.h"
#include "StopCondition.h"

#include <cstdint>
#include <ctime>
#include <functional>
#include <istream>

using namespace std;


enum EOptimizerType
{
	OPTIMIZER_P3 = 0,
	OPTIMIZER_LTGA = 1,
	OPTIMIZER_DSMGA2 = 2
};//enum EOptimizerType


template <class TGenotype, class TFenotype>
class COptimizer
{
public:
	COptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog);
	COptimizer(COptimizer<TGenotype, TFenotype> *pcOther);

	virtual ~COptimizer();

	virtual COptimizer<TGenotype, TFenotype> *pcCopy() = 0;

	virtual CError eConfigure(istream *psSettings);

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime) = 0;

	virtual void vRun();

	void vResetBestIndividual();

	CStopCondition<TGenotype, TFenotype> *pcGetStopCondition() { return pc_stop_condition; };

	CIndividual<TGenotype, TFenotype> *pcGetBestIndividual() { return pc_best_individual; };
	void vSetBestIndividual(CIndividual<TGenotype, TFenotype> *pcBestIndividual, bool bCopy = true);

	time_t tGetBestTime() { return t_best_time; };
	uint64_t iGetBestFFE() { return i_best_ffe; };

protected:
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, CIndividual<TGenotype, TFenotype> *pcIndividual);
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime, double dCurrentBestFitnessValue, function<void(TGenotype*)> &&fUpdateBestGenotype);

	virtual CIndividual<TGenotype, TFenotype> *pc_create_individual(TGenotype *pcGenotype)
	{
		return new CIndividual<TGenotype, TFenotype>(pcGenotype, pc_problem->pcGetEvaluation(), nullptr, nullptr, pc_problem->pcGetTransformation());
	};

	void v_clear_params();

	CProblem<TGenotype, TFenotype> *pc_problem;
	
	CStopCondition<TGenotype, TFenotype> *pc_stop_condition;

	uint32_t i_gene_pattern_min_length;
	uint32_t i_linkage_frequency;

	CGeneration<TGenotype> *pc_empty_generation;

	CIndividual<TGenotype, TFenotype> *pc_best_individual;

	time_t t_best_time;
	uint64_t i_best_ffe;

	CLog *pc_log;

	bool b_own_params;

private:
	void v_update_statistics_of_best(uint32_t iIterationNumber, time_t tStartTime);
};//class COptimizer

#endif//OPTIMIZER_H