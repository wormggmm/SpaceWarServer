
#ifndef _NET_COMMON_H_
#define _NET_COMMON_H_
#pragma	pack(push, 1)

#include "macro.h"

#ifdef WIN32
#define MSG_NOSIGNAL	0
#else
#ifdef MAC
#define MSG_NOSIGNAL	0
#endif

#define SOCKET_ERROR -1
typedef unsigned int SOCKET;
#endif

typedef int PORT;

#define MIN_PORT 1024
#define MAX_PORT 49151

#define SOCKET_INVALID -1
#define IP_LEN 64




struct TCPNetPack
{
	int ConnectionID;
	char buff[1];
};
#endif
