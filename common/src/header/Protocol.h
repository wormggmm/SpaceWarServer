#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


enum enProtocol_s2c
{
	s2c_begin = -1,
	s2c_sendMapInfo,
	s2c_end,
};


enum enProtocol_c2s
{
	c2s_begin = -1,
	c2s_requestMapInfo,
	c2s_end,
};


#endif