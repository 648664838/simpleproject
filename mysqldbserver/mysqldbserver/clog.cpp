#include "clog.h"

#include<iostream>

#include <string>

#include <stdio.h>
#include "log4cpp/Portability.hh"
#ifdef LOG4CPP_HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#ifdef LOG4CPP_HAVE_SYSLOG
#include "log4cpp/SyslogAppender.hh"
#endif
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"

using namespace std;

void af::InitLogCategory()
{
	const int nLogNum = 5;
	string strLogName[nLogNum][2] =
	{
		{ "", "./Log/default.txt" },
		{ "default", "./Log/default.txt" },
		{ "login", "./Log/login.txt" },
		{ "socket", "./Log/socket.txt" },
		{ "logic", "./Log/logic.txt" }
	};

	log4cpp::PatternLayout* pLayout = new log4cpp::PatternLayout();
	pLayout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} : %p] : %m%n");

	log4cpp::Category * Categories[nLogNum] = { NULL };
	log4cpp::Appender* appender[nLogNum] = { NULL };
	for (int i = 0; i < nLogNum; ++i)
	{
		Categories[i] = &log4cpp::Category::getInstance(strLogName[i][0]);
		if (Categories[i] == NULL)
		{
			continue;
		}
		appender[i] = new log4cpp::FileAppender(strLogName[i][0], strLogName[i][1]);
		if (appender[i])
		{
			appender[i]->setLayout(pLayout);
		}
		Categories[i]->setAppender(appender[i]);
	}
}

void af::ShutDownCategory()
{
	log4cpp::Category::shutdown();
}

void af::LogInfo(const char* vLogName, const char* vFmt, ...)
{
	log4cpp::Category::getInstance(vLogName).infoStream() << VaListBuff.GetBuffByFormat(&vFmt);
}

void af::LogError(const char* vLogName, const char* vFmt, ...)
{
	log4cpp::Category::getInstance(vLogName).errorStream() << VaListBuff.GetBuffByFormat(&vFmt);
}

void af::LogDebug(const char* vLogName, const char* vFmt, ...)
{
	log4cpp::Category::getInstance(vLogName).debugStream() << VaListBuff.GetBuffByFormat(&vFmt);
}

void af::LogWarn(const char* vLogName, const char* vFmt, ...)
{
	log4cpp::Category::getInstance(vLogName).warnStream() << VaListBuff.GetBuffByFormat(&vFmt);
}