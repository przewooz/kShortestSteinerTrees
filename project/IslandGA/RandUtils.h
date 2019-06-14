#ifndef RAND_UTILS_H
#define RAND_UTILS_H

#define DEF_PROBABILITY_MIN 0.0f
#define DEF_PROBABILITY_MAX 1.0f

#include <cstdint>
#include <unordered_set>

using namespace std;

namespace RandUtils
{
    uint32_t iInit();
    void vInit(uint32_t iSeed);

	uint32_t iRandIndex(uint32_t iSize);
	uint32_t iRandUniqueIndex(uint32_t iSize, unordered_set<uint32_t> *psSelected);
    uint32_t iRandNumber(uint32_t iMinValue, uint32_t iMaxValue);
	uint32_t iRandUniqueNumber(uint32_t iMinValue, uint32_t iMaxValue, unordered_set<uint32_t> *psSelected);

    int32_t iRandNumber(int32_t iMinValue, int32_t iMaxValue);

	double dRandNumber(double dMaxValue);
	double dRandNumber(double dMinValue, double dMaxValue);

    float fRandNumber(float fMinValue, float fMaxValue);
    float fRandProbability();
	bool bSatisfyProbability(float fProbability);
}//namespace RandUtils

#endif//RAND_UTILS_H