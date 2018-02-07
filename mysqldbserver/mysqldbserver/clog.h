#pragma once

#include<stdarg.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <cstring>

using namespace std;

#define MAX_LOG_BUFF_LENGTH 86400

namespace af
{
	void InitLogCategory();
	void ShutDownCategory();
	void LogInfo(const char* vLogName, const char* vFmt, ...);
	void LogError(const char* vLogName, const char* vFmt, ...);
	void LogDebug(const char* vLogName, const char* vFmt, ...);
	void LogWarn(const char* vLogName, const char* vFmt, ...);

	/*class CVaListBuff
	{
	public:
	CVaListBuff()
	{
	memset(mBuff, 0, sizeof(mBuff));
	}
	char * GetBuffByFormat(const char** vFmt,...)
	{
	memset(mBuff, 0, sizeof(mBuff));
	if (vFmt && *vFmt)
	{
	va_list ap;
	va_start(ap, *vFmt);
	vsprintf(mBuff, *vFmt, ap);
	va_end(ap);
	}
	return mBuff;
	}
	public:
	char mBuff[MAX_LOG_BUFF_LENGTH];
	};

	static CVaListBuff VaListBuff;*/
};