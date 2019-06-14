#ifndef DYNAMIC_FROM_STATIC_OPTIMIZER
#define DYNAMIC_FROM_STATIC_OPTIMIZER

#include "DynamicOptimizer.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"

#include <istream>

using namespace std;

namespace DynamicOptimizer
{
	template <class TGenotype, class TFenotype>
	class CDynamicFromStaticOptimizer : public CDynamicOptimizer<TGenotype, TFenotype>
	{
	public:
		virtual ~CDynamicFromStaticOptimizer();

		virtual CError eConfigure(istream *psSettings);

		virtual void vInitialize(time_t tStartTime);
		virtual void vRunState(time_t tStartTime, uint64_t iStartFFE);
		virtual void vChangeState(time_t tStartTime, uint64_t iStartFFE);

	protected:
		CDynamicFromStaticOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog);

		virtual CError e_configure_static_optimizer(istream *psSettings);

		COptimizer<TGenotype, TFenotype> *pc_static_optimizer;
	};//class CDynamicFromStaticOptimizer : public CDynamicOptimizer<TGenotype, TFenotype>
}//namespace DynamicOptimizer

#endif//DYNAMIC_FROM_STATIC_OPTIMIZER