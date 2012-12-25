
#include "random.h"
//---------------------------------------------------------------------------
#define IM 139968
#define IA 3877
#define IC 29573
//---------------------------------------------------------------------------
static UINT nRandomSeed = 42;
//---------------------------------------------------------------------------
// ����:	RandomnSeed
// ����:	�������������
// ����:	nRandomSeed	:	���������
// ����:	void
//---------------------------------------------------------------------------
void g_RandomSeed(UINT nSeed)
{
	nRandomSeed = nSeed;
}
//---------------------------------------------------------------------------
// ����:	Random
// ����:	����һ��С��nMax���������
// ����:	nMax	:	���ֵ
// ����:	һ��С��nMax�������
//---------------------------------------------------------------------------
UINT g_Random(UINT nMax)
{
	if (nMax)
	{
#ifdef _USENEWRANDOMFUNC
		//--> Rocker 2004/08/23
		unsigned int f = nRandomSeed * 0x08088405 + 1;
		nRandomSeed = f;
		#ifndef _WIN32
		long long t = (long long)f * (long long)nMax;
		#else
		_int64 t = (_int64)f * (_int64)nMax;
		#endif
		t = t >> 32;
		return (unsigned int)t;
		//<-- End
#else
		// Modify by cooler
		// liuyujun@263.net 2004/3/25 -->
		//nRandomSeed = nRandomSeed * IA + IC;
		//return nRandomSeed % nMax;
		// Standard C rand function, max rand number is 32767
		nRandomSeed = nRandomSeed * 214013L + 2531011L;
		return ((nRandomSeed >> 16) & 0x7fff) % nMax;
		// <-- End cooler modify.
#endif		
	}
	else
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
// ����:	GetRandomSeed
// ����:	ȡ�õ�ʱ��α�������
// ����:	���ص�ǰ��α�������
//---------------------------------------------------------------------------
UINT g_GetRandomSeed()
{
	return nRandomSeed;
}
