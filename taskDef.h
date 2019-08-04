/*
*taskDef.h
*任务相关参数定义
*
*/

#ifndef __TASK_DEF_H__
#define	__TASK_DEF_H__

#include "dp.h"
#include "dllLibEx.h"

typedef void(*EventCompleteFunc)(int stat, void* param);

struct TASK_EVENT_NODE
{
	DL_NODE				node;
	UINT				cmd;
	void*				param;
	EventCompleteFunc	completeFunc;
};

#define	EVENT_COMPLETE_OK		0
#define	EVENT_COMPLETE_FAIL		-1
#endif // !__TASK_DEF_H__
