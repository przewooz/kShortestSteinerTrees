#ifndef OPTIMIZATION_COMMAND_H
#define OPTIMIZATION_COMMAND_H

#include "BinaryCoding.h"
#include "CommandExecution.h"
#include "CommandParamParser.h"
#include "Error.h"

#include <atlstr.h>
#include <iostream>

using namespace std;


class COptimizationBasedCommand : public CCommandExecution
{
public:
	COptimizationBasedCommand(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser);

protected:
	virtual CError e_optimize_binary_binary() = 0;
};//class COptimizationBasedCommand : public CCommandExecution


class COptimizationCommand : public COptimizationBasedCommand
{
public:
	COptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary);

protected:
	virtual CError e_optimize_binary_binary() { return e_optimize<CBinaryCoding, CBinaryCoding>(); };

private:
	template <class TGenotype, class TFenotype>
	CError e_optimize();
};//class COptimizationCommand : public COptimizationBasedCommand


class CDynamicOptimizationCommand : public COptimizationBasedCommand
{
public:
	CDynamicOptimizationCommand(istream *psSettings, CLog *pcLog, ostream *psSummary);

protected:
	virtual CError e_optimize_binary_binary() { return e_optimize<CBinaryCoding, CBinaryCoding>(); };

private:
	template <class TGenotype, class TFenotype>
	CError e_optimize();
};//class COptimizationCommand : public COptimizationBasedCommand

#endif//OPTIMIZATION_COMMAND_H