#include "MutationUtils.h"

#include "EnumCommandParam.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>

template <class TGenotype>
CMutation<TGenotype> * MutationUtils::pcGetMutation(istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CMutation<TGenotype> *pc_mutation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, EMutationType> m_mutation_types;

	CEnumCommandParam<EMutationType> p_type(MUTATION_ARGUMENT_TYPE, &m_mutation_types, bIsObligatory);
	EMutationType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		switch (e_type)
		{
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "mutation");
				break;
			}//default
		}//switch (pcParams->eGetType())

		if (!*pcError)
		{
			*pcError = pc_mutation->eConfigure(psSettings);
		}//if (!*pcError)
	}//if (!*pcError && p_type.bHasValue())

	return pc_mutation;
}//CMutation<TGenotype> * MutationUtils::pcGetMutation(istream *psSettings, CError *pcError, bool bIsObligatory)


template CMutation<CBinaryCoding> * MutationUtils::pcGetMutation(istream*, CError*, bool);