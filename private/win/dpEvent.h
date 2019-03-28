/* dpEvent.h - windows style's event routine. for windows
 *
 * �����ṩ��EVENT���˹�����ģ���֧���Զ����飡
 *
 *   �ź�̬����ʹdpWaitForSingleEvent()/dpWaitForMultipleEvents()���ص�״̬��
 * ���ź�̬�����ź�̬�෴
 *
 * ע�⣺��Win7�£��¼����Ʋ��ܰ���'*', '?'�������ַ���������Ȼ�ܹ������ɹ���
 * ��ʹ��������Ϊ����֡�
 */
#ifndef EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H
#define EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H

typedef HANDLE					DP_EVENT_ID;
#define INVALID_DP_EVENT_ID		((DP_EVENT_ID)INVALID_HANDLE_VALUE)

#define DP_WAIT_OBJECT		(WAIT_OBJECT_0)
#define DP_WAIT_ABANDONED	WAIT_ABANDONED
#define DP_WAIT_TIMEOUT		WAIT_TIMEOUT
#define DP_WAIT_FAILED 		WAIT_FAILED
#define DP_WAIT_MAX			(DP_WAIT_ABANDONED - DP_WAIT_OBJECT) /* ����wait��event�����Ŀ */

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
���������������¼�
���������bInitialState: TRUE=�������¼�Ĭ�����ź�̬(��ʹWaitFor...Object����); FALSE=���ź�̬
          pszName: �¼����ƣ�ΪNULL��ʾ�Զ�����
          bAutoName: TRUE=�Զ�����Ψһ���¼�����, ���ɵ����ư���pszName;
                     FALSE=ֻ��pszName��Ϊ�¼�����
�� �� ֵ��INVALID_EVENT_DP_EVENT_ID=ʧ�ܣ�����ֵ=�¼�ID(������dpWaitFor...)
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
����������ʹ�¼�Ϊ�ź�̬(��ʹWaitFor...Object����)
���������hEvent: �¼����
�� �� ֵ��TRUE/FALSE
BOOL dpEventSet(DP_EVENT_ID hEvent)
*/
#define dpEventSet(hEvent)		SetEvent(hEvent)

/*
����������ʹ�¼�Ϊ���ź�̬(����ʹWaitFor...Object����)
���������hEvent: �¼����
�� �� ֵ��TRUE/FALSE
BOOL dpEventReset(ZYHD_EVENT_ID hEvent)
*/
#define dpEventReset(hEvent)		ResetEvent(hEvent)

/*
�����������ر��¼����ͷ������Դ
���������hEvent: �¼����
�� �� ֵ��TRUE/FALSE
BOOL dpEventClose(DP_EVENT_ID hEvent)
*/
#define dpEventClose(hEvent)		CloseHandle(hEvent)

/****************************************************************************************/
/****************************************************************************************/

/*
�����������ȴ�����¼��е���һ���¼�Ϊ�ź�̬
���������pEvents = ��ȴ��Ķ���¼�
         nCount = pEvents�¼���Ŀ
         nTimeout_ms = �ȴ���ʱ(ms), -1Ϊ���޵ȴ�
�� �� ֵ��DP_WAIT_xxx, pEvent[0]Ϊ�ź�̬�򷵻�DP_WAIT_OBJECT_0, ...
UINT dpWaitForMultipleEvents(const DP_EVENT_ID* pEvents, UINT nCount, UINT nTimeout_ms);
*/
#define dpWaitForMultipleEvents(pEvents, nCount, nTimeout_ms) \
		WaitForMultipleObjects(nCount, pEvents, FALSE, nTimeout_ms)

/*
�����������ȴ������¼���Ϊ�ź�̬
���������hEvent=�¼�ID
         nTimeout_ms = �ȴ���ʱ(ms), -1Ϊ���޵ȴ�
�� �� ֵ��DP_WAIT_xxx, pEvent[0]Ϊ�ź�̬�򷵻�ZYHD_WAIT_OBJECT_0, ...
UINT dpWaitForSingleEvent(DP_EVENT_ID hEvent, UINT nTimeout_ms);
*/
#define dpWaitForSingleEvent(hEvent, nTimeout_ms) \
		WaitForSingleObject(hEvent, nTimeout_ms)

#endif /* EVENT_LIB_AE8078C0_0B95_4493_93CD_34700CDC001C_H */
