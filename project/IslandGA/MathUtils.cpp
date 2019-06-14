#include "MathUtils.h"

#include "RandUtils.h"

#include <cmath>

double MathUtils::dComputeAngle(double *pdValues0, double *pdValues1, uint32_t iLength)
{
	double d_cosinus_value = dComputeDotProduct(pdValues0, pdValues1, iLength) / dComputeSecondNorm(pdValues0, iLength) / dComputeSecondNorm(pdValues1, iLength);
	
	if (d_cosinus_value > 1 && round(d_cosinus_value) == 1)
	{
		d_cosinus_value = 1;
	}//if (d_cosinus_value > 1 && round(d_cosinus_value) == 1)
	
	return acos(d_cosinus_value);
}//double MathUtils::dComputeAngle(double *pdValues0, double *pdValues1, uint32_t iLength)

double MathUtils::dComputeDotProduct(double *pdValues0, double *pdValues1, uint32_t iLength)
{
	double d_dot_product = 0;

	for (uint32_t i = 0; i < iLength; i++)
	{
		d_dot_product += *(pdValues0 + i) * *(pdValues1 + i);
	}//for (uint32_t i = 0; i < iLength; i++)

	return d_dot_product;
}//double MathUtils::dComputeDotProduct(double *pdValues0, double *pdValues1, uint32_t iLength)

double MathUtils::dComputeSecondNorm(double *pdValues, uint32_t iLength)
{
	return sqrt(dComputeDotProduct(pdValues, pdValues, iLength));
}//double MathUtils::dComputeSecondNorm(double *pdValues, uint32_t iLength)

double MathUtils::dComputeSquareDistance(double* pdValues0, double* pdValues1, uint32_t iLength, double dMaxValue)
{
	double d_square_distance = 0;

	for (uint32_t i = 0; i < iLength && d_square_distance < dMaxValue; i++)
	{
		d_square_distance += (*(pdValues0 + i) - *(pdValues1 + i)) * (*(pdValues0 + i) - *(pdValues1 + i));
	}//for (uint32_t i = 0; i < iLength && d_square_distance < dMaxValue; i++)

	if (d_square_distance > dMaxValue)
	{
		d_square_distance = dMaxValue;
	}//if (d_square_distance > dMaxValue)

	return d_square_distance;
}//double MathUtils::dComputeSquareDistance(double* pdValues0, double* pdValues1, uint32_t iLength, double dMaxValue)

double MathUtils::dMaxValue(double *pdValues, uint32_t iLength)
{
	double d_max_value = -DBL_MAX;

	for (uint32_t i = 0; i < iLength; i++)
	{
		if (*(pdValues + i) > d_max_value)
		{
			d_max_value = *(pdValues + i);
		}//if (*(pdValues + i) > d_max_value)
	}//for (uint32_t i = 0; i < iLength; i++)

	return d_max_value;
}//double MathUtils::dMaxValue(double *pdValues, uint32_t iLength)

double MathUtils::dComputeMonteCarloIntegral(function<double(double*)> fFunction, double dMaxFunctionValue, double *pdMinValues, double *pdMaxValues, uint8_t iNumberOfDimensions, uint32_t iNumberOfSamples)
{
	uint32_t i_number_of_hits = 0;

	double *pd_sample_values = new double[iNumberOfDimensions];

	for (uint32_t i = 0; i < iNumberOfSamples; i++)
	{
		for (uint8_t j = 0; j < iNumberOfDimensions; j++)
		{
			*(pd_sample_values + j) = RandUtils::dRandNumber(*(pdMinValues + j), *(pdMaxValues + j));
		}//for (uint8_t j = 0; j < iNumberOfDimensions; j++)

		if (RandUtils::dRandNumber(dMaxFunctionValue) <= fFunction(pd_sample_values))
		{
			i_number_of_hits++;
		}//if (RandUtils::dRandNumber(dMaxFunctionValue) <= fFunction(pd_sample_values))
	}//for (uint32_t i = 0; i < iNumberOfSamples; i++)

	delete pd_sample_values;

	double d_max_volume = dMaxFunctionValue;

	for (uint8_t i = 0; i < iNumberOfDimensions; i++)
	{
		d_max_volume *= *(pdMaxValues + i) - *(pdMinValues + i);
	}//for (uint8_t i = 0; i < iNumberOfDimensions; i++)

	return d_max_volume * (double)i_number_of_hits / (double)iNumberOfSamples;
}//double MathUtils::dComputeMonteCarloIntegral(function<double(double*)> fFunction, double dMaxFunctionValue, double *pdMinValues, double *pdMaxValues, uint8_t iNumberOfDimensions, uint32_t iNumberOfSamples)