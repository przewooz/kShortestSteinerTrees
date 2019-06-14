#include "StopCondition.h"

#include "BinaryCoding.h"
#include "UIntCommandParam.h"

template <class TGenotype, class TFenotype>
CStopCondition<TGenotype, TFenotype>::CStopCondition(CEvaluation<TFenotype> *pcEvaluation)
{
	pc_evaluation = pcEvaluation;
}//CStopCondition<TGenotype, TFenotype>::CStopCondition(CEvaluation<TFenotype> *pcEvaluation)

template <class TGenotype, class TFenotype>
CStopCondition<TGenotype, TFenotype>::~CStopCondition()
{

}//CStopCondition<TGenotype, TFenotype>::~CStopCondition()

template <class TGenotype, class TFenotype>
bool CStopCondition<TGenotype, TFenotype>::bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual)
{
	bool b_stop = false;

	if (pcBestIndividual)
	{
		b_stop = pc_evaluation->bIsMaxValue(pcBestIndividual->dGetFitnessValue());
	}//if (pcBestIndividual)

	return b_stop;
}//bool CStopCondition<TGenotype, TFenotype>::bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual)


template <class TGenotype, class TFenotype>
CIterationsStopCondition<TGenotype, TFenotype>::CIterationsStopCondition(CEvaluation<TFenotype> *pcEvaluation)
	: CStopCondition<TGenotype, TFenotype>(pcEvaluation)
{

}//CIterationsStopCondition<TGenotype, TFenotype>::CIterationsStopCondition(CEvaluation<TFenotype> *pcEvaluation)

template <class TGenotype, class TFenotype>
CError CIterationsStopCondition<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error = CStopCondition<TGenotype, TFenotype>::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_max_number_of_iterations(STOP_CONDITION_ARGUMENT_MAX_NUMBER_OF_ITERATIONS);
		i_max_number_of_iterations = p_max_number_of_iterations.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CIterationsStopCondition<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
bool CIterationsStopCondition<TGenotype, TFenotype>::bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual)
{
	return CStopCondition<TGenotype, TFenotype>::bStop(tStart, iIterationNumber, iFFE, pcBestIndividual) || iIterationNumber >= i_max_number_of_iterations;
}//bool CIterationsStopCondition<TGenotype, TFenotype>::bStop(time_t tStart, uint32_t iIterationNumber, uint64_t iFFE, CIndividual<TGenotype, TFenotype> *pcBestIndividual)


template class CStopCondition<CBinaryCoding, CBinaryCoding>;


template class CIterationsStopCondition<CBinaryCoding, CBinaryCoding>;