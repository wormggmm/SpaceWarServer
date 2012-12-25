
#ifndef _HEADER_DEBUG_
#define _HEADER_DEBUG_

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#ifdef _WIN32
#include <WinSock.h>
#include <Windows.h>
#pragma comment( lib, "ws2_32.lib" )
#else

#endif

#ifdef _DEBUG
// �����Ϣ��Output��Debug����
#define OutPutStringToDebugWin				_OutPutStringToDebugWin   		//e.g. OutPutStringToDebugWin( "Debug message %s\n", szMessage );
//���lasterrorno��lasterrornoת����info
#define OutPutLastErrorMsgToDebugWin		_OutPutLastErrorMsgToDebugWin		//e.g. OutPutLastErrorMsgDebugWin( );
// �����Ϣ�������д���
#define OutPutDebugStringToCMD				_OutPutDebugStringToCMD			//e.g. OutPutDebugStringCMD( "Debug message %s\n", szMessage );
//���lasterrorno��lasterrornoת����info
#define OutPutLastErrorMsgToCMD				_OutPutLastErrorMsgToCMD			//e.g. OutPutLastErrorMsgCMD( );
//���log��cmd����
#define OutPutStringToCMD							_OutPutStringToCMD				//e.g. OutPutStringToCMD( "message %s\n", szMessage );
#else
#define OutPutStringToDebugWin	
#define OutPutLastErrorMsgToDebugWin
#define OutPutDebugStringToCMD				_OutPutDebugStringToCMD			//e.g. OutPutDebugStringCMD( "Debug message %s\n", szMessage );
//#define OutPutLastErrorMsgToCMD()			(1)
#define OutPutLastErrorMsgToCMD				_OutPutLastErrorMsgToCMD			//e.g. OutPutLastErrorMsgCMD( );
#define OutPutStringToCMD							_OutPutStringToCMD				//e.g. OutPutStringToCMD( "message %s\n", szMessage );
#endif


extern void _OutPutStringToDebugWin(const char * strOutputString,...);

extern void _OutPutLastErrorMsgToDebugWin( );

extern void _OutPutDebugStringToCMD( const char * strOutputString, ... );

extern void _OutPutLastErrorMsgToCMD( );

extern void _OutPutStringToCMD( const char* strOutputString, ...);
#endif
