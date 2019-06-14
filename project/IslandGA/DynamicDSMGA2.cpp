#include "DynamicDSMGA2.h"

#include "DSMGA2.h"

#include "../DSMGA2/global.h"

using namespace DynamicOptimizer;

CDynamicDSMGA2::CDynamicDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)
	: CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>(pcProblem, pcLog)
{

}//CDynamicDSMGA2::CDynamicDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)

void CDynamicDSMGA2::vChangeState(time_t tStartTime, uint64_t iStartFFE)
{
	CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>::vChangeState(tStartTime, iStartFFE);

	CDSMGA2 *pc_dsmga2 = (CDSMGA2*)pc_static_optimizer;

	pc_dsmga2->pc_dsmga2->bestFitness = -INF;

	Chromosome::hit = false;

	Chromosome *pc_chromosome;

	for (int i = 0; i < pc_dsmga2->pc_dsmga2->nCurrent; i++)
	{
		pc_chromosome = (pc_dsmga2->pc_dsmga2->population + i);
		pc_chromosome->setEvaluated(false);
		pc_dsmga2->pc_dsmga2->pHash[pc_chromosome->getKey()] = pc_chromosome->getFitness();
	}//for (int i = 0; i < pc_dsmga2->pc_dsmga2->nCurrent; i++)

	pc_dsmga2->b_update_best_individual(0, tStartTime);
}//void CDynamicDSMGA2::vChangeState(time_t tStartTime, uint64_t iStartFFE)

CError CDynamicDSMGA2::e_configure_static_optimizer(istream *psSettings)
{
	pc_static_optimizer = new CDSMGA2(pc_problem, pc_log);
	return pc_static_optimizer->eConfigure(psSettings);
}//CError CDynamicDSMGA2::e_configure_static_optimizer(istream *psSettings)