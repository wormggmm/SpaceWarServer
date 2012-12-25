/***********************************************************************
 * Utilities files used to encapsulate the Thread, Mutex...
 * Created by Xu Jin jin.j.xu@intel.com
 ***********************************************************************/


#ifndef _I_UTILITIES_H_
#define _I_UTILITIES_H_

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif
class Thread
{
private:
	enum enum_status
	{
		new_created,
		running,
		stopped
	};
	enum_status status;
#ifdef WIN32
	HANDLE          thread_id;
#else
	pthread_t       thread_id;
	pthread_attr_t  th_attr;
#endif
public:
	Thread( int detached );
	virtual ~Thread();
	int start();
	int stop(int nKill = 1);
	virtual void* action() = 0;
};



class Mutex
{
private:
#ifdef WIN32
	HANDLE          mutex;
#else
	pthread_mutex_t mutex;
#endif
	int count;
public:
#ifdef WIN32
	Mutex( int mutex_type );
#else
	Mutex( int mutex_type );
#endif
	~Mutex();
	bool trylock();
	int lock();
	int unlock();
};

#endif
