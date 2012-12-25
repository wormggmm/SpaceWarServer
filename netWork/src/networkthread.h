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
	enTCPConnectionState_idle,			//��������ᱻ�õ���1��getConn��ʱ���õ���Idx���Ϸ���2�������Ѿ����Ͽ�������Send��������������ʱ
	enTCPConnectionState_Closing,		//���ӹرգ����ǻ���ȥ���ܻ������ݣ����Բ��ܱ�����
	enTCPConnectionState_free,			//�ɱ�ʹ�õĿ���
	enTCPConnectionState_WillClose,		//��Ҫ�ر�
	enTCPConnectionState_Connected,		//�Ѿ�������
};

struct TCPConnection
{
	int							m_ConnectIdx;
	enTCPConnectionState		m_State;
	SOCKET						m_Socket;
	//ClOSECALLBACK				m_CloseCallback;				//�Ͽ�����ʱ�����õ�CallBack����
	int (ItcpThreadOwner::*m_CloseCallback)( int connectIdx );
	unsigned int				m_CloseTimeStamp;				//�Ͽ�����ʱ��ʱ��
	unsigned int				m_lastPingTimeStamp;		//���һ��pingЭ���ʱ���


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