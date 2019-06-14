#ifndef DYNAMIC_LTGA_H
#define DYNAMIC_LTGA_H

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
	class CDynamicLTGA : public CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>
	{
	public:
		CDynamicLTGA(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog);

		virtual void vChangeState(time_t tStartTime, uint64_t iStartFFE);

	protected:
		virtual CError e_configure_static_optimizer(istream *psSettings);
	};//class CDynamicLTGA : public CDynamicFromStaticOptimizer<CBinaryCoding, CBinaryCoding>
}//namespace DynamicOptimizer

#endif//DYNAMIC_LTGA_H