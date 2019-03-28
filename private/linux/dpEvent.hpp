/*dpEvent.hpp
 *
 *
 *linux下使用fifo实现windows风格的事件
 * */

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/fd.h>
#include "sys/errno.h"
#include <pthread.h>
#include "dpEvent.h"

#define EVENT_PREFIX				"/tmp/event/"
#define	EVNET_PROC_PATH_PREFIX		"/proc/%d/fd/%d"
#define	EVNET_PROC_PATH_NAME		128
#define MAX_EVENT_PIPE_NAME			128
#define FIFO_MSG_SIZE				1
#define FIFO_MSG_READ_CLEAR_SIZE	16

DP_EVENT_ID dpEventCreate(BOOL bInitialState, const TCHAR* pszName, BOOL bAutoName)
{
	struct stat statPath;
	DP_EVENT_ID fd = INVALID_DP_EVENT_ID;
	if (stat(EVENT_PREFIX, &statPath) < 0)
	{
		if (errno != ENOENT)
			return fd;

		if (mkdir(EVENT_PREFIX, 0777) < 0)
			return fd;
	}

	char szPipeName[MAX_EVENT_PIPE_NAME];
	strcpy(szPipeName, EVENT_PREFIX);
	const int iOpenFlags = O_RDWR|O_NONBLOCK;

	if (pszName==NULL || pszName[0] == EOS)
	{
		pszName = "x";
		bAutoName = TRUE;
	}

	if (bAutoName)
	{
		// 自动产生名称
		for (UINT i=0; i<100U*1000U; i++)
		{
            sprintf(szPipeName+sizeof(EVENT_PREFIX)-1, "%s!%x:%x:%x", pszName, (UINT)pthread_self(), (UINT)times(NULL), i);
			szPipeName[sizeof(szPipeName)-1] = EOS;

			if (mkfifo(szPipeName, 0777) == OK)
				break;
		}

		fd = open(szPipeName, iOpenFlags);
	}
	else
	{
		sprintf(szPipeName+sizeof(EVENT_PREFIX)-1, "%s", pszName);
		szPipeName[sizeof(szPipeName)-1] = EOS;

		fd = open(szPipeName, iOpenFlags);
		if (fd < 0)
		{
			if (mkfifo(szPipeName, 0777) == OK)
				fd = open(szPipeName, iOpenFlags);
		}
	}

	if ((fd > 0) && bInitialState)
	{
		write(fd, szPipeName, FIFO_MSG_SIZE);
	}

	return (fd < 0)? INVALID_DP_EVENT_ID: fd;
}

BOOL dpEventSet(DP_EVENT_ID hEvent)
{
	char szTemp[FIFO_MSG_SIZE] = {0};
	write(hEvent, szTemp, FIFO_MSG_SIZE);

	return TRUE;
}

BOOL dpEventReset(DP_EVENT_ID hEvent)
{
	int nSize = 0;
	if (ioctl(hEvent, FIONREAD, &nSize) < 0)
		return FALSE;

	int readCnt = FIFO_MSG_READ_CLEAR_SIZE;
	char readBuf[FIFO_MSG_READ_CLEAR_SIZE];
	do
	{
		if (nSize < readCnt)
			readCnt = nSize;

		int ret = read(hEvent, readBuf, readCnt);
		if (ret <= 0)
			break;

		nSize -= ret;

	}while (nSize > 0);


	return (nSize == 0);
}

BOOL dpEventClose(DP_EVENT_ID hEvent)
{
	char szPipeName[MAX_EVENT_PIPE_NAME] = {0};
	char fdPath[EVNET_PROC_PATH_NAME] = {0};

	snprintf(fdPath, EVNET_PROC_PATH_NAME, EVNET_PROC_PATH_PREFIX, getpid(), hEvent);


	if (readlink(fdPath, szPipeName, MAX_EVENT_PIPE_NAME-1) < 0)
		return FALSE;

	close(hEvent);
	unlink(szPipeName);

	return TRUE;
}



UINT dpWaitForMultipleEvents(const DP_EVENT_ID* pEvents, UINT nCount, UINT nTimeout_ms)
{
	int ret;
	fd_set	set;
	struct timeval timeout;
	struct timeval* pTimeout;

	if (nTimeout_ms!=INFINITE)
	{
		timeout.tv_sec = nTimeout_ms/1000U;
		timeout.tv_usec= (nTimeout_ms%1000U)*1000U;
		pTimeout = &timeout;
	}
	else
	{
		pTimeout = NULL;
	}

	ret = 0;

	FD_ZERO(&set);
	for (UINT i=0; i<nCount; i++)
	{
		FD_SET(pEvents[i], &set);
		if (pEvents[i]>ret)
			ret = pEvents[i];
	}

	ret = select(ret+1, &set, NULL, NULL, pTimeout);
	if (ret>=1)
	{
		for (UINT i=0; i<nCount; i++)
		{
			if (FD_ISSET(pEvents[i], &set))
				return DP_WAIT_OBJECT_0+i;
		}
	}

	if (ret==0)
		return DP_WAIT_TIMEOUT;
	else
		return DP_WAIT_FAILED;
}

UINT dpWaitForSingleEvent(DP_EVENT_ID hEvent, UINT nTimeout_ms)
{
	fd_set	set;
	struct timeval timeout;
	struct timeval* pTimeout;

	if (nTimeout_ms!=INFINITE)
	{
		timeout.tv_sec = nTimeout_ms/1000U;
		timeout.tv_usec= (nTimeout_ms%1000U)*1000U;
		pTimeout = &timeout;
	}
	else
	{
		pTimeout = NULL;
	}

	FD_ZERO(&set);
    FD_SET(hEvent, &set);

    int ret = select(hEvent+1, &set, NULL, NULL, pTimeout);
    if (ret==1 && FD_ISSET(hEvent, &set))
		return DP_WAIT_OBJECT_0;
	if (ret==0)
		return DP_WAIT_TIMEOUT;
	else
		return DP_WAIT_FAILED;
}

