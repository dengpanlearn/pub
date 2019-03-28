/*dpEvent.h
 *
 *
 * linux下使用fifo实现windows风格的事件
 * */

#ifndef EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H
#define EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H

typedef int						DP_EVENT_ID;
#define INVALID_DP_EVENT_ID		((DP_EVENT_ID)-1)

#define DP_WAIT_OBJECT			(0)
#define DP_WAIT_ABANDONED		0x00000080
#define DP_WAIT_TIMEOUT			0x00000102
#define DP_WAIT_FAILED 			0xFFFFFFFF
#define DP_WAIT_MAX				(DP_WAIT_ABANDONED - DP_WAIT_OBJECT) /* 可以wait的event最大数目 */

#define DP_WAIT_OBJECT_0		(DP_WAIT_OBJECT + 0)
#define DP_WAIT_OBJECT_1		(DP_WAIT_OBJECT + 1)
#define DP_WAIT_OBJECT_2		(DP_WAIT_OBJECT + 2)
#define DP_WAIT_OBJECT_3		(DP_WAIT_OBJECT + 3)
#define DP_WAIT_OBJECT_4		(DP_WAIT_OBJECT + 4)
#define DP_WAIT_OBJECT_5		(DP_WAIT_OBJECT + 5)
#define DP_WAIT_OBJECT_6		(DP_WAIT_OBJECT + 6)
#define DP_WAIT_OBJECT_7		(DP_WAIT_OBJECT + 7)
#define DP_WAIT_OBJECT_MAX		(DP_WAIT_OBJECT + DP_WAIT_MAX - 1)

#define DP_WAIT_ABANDONED_0		(DP_WAIT_ABANDONED + 0)
#define DP_WAIT_ABANDONED_1		(DP_WAIT_ABANDONED + 1)
#define DP_WAIT_ABANDONED_2		(DP_WAIT_ABANDONED + 2)
#define DP_WAIT_ABANDONED_3		(DP_WAIT_ABANDONED + 3)
#define DP_WAIT_ABANDONED_4		(DP_WAIT_ABANDONED + 4)
#define DP_WAIT_ABANDONED_5		(DP_WAIT_ABANDONED + 5)
#define DP_WAIT_ABANDONED_6		(DP_WAIT_ABANDONED + 6)
#define DP_WAIT_ABANDONED_7		(DP_WAIT_ABANDONED + 7)

#ifndef INFINITE
 #define INFINITE	(UINT)(-1)
#endif

#ifdef __cplusplus
 extern "C" {
#endif

/*
功能描述：创建或打开事件
输入参数：bInitialState: TRUE=创建的事件默认是信号态; FALSE=非信号态
          pszName: 事件名称，为NULL表示自动命名
          bAutoName: TRUE=自动生成唯一的事件名称, 生成的名称包含pszName;
                     FALSE=只用pszName作为事件名称
返 回 值：INVALID_DP_EVENT_ID=失败；其它值=事件ID(可用于dpWaitFor...)
*/
DP_EVENT_ID dpEventCreate(BOOL bInitialState, const TCHAR* pszName, BOOL bAutoName);

/*
功能描述：使事件为信号态
输入参数：hEvent: 事件ID
返 回 值：TRUE/FALSE
*/
BOOL dpEventSet(DP_EVENT_ID hEvent);

/*
功能描述：使事件为非信号态
输入参数：hEvent: 事件ID
返 回 值：TRUE/FALSE
*/
BOOL dpEventReset(DP_EVENT_ID hEvent);

/*
功能描述：关闭事件，释放相关资源
输入参数：hEvent: 事件ID
返 回 值：TRUE/FALSE
*/
BOOL dpEventClose(DP_EVENT_ID hEvent);

/****************************************************************************************/
/****************************************************************************************/

/*
功能描述：等待多个事件中的任一个事件为信号态
输入参数：pEvents = 需等待的多个事件
         nCount = pEvents事件数目
         nTimeout_ms = 等待超时(ms), -1为无限等待
返 回 值：DP_WAIT_xxx, pEvent[0]为信号态则返回DP_WAIT_OBJECT_0, ...
*/
UINT dpWaitForMultipleEvents(const DP_EVENT_ID* pEvents, UINT nCount, UINT nTimeout_ms);

/*
功能描述：等待单个事件中为信号态
输入参数：hEvent=事件ID
         nTimeout_ms = 等待超时(ms), -1为无限等待
返 回 值：DP_WAIT_xxx, hEvent为信号态则返回_WAIT_OBJECT_0, ...
*/
UINT dpWaitForSingleEvent(DP_EVENT_ID hEvent, UINT nTimeout_ms);

#ifdef __cplusplus
 }
#endif
#endif	/*EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H*/
