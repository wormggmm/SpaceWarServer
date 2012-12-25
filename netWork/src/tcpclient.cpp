
#include "errorcode.h"
#include "tcpclient.h"
#include "file_log.h"

tcpclient::tcpclient( )
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
tcpclient::~tcpclient()
{
	if ( m_Conn )
	{
		OutPutDebugStringToCMD( "Error ~tcpclient unRelease Connect!\n" );
	}
}

int tcpclient::StartClient(char* serverIP,   PORT listenPort, int maxCon /*= MIN_CONN */ )
{
	if ( !serverIP )
		RETURN( errcode::en_tcpclient_StartClient_RetVal_ParamError );

	if ( listenPort <= MIN_PORT || listenPort >= MAX_PORT )
		RETURN( errcode::en_tcpclient_StartClient_RetVal_ParamError );

	if ( maxCon < MIN_CONN || maxCon > MAX_CONN )
		RETURN( errcode::en_tcpclient_StartClient_RetVal_ParamError );


	if ( m_Conn )
		RETURN( errcode::en_tcpclient_StartClient_RetVal_ConnUnRelease );


	m_MaxConn = maxCon;
	m_ServerPort = listenPort;
	strncpy( m_ServerIP, serverIP, IP_LEN );
	Init();

	m_SendThread.start( this );
	m_RecvThread.start( this );

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::StopClient(  )
{
	m_SendThread.stop();
	m_RecvThread.stop();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::ActionClient()
{

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::Init( ) 
{
	if ( m_Conn )
		RETURN( errcode::en_tcpclient_Init_RetVal_ConnUnRelease );


	m_Conn = new TCPConnection[m_MaxConn];

	if ( !m_Conn )
		RETURN( errcode::en_tcpclient_Init_RetVal_NewMallocFalse );


	m_FreeConnIdx.clear();
	for ( int i = 0; i < m_MaxConn; i++ )
	{
		m_FreeConnIdx.push_back( i );
	}
	m_ConnectedConnIdx.clear();


#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return -1;
	}
#endif


	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::Release()
{
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::PushDataToSendBuff( int connectIdx, void* pBuf, int dataSize )
{
	int connedIdx = getConnectedIdx();
	if ( connedIdx == INVALID_VALUE )
		RETURN( errcode::en_tcpclient_PushDataToSendBuff_RetVal_NoneConn );

	char buff[MAX_SENDPACK] = {0};
	TCPNetPack* pNetPack = (TCPNetPack*)buff;
	pNetPack->ConnectionID = connedIdx;
	memcpy( (void*)pNetPack->buff,  pBuf, dataSize );
	return m_SendBuff.SndPack( (void*)buff, dataSize+4 );
}

const void* tcpclient::GetDataFromRecvBuff( unsigned int& dataSize )
{
	return m_RecvBuff.RcvPack( dataSize );
}

int tcpclient::PushDataToRecvBuff( void* pBuf, int dataSize )
{
	return m_RecvBuff.SndPack( pBuf, dataSize );
}

const void* tcpclient::GetDataFromSendBuff( unsigned int& dataSize )
{
	return m_SendBuff.RcvPack( dataSize );
}

int tcpclient::popFreeConnectIdx()
{
	if ( m_FreeConnIdx.size() == 0 )
		RETURN( errcode::en_tcpclient_getFreeConnectIdx_RetVal_NoneFreeConnIdx );

	int freeIdx = INVALID_VALUE;

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

	RETURN(freeIdx);
}

int tcpclient::pushFreeConnectIdx( int freeIdx )
{
	if( freeIdx < 0 || freeIdx >= m_MaxConn )
		RETURN( errcode::en_tcpclient_addFreeConnectIdx_RetVal_InvalidFreeConnIdx );

	m_FreeConnIdx.push_back( freeIdx );

	CONNECTINDEX::iterator result = find( m_ConnectedConnIdx.begin(), m_ConnectedConnIdx.end(), freeIdx );
	if  ( result != m_ConnectedConnIdx.end( ) ) //√ª’“µΩ
		m_ConnectedConnIdx.erase( result );

	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpclient::OpenConnect( SOCKET sock )
{
	if ( sock == SOCKET_INVALID  )
		RETURN( errcode::en_tcpclient_OpenClientConnect_RetVal_ParamError);

	int freeIdx = popFreeConnectIdx();
	if ( INVALID_VALUE == freeIdx )
		RETURN( errcode::en_tcpclient_OpenClientConnect_RetVal_NoneFreeConnIdx);

	if( m_Conn[freeIdx].m_State != enTCPConnectionState_free ) 
		RETURN( errcode::en_tcpclient_OpenClientConnect_RetVal_FreeConnUnRelease);


	m_Conn[freeIdx].m_Socket = sock;
	m_Conn[freeIdx].m_ConnectIdx = freeIdx;
	m_Conn[freeIdx].m_State = enTCPConnectionState_Connected;
	m_Conn[freeIdx].m_CloseCallback = &ItcpThreadOwner::CloseConnectCallBack;
	m_Conn[freeIdx].m_lastPingTimeStamp = UNIX_TIME_STAMP;

	RETURN( errcode::enFunctionReturnValue_Sucess);
}


int tcpclient::CloseConnect( int connectIdx )
{
	if ( connectIdx < 0 && connectIdx >=  m_MaxConn )
		RETURN( errcode::en_tcpclient_CloseClientInfo_RetVal_ParamError);

	if ( enTCPConnectionState_Closing != m_Conn[connectIdx].m_State )
		RETURN( errcode::en_tcpclient_CloseClientInfo_RetVal_ConnHasBeenReleased );


	m_Conn[connectIdx].Release();
	m_Conn[connectIdx].m_State = enTCPConnectionState_Closing;
	m_Conn[connectIdx].m_CloseTimeStamp = UNIX_TIME_STAMP;
	m_Conn[connectIdx].m_CloseCallback = NULL;

	pushFreeConnectIdx( connectIdx );

	RETURN( errcode::enFunctionReturnValue_Sucess);
}

int tcpclient::ConnectToServer()
{
	SOCKET		sock;
	sockaddr_in	addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons( m_ServerPort );
	addr.sin_addr.s_addr = inet_addr( m_ServerIP );
	if ( ( sock = socket ( AF_INET, SOCK_STREAM, 0 )) < 0 ){
#ifdef _DEBUG
		FILELOGS::WriteLog( "socket failed,sock = %d\n", sock );
#endif
		return -1;
	}
	if ( connect( sock, (struct sockaddr*)&addr, sizeof(addr) ) < 0 ){

#ifdef _DEBUG
		OutPutLastErrorMsgToCMD();
		FILELOGS::WriteLog( "connect to sock = %d failed\n", sock );
#endif
#ifdef WIN32
		::closesocket( sock );
#else
		::close( sock );
#endif
		return -1;
	}
	// make it nonblock
#ifdef WIN32
#else
	int flags;
	if ( (flags = fcntl(sock, F_GETFL, 0)) < 0
		|| fcntl(sock, F_SETFL, flags | O_NONBLOCK | O_ASYNC ) < 0 ) {
			::close(sock);
			return -1;
	}
#endif

	OpenConnect( sock );

	RETURN( errcode::enFunctionReturnValue_Sucess);
}

int tcpclient::DisConnectFromServer( int ConntedIdx )
{
	CloseConnect( ConntedIdx );

	RETURN( errcode::enFunctionReturnValue_Sucess);
}

int tcpclient::CloseConnectCallBack( int connectIdx )
{
	OutPutStringToCMD("close connect!");
	RETURN( errcode::enFunctionReturnValue_Sucess);
}
