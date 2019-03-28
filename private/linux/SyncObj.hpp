/*SyncObj.hpp*/

#include <sys/time.h>
#include "SyncObj.h"

/////////////////////////////////////////////////////////////////////////////
// Basic synchronization object
CSyncObject::~CSyncObject()
{
}


/****************************CCriticalSection**************************************/
CCriticalSection::CCriticalSection()
{
	pthread_mutexattr_t  mutexAttr;
	pthread_mutexattr_init(&mutexAttr);

	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutexattr_setprotocol(&mutexAttr, PTHREAD_PRIO_INHERIT);

	pthread_mutex_init(&m_mutex, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
}

CCriticalSection::~CCriticalSection()
{
	pthread_mutex_destroy(&m_mutex);
}

BOOL CCriticalSection::Lock(UINT nTimeOut /* = INFINITE */)
{
	if (nTimeOut == INFINITE)
		return (pthread_mutex_lock(&m_mutex) == 0);

	timeval endTime;
	timespec endTs;

	gettimeofday(&endTime, NULL);

	int waitSec = nTimeOut/1000;
	int waitMs = nTimeOut%1000;

	endTime.tv_sec += waitSec;
	endTime.tv_usec += waitMs*1000;
	if (endTime.tv_usec >= 1000000)
	{
		endTime.tv_sec++;
		endTime.tv_usec -= 1000000;
	}

	endTs.tv_sec = endTime.tv_sec;
	endTs.tv_nsec = endTime.tv_usec*1000;

    return (pthread_mutex_timedlock(&m_mutex, &endTs) == 0);
}

BOOL CCriticalSection::Unlock()
{
	return pthread_mutex_lock(&m_mutex) == 0;
}
