/* dllLibEx.cpp - extend doubly linked list library routines */
#include <stdio.h>
#include <stdlib.h>
#include "dp.h"
#include "dllLibEx.h"

#ifdef _WIN32
 #include "private/win/dllLib.hpp"
#elif defined(_LINUX)
 #include "private/linux/dllLib.hpp"
#else
 #error
#endif

/*
��������������ָ���Ľڵ�
���������pList=˫������pNode=�����ҽڵ�
�� �� ֵ��TRUE=�����ҽڵ������������; FALSE=������
*/
extern "C" BOOL dllFind(const DL_LIST* pList, void* pNode)
{
	DL_NODE* p;
	for (p = DLL_FIRST(pList); p!=NULL; p = DLL_NEXT(p))
	{
		if(p==pNode)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*
������������pList2�ϲ���pList1�У��ϲ���pList2����ա�
���������pList1=Դ˫������
          pList1Node=�ϲ�ʱ��pList2�嵽pList1Node֮��NULL��ʾ�嵽ԭ��pList1����ǰ�档
          pList2=���ϲ�������
���������pList1=�ϲ����������
�� �� ֵ��TRUE=�����ҽڵ������������; FALSE=������
*/
extern "C" void dllJoin(DL_LIST* pList1, DL_NODE* pList1Node, DL_LIST* pList2)
{
	DL_NODE* p;
	DL_NODE* pPrev;
	for (p=DLL_LAST(pList2); p!=NULL; p=pPrev)
	{
		pPrev = DLL_PREVIOUS(p);
		dllInsert(pList1, pList1Node, p);
	}
	dllRemoveAll(pList2);
}
