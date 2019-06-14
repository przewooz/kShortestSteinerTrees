#include "DynamicOptimizer.h"

#include "BinaryCoding.h"
#include "DynamicDSMGA2.h"
#include "DynamicLTGA.h"
#include "DynamicP3.h"
#include "EnumCommandParam.h"
#include "Evaluation.h"
#include "StopConditionUtils.h"
#include "StringUtils.h"
#include "UIntCommandParam.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

using namespace DynamicOptimizer;

template <class TGenotype, class TFenotype>
CDynamicOptimizer<TGenotype, TFenotype>::CDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)
{
	pc_problem = pcProblem;

	pc_stop_condition = nullptr;

	pc_log = pcLog;

	ppc_best_individuals = nullptr;

	pt_best_times = nullptr;
	pi_best_ffes = nullptr;
}//CDynamicOptimizer<TGenotype, TFenotype>::CDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog)

template <class TGenotype, class TFenotype>
CDynamicOptimizer<TGenotype, TFenotype>::~CDynamicOptimizer()
{
	v_clear();
}//CDynamicOptimizer<TGenotype, TFenotype>::~CDynamicOptimizer()

template <class TGenotype, class TFenotype>
CError CDynamicOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)
{
	CError c_error;

	v_clear();

	CUIntCommandParam p_number_of_states(DYNAMIC_OPTIMIZER_ARGUMENT_NUMBER_OF_STATES, 1, UINT16_MAX);
	i_number_of_states = p_number_of_states.iGetValue(psSettings, &c_error);

	if (!c_error)
	{
		ppc_best_individuals = new CIndividual<TGenotype, TFenotype>*[i_number_of_states];

		for (uint16_t i = 0; i < i_number_of_states; i++)
		{
			*(ppc_best_individuals + i) = nullptr;
		}//for (uint16_t i = 0; i < i_number_of_states; i++)

		pt_best_times = new time_t[i_number_of_states];
		pi_best_ffes = new uint64_t[i_number_of_states];
	}//if (!c_error)

	if (!c_error)
	{
		pc_stop_condition = StopConditionUtils::pcGetStopCondition<TGenotype, TFenotype>(pc_problem->pcGetEvaluation(), psSettings, &c_error);
	}//if (!c_error)

	return c_error;
}//CError CDynamicOptimizer<TGenotype, TFenotype>::eConfigure(istream *psSettings)

template <class TGenotype, class TFenotype>
void CDynamicOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)
{
	for (uint16_t i = 0; i < i_number_of_states; i++)
	{
		delete *(ppc_best_individuals + i);
		*(ppc_best_individuals + i) = nullptr;
	}//for (uint16_t i = 0; i < i_number_of_states; i++)

	i_current_state_index = 0;
}//void CDynamicOptimizer<TGenotype, TFenotype>::vInitialize(time_t tStartTime)

template <class TGenotype, class TFenotype>
void CDynamicOptimizer<TGenotype, TFenotype>::vChangeState(time_t tStartTime, uint64_t iStartFFE)
{
	CDynamicEvaluation<TFenotype> *pc_dynamic_evaluation = (CDynamicEvaluation<TFenotype>*)pc_problem->pcGetEvaluation();
	pc_dynamic_evaluation->vChange();

	i_current_state_index++;
}//void CDynamicOptimizer<TGenotype, TFenotype>::vChangeState(time_t tStartTime, uint64_t iStartFFE)

template <class TGenotype, class TFenotype>
void CDynamicOptimizer<TGenotype, TFenotype>::vRun()
{
	uint64_t i_start_ffe = pc_problem->pcGetEvaluation()->iGetFFE();

	time_t t_start_time = time(nullptr);
	vInitialize(t_start_time);

	for (uint16_t i = 0; i < i_number_of_states; i++)
	{
		vRunState(t_start_time, i_start_ffe);

		t_start_time = time(nullptr);
		i_start_ffe = pc_problem->pcGetEvaluation()->iGetFFE();

		vChangeState(t_start_time, i_start_ffe);
	}//for (uint16_t i = 0; i < i_number_of_states; i++)
}//void CDynamicOptimizer<TGenotype, TFenotype>::vRun()

template <class TGenotype, class TFenotype>
void DynamicOptimizer::CDynamicOptimizer<TGenotype, TFenotype>::v_clear()
{
	delete pc_stop_condition;
	pc_stop_condition = nullptr;

	if (ppc_best_individuals)
	{
		for (uint16_t i = 0; i < i_number_of_states; i++)
		{
			delete *(ppc_best_individuals + i);
			*(ppc_best_individuals + i) = nullptr;
		}//for (uint16_t i = 0; i < i_number_of_states; i++)

		delete ppc_best_individuals;
		ppc_best_individuals = nullptr;
	}//if (ppc_best_individuals)

	delete pt_best_times;
	pt_best_times = nullptr;

	delete pi_best_ffes;
	pi_best_ffes = nullptr;
}//void DynamicOptimizer::CDynamicOptimizer<TGenotype, TFenotype>::v_clear()


template <class TGenotype, class TFenotype>
CDynamicOptimizer<TGenotype, TFenotype> * DynamicOptimizer::pcGetDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CDynamicOptimizer<TGenotype, TFenotype> *pc_dynamic_optimizer = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();
	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	unordered_map<CString, EDynamicOptimizerType> m_dynamic_optimizer_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_dynamic_optimizer_types.insert(pair<const CString, EDynamicOptimizerType>(DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_P3, DYNAMIC_OPTIMIZER_P3));
		m_dynamic_optimizer_types.insert(pair<const CString, EDynamicOptimizerType>(DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_LTGA, DYNAMIC_OPTIMIZER_LTGA));
		m_dynamic_optimizer_types.insert(pair<const CString, EDynamicOptimizerType>(DYNAMIC_OPTIMIZER_ARGUMENT_TYPE_DSMGA2, DYNAMIC_OPTIMIZER_DSMGA2));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	CEnumCommandParam<EDynamicOptimizerType> p_type(DYNAMIC_OPTIMIZER_ARGUMENT_TYPE, &m_dynamic_optimizer_types, bIsObligatory);
	EDynamicOptimizerType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case DYNAMIC_OPTIMIZER_P3:
			{
				pc_dynamic_optimizer = (CDynamicOptimizer<TGenotype, TFenotype>*)new CDynamicP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case DYNAMIC_OPTIMIZER_P3
			case DYNAMIC_OPTIMIZER_LTGA:
			{
				pc_dynamic_optimizer = (CDynamicOptimizer<TGenotype, TFenotype>*)new CDynamicLTGA((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case DYNAMIC_OPTIMIZER_LTGA
			case DYNAMIC_OPTIMIZER_DSMGA2:
			{
				pc_dynamic_optimizer = (CDynamicOptimizer<TGenotype, TFenotype>*)new CDynamicDSMGA2((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case DYNAMIC_OPTIMIZER_DSMGA2
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "dynamic optimizer");
				break;
			}//default
		}//switch (e_type)
	}//if (!*pcError && p_type.bHasValue())

	if (!*pcError)
	{
		*pcError = pc_dynamic_optimizer->eConfigure(psSettings);
	}//if (!*pcError)

	return pc_dynamic_optimizer;
}//CDynamicOptimizer<TGenotype, TFenotype> * DynamicOptimizer::pcGetDynamicOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, CError *pcError, bool bIsObligatory)


template class CDynamicOptimizer<CBinaryCoding, CBinaryCoding>;


template CDynamicOptimizer<CBinaryCoding, CBinaryCoding> * DynamicOptimizer::pcGetDynamicOptimizer(CProblem<CBinaryCoding, CBinaryCoding>*, CLog*, istream*, CError*, bool);