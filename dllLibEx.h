/* dllLibEx.h - extend doubly linked list library header */
#ifndef DLL_LIB_EX_7286A5F4_2644_42F9_9341_AFDD3DB0A75C_H
#define DLL_LIB_EX_7286A5F4_2644_42F9_9341_AFDD3DB0A75C_H

#ifdef __VXWORKS__
 #include <dllLib.h>
#elif defined(_WIN32)
 #include "private/win/dllLib.h"
#elif defined(_LINUX)
#include "private/linux/dllLib.h"
#else
#error
#endif

/*
�����������ж�˫�������Ƿ�Ϊ��
*/
#define DLL_IS_EMPTY(pList)		DLL_EMPTY(pList)

/*
�������������˫������
*/
#define DLL_REMOVE_ALL(pList)	DLL_INIT(pList)

/*
�������������˫������
void dllRemoveAll(DL_LIST* pList)
void dllEmpty(DL_LIST* pList)
*/
#define dllRemoveAll(pList)	dllInit(pList)
#define dllEmpty(pList)		dllInit(pList)

#ifdef __cplusplus
extern "C" {
#endif

/*
��������������ָ���Ľڵ�
���������pList=˫������pNode=�����ҽڵ�
�� �� ֵ��TRUE=�����ҽڵ������������; FALSE=������
*/
BOOL dllFind(const DL_LIST* pList, void* pNode);

/*
������������pList2�ϲ���pList1�У��ϲ���pList2����ա�
���������pList1=Դ˫������
          pList1Node=�ϲ�ʱ��pList2�嵽pList1Node֮��NULL��ʾ�嵽ԭ��pList1����ǰ�档
          pList2=���ϲ�������
���������pList1=�ϲ����������
�� �� ֵ��TRUE=�����ҽڵ������������; FALSE=������
*/
void dllJoin(DL_LIST* pList1, DL_NODE* pList1Node, DL_LIST* pList2);

#ifdef __cplusplus
}
#endif

#endif /* DLL_LIB_EX_7286A5F4_2644_42F9_9341_AFDD3DB0A75C_H */
