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

typedef void(*EventCompleteFunc)(int stat, void* param, int paramLen);

enum TASK_EVENT_JOB_STEP
{
	TASK_EVENT_JOB_STEP_NONE = 0,
	TASK_EVENT_JOB_STEP_WAITING_RESP,
	TASK_EVENT_JOB_STEP_COMPLETED,
};

struct TASK_EVENT_PARAM
{
	DP_EVENT_ID		completeEvent;
};

struct TASK_EVENT_NODE
{
	DL_NODE				node;
	UINT				cmd;
	void*				param;
	int					paramLen;
	EventCompleteFunc	completeFunc;
};

#define	EVENT_COMPLETE_OK		0
#define	EVENT_COMPLETE_FAIL		-1
#endif // !__TASK_DEF_H__
