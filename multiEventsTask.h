/*
*multiEventsTask.h
*多事件处理任务类
*
*/

#ifndef __MULTI_EVNETS_TASK_H__
#define	__MULTI_EVNETS_TASK_H__

#include "taskDef.h"
#include "timeOutTask.h"

class CMultiEventsTask: public CTimeOutTask
{
public:
	CMultiEventsTask();
	virtual ~CMultiEventsTask();

	BOOL Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs, int maxEvents);
	void Close();

protected:
	virtual BOOL WorkRun();
	virtual UINT PreActive(UINT timeout);
	virtual void OnExit();
	virtual void OnTimeout();
	virtual BOOL CheckSelf();
	virtual int GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const;

private:
	void*		m_pBuf;
	DL_LIST		m_list;
};
#endif // !__MULTI_EVNETS_TASK_H__
