#ifndef GENERATION_H
#define GENERATION_H

#define GENERATION_ARGUMENT_TYPE "generation_type"

#include "Error.h"

#include <cstdint>
#include <istream>
#include <vector>

using namespace std;


enum EGenerationType
{

};//enum EGenerationType


template <class TGenotype>
class CGeneration
{
public:
	CGeneration();

	virtual ~CGeneration();

	virtual CError eConfigure(istream *psSettings) { return CError(); };

	virtual TGenotype *pcGenerate() = 0;
	virtual TGenotype *pcGenerateEmpty() = 0;

	virtual void vUpdateBestGenotypes(uint8_t iCount, TGenotype **ppcBestGenotypes);


protected:
	void v_clear_best_genotypes();
	vector<TGenotype*> v_best_genotypes;
};//class CGeneration

#endif//GENERATION_H