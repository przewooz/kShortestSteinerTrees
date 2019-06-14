#ifndef MUTATION_UTILS_H
#define MUTATION_UTILS_H

#include "BinaryCoding.h"
#include "Error.h"
#include "Mutation.h"

#include <istream>

using namespace std;

namespace MutationUtils
{
	template <class TGenotype>
	CMutation<TGenotype> *pcGetMutation(istream *psSettings, CError *pcError, bool bIsObligatory = true);
}//namespace MutationUtils

#endif//MUTATION_UTILS_H