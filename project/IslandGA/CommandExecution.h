#ifndef COMMAND_EXECUTION_H
#define COMMAND_EXECUTION_H

#include "CommandParamParser.h"
#include "Error.h"
#include "Log.h"

#include <atlstr.h>
#include <iostream>

using namespace std;

class CCommandExecution
{
public:
	CCommandExecution(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary);

	virtual ~CCommandExecution();

	virtual CError eExecute(CCommandParamParser *pcParser) = 0;

	CString sGetName() { return s_name; };

protected:
	istream *ps_settings;
	CLog *pc_log;
	ostream *ps_summary;

private:
	CString s_name;
};//class CCommandExecution

#endif//COMMAND_EXECUTION_H