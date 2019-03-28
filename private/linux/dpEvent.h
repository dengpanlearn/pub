/*dpEvent.h
 *
 *
 * linux��ʹ��fifoʵ��windows�����¼�
 * */

#ifndef EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H
#define EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H

typedef int						DP_EVENT_ID;
#define INVALID_DP_EVENT_ID		((DP_EVENT_ID)-1)

#define DP_WAIT_OBJECT			(0)
#define DP_WAIT_ABANDONED		0x00000080
#define DP_WAIT_TIMEOUT			0x00000102
#define DP_WAIT_FAILED 			0xFFFFFFFF
#define DP_WAIT_MAX				(DP_WAIT_ABANDONED - DP_WAIT_OBJECT) /* ����wait��event�����Ŀ */

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
������������������¼�
���������bInitialState: TRUE=�������¼�Ĭ�����ź�̬; FALSE=���ź�̬
          pszName: �¼����ƣ�ΪNULL��ʾ�Զ�����
          bAutoName: TRUE=�Զ�����Ψһ���¼�����, ���ɵ����ư���pszName;
                     FALSE=ֻ��pszName��Ϊ�¼�����
�� �� ֵ��INVALID_DP_EVENT_ID=ʧ�ܣ�����ֵ=�¼�ID(������dpWaitFor...)
*/
DP_EVENT_ID dpEventCreate(BOOL bInitialState, const TCHAR* pszName, BOOL bAutoName);

/*
����������ʹ�¼�Ϊ�ź�̬
���������hEvent: �¼�ID
�� �� ֵ��TRUE/FALSE
*/
BOOL dpEventSet(DP_EVENT_ID hEvent);

/*
����������ʹ�¼�Ϊ���ź�̬
���������hEvent: �¼�ID
�� �� ֵ��TRUE/FALSE
*/
BOOL dpEventReset(DP_EVENT_ID hEvent);

/*
�����������ر��¼����ͷ������Դ
���������hEvent: �¼�ID
�� �� ֵ��TRUE/FALSE
*/
BOOL dpEventClose(DP_EVENT_ID hEvent);

/****************************************************************************************/
/****************************************************************************************/

/*
�����������ȴ�����¼��е���һ���¼�Ϊ�ź�̬
���������pEvents = ��ȴ��Ķ���¼�
         nCount = pEvents�¼���Ŀ
         nTimeout_ms = �ȴ���ʱ(ms), -1Ϊ���޵ȴ�
�� �� ֵ��DP_WAIT_xxx, pEvent[0]Ϊ�ź�̬�򷵻�DP_WAIT_OBJECT_0, ...
*/
UINT dpWaitForMultipleEvents(const DP_EVENT_ID* pEvents, UINT nCount, UINT nTimeout_ms);

/*
�����������ȴ������¼���Ϊ�ź�̬
���������hEvent=�¼�ID
         nTimeout_ms = �ȴ���ʱ(ms), -1Ϊ���޵ȴ�
�� �� ֵ��DP_WAIT_xxx, hEventΪ�ź�̬�򷵻�_WAIT_OBJECT_0, ...
*/
UINT dpWaitForSingleEvent(DP_EVENT_ID hEvent, UINT nTimeout_ms);

#ifdef __cplusplus
 }
#endif
#endif	/*EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H*/
