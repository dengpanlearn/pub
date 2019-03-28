/*dpThread.h
*/

#ifndef	__DP_THREAD_H__
#define	__DP_THREAD_H__
#include <process.h>

typedef HANDLE		DP_THREAD_ID;
typedef void*		DP_THREAD_ARG;

#define INVALID_DP_THREAD_ID	INVALID_HANDLE_VALUE
#define DP_THREAD_API			WINAPI

typedef int (DP_THREAD_API *DP_THREAD_FUNC)(DP_THREAD_ARG pArg);
typedef unsigned int (WINAPI *BEGIN_THREAD_FUNC)(void* pArg);

/*
/*
 * 功能描述: 创建线程
 * 输入参数:     name=线程名称
 *           priority=线程优先级(由OS定义)
 *            options=线程选项(由OS定义)
 *          stackSize=堆栈大小(与OS有关)
 *               func=线程入口函数
 *                arg=线程入口函数的参数
 * 返 回 值: 线程ID， INVALID_DP_THREAD_ID表示失败
 * DP_THREAD_ID dpThreadCreate(const char* name, int priority, int options,
 *		 size_t stackSize, DP_THREAD_FUNC func, DP_THREAD_ARG arg);
 */
#define dpThreadCreate(name, pri, opts, stack, func, arg) \
		dpThreadCreateWin(func, arg)

/*
 * 功能描述: 强行终止线程。一般不推荐使用。
 * 输入参数: id=线程ID
 *           iExitCode=线程退出代码
 * 返 回 值: 无
 * void zyhdThreadDelete(DP_THREAD_ID id, int iExitCode);
 */
#define dpThreadDelete(id,code)	TerminateThread(id, code)

/*
 * 功能描述: 等待线程退出。通知或要求线程退出应由调用者实现，与本API无关。
 * 输入参数: id=线程ID
 *           nTimeout_ms=等待超时(ms)
 * 返 回 值: DP_WAIT_xxx, DP_WAIT_OBJECT_0表示成功
 * UINT dpThreadWaitExit(DP_THREAD_ID id, UINT nTimeout_ms)
 */
#define dpThreadWaitExit(id, nTimeout_ms)	WaitForSingleObject(id, nTimeout_ms)

/*
 * 功能描述: 返回线程的唯一标识数字
 * 输入参数: id=线程ID
 * 返 回 值: 线程标识
 * ULONG dpThreadNoGet(DP_THREAD_ID id)
 */
#define dpThreadNoGet(id)	GetThreadId(id)

/*
 * uintptr_t _beginthreadex(void* security, unsigned stack_size,
 *         unsigned (__stdcall* entry)(void*), void* arglist,
 *         unsigned initflag, unsigned* thrdaddr);
 *
 * 某些系统，_beginthreadex()的最后一个参数为NULL将导致失败，所以不能传入NULL。
 * SetThreadPriority, ResumeThread
 *
 */
static inline DP_THREAD_ID dpThreadCreateWin(DP_THREAD_FUNC func, DP_THREAD_ARG arg)
{
	UINT nThreadId;
	uintptr_t h = _beginthreadex(NULL, 0, (BEGIN_THREAD_FUNC)func, arg, 0, &nThreadId);
	return (DP_THREAD_ID)h;
}

#endif	/*__DP_THREAD_H__*/
