/*triggerTask.cpp
*/

#include "triggerTask.h"



CTriggerTask::CTriggerTask()
{
	m_triggerEvent = INVALID_DP_EVENT_ID;
	m_triggerCmd = 0;
}


CTriggerTask::~CTriggerTask()
{
	Close();
}


BOOL CTriggerTask::Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs)
{ 
	TCHAR triggerEventName[32] = {0};

#ifdef UNICODE
	swprintf_s(triggerEventName, countof(triggerEventName) - 1, _T("%s%s"), TRIGGER_TASK_EVENT_NAME_PREFIX, pNameTask);
#else
	sprintf_s(triggerEventName, countof(triggerEventName) - 1, _T("%s%s"), TRIGGER_TASK_EVENT_NAME_PREFIX, pNameTask);
#endif 
	m_triggerEvent = dpEventCreate(FALSE, triggerEventName, FALSE);

	if (m_triggerEvent == INVALID_DP_EVENT_ID)
		return FALSE;

	if (!CTimeOutTask::Create(pNameTask, stackSize, priTask, optTask, timeoutMs))
	{
		dpEventClose(m_triggerEvent);

		m_triggerEvent = INVALID_DP_EVENT_ID;
		return FALSE;
	}

	return TRUE;
}

void CTriggerTask::Close()
{
	// 先关线程，所以先调用基类的Close()函数
	CTimeOutTask::Close();
	if (m_triggerEvent != INVALID_DP_EVENT_ID)
	{
		dpEventClose(m_triggerEvent);
		m_triggerEvent = INVALID_DP_EVENT_ID;
	}
}

BOOL CTriggerTask::StartTrigger(void* pTriggerArg)
{
	CSingleLock lock(&m_cs, TRUE);

	if (TRIGGER_STEP_NONE != m_triggerStep)
		return FALSE;

	if (!PrepareTriggerParam(pTriggerArg))
		return FALSE;

	m_triggerCmd = TRIGGER_CMD_START;
	dpEventSet(m_triggerEvent);

	return TRUE;
}

void CTriggerTask::StopTrigger()
{
	DP_UNUSED_EX(pTriggerArg);
	CSingleLock lock(&m_cs, TRUE);

	if (TRIGGER_STEP_NONE == m_triggerStep)
		return ;

	m_triggerCmd = TRIGGER_CMD_STOP;
	m_triggerStep = TRIGGER_STEP_WAIT_ACTIVE;
	dpEventSet(m_triggerEvent);
}

BOOL CTriggerTask::WorkRun()
{
	BOOL bContinue = TRUE;
	DP_EVENT_ID events[2] = { 0 };

	int eventNum = GetTaskEvent(events, countof(events));
	if (eventNum <= 0)
		return FALSE;

	UINT result = dpWaitForMultipleEvents(events, eventNum, PreActive(m_timeoutMs));
	if (result == DP_WAIT_FAILED)
	{
		bContinue = FALSE;
	}
	else if (result == DP_WAIT_TIMEOUT)
	{
		OnTimeout();
	}
	else if (result == DP_WAIT_OBJECT_0)
	{
		OnExit();
		bContinue = FALSE;
	}
	else if (result == DP_WAIT_OBJECT_1)
	{
		OnActive();
	}

	if (!CheckSelf())
	{
		StopWork();
	}
	return bContinue;
}

UINT CTriggerTask::PreActive(UINT timeout)
{
	return CTimeOutTask::PreActive(timeout);
}

void CTriggerTask::OnActive()
{
	CSingleLock lock(&m_cs, TRUE);
	if (m_triggerCmd == TRIGGER_CMD_START)
		m_triggerStep = TRIGGER_STEP_PREPARE_START;
	else if (m_triggerCmd == TRIGGER_CMD_STOP)
		m_triggerStep = TRIGGER_STEP_PREPARE_STOP;

	dpEventReset(m_triggerEvent);
}

void CTriggerTask::OnExit()
{
	CTimeOutTask::OnExit();
}

void CTriggerTask::OnTimeout()
{
	UINT curStep = m_triggerStep;
	
	if (curStep == TRIGGER_STEP_NONE)
		return;
	else if (curStep == TRIGGER_STEP_PREPARE_START)
		curStep = OnPrepareStartWork(curStep);
	else if (curStep == TRIGGER_STEP_PREPARE_STOP)
		curStep = OnPrepareStopWork(curStep);
	else if (curStep == TRIGGER_STEP_TIMEOUT_WORK)
		curStep = OnTimeoutWork(curStep);

	if (curStep != m_triggerStep)
	{
		CSingleLock lock(&m_cs, TRUE);
		m_triggerStep = curStep;
	}
}

BOOL CTriggerTask::CheckSelf()
{
	return CTimeOutTask::CheckSelf();
}

BOOL CTriggerTask::PrepareTriggerParam(void* pArg)
{
	DP_UNUSED_EX(pArg);
	m_workTimes = 0;
	return TRUE;
}

UINT CTriggerTask::OnPrepareStartWork(UINT step)
{
	DP_UNUSED_EX(step);
	return TRIGGER_STEP_TIMEOUT_WORK;
}

UINT CTriggerTask::OnTimeoutWork(UINT step)
{
	DP_UNUSED_EX(step);
	return TRIGGER_STEP_NONE;
}

UINT CTriggerTask::OnPrepareStopWork(UINT step)
{
	DP_UNUSED_EX(step);
	return TRIGGER_STEP_NONE;
}

int CTriggerTask::GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const
{
	// 基类的event 在前
	int eventCount = CTimeOutTask::GetTaskEvent(pEventsBuf, maxCount);
	if (eventCount > 0)
	{
		maxCount -= eventCount;

		if (maxCount < 1)
			return -1;

		pEventsBuf[eventCount++] = m_triggerEvent;
	}

	return eventCount;
}