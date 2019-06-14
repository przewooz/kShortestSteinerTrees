#ifndef POINTER_UTILS_H
#define POINTER_UTILS_H

#include <cstdint>
#include <utility>

using namespace std;

namespace PointerUtils
{
	template <typename T>
	void vSwap(T *ptElements, uint32_t iIndex0, uint32_t iIndex1) { swap(*(ptElements + iIndex0), *(ptElements + iIndex1)); };
}//namespace PointerUtils

#endif//POINTER_UTILS_H