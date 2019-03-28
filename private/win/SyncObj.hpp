/* SyncObj.cpp - for windows */
#ifndef _MFC_VER
#include "SyncObj.h"

/////////////////////////////////////////////////////////////////////////////
// Basic synchronization object
CSyncObject::~CSyncObject()
{
}

///////////////////////////////////////////////////////////////////////////////////////////
// CCriticalSection
CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(&m_crisec);
}

BOOL CCriticalSection::Lock(UINT nTimeOut /* = INFINITE */)
{
	UNUSED_EX(nTimeOut);
	EnterCriticalSection(&m_crisec);
	return TRUE;
}

BOOL CCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_crisec);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////
// CSemaphore
CSemaphore::CSemaphore(int iInitCount)
{
	m_iInitCount = iInitCount;
	m_hSem = CreateSemaphore(NULL, iInitCount, iInitCount, NULL);
}

BOOL CSemaphore::Lock(UINT nTimeOut)
{
	ASSERT(m_hSem!=INVALID_HANDLE_VALUE);
	int rc = WaitForSingleObject(m_hSem, nTimeOut);
	return (rc==WAIT_OBJECT_0);
}

BOOL CSemaphore::Unlock()
{
	ASSERT(m_hSem!=INVALID_HANDLE_VALUE);
	return ReleaseSemaphore(m_hSem, 1, NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////
// CSingleLock

#endif /* _MFC_VER */
