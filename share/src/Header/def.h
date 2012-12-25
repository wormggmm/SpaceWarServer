/////////////////////////////////////////////////////////////////////////////
//  FileName    :   def.h
//  Creator     :   zuolizhi
//  Date        :   2006-5-30 9:54:00
//  Comment     :   Interface Declare
//	Changes		:	
/////////////////////////////////////////////////////////////////////////////
#ifndef _MINISTER_DEF_H_
#define _MINISTER_DEF_H_

#pragma warning(disable : 4786)

#include "string.h"

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#ifndef MAC
#include <mntent.h>
#include <sys/vfs.h>
#endif

#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <errno.h>

#endif

#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>

#ifndef interface
#define interface struct
#endif


#define IN
#define OUT

#define INVALID_VALUE	( -1 )
#define SUCCESS_VALUE	( 0 )
#define WAIT_SELECT		( -2 )

#ifndef NULL
#define NULL			( 0 )
#endif
#define BYTE_WIDE				( 8 )
#define BIT						( 1 )
#define KB						( 1024 )
#define MB						( 1024 * 1024 )

/*
 *	unit define
 */
#define MAX_LOGIN_PROTOCOL 10
#define MAX_BUFLEN		( KB * 2 + 16 )
#define MAX_DBBUFLEN	( KB * 96 + 16 )
#define MAX_PACKAGE		( KB )

/*
 *	RB Buffer define
 */
#define RB_DEFHL	( 4	)
#define RB_SPACE	( 8 )
#define RB_DEFIBL	( KB * 16 + RB_SPACE )
#define RB_MAXPACK	( KB * 4  )


#define BYTE unsigned char
#define WORD unsigned short	

#define UINT unsigned int

#endif
