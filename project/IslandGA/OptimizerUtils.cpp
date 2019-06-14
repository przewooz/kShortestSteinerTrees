#include "OptimizerUtils.h"

#include "BinaryCoding.h"
#include "DSMGA2.h"
#include "EnumCommandParam.h"
#include "LTGA.h"
#include "P3.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype, class TFenotype>
COptimizer<TGenotype, TFenotype> * OptimizerUtils::pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	COptimizer<TGenotype, TFenotype> *pc_optimizer = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();
	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	unordered_map<CString, EOptimizerType> m_optimizer_types;

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_P3, OPTIMIZER_P3));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_LTGA, OPTIMIZER_LTGA));
		m_optimizer_types.insert(pair<const CString, EOptimizerType>(OPTIMIZER_ARGUMENT_TYPE_DSMGA2, OPTIMIZER_DSMGA2));
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code() && i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	CEnumCommandParam<EOptimizerType> p_type(OPTIMIZER_ARGUMENT_TYPE, &m_optimizer_types, bIsObligatory);
	EOptimizerType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			case OPTIMIZER_P3:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CP3((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case OPTIMIZER_P3
			case OPTIMIZER_LTGA:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CLTGA((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case OPTIMIZER_LTGA
			case OPTIMIZER_DSMGA2:
			{
				pc_optimizer = (COptimizer<TGenotype, TFenotype>*)new CDSMGA2((CProblem<CBinaryCoding, CBinaryCoding>*)pcProblem, pcLog);
				break;
			}//case OPTIMIZER_DSMGA2
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "optimizer");
				break;
			}//default
		}//switch (e_type)
	}//if (!*pcError && p_type.bHasValue())

	if (!*pcError)
	{
		*pcError = pc_optimizer->eConfigure(psSettings);
	}//if (!*pcError)

	return pc_optimizer;
}//COptimizer<TGenotype, TFenotype> * OptimizerUtils::pcGetOptimizer(CProblem<TGenotype, TFenotype> *pcProblem, CLog *pcLog, istream *psSettings, COptimizerParams<TGenotype, TFenotype> **ppcParams, CError *pcError, bool bIsObligatory)


template COptimizer<CBinaryCoding, CBinaryCoding> * OptimizerUtils::pcGetOptimizer(CProblem<CBinaryCoding, CBinaryCoding>*, CLog*, istream*, CError*, bool);