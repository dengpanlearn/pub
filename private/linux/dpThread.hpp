/*dpThread.hpp
 *
 * */

#include <sys/time.h>
#include "dpEvent.h"

DP_THREAD_ID dpThreadCreateLinux(int pri, int stackSize, DP_THREAD_FUNC func, DP_THREAD_ARG arg)
{

    DP_THREAD_ID threadId = INVALID_DP_THREAD_ID;

    pthread_attr_t	threadAttr;
    pthread_attr_init(&threadAttr);

#ifdef	_POSIX_THREAD_PRIORITY_SCHEDULING
    if (pthread_attr_setinheritsched(&threadAttr, PTHREAD_EXPLICIT_SCHED) < 0)
        goto _END;
        
    if (pthread_attr_setschedpolicy(&threadAttr, SCHED_FIFO) < 0)
        goto _END;

    if (pri < sched_get_priority_min(SCHED_FIFO))
        pri = sched_get_priority_min(SCHED_FIFO);

    if (pri > sched_get_priority_max(SCHED_FIFO))
        pri = sched_get_priority_max(SCHED_FIFO);

    sched_param param;
    param.__sched_priority = pri;
    if (pthread_attr_setschedparam(&threadAttr, &param) < 0)
        goto _END;

#else
    printf("unsupport priority sched\n");
#endif

    pthread_attr_setstacksize(&threadAttr, stackSize);

    if (pthread_create(&threadId, &threadAttr, func, arg) < 0)
    {
        threadId = INVALID_DP_THREAD_ID;
    }

_END:
    pthread_attr_destroy(&threadAttr);
    return threadId;
}

UINT dpThreadWaitExitLinux(DP_THREAD_ID id, UINT nTimeout_ms)
{
	timeval endTime;
	timespec endTs;

	gettimeofday(&endTime, NULL);

	int waitSec = nTimeout_ms/1000;
	int waitMs = nTimeout_ms%1000;

	endTime.tv_sec += waitSec;
	endTime.tv_usec += waitMs*1000;
	if (endTime.tv_usec >= 1000000)
	{
		endTime.tv_sec++;
		endTime.tv_usec -= 1000000;
	}

	endTs.tv_sec = endTime.tv_sec;
	endTs.tv_nsec = endTime.tv_usec*1000;
	if (pthread_timedjoin_np(id, NULL, &endTs) < 0)
        return DP_WAIT_TIMEOUT;

	return 0;
}
