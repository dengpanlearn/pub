/*
*taskDef.h
*任务相关参数定义
*
*/

#ifndef __TASK_DEF_H__
#define	__TASK_DEF_H__

#include "dp.h"
#include "dpEvent.h"
#include "dllLibEx.h"

typedef BOOL (*EventCompleteFunc)(UINT cmd, int result, void* param, int paramLen);
class CMultiEventsTask;
enum TASK_EVENT_JOB_STEP
{
	TASK_EVENT_JOB_STEP_NONE = 0,
	TASK_EVENT_JOB_STEP_WAITING_RESP,
	TASK_EVENT_JOB_STEP_COMPLETED_OK,
	TASK_EVENT_JOB_STEP_COMPLETED_FAIL,
};

struct TASK_EVENT_PARAM
{
	CMultiEventsTask*		pMultiEventTask;
};

struct TASK_EVENT_NODE
{
	DL_NODE				node;
	UINT				cmd;
	EventCompleteFunc	completeFunc;
	int					paramLen;
	char				paramBuf[0];
};

#define	EVENT_COMPLETE_OK		0
#define	EVENT_COMPLETE_FAIL		-1
#endif // !__TASK_DEF_H__
