#include "DynamicP3.h"

#include "P3.h"

#include <limits>

using namespace DynamicOptimizer;

CDynamicP3::CDynamicP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)
	: CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog)
{

}//CDynamicP3::CDynamicP3(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)

void CDynamicP3::vChangeState(time_t tStartTime, uint64_t iStartFFE)
{
	CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>::vChangeState(tStartTime, iStartFFE);

	CP3 *pc_p3 = (CP3*)pc_static_optimizer;

	pc_p3->pc_recorder->best_fitness = (numeric_limits<int>::min)();
	pc_p3->pc_recorder->evaluations = 0;
	pc_p3->pc_recorder->start_time = tStartTime;

	for (size_t i = 0; i < pc_p3->pc_p3->pops.size(); i++)
	{
		for (size_t j = 0; j < pc_p3->pc_p3->pops.at(i).solutions.size(); j++)
		{
			pc_p3->pc_recorder->evaluate(pc_p3->pc_p3->pops.at(i).solutions.at(j));
		}//for (size_t j = 0; j < pc_p3->pc_p3->pops.at(i).solutions.size(); j++)
	}//for (size_t i = 0; i < pc_p3->pc_p3->pops.size(); i++)

	pc_p3->b_update_best_individual(0, tStartTime);
}//void CDynamicP3::vChangeState(time_t tStartTime, uint64_t iStartFFE)

CError CDynamicP3::e_configure_static_optimizer(istream *psSettings)
{
	pc_static_optimizer = new CP3(pc_problem, pc_log);
	return pc_static_optimizer->eConfigure(psSettings);
}//CError CDynamicP3::e_configure_static_optimizer(istream *psSettings)