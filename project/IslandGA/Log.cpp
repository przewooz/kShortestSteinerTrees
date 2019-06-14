#include "Log.h"

#include <fstream>
#include <iostream>

CLog::~CLog()
{
	vClear();
}//CLog::~CLog()

void CLog::vPrintLine(CString sMessage, bool bEcho)
{
	if (bEcho)
	{
		cout << sMessage << endl;
	}//if (bEcho)

	v_messages.push_back(sMessage);
}//void CLog::vPrintLine(CString sMessage, bool bEcho)

CError CLog::eSave(ostream *psOutput)
{
	CError c_error;

	if (psOutput->good())
	{
		for (uint32_t i = 0; i < (uint32_t)v_messages.size(); i++)
		{
			*(psOutput) << v_messages.at(i) << endl;
		}//for (uint32_t i = 0; i < (uint32_t)v_messages.size(); i++)
	}//if (psOutput->good())
	else
	{
		c_error.vSetError(iERROR_CODE_LOG_CANNOT_WRITE_TO_STREAM);
	}//else if (psOutput->good())

	return c_error;
}//CError CLog::eSave(ostream *psOutput)

CError CLog::eSave(CString sFilePath)
{
	CError c_error;

	ofstream f_log(sFilePath);

	if (f_log.good())
	{
		c_error = eSave(&f_log);
	}//if (f_log.good())
	else
	{
		c_error.vSetError(CError::iERROR_CODE_SYSTEM_WRONG_FILE_PATH, sFilePath);
	}//else if (f_log.good())

	return c_error;
}//CError CLog::eSave(CString sFilePath)

uint32_t CLog::iERROR_CODE_LOG_CANNOT_WRITE_TO_STREAM = CError::iADD_ERROR("iERROR_CODE_LOG_CANNOT_WRITE_TO_STREAM");