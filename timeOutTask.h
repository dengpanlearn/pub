/*timeoutTask.h
*
* �����׼�����̻߳��࣬�����������˳��������塢��ʱ�ӿڣ�����һ����׼�̻߳��࣬���е������߳�
* ������̳��ڸû���
*/
#ifndef __TIMEOUT_TASK_H__
#define	__TIMEOUT_TASK_H__


#include "dp.h"
#include "dpThread.h"
#include "dpEvent.h"


#define TIMEOUT_TASK_TIMEOUT_DFT		500
#define	TIMEOUT_TASK_STACKSIZE_DFT		10*1024
#define	TIMEOUT_TASK_EVENT_NAME_PREFIX	_T("event_exit_")

class CTimeOutTask
{
public:
	CTimeOutTask();
	virtual ~CTimeOutTask();

	BOOL Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs);
	void Close();


public:
	inline void StopWork();
	inline BOOL WaitWorkExit(UINT waitTimeMs);

protected:
	virtual BOOL WorkRun();
	virtual UINT PreActive(UINT timeout);
	virtual void OnExit();
	virtual void OnTimeout();
	virtual BOOL CheckSelf();
	virtual int GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const;

private:
	static int Work(void* pArg);
	int WorkCore();
	
protected:
	UINT				m_timeoutMs;

private:
	DP_THREAD_ID		m_threadId;
	DP_EVENT_ID			m_ExitEvent;
	BOOL				m_bContinue;
};

inline void CTimeOutTask::StopWork() 
{ 
	if (m_ExitEvent != INVALID_DP_THREAD_ID)
		dpEventSet(m_ExitEvent); 
}

inline BOOL CTimeOutTask::WaitWorkExit(UINT waitTimeMs)
{
	if (m_threadId == INVALID_DP_THREAD_ID)
		return TRUE;

	UINT ret = dpThreadWaitExit(m_threadId, waitTimeMs);
	if (ret == DP_WAIT_TIMEOUT)
	{
		dpThreadDelete(m_threadId, DP_WAIT_TIMEOUT);
		m_threadId = INVALID_DP_THREAD_ID;
		return FALSE;
	}

	return TRUE;
}

#endif // !__TIMEOUT_TASK_H__