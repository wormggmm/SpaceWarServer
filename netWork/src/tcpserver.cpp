
#include "errorcode.h"
#include "tcpserver.h"
#include "file_log.h"


tcpserver::tcpserver( )
{
	m_MaxConn = 0;
	m_Conn = NULL;
	m_FreeConnIdx.clear();
	m_ConnectedConnIdx.clear();
	memset( (void*)m_SendSpace, 0, MAX_SENDBUFF );
	memset( (void*)m_RecvSpace, 0, MAX_RECVBUFF );
	m_SendBuff.Init( m_SendSpace, MAX_SENDBUFF, MAX_SENDPACK );
	m_RecvBuff.Init( m_RecvSpace, MAX_RECVBUFF, MAX_RECVPACK );
}
tcpserver::~tcpserver()
{
	if ( m_Conn )
	{
		OutPutDebugStringToCMD( "Error ~tcpserver unRelease Connect!\n" );
	}
}

int tcpserver::StartServer(  PORT listenPort, int maxCon /*= MIN_CONN */ )
{
	if ( listenPort <= MIN_PORT || listenPort >= MAX_PORT )
		RETURN( errcode::en_tcpserver_StartServer_RetVal_ParamError );

	if ( maxCon < MIN_CONN || maxCon > MAX_CONN )
		RETURN( errcode::en_tcpserver_StartServer_RetVal_ParamError );


	if ( m_Conn )
		RETURN( errcode::en_tcpserver_StartServer_RetVal_ConnUnRelease );


	m_MaxConn = maxCon;
	Init();

	m_ListenThread.start( this, listenPort );
	m_SendThread.start( this );
	m_RecvThread.start( this );

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpserver::StopServer(  )
{
	m_ListenThread.stop();
	m_SendThread.stop();
	m_RecvThread.stop();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpserver::ActionServer()
{

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpserver::Init( ) 
{
	if ( m_Conn )
		RETURN( errcode::en_tcpserver_Init_RetVal_ConnUnRelease );



	m_Conn = new TCPConnection[m_MaxConn];
	
	if ( !m_Conn )
		RETURN( errcode::en_tcpserver_Init_RetVal_NewMallocFalse );


	m_FreeConnIdx.clear();
	for ( int i = 0; i < m_MaxConn; i++ )
	{
		m_FreeConnIdx.push_back( i );
	}
	m_ConnectedConnIdx.clear();

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpserver::Release()
{
	RETURN( errcode::enFunctionReturnValue_Sucess );
}


int tcpserver::PushDataToSendBuff(int connectIdx, void* pBuf, int dataSize )
{
	if ( connectIdx == INVALID_VALUE )
		RETURN( errcode::en_tcpclient_PushDataToSendBuff_RetVal_NoneConn );

	char buff[MAX_SENDPACK] = {0};
	TCPNetPack* pNetPack = (TCPNetPack*)buff;
	pNetPack->ConnectionID = connectIdx;
	memcpy( (void*)pNetPack->buff,  pBuf, dataSize );
	return m_SendBuff.SndPack( (void*)buff, dataSize + 4 );
	//return m_SendBuff.SndPack( pBuf, dataSize );
}

const void* tcpserver::GetDataFromRecvBuff( unsigned int& dataSize )
{
	return m_RecvBuff.RcvPack( dataSize );
}

int tcpserver::PushDataToRecvBuff( void* pBuf, int dataSize )
{
	return m_RecvBuff.SndPack( pBuf, dataSize );
}

const void* tcpserver::GetDataFromSendBuff( unsigned int& dataSize )
{
	return m_SendBuff.RcvPack( dataSize );
}


int tcpserver::popFreeConnectIdx()
{
	int freeIdx = INVALID_VALUE;
	if ( m_FreeConnIdx.size() == 0 )
		return freeIdx;


	int i = 0;
	int freeIdxCount = (int)m_FreeConnIdx.size();
	for ( i = 0; i < freeIdxCount; i++ )
	{
		freeIdx =  m_FreeConnIdx.front();
		m_FreeConnIdx.pop_front();
		if ( m_Conn[freeIdx].m_State == enTCPConnectionState_free )
		{
			break;
		}
		else if ( m_Conn[freeIdx].m_State == enTCPConnectionState_Closing && 
			UNIX_TIME_STAMP - m_Conn[freeIdx].m_CloseTimeStamp >  CLOSE_SOCKET_REPEAT_TIME )
		{
			m_Conn[freeIdx].Release();
			break;
		}
		else
		{
			m_FreeConnIdx.push_back( freeIdx );
			continue;
		}
	}

	if ( i < freeIdxCount )
		m_ConnectedConnIdx.push_back( freeIdx );
	else
		freeIdx = INVALID_VALUE;

	return freeIdx;
}

int tcpserver::pushFreeConnectIdx( int freeIdx )
{
	if( freeIdx < 0 || freeIdx >= m_MaxConn )
		RETURN( errcode::en_tcpserver_addFreeConnectIdx_RetVal_InvalidFreeConnIdx );

	m_FreeConnIdx.push_back( freeIdx );

	CONNECTINDEX::iterator result = find( m_ConnectedConnIdx.begin(), m_ConnectedConnIdx.end(), freeIdx );
	if  ( result != m_ConnectedConnIdx.end( ) ) //√ª’“µΩ
		m_ConnectedConnIdx.erase( result );

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpserver::OpenConnect( SOCKET sock )
{
	if ( sock == SOCKET_INVALID  )
		RETURN( errcode::en_tcpserver_OpenClientConnect_RetVal_ParamError);
		
	int freeIdx = popFreeConnectIdx();
	if ( INVALID_VALUE == freeIdx )
		RETURN( errcode::en_tcpserver_OpenClientConnect_RetVal_NoneFreeConnIdx);

	if( m_Conn[freeIdx].m_State != enTCPConnectionState_free ) 
		RETURN( errcode::en_tcpserver_OpenClientConnect_RetVal_FreeConnUnRelease);


	m_Conn[freeIdx].m_Socket = sock;
	m_Conn[freeIdx].m_ConnectIdx = freeIdx;
	m_Conn[freeIdx].m_State = enTCPConnectionState_Connected;
	m_Conn[freeIdx].m_CloseCallback = &ItcpThreadOwner::CloseConnectCallBack;
	m_Conn[freeIdx].m_lastPingTimeStamp = UNIX_TIME_STAMP;

	return freeIdx;
}

int tcpserver::CloseConnectCallBack(int connectIdx)
{
	OutPutStringToCMD("close client connect:%d\n", connectIdx);
	return 0;
}

int tcpserver::CloseConnect( int connectIdx )
{
	if ( connectIdx < 0 && connectIdx >=  m_MaxConn )
		RETURN( errcode::en_tcpserver_CloseClientInfo_RetVal_ParamError);

	if ( enTCPConnectionState_Closing != m_Conn[connectIdx].m_State )
		RETURN( errcode::en_tcpserver_CloseClientInfo_RetVal_ConnHasBeenReleased );

#ifdef WIN32
	::closesocket( m_Conn[connectIdx].m_Socket );
#else
	::close( 	m_Conn[connectIdx].m_Socket );
#endif

	m_Conn[connectIdx].Release();
	m_Conn[connectIdx].m_State = enTCPConnectionState_Closing;
	m_Conn[connectIdx].m_CloseTimeStamp = UNIX_TIME_STAMP;

	m_Conn[connectIdx].m_CloseCallback = NULL;

	pushFreeConnectIdx( connectIdx );

	RETURN( errcode::enFunctionReturnValue_Sucess);
}
