#ifndef _TCP_CLIENT_
#define _TCP_CLIENT_

#ifndef _DEBUG
#ifndef _NET_API_
#define _NET_API_ _declspec(dllexport)
#endif
#else
#define  _NET_API_
#endif

#include <list>
#include "itcpclient.h"
typedef std::list<int> CONNECTINDEX;


class tcpclient : public ITCPClient
{
public:
	tcpclient();
	~tcpclient();
public:
	int StartClient( char* serverIP, PORT listenPort, int maxCon = MIN_CONN );
	int StopClient( );
	int ActionClient();

	int OpenConnect( SOCKET sock );
	int CloseConnect( int connectIdx );

	int ConnectToServer( );
	int DisConnectFromServer( int ConntedIdx  );

	int PushDataToSendBuff(int connectIdx,  void* pBuf, int dataSize );
	const void* GetDataFromRecvBuff( unsigned int& dataSize );

	int PushDataToRecvBuff( void* pBuf, int dataSize );
	const void* GetDataFromSendBuff( unsigned int& dataSize );

	int CloseConnectCallBack(int connectIdx);
public:
	SOCKET getListenSocket()
	{
		return SOCKET_INVALID;
	}

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

	int getConnectedIdx()
	{
			if ( m_ConnectedConnIdx.size() > 0 )
				return *(m_ConnectedConnIdx.begin());

			return INVALID_VALUE;
	}
private:
	int	popFreeConnectIdx();
	int	pushFreeConnectIdx( int freeIdx );
private:
	int Init( );
	int Release( );
	int ReleaseConnectInfo( int connectIdx );
private:
	char				m_ServerIP[IP_LEN];
	PORT			m_ServerPort;

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