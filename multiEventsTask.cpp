/*
* multiEventsTask.cpp
*
*/

#include "multiEventsTask.h"


CMultiEventsTask::CMultiEventsTask()
{
	m_pBuf = NULL;
	m_iEventParamSizeMax = 0;
	m_eventActive = INVALID_DP_EVENT_ID;
	dllInit(&m_freeList);
	dllInit(&m_usedList);
}

CMultiEventsTask::~CMultiEventsTask()
{
	Close();
}

BOOL CMultiEventsTask::Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs, int maxEvents, int maxEventBufSize)
{
	m_iEventParamSizeMax = ROUND_UP(maxEventBufSize, 64);
	m_pBuf = calloc(maxEvents, sizeof(TASK_EVENT_NODE)+ m_iEventParamSizeMax);
	if (m_pBuf == NULL)
		return FALSE;
	InitEventsBuf(m_pBuf, maxEvents);
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
	CTimeOutTask::Close();
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
		char*	pTmpBuf;
	};

	pTmpBuf = (char*)pBuf;
	for (int i = 0; i < iEventCounts; i++ )
	{
		DLL_ADD(&m_freeList, pNode);
		pTmpBuf += m_iEventParamSizeMax + sizeof(TASK_EVENT_NODE);
	}
}

BOOL CMultiEventsTask::WorkRun()
{
	int eventCounts = GetTaskEventCount();
	DP_EVENT_ID events[MULTI_TASK_EVENTS_MAX] = { 0 };

	int eventNum = GetTaskEvent(events, (eventCounts < MULTI_TASK_EVENTS_MAX)? eventCounts: MULTI_TASK_EVENTS_MAX);
	if (eventNum <= 0)
		return FALSE;

	BOOL bContinue = TRUE;
	UINT result = dpWaitForMultipleEvents(events, eventNum, PreActive(m_timeoutMs));
	switch (result)
	{
	default:
		OnSubEventActive(result);
		break;

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

void CMultiEventsTask::OnSubEventActive(UINT evnetNum)
{

}

void CMultiEventsTask::OnTimeout()
{
	CTimeOutTask::OnTimeout();
}

BOOL CMultiEventsTask::CheckSelf()
{
	return CTimeOutTask::CheckSelf();
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

int CMultiEventsTask::GetTaskEventCount()const
{
	return CTimeOutTask::GetTaskEventCount() + 1;
}

void CMultiEventsTask::OnActive()
{
	TASK_EVENT_NODE* pEventNode = TakeEvent();
	if (pEventNode == NULL)
		return;

	int result = OnEventActive(pEventNode->cmd, pEventNode->paramBuf, pEventNode->paramLen);

	if (pEventNode->completeFunc != NULL)
	{
		if (!pEventNode->completeFunc(pEventNode->cmd,result, pEventNode->paramBuf, pEventNode->paramLen))
		{
			// 失败，将命令事件重新加入待处理列表等待处理
			BackEvent(pEventNode);
			return;
		}
	}
		
	ReleaseEvent(pEventNode);
}

int CMultiEventsTask::OnEventActive(UINT cmd, void* param, int paramLen)
{
	return EVENT_COMPLETE_OK;
}

void* CMultiEventsTask::AllocPktByEvent(UINT cmd, int paramLen, EventCompleteFunc completeFunc, CMultiEventsTask* pEventTask)
{
	if (paramLen > m_iEventParamSizeMax)
		return NULL;

	CSingleLock lock(&m_cs, TRUE);
	union
	{
		TASK_EVENT_NODE* pEventNode;
		DL_NODE*	pNode;
	};

	pNode = dllGet(&m_freeList);
	if (pNode == NULL)
		return  NULL;

	pEventNode->cmd = cmd;
	pEventNode->paramLen = paramLen;
	pEventNode->completeFunc = completeFunc;
	TASK_EVENT_PARAM* pEventParam = (TASK_EVENT_PARAM*)pEventNode->paramBuf;
	pEventParam->pMultiEventTask = pEventTask;

	return pEventParam;
}

void CMultiEventsTask::PostPktByEvent(void* pBuf)
{
	CSingleLock lock(&m_cs, TRUE);
	union
	{
		TASK_EVENT_NODE* pEventNode;
		DL_NODE*	pNode;
	};

	pEventNode = member_to_object(pBuf, TASK_EVENT_NODE, paramBuf);

	int usedCounts = dllCount(&m_usedList);
	dllAdd(&m_usedList, pNode);


	if (usedCounts == 0)
		dpEventSet(m_eventActive);
}

void CMultiEventsTask::BackEvent(TASK_EVENT_NODE* pEventNode)
{
	CSingleLock lock(&m_cs, TRUE);
	int usedCounts = dllCount(&m_usedList);
	dllInsert(&m_usedList, NULL, &pEventNode->node);

	if (usedCounts == 0)
		dpEventSet(m_eventActive);
}

void CMultiEventsTask::ReleaseEvent(TASK_EVENT_NODE* pEventNode)
{
	CSingleLock lock(&m_cs, TRUE);
	dllAdd(&m_freeList, &pEventNode->node);
}

BOOL CMultiEventsTask::MultiTaskEventComplete(UINT cmd, int result, void* param, int paramLen)
{
	TASK_EVENT_PARAM* pEventParam = (TASK_EVENT_PARAM*)param;
	CMultiEventsTask* pMultiEventTask = pEventParam->pMultiEventTask;
	if (pMultiEventTask == NULL)
		return TRUE;
	return pMultiEventTask->OnEventComplete(cmd, result, param, paramLen);
}

BOOL CMultiEventsTask::OnEventComplete(UINT cmd, int result, void* param, int paramLen)
{
	return TRUE;
}

TASK_EVENT_NODE* CMultiEventsTask::TakeEvent()
{
	CSingleLock lock(&m_cs, TRUE);
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