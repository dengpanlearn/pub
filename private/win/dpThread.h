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
 * ��������: �����߳�
 * �������:     name=�߳�����
 *           priority=�߳����ȼ�(��OS����)
 *            options=�߳�ѡ��(��OS����)
 *          stackSize=��ջ��С(��OS�й�)
 *               func=�߳���ں���
 *                arg=�߳���ں����Ĳ���
 * �� �� ֵ: �߳�ID�� INVALID_DP_THREAD_ID��ʾʧ��
 * DP_THREAD_ID dpThreadCreate(const char* name, int priority, int options,
 *		 size_t stackSize, DP_THREAD_FUNC func, DP_THREAD_ARG arg);
 */
#define dpThreadCreate(name, pri, opts, stack, func, arg) \
		dpThreadCreateWin(func, arg)

/*
 * ��������: ǿ����ֹ�̡߳�һ�㲻�Ƽ�ʹ�á�
 * �������: id=�߳�ID
 *           iExitCode=�߳��˳�����
 * �� �� ֵ: ��
 * void zyhdThreadDelete(DP_THREAD_ID id, int iExitCode);
 */
#define dpThreadDelete(id,code)	TerminateThread(id, code)

/*
 * ��������: �ȴ��߳��˳���֪ͨ��Ҫ���߳��˳�Ӧ�ɵ�����ʵ�֣��뱾API�޹ء�
 * �������: id=�߳�ID
 *           nTimeout_ms=�ȴ���ʱ(ms)
 * �� �� ֵ: DP_WAIT_xxx, DP_WAIT_OBJECT_0��ʾ�ɹ�
 * UINT dpThreadWaitExit(DP_THREAD_ID id, UINT nTimeout_ms)
 */
#define dpThreadWaitExit(id, nTimeout_ms)	WaitForSingleObject(id, nTimeout_ms)

/*
 * ��������: �����̵߳�Ψһ��ʶ����
 * �������: id=�߳�ID
 * �� �� ֵ: �̱߳�ʶ
 * ULONG dpThreadNoGet(DP_THREAD_ID id)
 */
#define dpThreadNoGet(id)	GetThreadId(id)

/*
 * uintptr_t _beginthreadex(void* security, unsigned stack_size,
 *         unsigned (__stdcall* entry)(void*), void* arglist,
 *         unsigned initflag, unsigned* thrdaddr);
 *
 * ĳЩϵͳ��_beginthreadex()�����һ������ΪNULL������ʧ�ܣ����Բ��ܴ���NULL��
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
