/*
* multiEventsTask.cpp
*
*/

#include "multiEventsTask.h"


CMultiEventsTask::CMultiEventsTask()
{
	m_pBuf = NULL;
	m_eventActive = INVALID_DP_EVENT_ID;
	dllInit(&m_freeList);
	dllInit(&m_usedList);
}

CMultiEventsTask::~CMultiEventsTask()
{
	Close();
}

BOOL CMultiEventsTask::Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs, int maxEvents)
{
	m_pBuf = calloc(maxEvents, sizeof(TASK_EVENT_NODE));
	if (m_pBuf == NULL)
		return FALSE;

	TCHAR triggerEventName[32] = { 0 };

#ifdef UNICODE
	swprintf_s(triggerEventName, countof(triggerEventName) - 1, _T("%s%s"), MULTI_TASK_EVENT_NAME_PREFIX, pNameTask);
#else
	sprintf_s(triggerEventName, countof(triggerEventName) - 1, _T("%s%s"), MULTI_TASK_EVENT_NAME_PREFIX, pNameTask);
#endif 

	do
	{
		m_eventActive = dpEventCreate(FALSE, triggerEventName, FALSE);
		if (m_eventActive == INVALID_DP_EVENT_ID)
			break;

		if (!CTimeOutTask::Create(pNameTask, stackSize, priTask, optTask, timeoutMs))
			break;

		return TRUE;
	} while (FALSE);

	Close();
	return FALSE;
}

void CMultiEventsTask::Close()
{
	if (m_eventActive != INVALID_DP_EVENT_ID)
	{
		dpEventClose(m_eventActive);
		m_eventActive = INVALID_DP_EVENT_ID;
	}

	if (m_pBuf != NULL)
	{
		free(m_pBuf);
		m_pBuf = NULL;
	}

	dllInit(&m_freeList);
	dllInit(&m_usedList);
}

void CMultiEventsTask::InitEventsBuf(void* pBuf, int iEventCounts)
{
	union
	{
		DL_NODE* pNode;
		TASK_EVENT_NODE* pEventNode;
	};

	pEventNode = (TASK_EVENT_NODE*)pBuf;
	for (int i = 0; i < iEventCounts; i++, pEventNode++)
	{
		DLL_ADD(&m_freeList, pNode);
	}
}

BOOL CMultiEventsTask::WorkRun()
{
	DP_EVENT_ID events[2] = { 0 };

	int eventNum = GetTaskEvent(events, countof(events));
	if (eventNum <= 0)
		return FALSE;

	BOOL bContinue = TRUE;
	UINT result = dpWaitForMultipleEvents(events, eventNum, PreActive(m_timeoutMs));
	switch (result)
	{
	default:
	case DP_WAIT_FAILED:
	case DP_WAIT_OBJECT_0:
		OnExit();
		bContinue = FALSE;
		break;

	case DP_WAIT_TIMEOUT:
		OnTimeout();
		break;


	case DP_WAIT_OBJECT_1:
		OnActive();
		break;
	}
	

	if (!CheckSelf())
	{
		StopWork();
	}

	return bContinue;
}

UINT CMultiEventsTask::PreActive(UINT timeout)
{
	return CTimeOutTask::PreActive(timeout);
}

void CMultiEventsTask::OnExit()
{
	CTimeOutTask::OnExit();
}

void CMultiEventsTask::OnTimeout()
{
	CTimeOutTask::OnTimeout();
}

BOOL CMultiEventsTask::CheckSelf()
{
	CTimeOutTask::CheckSelf();
}

int CMultiEventsTask::GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const
{
	// 基类的event 在前
	int eventCount = CTimeOutTask::GetTaskEvent(pEventsBuf, maxCount);
	if (eventCount > 0)
	{
		maxCount -= eventCount;

		if (maxCount < 1)
			return -1;

		pEventsBuf[eventCount++] = m_eventActive;
	}

	return eventCount;
}

void CMultiEventsTask::OnActive()
{
	TASK_EVENT_NODE* pEventNode = TakeEvent();
	if (pEventNode == NULL)
		return;

	int stat = OnEventActive(pEventNode->cmd, pEventNode->param);

	pEventNode->completeFunc(stat, pEventNode->param);
	ReleaseEvent(pEventNode);
}

int CMultiEventsTask::OnEventActive(UINT cmd, void* param)
{
	return EVENT_COMPLETE_OK;
}

BOOL CMultiEventsTask::PostEvent(UINT cmd, void* param, EventCompleteFunc completeFunc)
{
	CSingleLock lock(&m_cs);
	union
	{
		TASK_EVENT_NODE* pEventNode;
		DL_NODE*	pNode;
	};

	pNode = dllGet(&m_freeList);
	if (pNode == NULL)
		return  FALSE;

	pEventNode->cmd = cmd;
	pEventNode->param = param;
	pEventNode->completeFunc = completeFunc;

	int usedCounts = dllCount(&m_usedList);

	dllAdd(&m_usedList, pNode);

	if (usedCounts == 0)
		dpEventSet(m_eventActive);

	return TRUE;
}

TASK_EVENT_NODE* CMultiEventsTask::TakeEvent()
{
	CSingleLock lock(&m_cs);
	union
	{
		TASK_EVENT_NODE* pEventNode;
		DL_NODE*	pNode;
	};

	pNode = dllGet(&m_usedList);
	if (pNode == NULL)
		return  FALSE;

	int usedCounts = dllCount(&m_usedList);
	if (usedCounts == 0)
		dpEventReset(m_eventActive);

	return pEventNode;
}

void CMultiEventsTask::ReleaseEvent(TASK_EVENT_NODE* pEventNode)
{
	CSingleLock lock(&m_cs);
	dllAdd(&m_freeList, &pEventNode->node);
}