/*SyncObj.h
 *
 * */

#ifndef	__SYNC_OBJ_H__
#define	__SYNC_OBJ_H__

#include <pthread.h>
// Basic synchronization object
class CSyncObject
{
// Constructor
public:
	//CSyncObject();
	virtual ~CSyncObject();

// Operations
protected:
	virtual BOOL Lock(UINT nTimeOut = INFINITE)=0;
	virtual BOOL Unlock() = 0;

	friend class CSingleLock;
};

/////////////////////////////////////////////////////////////////////////////
// CCriticalSection
class CCriticalSection : public CSyncObject
{
// Constructor
public:
	inline CCriticalSection();
	~CCriticalSection();

// Operations
protected:
	virtual BOOL Lock(UINT nTimeOut = INFINITE);
	virtual BOOL Unlock();

protected:
	pthread_mutex_t m_mutex;	/* mutual-exclusion semaphore for syncronization */
};

/////////////////////////////////////////////////////////////////////////////
// CSingleLock
class CSingleLock
{
// Constructors
public:
	inline CSingleLock(CSyncObject* pObject, BOOL bInitialLock = FALSE);
	inline ~CSingleLock();

// Operations
public:
	inline BOOL Lock(UINT nTimeOut = INFINITE);
	inline BOOL Unlock();
	inline BOOL IsLocked()const;

protected:
	CSyncObject* m_pObject;
	BOOL    m_bAcquired;
};

inline CSingleLock::CSingleLock(CSyncObject* pObject, BOOL bInitialLock)
{
	ASSERT(pObject != NULL);
	m_pObject = pObject;
	m_bAcquired = FALSE;
	if (bInitialLock)
		Lock();
}

inline CSingleLock::~CSingleLock()
{
	Unlock();
}

inline BOOL CSingleLock::Lock(UINT nTimeOut /* = INFINITE */)
{
	ASSERT(m_pObject != NULL);
	ASSERT(!m_bAcquired);

	m_bAcquired = m_pObject->Lock(nTimeOut);
	return m_bAcquired;
}

inline BOOL CSingleLock::Unlock()
{
	ASSERT(m_pObject != NULL);
	if (m_bAcquired)
	{
		m_bAcquired = !m_pObject->Unlock();
	}
	return !m_bAcquired;
}

inline BOOL CSingleLock::IsLocked()const
{
	return m_bAcquired;
}

#endif	/*__SYNC_OBJ_H__*/
