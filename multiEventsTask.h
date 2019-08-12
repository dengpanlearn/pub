/*
*multiEventsTask.h
*多事件处理任务类
*
*/

#ifndef __MULTI_EVNETS_TASK_H__
#define	__MULTI_EVNETS_TASK_H__

#include "dp.h"
#include "SyncObj.h"
#include "taskDef.h"
#include "timeOutTask.h"

#define	MULTI_TASK_EVENT_NAME_PREFIX	_T("event_multi_")

class CMultiEventsTask: public CTimeOutTask
{
public:
	CMultiEventsTask();
	virtual ~CMultiEventsTask();

	BOOL Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs, int maxEvents, int maxEventBufSize);
	void Close();

protected:
	virtual BOOL WorkRun();
	virtual UINT PreActive(UINT timeout);
	virtual void OnExit();
	virtual void OnTimeout();
	virtual BOOL CheckSelf();
	virtual int GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const;

	virtual void OnActive();
	virtual int OnEventActive(UINT cmd, void* param, int paramLen);

public:
	BOOL PostEvent(UINT cmd, void* param, int paramLen, EventCompleteFunc completeFunc);

protected:
	inline DP_EVENT_ID	GetTaskActiveEvent();
	inline void InitMultiTaskEventParam(void* param, int paramLen);

	TASK_EVENT_NODE* TakeEvent();

	void BackEvent(TASK_EVENT_NODE* pEventNode);
	void ReleaseEvent(TASK_EVENT_NODE* pEventNode);

protected:
	static BOOL MultiTaskEventComplete(UINT cmd, int result, void* param, int paramLen);

	virtual BOOL OnEventComplete(UINT cmd, int result, void* param, int paramLen);

private:
	void InitEventsBuf(void* pBuf, int iEventCounts);

private:
	void*		m_pBuf;
	DL_LIST		m_freeList;
	DL_LIST		m_usedList;
	CCriticalSection	m_cs;
	DP_EVENT_ID	m_eventActive;
	int			m_iEventParamSizeMax;
};

inline DP_EVENT_ID	CMultiEventsTask::GetTaskActiveEvent()
{
	return m_eventActive;
}

inline void CMultiEventsTask::InitMultiTaskEventParam(void* param, int paramLen)
{
	TASK_EVENT_PARAM* pEventParam = (TASK_EVENT_PARAM*)param;
	pEventParam->pMultiEventTask = this;
}
#endif // !__MULTI_EVNETS_TASK_H__
