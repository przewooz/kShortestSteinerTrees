#include "LTGA.h"

#include "BinaryEvaluation.h"

#include <atlstr.h>
#include <vector>

CLTGA::CLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)
	: CP3BasedOptimizer("LTGA", pcProblem, pcLog)
{

}//CLTGA::CLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog)

CLTGA::CLTGA(CLTGA *pcOther)
	: CP3BasedOptimizer(pcOther)
{

}//CLTGA::CLTGA(CLTGA *pcOther)

void CLTGA::vInitialize(time_t tStartTime)
{
	CP3BasedOptimizer::vInitialize(tStartTime);
	pc_ltga = static_pointer_cast<LTGA>(pc_optimizer);
}//void CLTGA::vInitialize(time_t tStartTime)

bool CLTGA::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)
{
	bool b_updated = CP3BasedOptimizer::bRunIteration(iIterationNumber, tStartTime);

#ifdef EVALUATION_BINARY_STEINER_TREE_SET_DYNAMIC
	if (dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))
	{
		CBinarySteinerTreeSetDynamicEvaluation *pc_steiner_tree_set = (CBinarySteinerTreeSetDynamicEvaluation*)pc_problem->pcGetEvaluation();
		pc_steiner_tree_set->vChange();

		pc_ltga->fitnesses.clear();

		for (size_t i = 0; i < pc_ltga->pop.solutions.size(); i++)
		{
			pc_recorder->evaluate(pc_ltga->pop.solutions.at(i));
		}//for (size_t i = 0; i < pc_ltga->pop.solutions.size(); i++)
	}//if (dynamic_cast<CBinarySteinerTreeSetDynamicEvaluation*>(pc_problem->pcGetEvaluation()))
#endif//EVALUATION_BINARY_STEINER_TREE_SET_DYNAMIC

	return b_updated;
}//bool CLTGA::bRunIteration(uint32_t iIterationNumber, time_t tStartTime)