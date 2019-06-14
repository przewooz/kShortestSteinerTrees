#include "EvaluationUtils.h"

#include "BinaryCoding.h"
#include "BinaryEvaluation.h"
#include "EnumCommandParam.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TFenotype>
CEvaluation<TFenotype> * EvaluationUtils::pcGetEvaluation(istream *psSettings, CError *pcError)
{
	CEvaluation<TFenotype> *pc_evaluation = nullptr;

	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	unordered_map<CString, EEvaluationType> m_evaluation_types;

	if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_CYCLIC_FENOTYPE, EVALUATION_BINARY_CYCLIC_FENOTYPE));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE, EVALUATION_BINARY_DECEPTIVE));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE_CONCATENATION, EVALUATION_BINARY_DECEPTIVE_CONCATENATION));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_KNAPSACK, EVALUATION_BINARY_KNAPSACK));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_MAX_SAT, EVALUATION_BINARY_MAX_SAT));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE_STEP_TRAP, EVALUATION_BINARY_DECEPTIVE_STEP_TRAP));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_NEAREST_NEIGHBOR_NK, EVALUATION_BINARY_NEAREST_NEIGHBOR_NK));
		m_evaluation_types.insert(pair<const CString, EEvaluationType>(EVALUATION_ARGUMENT_TYPE_BINARY_STEINER_TREE_SET, EVALUATION_BINARY_STEINER_TREE_SET));
	}//if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	CEnumCommandParam<EEvaluationType> p_type(EVALUATION_ARGUMENT_TYPE, &m_evaluation_types);
	EEvaluationType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError)
	{
		switch (e_type)
		{
			case EVALUATION_BINARY_CYCLIC_FENOTYPE:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryCyclicFenotypeEvaluation();
				break;
			}//case EVALUATION_BINARY_CYCLIC_FENOTYPE
			case EVALUATION_BINARY_DECEPTIVE:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryDeceptiveEvaluation();
				break;
			}//case EVALUATION_BINARY_DECEPTIVE
			case EVALUATION_BINARY_DECEPTIVE_CONCATENATION:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryDeceptiveConcatenationEvaluation();
				break;
			}//case EVALUATION_BINARY_DECEPTIVE_CONCATENATION
			case EVALUATION_BINARY_KNAPSACK:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryKnapsackEvaluation();
				break;
			}//case EVALUATION_BINARY_KNAPSACK
			case EVALUATION_BINARY_MAX_SAT:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryMaxSatEvaluation();
				break;
			}//case EVALUATION_BINARY_MAX_SAT
			case EVALUATION_BINARY_DECEPTIVE_STEP_TRAP:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryDeceptiveStepTrapEvaluation();
				break;
			}//case EVALUATION_BINARY_DECEPTIVE_STEP_TRAP
			case EVALUATION_BINARY_NEAREST_NEIGHBOR_NK:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinaryNearestNeighborNKEvaluation();
				break;
			}//case EVALUATION_BINARY_NEAREST_NEIGHBOR_NK
			case EVALUATION_BINARY_STEINER_TREE_SET:
			{
				pc_evaluation = (CEvaluation<TFenotype>*)new CBinarySteinerTreeSetDynamicEvaluation();
				break;
			}//case EVALUATION_BINARY_STEINER_TREE_SET
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "evaluation");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_evaluation->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError)

	return pc_evaluation;
}//CEvaluation<TFenotype> * EvaluationUtils::pcGetEvaluation(istream *psSettings, CError *pcError)

template <class TFenotype>
CDynamicEvaluation<TFenotype> * EvaluationUtils::pcGetDynamicEvaluation(istream *psSettings, CError *pcError)
{
	CDynamicEvaluation<TFenotype> *pc_dynamic_evaluation = new CDynamicEvaluation<TFenotype>();
	*pcError = pc_dynamic_evaluation->eConfigure(psSettings);

	return pc_dynamic_evaluation;
}//CDynamicEvaluation<TFenotype> * EvaluationUtils::pcGetDynamicEvaluation(istream *psSettings, CError *pcError)


template CEvaluation<CBinaryCoding> * EvaluationUtils::pcGetEvaluation(istream*, CError*);

template CDynamicEvaluation<CBinaryCoding> * EvaluationUtils::pcGetDynamicEvaluation(istream*, CError*);