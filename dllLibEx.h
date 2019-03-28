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
功能描述：判断双向链表是否为空
*/
#define DLL_IS_EMPTY(pList)		DLL_EMPTY(pList)

/*
功能描述：清空双向链表
*/
#define DLL_REMOVE_ALL(pList)	DLL_INIT(pList)

/*
功能描述：清空双向链表
void dllRemoveAll(DL_LIST* pList)
void dllEmpty(DL_LIST* pList)
*/
#define dllRemoveAll(pList)	dllInit(pList)
#define dllEmpty(pList)		dllInit(pList)

#ifdef __cplusplus
extern "C" {
#endif

/*
功能描述：查找指定的节点
输入参数：pList=双向链表；pNode=被查找节点
返 回 值：TRUE=被查找节点存在于链表中; FALSE=不存在
*/
BOOL dllFind(const DL_LIST* pList, void* pNode);

/*
功能描述：将pList2合并到pList1中，合并后pList2被清空。
输入参数：pList1=源双向链表；
          pList1Node=合并时，pList2插到pList1Node之后，NULL表示插到原来pList1的最前面。
          pList2=被合并的链表
输出参数：pList1=合并后的新链表
返 回 值：TRUE=被查找节点存在于链表中; FALSE=不存在
*/
void dllJoin(DL_LIST* pList1, DL_NODE* pList1Node, DL_LIST* pList2);

#ifdef __cplusplus
}
#endif

#endif /* DLL_LIB_EX_7286A5F4_2644_42F9_9341_AFDD3DB0A75C_H */
