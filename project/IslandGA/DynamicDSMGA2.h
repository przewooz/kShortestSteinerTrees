#ifndef DYNAMIC_DSMGA2_H
#define DYNAMIC_DSMGA2_H

#include "BinaryCoding.h"
#include "DynamicFromStaticOptimizer.h"
#include "Error.h"
#include "Log.h"
#include "Problem.h"

#include <ctime>
#include <istream>

using namespace std;

namespace DynamicOptimizer
{
	class CDynamicDSMGA2 : public CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>
	{
	public:
		CDynamicDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog);

		virtual void vChangeState(time_t tStartTime, uint64_t iStartFFE);

	protected:
		virtual CError e_configure_static_optimizer(istream *psSettings);
	};//class CDynamicDSMGA2 : public CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>
}//namespace DynamicOptimizer

#endif//DYNAMIC_DSMGA2_H