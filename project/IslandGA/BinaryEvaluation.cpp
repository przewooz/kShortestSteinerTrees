#include "BinaryEvaluation.h"

#include "StringCommandParam.h"
#include "UIntCommandParam.h"

#include <atlstr.h>
#include <cfloat>
#include <functional>


void CBinaryCyclicFenotypeEvaluation::v_prepare_evaluation_fenotype(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	uint16_t i_index_without_shift;

	for (uint16_t i = 0; i < i_number_of_elements; i++)
	{
		i_index_without_shift = (i + i_cyclic_shift) % i_number_of_elements;
		*(pc_evaluation_fenotype->piGetBits() + i + iShift) = *(pcFenotype->piGetBits() + i_index_without_shift + iShift);
	}//for (uint16_t i = 0; i < i_number_of_elements; i++)
}//void CBinaryCyclicFenotypeEvaluation::v_prepare_evaluation_fenotype(CBinaryCoding *pcFenotype, uint16_t iShift)


CBinaryEvaluation::CBinaryEvaluation()
	: CEvaluation<CBinaryCoding>()
{

}//CBinaryEvaluation::CBinaryEvaluation()

CBinaryEvaluation::CBinaryEvaluation(uint16_t iNumberOfElements, double dMaxValue)
	: CEvaluation<CBinaryCoding>(iNumberOfElements, dMaxValue)
{

}//CBinaryEvaluation::CBinaryEvaluation(uint16_t iNumberOfElements, double dMaxValue)

CBinaryCoding * CBinaryEvaluation::pcCreateSampleFenotype()
{
	return new CBinaryCoding(i_number_of_elements);
}//CBinaryCoding * CBinaryEvaluation::pcCreateSampleFenotype()


CError CBinaryFileConfigEvaluation::eConfigure(istream * psSettings)
{
	CError c_error = CEvaluation<CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CStringCommandParam p_config_file_path(EVALUATION_ARGUMENT_CONFIG_FILE_PATH);
		CString s_config_file_path = p_config_file_path.sGetValue(psSettings, &c_error);

		FILE *pf_config = fopen(s_config_file_path, "r");

		if (pf_config)
		{
			c_error = e_init(pf_config);
		}//if (pf_config)
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_FILE_NOT_FOUND, s_config_file_path);
		}//else if (pf_config)
	}//if (!c_error)

	return c_error;
}//CError CBinaryFileConfigEvaluation::eConfigure(istream * psSettings)


CBinaryDeceptiveEvaluation::CBinaryDeceptiveEvaluation()
{
	pc_deceptive_function = nullptr;
}//CBinaryDeceptiveEvaluation::CBinaryDeceptiveEvaluation()

CBinaryDeceptiveEvaluation::CBinaryDeceptiveEvaluation(FILE *pfConfig, CError *pcError)
{
	*pcError = e_init(pfConfig);
}//CBinaryDeceptiveEvaluation::CBinaryDeceptiveEvaluation(FILE *pfConfig, CError *pcError)

CBinaryDeceptiveEvaluation::~CBinaryDeceptiveEvaluation()
{
	delete pc_deceptive_function;
}//CBinaryDeceptiveEvaluation::~CBinaryDeceptiveEvaluation()

CError CBinaryDeceptiveEvaluation::e_init(FILE *pfConfig)
{
	delete pc_deceptive_function;
	pc_deceptive_function = nullptr;
	
	CError c_error = eLoadSingleCompProblem(pfConfig, &pc_deceptive_function);

	if (pc_deceptive_function)
	{
		i_number_of_elements = (uint16_t)pc_deceptive_function->iGetBitLength();
		d_max_value = pc_deceptive_function->dGetMaxFuncVal();
	}//if (pc_deceptive_function)

	fclose(pfConfig);

	return c_error;
}//CError CBinaryDeceptiveEvaluation::e_init(FILE *pfConfig)

double CBinaryDeceptiveEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	return pc_deceptive_function->dGetFuncValue(iShift, pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits());
}//double CBinaryDeceptiveEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)


CBinaryDeceptiveConcatenationEvaluation::CBinaryDeceptiveConcatenationEvaluation()
{
	pc_deceptive_concatenation_function = nullptr;
}//CBinaryDeceptiveConcatenationEvaluation::CBinaryDeceptiveConcatenationEvaluation()

CBinaryDeceptiveConcatenationEvaluation::CBinaryDeceptiveConcatenationEvaluation(FILE *pfConfig, CError *pcError)
{
	*pcError = e_init(pfConfig);
}//CBinaryDeceptiveConcatenationEvaluation::CBinaryDeceptiveConcatenationEvaluation(FILE *pfConfig, CError *pcError)

CBinaryDeceptiveConcatenationEvaluation::~CBinaryDeceptiveConcatenationEvaluation()
{
	delete pc_deceptive_concatenation_function;
}//CBinaryDeceptiveConcatenationEvaluation::~CBinaryDeceptiveConcatenationEvaluation()

CError CBinaryDeceptiveConcatenationEvaluation::e_init(FILE *pfConfig)
{
	delete pc_deceptive_concatenation_function;
	pc_deceptive_concatenation_function = new CConcatCompProblem();
	
	CError c_error = pc_deceptive_concatenation_function->eLoad(pfConfig);

	if (!c_error)
	{
		i_number_of_elements = (uint16_t)pc_deceptive_concatenation_function->iGetProblemBitLength();
		d_max_value = pc_deceptive_concatenation_function->dGetMaxFuncVal();
	}//if (!c_error)

	fclose(pfConfig);

	return c_error;
}//CError CBinaryDeceptiveConcatenationEvaluation::e_init(FILE *pfConfig)

double CBinaryDeceptiveConcatenationEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	double d_fitness_value;
	pc_deceptive_concatenation_function->eGetFuncValue(pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits(), &d_fitness_value);

	return d_fitness_value;
}//void CBinaryDeceptiveConcatenationEvaluation::vEvaluate(CBinaryCoding *pcFenotype, double *pdFitnessValue)

CError CBinaryDeceptiveConcatenationEvaluation::eReport(FILE *pfReport)
{
	return pc_deceptive_concatenation_function->eCreateReport(pfReport);
}//CError CBinaryDeceptiveConcatenationEvaluation::eReport(FILE *pfReport)


CBinaryKnapsackEvaluation::CBinaryKnapsackEvaluation()
{
	
}//CBinaryKnapsackEvaluation::CBinaryKnapsackEvaluation()

CBinaryKnapsackEvaluation::CBinaryKnapsackEvaluation(FILE *pfConfig, CError *pcError)
{
	*pcError = e_init(pfConfig);
}//CBinaryKnapsackEvaluation::CBinaryKnapsackEvaluation(uint16_t iNumberOfElements, double dCapacity, double *pdValues, double *pdWeights)

CError CBinaryKnapsackEvaluation::e_init(FILE *pfConfig)
{
	CError c_error = c_knapsack.eLoadSettings(pfConfig);

	if (!c_error)
	{
		i_number_of_elements = (uint16_t)c_knapsack.iGetBitLength();
		d_max_value = c_knapsack.dGetMaxFuncVal();
	}//if (!c_error)

	fclose(pfConfig);

	return c_error;
}//CError CBinaryKnapsackEvaluation::e_init(FILE *pfConfig)

double CBinaryKnapsackEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	//c_knapsack.bRepairGreedy(iShift, pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits());
	return c_knapsack.dGetFuncValue(iShift, pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits());
}//double CBinaryKnapsackEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)


CBinaryMaxSatEvaluation::CBinaryMaxSatEvaluation()
{

}//CBinaryMaxSatEvaluation::CBinaryMaxSatEvaluation()

CBinaryMaxSatEvaluation::CBinaryMaxSatEvaluation(FILE *pfConfig, CError *pcError)
{
	*pcError = e_init(pfConfig);
}//CBinaryMaxSatEvaluation::CBinaryMaxSatEvaluation(FILE *pfConfig, CError *pcError)

CError CBinaryMaxSatEvaluation::e_init(FILE *pfConfig)
{
	CError c_error = c_max_sat.eLoadSettings(pfConfig);

	if (!c_error)
	{
		i_number_of_elements = (uint16_t)c_max_sat.iGetBitLength();
		d_max_value = c_max_sat.dGetMaxFuncVal();
	}//if (!c_error)

	fclose(pfConfig);

	return c_error;
}//CError CBinaryMaxSatEvaluation::e_init(FILE *pfConfig)

double CBinaryMaxSatEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	return c_max_sat.dGetFuncValue(iShift, pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits());
}//double CBinaryMaxSatEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)


CBinaryDeceptiveStepTrapEvaluation::CBinaryDeceptiveStepTrapEvaluation()
{
	d_max_value = 1.0;
}//CBinaryDeceptiveStepTrapEvaluation::CBinaryDeceptiveStepTrapEvaluation()

CError CBinaryDeceptiveStepTrapEvaluation::eConfigure(istream *psSettings)
{
	CError c_error = CBinaryEvaluation::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_elements(EVALUATION_ARGUMENT_NUMBER_OF_ELEMENTS, 1, UINT16_MAX);
		i_number_of_elements = p_number_of_elements.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_trap_size(EVALUATION_BINARY_DECEPTIVE_STEP_TRAP_ARGUMENT_TRAP_SIZE, 1, UINT16_MAX);
		i_trap_size = p_trap_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_step_size(EVALUATION_BINARY_DECEPTIVE_STEP_TRAP_ARGUMENT_STEP_SIZE, 1, i_trap_size);
		i_step_size = p_step_size.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		i_offset = (i_trap_size - i_step_size) % i_step_size;
	}//if (!c_error)

	return c_error;
}//CError CBinaryDeceptiveStepTrapEvaluation::eConfigure(istream *psSettings)

double CBinaryDeceptiveStepTrapEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	uint16_t i_total = 0;

	uint16_t i_trap_maximum = (i_offset + i_trap_size) / i_step_size;

	uint16_t i_partial;

	for (uint16_t i = 0; i < i_number_of_elements; i += i_trap_size)
	{
		i_partial = 0;

		for (uint16_t j = i; j < i + i_trap_size; j++)
		{
			i_partial += (uint16_t)(*(pcFenotype->piGetBits() + j + iShift));
		}//for (uint16_t j = 0; j < i + i_trap_size; j++)

		if (i_partial < i_trap_size)
		{
			i_partial = i_trap_size - i_partial - 1;
		}//if (i_partial < i_trap_size)

		i_total += (i_offset + i_partial) / i_step_size;
	}//for (uint16_t i = 0; i < i_number_of_elements; i += i_trap_size)

	return (double)(i_total * i_trap_size) / (double)(i_number_of_elements * i_trap_maximum);
}//double CBinaryDeceptiveStepTrapEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)


CBinaryNearestNeighborNKEvaluation::CBinaryNearestNeighborNKEvaluation()
{
	d_max_value = 1.0;
	pc_native_nearest_neighbor_nk = nullptr;
}//CBinaryNearestNeighborNKEvaluation::CBinaryNearestNeighborNKEvaluation()

CBinaryNearestNeighborNKEvaluation::~CBinaryNearestNeighborNKEvaluation()
{
	v_clear();
}//CBinaryNearestNeighborNKEvaluation::~CBinaryNearestNeighborNKEvaluation()

CError CBinaryNearestNeighborNKEvaluation::eConfigure(istream *psSettings)
{
	v_clear();

	CError c_error = CBinaryEvaluation::eConfigure(psSettings);

	if (!c_error)
	{
		CUIntCommandParam p_number_of_elements(EVALUATION_ARGUMENT_NUMBER_OF_ELEMENTS, 1, UINT16_MAX);
		i_number_of_elements = (uint16_t)p_number_of_elements.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	//if (!c_error)
	//{
	//	CUIntCommandParam p_problem_seed(EVALUATION_BINARY_P3_ARGUMENT_PROBLEM_SEED);
	//	i_problem_seed = p_problem_seed.iGetValue(psSettings, &c_error);
	//}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_precision(EVALUATION_BINARY_P3_ARGUMENT_PRECISION);
		i_precision = p_precision.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		CUIntCommandParam p_k(EVALUATION_BINARY_NEAREST_NEIGHBOR_NK_ARGUMENT_K, 1, UINT16_MAX);
		i_k = (uint16_t)p_k.iGetValue(psSettings, &c_error);
	}//if (!c_error)

	if (!c_error)
	{
		Configuration c_config;

		c_config.set("length", (int)i_number_of_elements);
		
		//c_config.set("problem_seed", (int)i_problem_seed);
		c_config.set("problem_seed", 0);
		
		c_config.set("precision", (int)i_precision);
		c_config.set("k", (int)i_k);

		c_config.set("problem_folder", string(""));

		pc_native_nearest_neighbor_nk = new NearestNeighborNK(c_config, 0);

		v_native_solution.resize(i_number_of_elements);
	}//if (!c_error)

	return c_error;
}//CError CBinaryNearestNeighborNKEvaluation::eConfigure(istream *psSettings)

double CBinaryNearestNeighborNKEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	for (uint16_t i = 0; i < pcFenotype->iGetNumberOfBits(); i++)
	{
		v_native_solution.at(i) = *(pcFenotype->piGetBits() + i + iShift) == 1;
	}//for (uint16_t i = 0; i < pcFenotype->iGetNumberOfBits(); i++)

	return (double)pc_native_nearest_neighbor_nk->evaluate(v_native_solution);
}//double CBinaryNearestNeighborNKEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)

void CBinaryNearestNeighborNKEvaluation::v_clear()
{
	delete pc_native_nearest_neighbor_nk;
	pc_native_nearest_neighbor_nk = nullptr;
}//void CBinaryNearestNeighborNKEvaluation::v_clear()


CBinarySteinerTreeSetDynamicEvaluation::CBinarySteinerTreeSetDynamicEvaluation()
{
	pc_steiner_tree_set_function = new CSteinerTreeSet();
}//CBinarySteinerTreeSetDynamicEvaluation::CBinarySteinerTreeSetDynamicEvaluation()

CError CBinarySteinerTreeSetDynamicEvaluation::eConfigure(istream *psSettings)
{
	CError c_error = CEvaluation<CBinaryCoding>::eConfigure(psSettings);

	if (!c_error)
	{
		CStringCommandParam p_config_file_path(EVALUATION_ARGUMENT_CONFIG_FILE_PATH);
		CString s_config_file_path = p_config_file_path.sGetValue(psSettings, &c_error);

		FILE *pf_config = fopen(s_config_file_path, "r");

		if (pf_config)
		{
			c_error = pc_steiner_tree_set_function->eLoadSettings(pf_config);

			if (!c_error)
			{
				i_number_of_elements = pc_steiner_tree_set_function->iGetBitLength();
				d_max_value = DBL_MAX;
			}//if (!c_error)

			fclose(pf_config);
		}//if (pf_config)
		else
		{
			c_error.vSetError(CError::iERROR_CODE_SYSTEM_FILE_NOT_FOUND, s_config_file_path);
		}//else if (pf_config)
	}//if (!c_error)

	return c_error;
}//CError CBinarySteinerTreeSetDynamicEvaluation::eConfigure(istream *psSettings)

void CBinarySteinerTreeSetDynamicEvaluation::vChange()
{
	double d_penalty_factor = 1.2 * pc_steiner_tree_set_function->dGetPenaltyFactor();
	pc_steiner_tree_set_function->vPenaltyFactorSet(d_penalty_factor);
}//void CBinarySteinerTreeSetDynamicEvaluation::vChange()

double CBinarySteinerTreeSetDynamicEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)
{
	return pc_steiner_tree_set_function->dGetFuncValue(iShift, pcFenotype->piGetBits(), pcFenotype->iGetNumberOfBits());
}//double CBinarySteinerTreeSetDynamicEvaluation::d_evaluate(CBinaryCoding *pcFenotype, uint16_t iShift)