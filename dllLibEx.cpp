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
功能描述：查找指定的节点
输入参数：pList=双向链表；pNode=被查找节点
返 回 值：TRUE=被查找节点存在于链表中; FALSE=不存在
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
功能描述：将pList2合并到pList1中，合并后pList2被清空。
输入参数：pList1=源双向链表；
          pList1Node=合并时，pList2插到pList1Node之后，NULL表示插到原来pList1的最前面。
          pList2=被合并的链表
输出参数：pList1=合并后的新链表
返 回 值：TRUE=被查找节点存在于链表中; FALSE=不存在
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
