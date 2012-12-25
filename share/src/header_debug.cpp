#include "header_debug.h"
#ifdef _WIN32
#else
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#endif
void _OutPutStringToDebugWin(const char * strOutputString,...)
{
	char strBuffer[4096]={0};
	va_list args;
	va_start(args,strOutputString);
#ifdef _WIN32
	_vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#else
	vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#endif
	va_end(args);
#ifdef _WIN32
	OutputDebugStringA(strBuffer);
#else
	printf( strBuffer );
#endif
} 
void _OutPutLastErrorMsgToDebugWin( )
{
#ifdef _WIN32
	_OutPutStringToDebugWin( "LastErrorno: %d <Error Info: %s>\n", GetLastError(), strerror( GetLastError() ) );
#else
	_OutPutStringToDebugWin( "LastErrorno: %d <Error Info: %s>\n", errno, strerror(errno) );
#endif
}
void _OutPutDebugStringToCMD( const char * strOutputString, ... )
{
	char strBuffer[4096]={0};
	va_list args;
	va_start(args,strOutputString);
#ifdef _WIN32
	_vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#else
	vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#endif
	va_end(args);
	printf( strBuffer );
}

void _OutPutLastErrorMsgToCMD( )
{
#ifdef _WIN32
	_OutPutDebugStringToCMD( "LastErrorno: %d <Error Info: %s>\n", GetLastError(), strerror( GetLastError() ) );
#else
	_OutPutDebugStringToCMD( "LastErrorno: %d <Error Info: %s>\n", errno, strerror(errno) );
#endif
}


void _OutPutStringToCMD(const char * strOutputString,...)
{
	char strBuffer[4096]={0};
	va_list args;
	va_start(args,strOutputString);
#ifdef _WIN32
	_vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#else
	vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,args);
#endif
	va_end(args);
	printf( strBuffer );
} 
