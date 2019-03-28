/*dpThread.*/


#ifndef	__DP_THREAD_H__
#define	__DP_THREAD_H__


#include <pthread.h>

typedef pthread_t					DP_THREAD_ID;
typedef void*						DP_THREAD_ARG;

#define INVALID_DP_THREAD_ID		((DP_THREAD_ID)-1)
#define DP_THREAD_API

typedef void* (DP_THREAD_API *DP_THREAD_FUNC)(DP_THREAD_ARG pArg);


/*
 * 功能描述: 创建线程
 * 输入参数:   name=线程名称
 *          priority=线程优先级(由OS定义)
 *          options=线程选项(由OS定义)
 *          stackSize=堆栈大小(与OS有关)
 *          func=线程入口函数
 *          arg=线程入口函数的参数
 *
 * 返 回 值: 线程ID， INVALID_DP_THREAD_ID表示失败
 *
 */
#define dpThreadCreate(name, pri, opts, stack, func, arg) \
		dpThreadCreateLinux(pri, stack, func, arg)

/*
 * 功能描述: 强行终止线程。一般不推荐使用。
 * 输入参数: 	 id=线程ID
 *           iExitCode=线程退出代码
 * 返 回 值: 	无
 * void zyhdThreadDelete(DP_THREAD_ID id, int iExitCode);
 */
#define dpThreadDelete(id,code)	pthread_cancel(id)

/*
 * 功能描述: 等待线程退出。通知或要求线程退出应由调用者实现，与本API无关。
 * 输入参数: id=线程ID
 *           nTimeout_ms=等待超时(ms)
 * 返 回 值: DP_WAIT_xxx, DP_WAIT_OBJECT_0表示成功
 * UINT dpThreadWaitExit(DP_THREAD_ID id, UINT nTimeout_ms)
 */
#define dpThreadWaitExit(id, nTimeout_ms)\
		dpThreadWaitExitLinux(id, nTimeout_ms)

#ifdef	__cplusplus
extern "C"{
#endif
DP_THREAD_ID dpThreadCreateLinux(int pri, int stackSize, DP_THREAD_FUNC func, DP_THREAD_ARG arg);
UINT dpThreadWaitExitLinux(DP_THREAD_ID id, UINT nTimeout_ms);
#ifdef	__cplusplus
}
#endif


#endif	/*__DP_THREAD_H__*/
