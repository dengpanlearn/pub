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
 * ��������: �����߳�
 * �������:   name=�߳�����
 *          priority=�߳����ȼ�(��OS����)
 *          options=�߳�ѡ��(��OS����)
 *          stackSize=��ջ��С(��OS�й�)
 *          func=�߳���ں���
 *          arg=�߳���ں����Ĳ���
 *
 * �� �� ֵ: �߳�ID�� INVALID_DP_THREAD_ID��ʾʧ��
 *
 */
#define dpThreadCreate(name, pri, opts, stack, func, arg) \
		dpThreadCreateLinux(pri, stack, func, arg)

/*
 * ��������: ǿ����ֹ�̡߳�һ�㲻�Ƽ�ʹ�á�
 * �������: 	 id=�߳�ID
 *           iExitCode=�߳��˳�����
 * �� �� ֵ: 	��
 * void zyhdThreadDelete(DP_THREAD_ID id, int iExitCode);
 */
#define dpThreadDelete(id,code)	pthread_cancel(id)

/*
 * ��������: �ȴ��߳��˳���֪ͨ��Ҫ���߳��˳�Ӧ�ɵ�����ʵ�֣��뱾API�޹ء�
 * �������: id=�߳�ID
 *           nTimeout_ms=�ȴ���ʱ(ms)
 * �� �� ֵ: DP_WAIT_xxx, DP_WAIT_OBJECT_0��ʾ�ɹ�
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
