
#ifndef _FILE_LOG_H_
#define _FILE_LOG_H_


#include <stdio.h>

// Macro define region
#define FILESTATUS_CLOSE			0
#define FILESTATUS_OPEN				1

#define MAXSIZE_CURTIME				64
#define MAXSIZE_PATHNAME			512
#define MAXSIZE_PERMSG				(2*1024)
#define MAXSIZE_MSGBUF				(8*1024)
#ifdef WIN32
#define SUBDIRNAME_LOG				"logs\\"
#else
#define SUBDIRNAME_LOG				"./logs/"
#endif
#define SERVERLOG_SKIP				" -> "

#define LOGLEADNAME_DEBUG						"Debug"



class FILELOGS  
{
public:
	FILELOGS();
	virtual ~FILELOGS();

	static void WriteLog(const char*pcLogMsg, ...);
private:
	static FILE *m_fileSvrLog;
	static int m_nFileStatus;

	static void CloseLog();
	static int OpenLog(const char*pcLogLead);
	static void WriteData(const char*pcLogMsg, const int len = -1);
	static void WriteLogInner(const char*pcLogMsg, const char*pcLogLead, const int len = -1, bool addTimeStamp = true);

};

#endif