#ifndef EVALUATION_H
#define EVALUATION_H

#define EVALUATION_ARGUMENT_TYPE "evaluation_type"
#define EVALUATION_ARGUMENT_TYPE_BINARY_CYCLIC_FENOTYPE "binary_cyclic_fenotype"
#define EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE "binary_deceptive"
#define EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE_CONCATENATION "binary_deceptive_concatenation"
#define EVALUATION_ARGUMENT_TYPE_BINARY_KNAPSACK "binary_knapsack"
#define EVALUATION_ARGUMENT_TYPE_BINARY_MAX_SAT "binary_max_sat"
#define EVALUATION_ARGUMENT_TYPE_BINARY_DECEPTIVE_STEP_TRAP "binary_deceptive_step_trap"
#define EVALUATION_ARGUMENT_TYPE_BINARY_NEAREST_NEIGHBOR_NK "binary_nearest_neighbor_nk"
#define EVALUATION_ARGUMENT_TYPE_BINARY_STEINER_TREE_SET "binary_steiner_tree_set"

#define EVALUATION_ARGUMENT_NUMBER_OF_ELEMENTS "evaluation_number_of_elements"

#define EVALUATION_ARGUMENT_SHIFT "evaluation_shift"

#define EVALUATION_ARGUMENT_NUMBER_OF_STATES "evaluation_number_of_states"

#define EVALUATION_ARGUMENT_CONFIG_FILE_PATH "evaluation_config_file_path"

#define EVALUATION_ARGUMENT_WIDTH "evaluation_width"
#define EVALUATION_ARGUMENT_HEIGHT "evaluation_height"
#define EVALUATION_ARGUMENT_GREY_VALUES_ORIGINAL "evaluation_grey_values_original"
#define EVALUATION_ARGUMENT_GREY_VALUES_MAPPING "evaluation_grey_values_mapping"

#include "Error.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace std;

template <class TFenotype> class CInversedEvaluation;
template <class TFenotype> class CConcatenationEvaluation;
template <class TFenotype> class CCyclicFenotypeEvaluation;
template <class TFenotype> class CDynamicEvaluation;


enum EEvaluationType
{
	EVALUATION_BINARY_CYCLIC_FENOTYPE = 0,
	EVALUATION_BINARY_DECEPTIVE = 1,
	EVALUATION_BINARY_DECEPTIVE_CONCATENATION = 2,
	EVALUATION_BINARY_KNAPSACK = 3,
	EVALUATION_BINARY_MAX_SAT = 4,
	EVALUATION_BINARY_DECEPTIVE_STEP_TRAP = 5,
	EVALUATION_BINARY_NEAREST_NEIGHBOR_NK = 6,
	EVALUATION_BINARY_STEINER_TREE_SET = 7
};//enum EEvaluationType


template <class TFenotype>
class CEvaluation
{
friend class CInversedEvaluation<TFenotype>;
friend class CConcatenationEvaluation<TFenotype>;
friend class CCyclicFenotypeEvaluation<TFenotype>;
friend class CDynamicEvaluation<TFenotype>;

public:
	CEvaluation();
	CEvaluation(uint16_t iNumberOfElements, double dMaxValue);

	virtual ~CEvaluation();

	virtual CError eConfigure(istream *psSettings) { return CError(); };

	double dEvaluate(TFenotype *pcFenotype);

	virtual TFenotype *pcCreateSampleFenotype() = 0;

	virtual bool bIsMaxValue(double dValue) { return dValue == d_max_value; };

	uint16_t iGetNumberOfElements() { return i_number_of_elements; };

	double dGetMaxValue() { return d_max_value; };

	uint64_t iGetFFE() { return i_ffe; };

	void vSetZeroFFE() { i_ffe = 0; };

	void vIncreaseFFE() { i_ffe++; };
	void vDecreaseFFE() { i_ffe--; };

protected:
	virtual double d_evaluate(TFenotype *pcFenotype, uint16_t iShift) = 0;

	void v_init(uint16_t iNumberOfElements, double dMaxValue);

	uint16_t i_number_of_elements;

	double d_max_value;

	uint64_t i_ffe;
};//class CEvaluation


template <class TFenotype>
class CInversedEvaluation : public CEvaluation<TFenotype>
{
public:
	CInversedEvaluation(CEvaluation<TFenotype> *pcEvaluation);

	virtual ~CInversedEvaluation();

protected:
	virtual double d_evaluate(TFenotype *pcFenotype, uint16_t iShift);

private:
	CEvaluation<TFenotype> *pc_evaluation;
};//class CInversedEvaluation : public CEvaluation<TFenotype>


template <class TFenotype>
class CConcatenationEvaluation : public CEvaluation<TFenotype>
{
public:
	virtual ~CConcatenationEvaluation();

protected:
	CConcatenationEvaluation();

	virtual double d_evaluate(TFenotype *pcFenotype, uint16_t iShift);

	void v_add_component(CEvaluation<TFenotype> *pcComponent, uint16_t iMultiplier);

private:
	vector<CEvaluation<TFenotype>*> v_components;
};//class CConcatenationEvaluation : public CEvaluation<TFenotype>


template <class TFenotype>
class CCyclicFenotypeEvaluation : public CEvaluation<TFenotype>
{
public:
	virtual ~CCyclicFenotypeEvaluation();

	virtual CError eConfigure(istream *psSettings);

	virtual TFenotype *pcCreateSampleFenotype() { return pc_actual_evaluation->pcCreateSampleFenotype(); };

protected:
	virtual double d_evaluate(TFenotype *pcFenotype, uint16_t iShift);

	virtual void v_prepare_evaluation_fenotype(TFenotype *pcFenotype, uint16_t iShift) = 0;

	uint16_t i_cyclic_shift;

	TFenotype *pc_evaluation_fenotype;

private:
	void v_clear();

	CEvaluation<TFenotype> *pc_actual_evaluation;
};//class CCyclicFenotypeEvaluation : public CEvaluation<TFenotype>


template <class TFenotype>
class CDynamicEvaluation : public CEvaluation<TFenotype>
{
public:
	CDynamicEvaluation();

	virtual ~CDynamicEvaluation();

	virtual CError eConfigure(istream *psSettings);

	virtual void vChange();

	virtual TFenotype *pcCreateSampleFenotype() { return v_states.front()->pcCreateSampleFenotype(); };

	uint16_t iGetNumberOfStates() { return (int16_t)v_states.size(); };
	double *pdGetMaxValues() { return &v_max_values.front(); };

protected:
	virtual double d_evaluate(TFenotype *pcFenotype, uint16_t iShift);

private:
	void v_clear();

	uint16_t i_current_state_index;

	vector<CEvaluation<TFenotype>*> v_states;
	vector<double> v_max_values;
};//class CDynamicEvaluation : public CEvaluation<TFenotype>

#endif//EVALUATION_H