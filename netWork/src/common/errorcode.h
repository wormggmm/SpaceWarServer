#ifndef _ERRORCODE_H_
#define _ERRORCODE_H_

namespace errcode
{
	// begin
	enum en_FunctionReturnValue_Begin
	{
		enFunctionReturnValue_Begin = -99999999,
	};
		//class tcpserver
	enum en_tcpserver_RetVal
	{
		en_tcpserver_RetVal_Begin = enFunctionReturnValue_Begin + 1,

		en_tcpserver_StartServer_RetVal_ParamError,
		en_tcpserver_StartServer_RetVal_ConnUnRelease,

		en_tcpserver_Init_RetVal_NewMallocFalse,
		en_tcpserver_Init_RetVal_ConnUnRelease,

		en_tcpserver_InitListenSocket_RetVal_UnFindDll,
		en_tcpserver_InitListenSocket_RetVal_SocketCreatError,
		en_tcpserver_InitListenSocket_RetVal_BindError,
		en_tcpserver_InitListenSocket_RetVal_ListenError,

		en_tcpserver_getFreeConnectIdx_RetVal_NoneFreeConnIdx,

		en_tcpserver_OpenClientConnect_RetVal_NoneFreeConnIdx,
		en_tcpserver_OpenClientConnect_RetVal_FreeConnUnRelease,
		en_tcpserver_OpenClientConnect_RetVal_ParamError,

		en_tcpserver_addFreeConnectIdx_RetVal_InvalidFreeConnIdx,

		en_tcpserver_CloseClientInfo_RetVal_ParamError,
		en_tcpserver_CloseClientInfo_RetVal_ConnHasBeenReleased,


		en_tcpserver_RetVal_End,
	};

	enum en_tcpclient_RetVal
	{
		en_tcpclient_RetVal_Begin = en_tcpserver_RetVal_End + 1,

		en_tcpclient_StartClient_RetVal_ParamError,
		en_tcpclient_StartClient_RetVal_ConnUnRelease,
		en_tcpclient_Init_RetVal_ConnUnRelease,
		en_tcpclient_Init_RetVal_NewMallocFalse,
		en_tcpclient_PushDataToSendBuff_RetVal_NoneConn,
		en_tcpclient_getFreeConnectIdx_RetVal_NoneFreeConnIdx,
		en_tcpclient_addFreeConnectIdx_RetVal_InvalidFreeConnIdx,
		en_tcpclient_OpenClientConnect_RetVal_ParamError,
		en_tcpclient_OpenClientConnect_RetVal_NoneFreeConnIdx,
		en_tcpclient_OpenClientConnect_RetVal_FreeConnUnRelease,
		en_tcpclient_CloseClientInfo_RetVal_ParamError,
		en_tcpclient_CloseClientInfo_RetVal_ConnHasBeenReleased,

		en_tcpclient_DisConnectFromServer_RetVal_InvalidConnIdx,

		en_tcpclient_RetVal_End,
	};

	enum tcpListenThread_RetVal
	{
				en_tcpListenThread_RetVal_Begin = en_tcpclient_RetVal_End + 1,
				en_tcpListenThread_start_RetVal_ParamError,
				en_tcpListenThread_tcpListenThread_RetVal_ParamError,
				en_tcpListenThread_InitListenSocket_RetVal_UnFindDll,
				en_tcpListenThread_InitListenSocket_RetVal_SocketCreatError,
				en_tcpListenThread_InitListenSocket_RetVal_BindError,
				en_tcpListenThread_InitListenSocket_RetVal_ListenError,
				en_tcpListenThread_RetVal_End,
	};

	enum tcpRecvThread_RetVal
	{
				en_tcpRecvThread_RetVal_Begin = en_tcpListenThread_RetVal_End + 1,
				en_tcpRecvThread_start_RetVal_ParamError,
				en_tcpRecvThread_tcpRecvThread_RetVal_ParamError,
				en_tcpRecvThread_InitListenSocket_RetVal_UnFindDll,
				en_tcpRecvThread_InitListenSocket_RetVal_SocketCreatError,
				en_tcpRecvThread_InitListenSocket_RetVal_BindError,
				en_tcpRecvThread_InitListenSocket_RetVal_ListenError,
				en_tcpRecvThread_RetVal_End,
	};
	enum tcpSendThread_RetVal
	{
		en_tcpSendThread_RetVal_Begin = en_tcpRecvThread_RetVal_End + 1,
		en_tcpSendThread_start_RetVal_ParamError,
		en_tcpSendThread_RetVal_End,
	};


	// end
	enum en_FunctionReturnValue_End
	{
		enFunctionReturnValue_Sucess = 0,
	};

}




#endif