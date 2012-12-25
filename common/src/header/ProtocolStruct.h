#ifndef _PROTOCOL_STRUCT_H_
#define _PROTOCOL_STRUCT_H_
#include "def.h"
#pragma	pack(push, 1)

//共用协议头
struct PROTOCOL_HEADER
{
	WORD	Protocol;
	WORD	Length;
};
//压缩协议头
struct COMPRESSED_PROTOCOL_HEADER : public PROTOCOL_HEADER
{
	WORD	Length;
};





#endif