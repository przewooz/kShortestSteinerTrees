#include "CrossoverUtils.h"

#include "EnumCommandParam.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype>
CCrossover<TGenotype> * CrossoverUtils::pcGetCrossover(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CCrossover<TGenotype> *pc_crossover = nullptr;

	unordered_map<CString, ECrossoverType> m_crossover_types;

	CEnumCommandParam<ECrossoverType> p_type(CROSSOVER_ARGUMENT_TYPE, &m_crossover_types, bIsObligatory);
	ECrossoverType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "crossover");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_crossover->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_crossover;
}//CCrossover<TGenotype> * CrossoverUtils::pcGetCrossover(istream *psSettings, CError *pcError, bool bIsObligatory)


template CCrossover<CBinaryCoding> * CrossoverUtils::pcGetCrossover(istream*, CError*, bool);