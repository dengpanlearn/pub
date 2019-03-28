/* SyncObj.h */
#if !defined(_MFC_VER) && !defined(WINDOWS_SYNCHRONIZATION_H)
#define WINDOWS_SYNCHRONIZATION_H

// CSyncObject
//   CCriticalSection
//   CEvent
//   CSemaphore
//
// CSingleLock
// 注意:
// CExclusiveLock: 独占式锁定不再被支持，软件应该使用效率更高的读写锁。
// InitializeSRWLock

/////////////////////////////////////////////////////////////////////////////
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
	CRITICAL_SECTION	m_crisec;
};

inline CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection(&m_crisec);
}

/////////////////////////////////////////////////////////////////////////////
// CSemaphore - 计数性互斥
class CSemaphore : public CSyncObject
{
// Constructor
public:
	CSemaphore(int iInitCount=64);
	//virtual ~CSemaphore();

// Operations
protected:
	virtual BOOL Lock(UINT nTimeOut = INFINITE);
	virtual BOOL Unlock();

protected:
	HANDLE	m_hSem;
	int		m_iInitCount;	// 初始数量，也是最大数量
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

#endif /* !_MFC_VER && !WINDOWS_SYNCHRONIZATION_H */
