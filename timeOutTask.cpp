
#include <string.h>
#include "timeOutTask.h"


CTimeOutTask::CTimeOutTask()
{
	m_threadId = INVALID_DP_THREAD_ID;
	m_ExitEvent = INVALID_DP_EVENT_ID;

	m_timeoutMs = TIMEOUT_TASK_TIMEOUT_DFT;
	m_bContinue = TRUE;
}


CTimeOutTask::~CTimeOutTask()
{
	Close();
}

BOOL CTimeOutTask::Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs)
{
	if (stackSize < TIMEOUT_TASK_STACKSIZE_DFT)
		stackSize = TIMEOUT_TASK_STACKSIZE_DFT;

	if (timeoutMs > 0)
		m_timeoutMs = timeoutMs;

	TCHAR eventExitName[32] = { 0 };
	swprintf_s(eventExitName, countof(eventExitName)-1, _T("%s%s"), TIMEOUT_TASK_EVENT_NAME_PREFIX, pNameTask);
	
	
	m_ExitEvent = dpEventCreate(FALSE, eventExitName, FALSE);
	if (m_ExitEvent == INVALID_DP_EVENT_ID)
	{
		return FALSE;
	}

	m_threadId = dpThreadCreate(pNameTask, priTask, optTask, stackSize, (DP_THREAD_FUNC)Work, this);
	if (m_threadId == INVALID_DP_THREAD_ID)
	{
		dpEventClose(m_ExitEvent);
		m_ExitEvent = INVALID_DP_EVENT_ID;
		return FALSE;
	}

	return TRUE;
}

void CTimeOutTask::Close()
{
	if (m_threadId != INVALID_DP_THREAD_ID)
	{
		dpThreadDelete(m_threadId, 0);
		m_threadId = INVALID_DP_THREAD_ID;
	}

	if (m_ExitEvent != INVALID_DP_EVENT_ID)
	{
		dpEventClose(m_ExitEvent);
		m_ExitEvent = INVALID_DP_EVENT_ID;
	}
}

BOOL CTimeOutTask::WorkRun()
{
	UINT result = dpWaitForSingleEvent(m_ExitEvent, PreActive(m_timeoutMs));
	if (result == DP_WAIT_FAILED)
	{
		return FALSE;
	}
	else if (result == DP_WAIT_TIMEOUT)
	{
		OnTimeout();
	}
	else
	{
		// exit
		OnExit();
		return FALSE;
	}


	if (!CheckSelf())
	{
		dpEventSet(m_ExitEvent);
	}
	return TRUE;
}


UINT CTimeOutTask::PreActive(UINT timeout)
{
	return timeout;
}

void CTimeOutTask::OnExit()
{
	dpEventReset(m_ExitEvent);
}

void CTimeOutTask::OnTimeout()
{

}

BOOL CTimeOutTask::CheckSelf()
{
	return TRUE;
}

int CTimeOutTask::GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const
{
	if (maxCount < 1)
		return -1;

	pEventsBuf[0] = m_ExitEvent;
	return 1;
}

int CTimeOutTask::Work(void* pArg)
{
	CTimeOutTask* pTask = (CTimeOutTask*)pArg;

	return pTask->WorkCore();
}

int CTimeOutTask::WorkCore()
{
	while (m_bContinue)
	{
		m_bContinue = WorkRun();
	}

	return TRUE;
}