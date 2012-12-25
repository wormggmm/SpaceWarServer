
#include "networkthread.h"
#include "errorcode.h"
#include "file_log.h"
#include "common.h"
#ifdef WIN32
//#include <WinSock2.h>
#endif
int tcpListenThread::stop()
{
	CloseListenSocket();
	Thread::stop();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

void* tcpListenThread::action()
{

	while(1)
	{
		sockaddr_in	client_addr;
#ifdef WIN32
		int length = sizeof(client_addr);
#else
		socklen_t	length = sizeof(client_addr);
#endif
		SOCKET	sock;
		OutPutDebugStringToCMD( "Listen Thread action!\n" );
		if ( (sock = accept( m_ListenSocket, (sockaddr*)&client_addr, &length)) == -1 )
		{
			FILELOGS::WriteLog( "TcpServerListenThread: ERROR on accept" );
			continue;
		}

#ifdef WIN32
	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode );
#endif
		int clientidx = getThreadOwner()->OpenConnect( sock );

		OutPutDebugStringToCMD("accpet client idx:%d  ip:%s\n", clientidx, inet_ntoa(client_addr.sin_addr) );
		Sleep(30);
	}
}

tcpListenThread::tcpListenThread( ) : Thread( 1 )
{

	m_bRun = false;
}

tcpListenThread::~tcpListenThread()
{
	m_bRun = false;
}

int tcpListenThread::start( ItcpThreadOwner* owner, PORT listenPort )
{
	if ( listenPort <= MIN_PORT || listenPort >= MAX_PORT )
		RETURN( errcode::en_tcpListenThread_start_RetVal_ParamError );
	
	if ( !owner )
		RETURN( errcode::en_tcpListenThread_start_RetVal_ParamError );

	m_ListenPort = listenPort;

	setThreadOwner( owner );
	InitListenSocket();

	Thread::start();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpListenThread::InitListenSocket()
{
#ifdef _WIN32
	WORD   wVersionRequested; 
	WSADATA   wsadata; 
	int   err; 
	wVersionRequested=MAKEWORD(2,2);
	err=WSAStartup(wVersionRequested,&wsadata); 
	if   (err!=0) 
	{ 
		//未找到可用的DLL,返回 
		OutPutLastErrorMsgToCMD();
		RETURN( errcode::en_tcpListenThread_InitListenSocket_RetVal_UnFindDll );
	} 
#else
#endif


	m_ListenSocket = socket( PF_INET,  SOCK_STREAM,  0 );
	if ( -1 == m_ListenSocket )
	{
		OutPutLastErrorMsgToCMD();
#ifdef WIN32
		closesocket( m_ListenSocket );
		WSACleanup();
#else
		close(m_ListenSocket);
#endif
		RETURN( errcode::en_tcpListenThread_InitListenSocket_RetVal_SocketCreatError );
	}


	sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_in.sin_port = htons( m_ListenPort );
	memset( (void*)addr_in.sin_zero, 0, sizeof( addr_in.sin_zero ) );

	int addrlen = sizeof( sockaddr );
	int nOnFlag = 1;
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&nOnFlag, sizeof(nOnFlag));

	if ( -1 == bind( m_ListenSocket, (sockaddr*)(&addr_in), addrlen ) )
	{
		OutPutLastErrorMsgToCMD();
#ifdef WIN32
		closesocket( m_ListenSocket );
		WSACleanup();
#else
		close(m_ListenSocket);
#endif
		RETURN( errcode::en_tcpListenThread_InitListenSocket_RetVal_BindError );
	}


	if(listen(m_ListenSocket, MAX_CONN_QUEUE))
	{
		OutPutLastErrorMsgToCMD();
#ifdef WIN32
		closesocket( m_ListenSocket );
		WSACleanup();
#else
		close(m_ListenSocket);
#endif
		RETURN( errcode::en_tcpListenThread_InitListenSocket_RetVal_ListenError );
	}
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

int tcpListenThread::CloseListenSocket()
{
#ifdef WIN32
	closesocket( m_ListenSocket );
#else
	close( m_ListenSocket );
#endif
	RETURN( errcode::enFunctionReturnValue_Sucess );
}
int tcpSendThread::stop()
{
	Thread::stop();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

void* tcpSendThread::action()
{

	OutPutDebugStringToCMD( "Send Thread action!\n" );
	while(1)
	{
		do 
		{
			unsigned int datasize = 0;
			const void* data = getThreadOwner()->GetDataFromSendBuff( datasize );
			if ( !data )
				break;

			TCPNetPack* netPack = (TCPNetPack*)data;

			TCPConnection* conn = getThreadOwner()->getConn( netPack->ConnectionID );

			if( !conn )
				break;

			if ( conn->m_State != enTCPConnectionState_Connected )
				break;

			int sendsize = send( conn->m_Socket,  (const char*)(netPack->buff), datasize-4, MSG_NOSIGNAL );

			if ( sendsize == SOCKET_ERROR )
			{
#ifdef WIN32
				if (GetLastError() != WSAEWOULDBLOCK)
#else
				if (errno == EWOULDBLOCK)
#endif
				{
					OutPutLastErrorMsgToCMD();
					closeConnect(conn);
				}
			}
		} while (0);
		Sleep(30);
	}
}

tcpSendThread::tcpSendThread(  ) : Thread( 1 )
{
	m_bRun = false;
}

tcpSendThread::~tcpSendThread()
{
	m_bRun = false;
}

int tcpSendThread::start( ItcpThreadOwner* owner )
{
	if ( !owner )
		RETURN( errcode::en_tcpSendThread_start_RetVal_ParamError );

	setThreadOwner( owner );
	Thread::start();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}



int tcpRecvThread::stop()
{
	Thread::stop();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

void* tcpRecvThread::action()
{

	OutPutDebugStringToCMD( "Recv Thread action!\n" );
	while(1)
	{
		for ( int i = 0; i < getThreadOwner()->getMaxConn(); i++ )
		{
			do 
			{
				TCPConnection* conInfo = getThreadOwner()->getConn( i );
				if ( !conInfo )
					break;

				if ( conInfo->m_State != enTCPConnectionState_Connected )
					break;

				char recvBuff[MAX_RECVPACK] = {0};
				TCPNetPack* pNetPack = (TCPNetPack*)recvBuff;

				pNetPack->ConnectionID = i;
				//int recvRet = recv( conInfo->m_Socket, pNetPack->buff,  MAX_RECVPACK, 0 );
				int recvRet = recv( conInfo->m_Socket, pNetPack->buff,  MAX_RECVPACK, 0 );
				if ( 0 > recvRet )
				{
#ifdef WIN32
				if (GetLastError() != WSAEWOULDBLOCK)
#else
				if (errno == EWOULDBLOCK)
#endif
					{
						OutPutLastErrorMsgToCMD();
						closeConnect(conInfo);
						break;
					}
					if(UNIX_TIME_STAMP - conInfo->m_lastPingTimeStamp > KEEP_ALIVE_TIME)
					{
						OutPutDebugStringToCMD("close socket: client long time send no pack\n");
						closeConnect(conInfo);
						break;
					}
				}
				else if ( 0 < recvRet )
				{
					//OutPutDebugStringToCMD("Recv data:%s\n", pNetPack->buff );
					conInfo->m_lastPingTimeStamp = UNIX_TIME_STAMP;
					getThreadOwner()->PushDataToRecvBuff( recvBuff,  recvRet + 4  );
					break;
				}
				else if (0 == recvRet)
				{
						OutPutDebugStringToCMD("close socket: client close socket\n");
						closeConnect(conInfo);
				}
			} while (0);
		}
		Sleep(20);
	}
}


tcpRecvThread::tcpRecvThread(  ) : Thread( 1 )
{
	m_bRun = false;
}

tcpRecvThread::~tcpRecvThread()
{
	m_bRun = false;
}

int tcpRecvThread::start( ItcpThreadOwner* owner )
{
	if ( !owner )
		RETURN( errcode::en_tcpRecvThread_start_RetVal_ParamError );

	setThreadOwner( owner );
	Thread::start();
	RETURN( errcode::enFunctionReturnValue_Sucess );
}

ItcpThreadOwner* tcpThreadInfo::getThreadOwner()
{
	return m_tcpThreadOwner;
}

void tcpThreadInfo::setThreadOwner( ItcpThreadOwner* owner )
{
	if ( !owner )
		return;
	m_tcpThreadOwner = owner;
}

void tcpThreadInfo::closeConnect( TCPConnection *connInfo )
{
	OutPutLastErrorMsgToCMD();
	connInfo->CloseCallBack(getThreadOwner());
	connInfo->m_State = enTCPConnectionState_Closing;
	getThreadOwner()->CloseConnect(connInfo->m_ConnectIdx);
}
