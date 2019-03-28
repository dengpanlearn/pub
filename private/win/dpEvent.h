/* dpEvent.h - windows style's event routine. for windows
 *
 * 本库提供的EVENT是人工复归的，不支持自动复归！
 *
 *   信号态：能使dpWaitForSingleEvent()/dpWaitForMultipleEvents()返回的状态。
 * 非信号态：与信号态相反
 *
 * 注意：在Win7下，事件名称不能包含'*', '?'等特殊字符。否则虽然能够创建成功，
 * 但使用起来行为很奇怪。
 */
#ifndef EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H
#define EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H

typedef HANDLE					DP_EVENT_ID;
#define INVALID_DP_EVENT_ID		((DP_EVENT_ID)INVALID_HANDLE_VALUE)

#define DP_WAIT_OBJECT		(WAIT_OBJECT_0)
#define DP_WAIT_ABANDONED	WAIT_ABANDONED
#define DP_WAIT_TIMEOUT		WAIT_TIMEOUT
#define DP_WAIT_FAILED 		WAIT_FAILED
#define DP_WAIT_MAX			(DP_WAIT_ABANDONED - DP_WAIT_OBJECT) /* 可以wait的event最大数目 */

#define DP_WAIT_OBJECT_0		(DP_WAIT_OBJECT + 0U)
#define DP_WAIT_OBJECT_1		(DP_WAIT_OBJECT + 1U)
#define DP_WAIT_OBJECT_2		(DP_WAIT_OBJECT + 2U)
#define DP_WAIT_OBJECT_3		(DP_WAIT_OBJECT + 3U)
#define DP_WAIT_OBJECT_4		(DP_WAIT_OBJECT + 4U)
#define DP_WAIT_OBJECT_5		(DP_WAIT_OBJECT + 5U)
#define DP_WAIT_OBJECT_6		(DP_WAIT_OBJECT + 6U)
#define DP_WAIT_OBJECT_7		(DP_WAIT_OBJECT + 7U)
#define DP_WAIT_OBJECT_MAX		(DP_WAIT_OBJECT + DP_WAIT_MAX - 1U)

#define DP_WAIT_ABANDONED_0	(DP_WAIT_ABANDONED + 0U)
#define DP_WAIT_ABANDONED_1	(DP_WAIT_ABANDONED + 1U)
#define DP_WAIT_ABANDONED_2	(DP_WAIT_ABANDONED + 2U)
#define DP_WAIT_ABANDONED_3	(DP_WAIT_ABANDONED + 3U)
#define DP_WAIT_ABANDONED_4	(DP_WAIT_ABANDONED + 4U)
#define DP_WAIT_ABANDONED_5	(DP_WAIT_ABANDONED + 5U)
#define DP_WAIT_ABANDONED_6	(DP_WAIT_ABANDONED + 6U)
#define DP_WAIT_ABANDONED_7	(DP_WAIT_ABANDONED + 7U)

/*
功能描述：创建事件
输入参数：bInitialState: TRUE=创建的事件默认是信号态(可使WaitFor...Object返回); FALSE=非信号态
          pszName: 事件名称，为NULL表示自动命名
          bAutoName: TRUE=自动生成唯一的事件名称, 生成的名称包含pszName;
                     FALSE=只用pszName作为事件名称
返 回 值：INVALID_EVENT_DP_EVENT_ID=失败；其它值=事件ID(可用于dpWaitFor...)
ZYHD_EVENT_ID zyhdEventCreate(BOOL bInitialState, const TCHAR* pszName, BOOL bAutoName);
*/
inline DP_EVENT_ID dpEventCreate(BOOL bInitialState, const TCHAR* pszName, BOOL bAutoName)
{
	if (bAutoName)
		return CreateEvent(NULL, TRUE, bInitialState, NULL);
	else
		return CreateEvent(NULL, TRUE, bInitialState, pszName);
}

/*
功能描述：使事件为信号态(可使WaitFor...Object返回)
输入参数：hEvent: 事件句柄
返 回 值：TRUE/FALSE
BOOL dpEventSet(DP_EVENT_ID hEvent)
*/
#define dpEventSet(hEvent)		SetEvent(hEvent)

/*
功能描述：使事件为非信号态(不可使WaitFor...Object返回)
输入参数：hEvent: 事件句柄
返 回 值：TRUE/FALSE
BOOL dpEventReset(ZYHD_EVENT_ID hEvent)
*/
#define dpEventReset(hEvent)		ResetEvent(hEvent)

/*
功能描述：关闭事件，释放相关资源
输入参数：hEvent: 事件句柄
返 回 值：TRUE/FALSE
BOOL dpEventClose(DP_EVENT_ID hEvent)
*/
#define dpEventClose(hEvent)		CloseHandle(hEvent)

/****************************************************************************************/
/****************************************************************************************/

/*
功能描述：等待多个事件中的任一个事件为信号态
输入参数：pEvents = 需等待的多个事件
         nCount = pEvents事件数目
         nTimeout_ms = 等待超时(ms), -1为无限等待
返 回 值：DP_WAIT_xxx, pEvent[0]为信号态则返回DP_WAIT_OBJECT_0, ...
UINT dpWaitForMultipleEvents(const DP_EVENT_ID* pEvents, UINT nCount, UINT nTimeout_ms);
*/
#define dpWaitForMultipleEvents(pEvents, nCount, nTimeout_ms) \
		WaitForMultipleObjects(nCount, pEvents, FALSE, nTimeout_ms)

/*
功能描述：等待单个事件中为信号态
输入参数：hEvent=事件ID
         nTimeout_ms = 等待超时(ms), -1为无限等待
返 回 值：DP_WAIT_xxx, pEvent[0]为信号态则返回ZYHD_WAIT_OBJECT_0, ...
UINT dpWaitForSingleEvent(DP_EVENT_ID hEvent, UINT nTimeout_ms);
*/
#define dpWaitForSingleEvent(hEvent, nTimeout_ms) \
		WaitForSingleObject(hEvent, nTimeout_ms)

#endif /* EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H */
