#include "GenerationUtils.h"

#include "BinaryCoding.h"
#include "BinaryEvaluation.h"
#include "BinaryGeneration.h"
#include "EnumCommandParam.h"
#include "StringUtils.h"

#include <atlstr.h>
#include <unordered_map>
#include <utility>

template <class TGenotype, class TFenotype>
CGeneration<TFenotype> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<TGenotype, TFenotype> *pcProblem)
{
	CGeneration<TFenotype> *pc_sample_fenotype_generation = nullptr;

	TFenotype *pc_sample_fenotype = pcProblem->pcGetEvaluation()->pcCreateSampleFenotype();

	size_t i_fenotype_type_hash_code = typeid(TFenotype).hash_code();

	if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		pc_sample_fenotype_generation = (CGeneration<TFenotype>*)new CBinaryGeneration((CBinaryCoding*)pc_sample_fenotype);
	}//if (i_fenotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	
	delete pc_sample_fenotype;

	return pc_sample_fenotype_generation;
}//CGeneration<TFenotype> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CGeneration<TGenotype> * GenerationUtils::pcGetEmptyGeneration(CProblem<TGenotype, TFenotype> *pcProblem)
{
	CGeneration<TGenotype> *pc_empty_generation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TFenotype).hash_code();

	CGeneration<TFenotype> *pc_sample_fenotype_generation = pcGetSampleFenotypeGeneration(pcProblem);

	TFenotype *pc_sample_fenotype = pc_sample_fenotype_generation->pcGenerateEmpty();
	TGenotype *pc_sample_genotype = pcProblem->pcGetTransformation()->pcInverselyTransform(pc_sample_fenotype);

	if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())
	{
		pc_empty_generation = (CGeneration<TGenotype>*)new CBinaryGeneration((CBinaryCoding*)pc_sample_genotype);
	}//if (i_genotype_type_hash_code == typeid(CBinaryCoding).hash_code())

	delete pc_sample_fenotype_generation;

	if (pc_sample_genotype != pc_sample_fenotype)
	{
		delete pc_sample_genotype;
	}//if (pc_sample_genotype != pc_sample_fenotype)

	delete pc_sample_fenotype;

	return pc_empty_generation;
}//CGeneration<TGenotype> * GenerationUtils::pcGetEmptyGeneration(CProblem<TGenotype, TFenotype> *pcProblem)

template <class TGenotype, class TFenotype>
CGeneration<TGenotype> * GenerationUtils::pcGetGeneration(CProblem<TGenotype, TFenotype> *pcProblem, istream *psSettings, CError *pcError, bool bIsObligatory)
{
	CGeneration<TGenotype> *pc_generation = nullptr;

	size_t i_genotype_type_hash_code = typeid(TGenotype).hash_code();

	unordered_map<CString, EGenerationType> m_generation_types;

	CEnumCommandParam<EGenerationType> p_type(GENERATION_ARGUMENT_TYPE, &m_generation_types, bIsObligatory);
	EGenerationType e_type = p_type.eGetValue(psSettings, pcError);

	if (!*pcError && p_type.bHasValue())
	{
		CGeneration<TFenotype> *pc_sample_fenotype_generation = pcGetSampleFenotypeGeneration(pcProblem);

		TFenotype *pc_sample_fenotype = pc_sample_fenotype_generation->pcGenerateEmpty();
		TGenotype *pc_sample_genotype = pcProblem->pcGetTransformation()->pcInverselyTransform(pc_sample_fenotype);

		switch (e_type)
		{
			default:
			{
				pcError->vSetError(CError::iERROR_CODE_OPERATOR_NOT_FOUND, "generation");
				break;
			}//default
		}//switch (e_type)

		if (!*pcError)
		{
			*pcError = pc_generation->eConfigure(psSettings);
		}//if (!*pcError)

		delete pc_sample_fenotype_generation;

		if (pc_sample_genotype != pc_sample_fenotype)
		{
			delete pc_sample_genotype;
		}//if (pc_sample_genotype != pc_sample_fenotype)

		delete pc_sample_fenotype;
	}//if (!*pcError && p_type.bHasValue())

	return pc_generation;
}//CGeneration<TGenotype> * GenerationUtils::pcGetGeneration(CProblem<TGenotype, TFenotype> *pcProblem, istream *psSettings, CError *pcError, bool bIsObligatory)

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetSampleFenotypeGeneration(CProblem<CBinaryCoding, CBinaryCoding>*);

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetEmptyGeneration(CProblem<CBinaryCoding, CBinaryCoding>*);;

template CGeneration<CBinaryCoding> * GenerationUtils::pcGetGeneration(CProblem<CBinaryCoding, CBinaryCoding>*, istream*, CError*, bool);