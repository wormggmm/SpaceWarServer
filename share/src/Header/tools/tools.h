/////////////////////////////////////////////////////////////////////////////
//  FileName    :   minister.cpp
//  Creator     :   zuolizhi
//  Date        :   2006-5-30 9:54:00
//  Comment     :   Function Routine Define
//	Changes		:	
/////////////////////////////////////////////////////////////////////////////
#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <time.h>
#include "def.h"
#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////

#define MY_ALIGN(A,L)	(((A) + (L) - 1) & ~((L) - 1))
#define ALIGN_SIZE(A)	MY_ALIGN((A),sizeof(double))
/* Size to make adressable obj. */
#define ALIGN_PTR(A, t) ((t*) MY_ALIGN((A),sizeof(t)))
/* Offset of field f in structure t */
#define OFFSET(t, f)	((size_t)(char *)&((t *)0)->f)


//time or timer

#define MAXSIZE_TIMESTRING				64

extern const char *GetCurDateTimeWithString(char *pstringbuf = NULL );
extern const char *GetCurYearMonthWithString(char *pstringbuf = NULL );
extern void TimerStart();
extern unsigned long	UNIX_TIME_STAMP;

extern bool string_md5( IN char* inputString, OUT char* md5String );
extern bool file_md5( IN char* fileName, OUT char* md5String );


#ifndef WIN32
extern void Sleep( unsigned long ulTime);
#endif


class timer: public Thread
{
private:
	bool m_Run;
public:
	timer();
	~timer();
public:
	bool IsRun();
	int start();
	void* action();
	int stop();
};


//inline void tyield( unsigned long ulTime = 10 )
//{
//	#ifdef WIN32
//	Sleep( ulTime );
//	#else
//	struct timeval sleeptime;
//	sleeptime.tv_sec = 0;
//	sleeptime.tv_usec=ulTime*1000;
//	select(0,0,0,0,&sleeptime);
//	//usleep(ulTime*1000);//geoffyan
//	#endif
//}
//

class RingBuffer
{
	
public:
	RingBuffer( );
	~RingBuffer( );
	
	int SndPack( const void* pData, unsigned datasize );
	int SndPack( const void *pData1, const void *pData2, unsigned datasize1, unsigned datasize2 );
	const void* RcvPack( unsigned& datasize );
	
	int Init( char* StartPtr, unsigned uBufLen,unsigned uMaxPack );

	int Reinit( );

private:

	/*
	 *	写入数据,返回右指针
	 */
	char* PutData( char* leftptr, char* rightptr, char* pData, unsigned datasize );
	
	unsigned GetDataLen( char* leftptr, char* rightptr );

	void Clear( );
	
private:

	char* m_startptr;
	char* m_leftptr;
	char* m_rightptr;
	char* m_tmpptr;
	char* m_nextleft;
	/*
	*	var
	*/
	
	unsigned	m_uBufLen;
	unsigned	m_uMaxPack;
};


#endif
