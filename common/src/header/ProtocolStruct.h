#ifndef _PROTOCOL_STRUCT_H_
#define _PROTOCOL_STRUCT_H_
#include "def.h"
#pragma	pack(push, 1)

//����Э��ͷ
struct PROTOCOL_HEADER
{
	WORD	Protocol;
	WORD	Length;
};
//ѹ��Э��ͷ
struct COMPRESSED_PROTOCOL_HEADER : public PROTOCOL_HEADER
{
	WORD	Length;
};





#endif