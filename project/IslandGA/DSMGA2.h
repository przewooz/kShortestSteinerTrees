#ifndef DSMGA2_H
#define DSMGA2_H

#include "BinaryCoding.h"
#include "Error.h"
#include "Log.h"
#include "Optimizer.h"
#include "Problem.h"

#include "../DSMGA2/dsmga2.h"

#include <ctime>
#include <cstdint>
#include <istream>

using namespace std;

namespace DynamicOptimizer
{
	class CDynamicDSMGA2;
}//namespace DynamicOptimizer

class CDSMGA2 : public COptimizer<CBinaryCoding, CBinaryCoding>
{
friend class DynamicOptimizer::CDynamicDSMGA2;

public:
	CDSMGA2(CProblem<CBinaryCoding, CBinaryCoding> *pcProblem, CLog *pcLog);
	CDSMGA2(CDSMGA2 *pcOther);
	
	~CDSMGA2();

	virtual CError eConfigure(istream *psSettings);

	virtual COptimizer<CBinaryCoding, CBinaryCoding> *pcCopy() { return new CDSMGA2(this); };

	virtual void vInitialize(time_t tStartTime);
	virtual bool bRunIteration(uint32_t iIterationNumber, time_t tStartTime);

private:
	using COptimizer<CBinaryCoding, CBinaryCoding>::b_update_best_individual;
	bool b_update_best_individual(uint32_t iIterationNumber, time_t tStartTime);

	DSMGA2 *pc_dsmga2;

	uint32_t i_population_size;
};//class CDSMGA2 : public COptimizer<CBinaryCoding, CBinaryCoding>

#endif//DSMGA2_H
