/*
*triggerTask.h
* 
*/

#ifndef __TRIGGER_TASK_H__
#define	__TRIGGER_TASK_H__


#include "timeOutTask.h"
#include "SyncObj.h"

#define	TRIGGER_TASK_EVENT_NAME_PREFIX			_T("event_trigger_")

class CTriggerTask: public CTimeOutTask
{
public:
	CTriggerTask();
	virtual ~CTriggerTask();

	BOOL Create(LPCTSTR pNameTask, int stackSize, int priTask, int optTask, int timeoutMs);
	void Close();

public:
	BOOL StartTrigger(void* pTriggerArg);
	void StopTrigger();

	inline BOOL IsTriggerStop();
	inline UINT GetTriggerWorkProgress();

	enum TRIGGER_CMD {
		TRIGGER_CMD_START = 0x01,
		TRIGGER_CMD_STOP  = 0x02,
	};

protected:
	virtual BOOL WorkRun();
	virtual UINT PreActive(UINT timeout);
	virtual void OnActive();
	virtual void OnExit();
	virtual void OnTimeout();
	virtual BOOL CheckSelf();

	virtual BOOL PrepareTriggerParam(void* pArg);
	virtual UINT OnPrepareStartWork(UINT step);
	virtual UINT OnTimeoutWork(UINT step);
	virtual UINT OnPrepareStopWork(UINT step);
	
	virtual int GetTaskEvent(DP_EVENT_ID* pEventsBuf, int maxCount)const;


protected:
	inline void UpdateTriggerWorkProgress();
	
private:
	enum TRIGGER_STEP {
		TRIGGET_STEP_NONE = 0,
		TRIGGET_STEP_PREPARE_START,
		TRIGGET_STEP_PREPARE_STOP,
		TRIGGET_STEP_TIMEOUT_WORK,
	};

	DP_EVENT_ID		m_triggerEvent;
	CCriticalSection	m_cs;
	UINT			m_triggerCmd;

	UINT			m_triggerStep;
	UINT			m_workTimes;
};

inline BOOL CTriggerTask::IsTriggerStop()
{
	CSingleLock lock(&m_cs, TRUE);

	return (TRIGGET_STEP_NONE == m_triggerStep);
}

inline void CTriggerTask::UpdateTriggerWorkProgress()
{
	CSingleLock lock(&m_cs, TRUE);
	m_workTimes++;
}

inline UINT CTriggerTask::GetTriggerWorkProgress()
{
	CSingleLock lock(&m_cs, TRUE);
	return m_workTimes;
}

#endif // !__TRIGGER_TASK_H__