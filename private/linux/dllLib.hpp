/* dllLib.c - efficient doubly linked list subroutine library */
#ifndef DLL_LIB_EX_7286A5F4_2644_42F9_9341_AFDD3DB0A75C_H
 #error "DO NOT include this, please include dllLibEx.cpp"
#endif


EXTRN DL_LIST* dllCreate (void)
{
    DL_LIST *pList = (DL_LIST *) malloc ((unsigned) sizeof (DL_LIST));
    dllInit (pList);
    return (pList);
}

EXTRN STATUS dllInit(DL_LIST *pList )
{
    pList->head	 = NULL;
    pList->tail  = NULL;
    return (STATUS_OK);
}

EXTRN STATUS dllDelete(DL_LIST *pList )
{
    free ((char *) pList);			/* free list */
    return STATUS_OK;
}

/*STATUS dllTerminate(DL_LIST *pList)
{
    return (STATUS_OK);
}
*/
//#ifdef dllLib_PORTABLE

EXTRN void dllInsert(DL_LIST *pList,  DL_NODE *pPrev,DL_NODE *pNode)
{
    DL_NODE *pNext;
    if (pPrev == NULL)
	{				/* new node is to be first in list */
		pNext = pList->head;
		pList->head = pNode;
	}
    else
	{				/* make prev node point fwd to new */
		pNext = pPrev->next;
		pPrev->next = pNode;
	}
    if (pNext == NULL)
		pList->tail = pNode;		/* new node is to be last in list */
    else
		pNext->previous = pNode;	/* make next node point back to new */
    /* set pointers in new node */
    pNode->next		= pNext;
    pNode->previous	= pPrev;
}

EXTRN void dllAdd(DL_LIST *pList,DL_NODE *pNode)
{
    dllInsert (pList, pList->tail, pNode);
}

EXTRN void dllRemove(DL_LIST *pList, DL_NODE *pNode )
{
    if (pNode->previous == NULL)
		pList->head = pNode->next;
    else
		pNode->previous->next = pNode->next;

    if (pNode->next == NULL)
		pList->tail = pNode->previous;
    else
		pNode->next->previous = pNode->previous;
}

EXTRN DL_NODE *dllGet(DL_LIST *pList)
{
    DL_NODE *pNode = pList->head;
    if (pNode != NULL)                      /* is list empty? */
	{
        pList->head = pNode->next;          /* make next node be 1st */
        if (pNode->next == NULL)            /* is there any next node? */
            pList->tail = NULL;             /*   no - list is empty */
        else
            pNode->next->previous = NULL;   /*   yes - make it 1st node */
	}
    return (pNode);
}

//#endif	/* dllLib_PORTABLE */

EXTRN int dllCount(DL_LIST *pList )
{
    DL_NODE *pNode = DLL_FIRST (pList);
    int count = 0;
    while (pNode != NULL)
	{
		count++;
		pNode = DLL_NEXT (pNode);
	}
    return (count);
}

EXTRN DL_NODE *dllEach(DL_LIST *pList,FUNCPTR routine,int  routineArg)
{
    DL_NODE *pNode = DLL_FIRST (pList);
    while ((pNode != NULL) && ((* routine) (pNode, routineArg)))
		pNode = DLL_NEXT (pNode);
    return (pNode);			/* return node we ended with */
}
