#ifndef DYNAMIC_OPTIMIZER_H
#define DYNAMIC_OPTIMIZER_H

#define DYNAMIC_OPTIMIZER_ARGUMENT_TYPE "dynamic_optimizer_type"
#define DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_P3 "dynamic_p3"
#define DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_LTGA "dynamic_ltga"
#define DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_DSMGA2 "dynamic_dsmga2"

#define DYNAMIC_OPTIMIZER_ARGUMENT_NUMBER_OF_STATES "dynamic_optimizer_number_of_states"

#include "Error.h"
#include "Individual.h"
#include "Log.h"
#include "Problem.h"
#include "StopCondition.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;


namespace DynamicOptimizer
{
	enum EDynamicOptimizerType
	{
		DYNAMIC_OPTIMIZER_P3 = 0,
		DYNAMIC_OPTIMIZER_LTGA = 1,
		DYNAMIC_OPTIMIZER_DSMGA2 = 2
	};//enum EDynamicOptimizerType


	template <class TGenotype, class TFenotype>
	class CDynamicOptimizer
	{
	public:
		CDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog);

		virtual ~CDynamicOptimizer();

		virtual CError eConfigure(istream *psSettings);

		virtual void vInitialize(time_t tStartTime);
		virtual void vRunState(time_t tStartTime, uint64_t iStartFFE) = 0;
		virtual void vChangeState(time_t tStartTime, uint64_t iStartFFE);

		virtual void vRun();

		uint16_t iGetNumberOfStates() { return i_number_of_states; };

		CIndividual<TGenotype, TFenotype> **ppcGetBestIndividuals() { return ppc_best_individuals; };

		time_t *ptGetBestTimes() { return pt_best_times; };
		uint64_t *piGetBestFFEs() { return pi_best_ffes; };

	protected:
		CProblem<TGenotype, TFenotype> *pc_problem;

		CStopCondition<TGenotype, TFenotype> *pc_stop_condition;

		uint16_t i_number_of_states;
		uint16_t i_current_state_index;

		CIndividual<TGenotype, TFenotype> **ppc_best_individuals;

		time_t *pt_best_times;
		uint64_t *pi_best_ffes;

		CLog *pc_log;

	private:
		void v_clear();
	};//class CDynamicOptimizer

	template <class TGenotype, class TFenotype>
	CDynamicOptimizer<TGenotype, TFenotype> *pcGetDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace DynamicOptimizer

#endif//DYNAMIC_OPTIMIZER_H