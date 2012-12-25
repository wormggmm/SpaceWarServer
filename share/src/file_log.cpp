//////////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-7-25 11:05
//      File_base        : cfs_filelogs
//      File_ext         : cpp
//      Author           : Cooler(liuyujun@263.net)
//      Description      : 9Sword series file logs writer
//
//      <Change_list>
//
//      Example:
//      {
//      Change_datetime  : year-month-day hour:minute
//      Change_by        : changed by who
//      Change_purpose   : change reason
//      }
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Include region
#ifdef WIN32
#include <windows.h>
#endif

#include <stdarg.h>
#include "file_log.h"
#include "tools.h"
static Mutex m_gLogMutex(0);

FILE *FILELOGS::m_fileSvrLog = NULL;
int FILELOGS::m_nFileStatus = FILESTATUS_CLOSE;


//////////////////////////////////////////////////////////////////////////
// CFS_FILELOGS class implement region

// CFS_FILELOGS class CONSTruct&deCONSTruct functions

FILELOGS::FILELOGS()
{
}

FILELOGS::~FILELOGS()
{
}
void FILELOGS::WriteLog(const char*pcLogMsg, ...)
{
	char szMsgBuf[MAXSIZE_MSGBUF] = {0};
	va_list va;
	va_start(va, pcLogMsg);
	vsprintf(szMsgBuf, pcLogMsg, va);
	va_end(va);
	WriteLogInner(szMsgBuf, LOGLEADNAME_DEBUG);
}
// CFS_FILELOGS class member functions
void FILELOGS::WriteLogInner(const char*pcLogMsg, const char*pcLogLead, const int len, bool addTimeStamp)
{
	if(pcLogMsg == NULL || pcLogLead == NULL)
	{
		return;
	}

	m_gLogMutex.lock();

	int nStatus = OpenLog(pcLogLead);
	if(nStatus != -1)
	{
		if(addTimeStamp)
		{
			char szCurTime[MAXSIZE_CURTIME] = {0};
			GetCurDateTimeWithString(szCurTime);
			strcat(szCurTime, SERVERLOG_SKIP);
			WriteData(szCurTime);
		}
		WriteData(pcLogMsg, len);
		CloseLog();
	}

	m_gLogMutex.unlock();
}


int FILELOGS::OpenLog(const char*pcLogLead)
{
	int nRet = 0;

	if(m_nFileStatus == FILESTATUS_CLOSE)
	{
		char szOpenPathName[MAXSIZE_PATHNAME] = {0};

		time_t	ltime;
		tm		tmStruct;
		time(&ltime);
		tmStruct = *localtime(&ltime);

		strcat(szOpenPathName, SUBDIRNAME_LOG);
		sprintf(szOpenPathName + strlen(szOpenPathName), 
			"%s%04d-%02d.log",
			pcLogLead, 
			tmStruct.tm_year+1900,
			tmStruct.tm_mon+1);
#ifdef WIN32
		CreateDirectory(TEXT(SUBDIRNAME_LOG), NULL);
#else /* LINUX */
		mkdir(SUBDIRNAME_LOG, 0700);
#endif

		m_fileSvrLog = fopen(szOpenPathName, "a+");
		if(m_fileSvrLog) 
		{
			m_nFileStatus = FILESTATUS_OPEN;
		}
		else
		{
			nRet = -1;
		}
	}

	return nRet;
}

void FILELOGS::CloseLog()
{
	if(m_nFileStatus == FILESTATUS_OPEN)
	{
		fclose(m_fileSvrLog);
		m_fileSvrLog = NULL;
		m_nFileStatus = FILESTATUS_CLOSE;
	}
}

void FILELOGS::WriteData(const char*pcLogMsg, const int len)
{
	if(pcLogMsg == NULL)
	{
		return;
	}

	if(m_nFileStatus == FILESTATUS_OPEN)
	{
		if( len == -1 )
			fwrite(pcLogMsg, 1, strlen(pcLogMsg), m_fileSvrLog);
		else
			fwrite(pcLogMsg, 1, len, m_fileSvrLog);
	}
}
