#ifndef STOP_CONDITION_H
#define STOP_CONDITION_H

#define STOP_CONDITION_ARGUMENT_TYPE "stop_condition_type"
#define STOP_CONDITION_ARGUMENT_TYPE_NONE "none"
#define STOP_CONDITION_ARGUMENT_TYPE_RUNNING_TIME "running_time"
#define STOP_CONDITION_ARGUMENT_TYPE_ITERATIONS "iterations"

#define STOP_CONDITION_ARGUMENT_TIME "stop_condition_time"

#define STOP_CONDITION_ARGUMENT_MAX_NUMBER_OF_ITERATIONS "stop_condition_max_number_of_iterations"

#include "Error.h"
#include "Evaluation.h"
#include "Problem.h"

#include <cstdint>
#include <ctime>
#include <istream>

using namespace std;


enum EStopConditionType
{
	STOP_CONDITION_NONE = 0,
	STOP_CONDITION_RUNNING_TIME = 1,
	STOP_CONDITION_ITERATIONS = 2
};//enum EStopConditionType


template <class TGenotype, class TFenotype>
class CStopCondition
{
public:
	CStopCondition(CEvaluation<TFenotype> *pcEvaluation);
	
	virtual ~CStopCondition();

	virtual CError eConfigure(istream *psSettings) { return CError(); };

	virtual bool bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual);

private:
	CEvaluation<TFenotype> *pc_evaluation;
};//class CStopCondition


template <class TGenotype, class TFenotype>
class CNoneStopCondition : public CStopCondition<TGenotype, TFenotype>
{
public:
	CNoneStopCondition() : CStopCondition(nullptr) {};

	virtual bool bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual) { return false; };
};//class CStopCondition


template <class TGenotype, class TFenotype>
class CIterationsStopCondition : public CStopCondition<TGenotype, TFenotype>
{
public:
	CIterationsStopCondition(CEvaluation<TFenotype> *pcEvaluation);

	virtual CError eConfigure(istream *psSettings);

	virtual bool bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual);

private:
	uint32_t i_max_number_of_iterations;
};//class CStopCondition

#endif//STOP_CONDITION_H