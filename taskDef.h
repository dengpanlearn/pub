/*
*taskDef.h
*任务相关参数定义
*
*/

#ifndef __TASK_DEF_H__
#define	__TASK_DEF_H__

#include "dp.h"
#include "dllLibEx.h"

typedef void(*EventCompleteFunc)(int status, void* param);

struct EVENTS_NODE
{
	DL_NODE				node;
	UINT				cmd;
	void*				param;
	EventCompleteFunc	completeFunc;
};
#endif // !__TASK_DEF_H__
