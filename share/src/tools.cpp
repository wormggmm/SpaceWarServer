/////////////////////////////////////////////////////////////////////////////
//  FileName    :   minister.cpp
//  Creator     :   zuolizhi
//  Date        :   2006-5-30 9:54:00
//  Comment     :   Function Routine Define
//	Changes		:	
/////////////////////////////////////////////////////////////////////////////
#include "tools.h"
#include "header_debug.h"
#include "md5.h"
#include "md5c.h"
/////////////////////////////////////////////////////////////////////////////
//
//              Global variables and Macro and Structure Definitions
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
//              Function Definitions
//
/////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
void Sleep( unsigned long ulTime = 10 )
{
	struct timeval sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_usec=ulTime*1000;
	select(0,0,0,0,&sleeptime);
}

#endif

unsigned long		UNIX_TIME_STAMP;
void TimerStart()
{
	static timer _timer;
	if( !_timer.IsRun() )
		_timer.start();
}
const char *GetCurDateTimeWithString(char *pStringBuf)
{
	time_t	tCurDateTime;
	tm		*ptagCurDataTime;
	char	szTimeStringBuf[MAXSIZE_TIMESTRING] = {0};

	char *pWriteStringBuf = pStringBuf;
	if(NULL == pWriteStringBuf)
	{
		pWriteStringBuf = szTimeStringBuf;
	}

	time(&tCurDateTime);
	ptagCurDataTime = localtime(&tCurDateTime);

	sprintf(pWriteStringBuf, "%d-%02d-%02d %02d:%02d:%02d", 
		ptagCurDataTime->tm_year+1900, 
		ptagCurDataTime->tm_mon+1, 
		ptagCurDataTime->tm_mday, 
		ptagCurDataTime->tm_hour, 
		ptagCurDataTime->tm_min, 
		ptagCurDataTime->tm_sec);

	return pWriteStringBuf;
}

const char *GetCurYearMonthWithString(char *pStringBuf)
{
	time_t	tCurDateTime;
	tm		*ptagCurDataTime;
	char	szTimeStringBuf[MAXSIZE_TIMESTRING] = {0};

	char *pWriteStringBuf = pStringBuf;
	if(NULL == pWriteStringBuf)
	{
		pWriteStringBuf = szTimeStringBuf;
	}

	time(&tCurDateTime);
	ptagCurDataTime = localtime(&tCurDateTime);

	sprintf(pWriteStringBuf, "%d%02d", 
		ptagCurDataTime->tm_year+1900, 
		ptagCurDataTime->tm_mon+1);

	return pWriteStringBuf;
}


RingBuffer::RingBuffer( )
{
	Clear( );
}

RingBuffer::~RingBuffer( )
{

}

void RingBuffer::Clear( )
{
	m_startptr	=	NULL;
	m_leftptr	=	NULL;
	m_rightptr	=	NULL;
	m_tmpptr	=	NULL;
	m_nextleft	=	NULL;
	m_uBufLen	=	0;
	m_uMaxPack	=	0;
}

int RingBuffer::Init( char* StartPtr, unsigned uBufLen,unsigned uMaxPack )
{
	if( uBufLen <= ( 2 * uMaxPack + RB_SPACE ) )
		return INVALID_VALUE;

	if( StartPtr == NULL )
		return INVALID_VALUE;
	
	Clear( );

	memset( StartPtr, 0, uBufLen );

	m_startptr		=	StartPtr;

	m_uMaxPack		=	uMaxPack;
	m_uBufLen		=	uBufLen - ( m_uMaxPack + RB_SPACE );	//隔开8字节

	m_leftptr		=	m_startptr;
	m_rightptr		=	m_startptr;
	m_tmpptr		=	m_startptr + ( m_uBufLen + RB_SPACE );
	m_nextleft		=	m_startptr;

	return SUCCESS_VALUE;
}

int RingBuffer::Reinit( )
{
	if( m_startptr == NULL )
		return INVALID_VALUE;
	
	m_leftptr		=	m_startptr;
	m_rightptr		=	m_startptr;
	m_nextleft		=	m_startptr;

	return SUCCESS_VALUE;
}
/*
 *	写入数据,返回右指针
 */
char* RingBuffer::PutData( char* leftptr, char* rightptr, char* pData, unsigned datasize )
{
	//右边空间
	unsigned uRightMargin = m_uBufLen - ( rightptr - m_startptr );

	if( rightptr >= leftptr &&
		datasize >= uRightMargin )
	{
		/*
		 *	没有环绕,但是右边空间不够,即将环绕
		 */
		memcpy( rightptr, pData, uRightMargin );
		rightptr = m_startptr;
		memcpy( rightptr, pData + uRightMargin, datasize - uRightMargin );
		rightptr += datasize - uRightMargin;
		
		if( rightptr > m_startptr + m_uBufLen )
			return NULL;

		return rightptr;
	}

	//环绕了,或者没有环绕，但是右边空间够用
	memcpy( rightptr, pData, datasize );
	rightptr += datasize;

	if( rightptr > m_startptr + m_uBufLen )
		return NULL;

	return rightptr;
}

unsigned RingBuffer::GetDataLen( char* leftptr, char* rightptr )
{
	//左指针右边空间
	unsigned uRightMargin = m_uBufLen - ( leftptr - m_startptr );

	if( rightptr < leftptr &&
		sizeof(unsigned) > uRightMargin )
	{
		/*
		 *	环绕了，但是数据长度不够读取
		 */
		unsigned uDataLen = 0;
		char* ptr = (char*)&uDataLen;
		memcpy( ptr, leftptr, uRightMargin );
		memcpy( ptr + uRightMargin, m_startptr, sizeof(unsigned) - uRightMargin );
		return uDataLen;
	}
	
	return *((unsigned*)leftptr);
}

int RingBuffer::SndPack( const void* pData, unsigned datasize )
{
	if( pData == NULL || datasize == 0 )
		return INVALID_VALUE;
	
	if( m_startptr == NULL )
		return INVALID_VALUE;
	
	if( datasize > m_uMaxPack )
		return INVALID_VALUE;

	char* leftptr	= m_leftptr;
	char* rightptr	= m_rightptr;

	/*
	 *	判断是否环绕
	 */
	////////////////////////////////////////////////////////
	unsigned uContentSize	=	0;
	unsigned uEmptySize		=	0;

	if( rightptr >= leftptr )	//没有环绕
		uContentSize = ( rightptr - leftptr );
	else
		uContentSize =  m_uBufLen - ( leftptr - rightptr );

	if( uContentSize > m_uBufLen - 1 )
		return INVALID_VALUE;
	
	uEmptySize = m_uBufLen - uContentSize - 1;
	////////////////////////////////////////////////////////

	/*
	 *	没空间了
	 */
	if( datasize + sizeof(unsigned) > uEmptySize )
		return INVALID_VALUE;

	//写入长度
	rightptr = PutData( leftptr, rightptr, (char*)&datasize, sizeof(unsigned) );

	if( rightptr == NULL )
		return INVALID_VALUE;

	//写入数据
	rightptr = PutData( leftptr, rightptr, (char*)pData, datasize );

	if( rightptr == NULL )
		return INVALID_VALUE;

	//修改指针
	m_rightptr = rightptr;

	return SUCCESS_VALUE;
}
//===================================================================================
// 这个函数要特别注意，datasize的大小为pData1+pData2指针里的数据内容大小
//
int RingBuffer::SndPack( const void *pData1, const void *pData2, unsigned datasize1, unsigned datasize2 )
{
	if( pData1 == NULL || pData2 || 0 == datasize1 || 0 == datasize2 )
		return INVALID_VALUE;

	unsigned	uiTotalSize	= datasize1 + datasize2;
	
	if( m_startptr == NULL )
		return INVALID_VALUE;
	
	if( uiTotalSize > m_uMaxPack )
		return INVALID_VALUE;

	char* leftptr	= m_leftptr;
	char* rightptr	= m_rightptr;

	/*
	 *	判断是否环绕
	 */
	////////////////////////////////////////////////////////
	unsigned uContentSize	=	0;
	unsigned uEmptySize		=	0;

	if( rightptr >= leftptr )	//没有环绕
		uContentSize = ( rightptr - leftptr );
	else
		uContentSize =  m_uBufLen - ( leftptr - rightptr );

	if( uContentSize > m_uBufLen - 1 )
		return INVALID_VALUE;
	
	uEmptySize = m_uBufLen - uContentSize - 1;
	////////////////////////////////////////////////////////

	/*
	 *	没空间了
	 */
	if( uiTotalSize + sizeof(unsigned) > uEmptySize )
		return INVALID_VALUE;

	//写入长度
	rightptr = PutData( leftptr, rightptr, (char*)&uiTotalSize, sizeof(unsigned) );

	if( rightptr == NULL )
		return INVALID_VALUE;

	//写入数据1
	rightptr = PutData( leftptr, rightptr, (char*)pData1, datasize1 );

	if( rightptr == NULL )
		return INVALID_VALUE;

	//写入数据2
	rightptr = PutData( leftptr, rightptr, (char*)pData2, datasize2 );

	if( rightptr == NULL )
		return INVALID_VALUE;

	//修改指针
	m_rightptr = rightptr;

	return SUCCESS_VALUE;
}

const void* RingBuffer::RcvPack( unsigned& datasize )
{
	datasize = 0;

	if( m_startptr == NULL )
		return NULL;
	
	//释放上一次内容
	m_leftptr		= m_nextleft;
	char* leftptr	= m_leftptr;
	char* rightptr	= m_rightptr;

	/*
	 *	判断是否环绕
	 */
	////////////////////////////////////////////////////////
	unsigned uContentSize	=	0;

	if( rightptr >= leftptr )	//没有环绕
		uContentSize = ( rightptr - leftptr );
	else
		uContentSize =  m_uBufLen - ( leftptr - rightptr );

	if( uContentSize > m_uBufLen - 1 )
		return NULL;
	////////////////////////////////////////////////////////

	if( uContentSize <= sizeof(unsigned) )
		return NULL;

	datasize = GetDataLen( leftptr, rightptr );

	if( uContentSize < datasize ||
		datasize > m_uMaxPack )
	{
		/*
		 *	不应该产生的情况
		 */
		datasize = 0;
		return NULL;
	}

	/*
	 *	一起拷贝,总长度
	 */
	unsigned uReadLen = datasize + sizeof(unsigned);

	//左指针右边空间
	unsigned uRightMargin = m_uBufLen - ( leftptr - m_startptr );

	if( rightptr < leftptr &&
		uReadLen >= uRightMargin )
	{
		/*
		 *	环绕了，但是数据长度不够读取
		 */
		memcpy( m_tmpptr, leftptr, uRightMargin );

		memcpy( m_tmpptr + uRightMargin, m_startptr, uReadLen - uRightMargin );

		if( uReadLen - uRightMargin > m_uBufLen )
		{
			datasize = 0;
			return NULL;
		}
		//修改指针
		m_nextleft = m_startptr + ( uReadLen - uRightMargin );

		return ( m_tmpptr + sizeof(unsigned) );
	}
	else
	{
		//修改指针
		if( m_nextleft + uReadLen > m_startptr + m_uBufLen )
		{
			datasize = 0;
			return NULL;
		}

		m_nextleft += uReadLen; 
		return ( leftptr + sizeof(unsigned) );
	}
}


//===========================================================

//===========================================================

int timer::stop()
{
	Thread::stop();
	return 0;
}

void* timer::action()
{
	while( m_Run )
	{
		UNIX_TIME_STAMP = (unsigned long)time( NULL );
		Sleep(100);
	}
	return NULL;
}

int timer::start()
{
	m_Run = true;
	Thread::start();
	return 0;
}

timer::timer():Thread(1)
{
	m_Run = false;
}

timer::~timer()
{
	m_Run = false;
}

bool timer::IsRun()
{
	return m_Run;
}

bool string_md5( IN char* inputString, OUT char* md5String )
{
	if ( inputString || !md5String)
		return false;
	md5_state_t state;
	md5_byte_t digest[16];
	//int di;

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)inputString, strlen(inputString));
	md5_finish(&state, digest);
	md5String[0] = '\0';

	for ( int i = 0; i < 16; i++)
	{
		char temp[5] = {0};
		sprintf(temp, "%02X ", digest[i]);
		strcat( md5String, temp );
	}
		return true;
}

bool file_md5( IN char* fileName, OUT char* md5String )
{
	if ( !fileName || !md5String)
		return false;

	md5String[0] = '\0';

	md5_state_t state;
	unsigned char ReadBuffer[65536];
	size_t ReadBytes = 0;
	md5_byte_t digest[16];
	md5_init(&state);
	FILE *fp = fopen(fileName, "rb");
	if (!fp) 
		return false;

	while (true)
	{
		ReadBytes = fread(ReadBuffer, 1, 65536, fp);
		if (ReadBytes > 0)
			md5_append(&state, (const md5_byte_t *)ReadBuffer, ReadBytes);

		if (feof(fp))
		{
			md5_finish(&state, digest);
			break;
		}
	}
	fclose(fp);
	fp = NULL;
	for ( int i = 0; i < 16; i++)
	{
		char temp[5] = {0};
		sprintf(temp, "%02X ", digest[i]);
		strcat( md5String, temp );
	}

	return true;
}
