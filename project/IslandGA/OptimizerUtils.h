#ifndef OPTIMIZER_UTILS_H
#define OPTIMIZER_UTILS_H

#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"

#include <istream>

using namespace std;

namespace OptimizerUtils
{
	template <class TGenotype, class TFenotype>
	COptimizer<TGenotype, TFenotype> *pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace OptimizerUtils

#endif//OPTIMIZER_UTILS_H
