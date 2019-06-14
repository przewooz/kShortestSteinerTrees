#include "DynamicFromStaticOptimizer.h"

#include "BinaryCoding.h"
#include "Individual.h"
#include "OptimizerUtils.h"

#include <cstdint>
#include <ctime>

using namespace DynamicOptimizer;

template <class TGenotype, class TFenotype>
CDynamicFromStaticOptimizer<TGenotype, TFenotype>::CDynamicFromStaticOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)
	: CDynamicOptimizer<TGenotype, TFenotype>(pcProblem, pcLog)
{
	pc_static_optimizer = nullptr;
}//CDynamicFromStaticOptimizer<TGenotype, TFenotype>::CDynamicFromStaticOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)

template <class TGenotype, class TFenotype>
CDynamicFromStaticOptimizer<TGenotype, TFenotype>::~CDynamicFromStaticOptimizer()
{
	delete pc_static_optimizer;
}//CDynamicFromStaticOptimizer<TGenotype, TFenotype>::~CDynamicFromStaticOptimizer()

template <class TGenotype, class TFenotype>
CError CDynamicFromStaticOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	delete pc_static_optimizer;

	CError c_error = CDynamicOptimizer<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		c_error = e_configure_static_optimizer(psSettings);
	}//if (!c_error)

	return c_error;
}//CError CDynamicFromStaticOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	CDynamicOptimizer<TGenotype, TFenotype>::vInitialize(tStartTime);
	pc_static_optimizer->vInitialize(tStartTime);
}//void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vRunState(time_t tStartTime, uint64_t iStartFFE)
{
	uint32_t i_iteration_number = 0;

	while (!pc_stop_condition->bStop(tStartTime, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE() - iStartFFE, pc_static_optimizer->pcGetBestIndividual()))
	{
		pc_static_optimizer->bRunIteration(i_iteration_number, tStartTime);
		i_iteration_number++;
	}//while (!pc_stop_condition->bStop(tStartTime, i_iteration_number, pc_problem->pcGetEvaluation()->iGetFFE() - iStartFFE, pc_static_optimizer->pcGetBestIndividual()))

	*(ppc_best_individuals + i_current_state_index) = new CIndividual<TGenotype, TFenotype>(pc_static_optimizer->pcGetBestIndividual());

	*(pt_best_times + i_current_state_index) = pc_static_optimizer->tGetBestTime();
	*(pi_best_ffes + i_current_state_index) = pc_static_optimizer->iGetBestFFE() - iStartFFE;
}//void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vRunState(time_t tStartTime, uint64_t iStartFFE)

template <class TGenotype, class TFenotype>
void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vChangeState(time_t tStartTime, uint64_t iStartFFE)
{
	CDynamicOptimizer<TGenotype, TFenotype>::vChangeState(tStartTime, iStartFFE);
	pc_static_optimizer->vResetBestIndividual();
}//void CDynamicFromStaticOptimizer<TGenotype, TFenotype>::vChangeState(time_t tStartTime, uint64_t iStartFFE)

template <class TGenotype, class TFenotype>
CError CDynamicFromStaticOptimizer<TGenotype, TFenotype>::e_configure_static_optimizer(istream *psSettings)
{
	CError c_error;

	pc_static_optimizer = OptimizerUtils::pcGetOptimizer(pc_problem, pc_log, psSettings, &c_error);

	return c_error;
}//CError CDynamicFromStaticOptimizer<TGenotype, TFenotype>::e_configure_static_optimizer(istream *psSettings)

template class CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>;