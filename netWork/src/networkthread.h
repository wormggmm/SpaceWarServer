#ifndef _NETWORK_THREAD_H_
#define _NETWORK_THREAD_H_

#include "def.h"
#include "utilities.h"
#include "tools.h"
#include "netcommon.h"

#define INVALID_CONNECT_INDEX -1

#define MAX_SENDBUFF 1024 * 1024
#define MAX_RECVBUFF 1024 * 1024

#define MAX_SENDPACK 2048
#define MAX_RECVPACK 2048


#define MAX_CONN_QUEUE	1000
#define CLOSE_SOCKET_REPEAT_TIME 30
//typedef int (ItcpThreadOwner::*ClOSECALLBACK)( int connectIdx );

class tcpserver;
class tcpclient;
interface ItcpThreadOwner;

enum enTCPConnectionState
{
	enTCPConnectionState_idle,			//两种情况会被用到，1、getConn的时候用到的Idx不合法；2、连接已经被断开，但是Send缓冲区还有数据时
	enTCPConnectionState_Closing,		//链接关闭，但是缓冲去可能还有数据，所以不能被复用
	enTCPConnectionState_free,			//可被使用的空闲
	enTCPConnectionState_WillClose,		//将要关闭
	enTCPConnectionState_Connected,		//已经连接上
};

struct TCPConnection
{
	int							m_ConnectIdx;
	enTCPConnectionState		m_State;
	SOCKET						m_Socket;
	//ClOSECALLBACK				m_CloseCallback;				//断开连接时，调用的CallBack函数
	int (ItcpThreadOwner::*m_CloseCallback)( int connectIdx );
	unsigned int				m_CloseTimeStamp;				//断开连接时的时间
	unsigned int				m_lastPingTimeStamp;		//最后一次ping协议的时间戳


	void CloseCallBack(ItcpThreadOwner *owner)
	{
		if ( m_CloseCallback )
			(owner->*m_CloseCallback)( m_ConnectIdx );
	}
	void Release()
	{
		m_ConnectIdx = INVALID_CONNECT_INDEX;
		m_State = enTCPConnectionState_free;
		m_Socket = SOCKET_INVALID;
		m_CloseCallback = NULL;
		m_CloseTimeStamp = 0;
		m_lastPingTimeStamp = 0;
	}
	TCPConnection()
	{
		Release();
	}
	~TCPConnection()
	{
		Release();
	}
};

interface ItcpThreadOwner
{
	virtual int	getMaxConn() = 0;
	virtual TCPConnection* getConn( int connIdx ) = 0;
	virtual int CloseConnect( int connectIdx ) = 0;
	virtual int OpenConnect( SOCKET sock ) = 0;

	virtual int PushDataToSendBuff(  int connectIdx, void* pBuf, int dataSize ) = 0;
	virtual const void* GetDataFromRecvBuff( unsigned int& dataSize ) = 0;

	virtual int PushDataToRecvBuff( void* pBuf, int dataSize ) = 0;
	virtual const void* GetDataFromSendBuff( unsigned int& dataSize ) = 0;

	virtual int CloseConnectCallBack(int connectIdx) = 0;
};

class tcpThreadInfo
{
private:
	ItcpThreadOwner*	m_tcpThreadOwner;
public:
	ItcpThreadOwner* getThreadOwner();
	void setThreadOwner( ItcpThreadOwner* owner );

	void closeConnect(TCPConnection *connInfo);
};
class tcpListenThread : public Thread, public tcpThreadInfo
{
private:
	bool				m_bRun;
	SOCKET			m_ListenSocket;
	PORT			m_ListenPort;
public:

	tcpListenThread( );
	~tcpListenThread();
private:
	int InitListenSocket( );
	int CloseListenSocket();
public:
	int start( ItcpThreadOwner* owner, PORT listenPort );
	void* action();
	int stop();
};

class tcpSendThread : public Thread, public tcpThreadInfo
{
private:
	bool	m_bRun;

public:
	tcpSendThread(  );
	~tcpSendThread();
public:
	int start( ItcpThreadOwner* owner );
	void* action();
	int stop();
};

class tcpRecvThread : public Thread, public tcpThreadInfo
{
private:
	bool	m_bRun;
public:
	tcpRecvThread(  );
	~tcpRecvThread();
public:
	int start( ItcpThreadOwner* owner );
	void* action();
	int stop();
};
#endif