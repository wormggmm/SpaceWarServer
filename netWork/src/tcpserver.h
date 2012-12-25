#ifndef _TCP_SERVER_
#define _TCP_SERVER_

#ifndef _DEBUG
	#ifndef _NET_API_
		#define _NET_API_ _declspec(dllexport)
	#endif
#else
		#define  _NET_API_
#endif

#include <list>
#include "itcpserver.h"
typedef std::list<int> CONNECTINDEX;

class tcpserver : public ITCPServer
{
public:
	tcpserver();
	~tcpserver();
public:
	int StartServer( PORT listenPort, int maxCon = MIN_CONN );
	int StopServer( );
	int ActionServer();

	int OpenConnect( SOCKET sock );
	int CloseConnect( int connectIdx );


	int PushDataToSendBuff( int connectIdx,  void* pBuf, int dataSize );
	const void* GetDataFromRecvBuff( unsigned int& dataSize );

	int PushDataToRecvBuff( void* pBuf, int dataSize );
	const void* GetDataFromSendBuff( unsigned int& dataSize );

	int CloseConnectCallBack(int connectIdx);
public:

	int	getMaxConn()
	{
		return m_MaxConn;
	}
	TCPConnection* getConn( int connIdx )
	{
		if ( connIdx < 0 || connIdx >= m_MaxConn )
			return NULL;
		return &m_Conn[connIdx];
	}

	CONNECTINDEX* getConnectedIdxList()
	{
		return &m_ConnectedConnIdx;
	}
private:
	int	popFreeConnectIdx();
	int	pushFreeConnectIdx( int freeIdx );
private:
	int Init( );
	int Release( );
	int ReleaseConnectInfo( int connectIdx );
private:


	tcpListenThread m_ListenThread;
	tcpSendThread	m_SendThread;
	tcpRecvThread	m_RecvThread;

	char			m_SendSpace[MAX_SENDBUFF];
	RingBuffer		m_SendBuff;
	char			m_RecvSpace[MAX_RECVBUFF];
	RingBuffer		m_RecvBuff;

	TCPConnection*	m_Conn;
	int				m_MaxConn;
	CONNECTINDEX	m_FreeConnIdx;
	CONNECTINDEX	m_ConnectedConnIdx;
};


#endif