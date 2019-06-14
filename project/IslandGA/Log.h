#ifndef LOG_H
#define LOG_H

#include "Error.h"

#include <atlstr.h>
#include <cstdint>
#include <ostream>
#include <vector>

using namespace std;

class CLog
{
public:
	~CLog();

	void vClear() { v_messages.clear(); };
	
	void vPrintLine(CString sMessage, bool bEcho = false);
	void vPrintEmptyLine(bool bEcho = false) { vPrintLine("", bEcho); };

	CError eSave(ostream *psOutput);
	CError eSave(CString sFilePath);

	bool bIsClear() { return v_messages.size() == 0; };

private:
	static uint32_t iERROR_CODE_LOG_CANNOT_WRITE_TO_STREAM;

	CString s_log_file_path;
	vector<CString> v_messages;
};//class CLog

#endif//LOG_H