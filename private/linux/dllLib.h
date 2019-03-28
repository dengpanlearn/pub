/* dllLib.h - doubly linked list library header */
#ifndef DLL_LIB_92CF3DC0_2071_4337_9C9B_197B18F90F6A_LINUX_H
#define DLL_LIB_92CF3DC0_2071_4337_9C9B_197B18F90F6A_LINUX_H

/* type definitions */
typedef struct dlnode		/* Node of a linked list. */
{
	struct dlnode *next;	/* Points at the next node in the list */
	struct dlnode *previous;	/* Points at the previous node in the list */
} DL_NODE;


/* HIDDEN */
typedef struct			/* Header for a linked list. */
{
	DL_NODE *head;	/* header of list */
	DL_NODE *tail;	/* tail of list */
} DL_LIST;
/* END_HIDDEN */

/******************************************************************************
*
* DLL_INIT - initialize doubly linked list descriptor
*
* Initialize the specified list to an empty list.
*
* NOMANUAL
*/

#define DLL_INIT(list)                                                      \
	{                                                                       \
	((DL_LIST *)(list))->head = NULL;                                       \
	((DL_LIST *)(list))->tail = NULL;                                       \
	}

/*******************************************************************************
*
* dllFirst - find first node in list
*
* DESCRIPTION
* Finds the first node in a doubly linked list.
*
* RETURNS
*	Pointer to the first node in a list, or
*	NULL if the list is empty.
*
* NOMANUAL
*/

#define DLL_FIRST(pList)		\
	(					\
	(((DL_LIST *)(pList))->head)	\
	)

/*******************************************************************************
*
* dllLast - find last node in list
*
* Finds the last node in a doubly linked list.
*
* RETURNS
*  pointer to the last node in list, or
*  NULL if the list is empty.
*
* NOMANUAL
*/

#define DLL_LAST(pList)			\
	(					\
	(((DL_LIST *)(pList))->tail)	\
	)

/*******************************************************************************
*
* dllNext - find next node in list
*
* Locates the node immediately after the node pointed to by the pNode.
*
* RETURNS:
* 	Pointer to the next node in the list, or
*	NULL if there is no next node.
*
* NOMANUAL
*/

#define DLL_NEXT(pNode)			\
	(					\
	(((DL_NODE *)(pNode))->next)	\
	)

/*******************************************************************************
*
* dllPrevious - find preceding node in list
*
* Locates the node immediately before the node pointed to by the pNode.
*
* RETURNS:
* 	Pointer to the preceding node in the list, or
*	NULL if there is no next node.
*
* NOMANUAL
*/

#define DLL_PREVIOUS(pNode)		\
	(					\
	(((DL_NODE *)(pNode))->previous)	\
	)

/*******************************************************************************
*
* dllEmpty - boolean function to check for empty list
*
* RETURNS:
* 	TRUE if list is empty
*	FALSE otherwise
*
* NOMANUAL
*/

#define DLL_EMPTY(pList)			\
	(						\
	(((DL_LIST *)pList)->head == NULL)		\
	)

/******************************************************************************
*
* DLL_INSERT - insert node in list after specified node
*
* This macro inserts the specified node in the specified list.
* The new node is placed following the specified 'previous' node in the list.
* If the specified previous node is NULL, the node is inserted at the head
* of the list.
*
* NOMANUAL
*/

#define DLL_INSERT(list, previousNode, node)                                \
	{                                                                       \
	DL_NODE *temp;                                                          \
	if ((previousNode) == NULL)                                             \
		{                                                                   \
		temp = ((DL_LIST *)(list))->head;                                   \
		((DL_LIST *)(list))->head = (DL_NODE *)(node);                      \
		}                                                                   \
	else                                                                    \
		{                                                                   \
		temp = ((DL_NODE *)(previousNode))->next;                           \
		((DL_NODE *)(previousNode))->next = (DL_NODE *)(node);              \
		}                                                                   \
	if (temp == NULL)                                                       \
		((DL_LIST *)(list))->tail = (DL_NODE *)(node);                      \
	else                                                                    \
	   temp->previous = (DL_NODE *)(node);                                  \
	((DL_NODE *)(node))->next = temp;                                       \
	((DL_NODE *)(node))->previous = (DL_NODE *)(previousNode);              \
	}

/******************************************************************************
*
* DLL_ADD - add node to end of list
*
* This macro adds the specified node to the end of the specified list.
*
* NOMANUAL
*/

#define DLL_ADD(list, node)                                                 \
	{                                                                       \
	DL_NODE *listTail = (list)->tail;                                       \
	DLL_INSERT ((list), listTail, (node));                                  \
	}

/******************************************************************************
*
* DLL_REMOVE - remove specified node in list
*
* Remove the specified node in the doubly linked list.
*
* NOMANUAL
*/

#define DLL_REMOVE(list, node)                                               \
	{                                                                        \
	if (((DL_NODE *)(node))->previous == NULL)                               \
		((DL_LIST *)(list))->head = ((DL_NODE *)(node))->next;               \
	else                                                                     \
		((DL_NODE *)(node))->previous->next = ((DL_NODE *)(node))->next;     \
	if (((DL_NODE *)(node))->next == NULL)                                   \
		((DL_LIST *)(list))->tail = ((DL_NODE *)(node))->previous;           \
	else                                                                     \
		((DL_NODE *)(node))->next->previous = ((DL_NODE *)(node))->previous; \
	}

/******************************************************************************
*
* DLL_GET - get (delete and return) first node from list
*
* This macro gets the first node from the specified list, deletes the node
* from the list, and returns a pointer to the node gotten.
*
* NOMANUAL
*/

#define DLL_GET(list, node)                                                 \
	{                                                                       \
	(node) = (void *)((DL_LIST *)(list))->head;                             \
	if ((node) != NULL)                                                     \
		{                                                                   \
		((DL_LIST *)(list))->head = ((DL_NODE *)(node))->next;              \
		if (((DL_NODE *)(node))->next == NULL)                              \
			((DL_LIST *)(list))->tail = NULL;                               \
		else                                                                \
			((DL_NODE *)(node))->next->previous = NULL;                     \
		}                                                                   \
	}

/* function declarations */
#ifdef __cplusplus
extern "C" {
#endif

extern DL_LIST* dllCreate (void);
extern DL_NODE* dllEach (DL_LIST *pList, FUNCPTR routine, int routineArg);
extern DL_NODE* dllGet (DL_LIST *pList);
extern STATUS 	dllDelete (DL_LIST *pList);
extern STATUS 	dllInit (DL_LIST *pList);
extern STATUS 	dllTerminate (DL_LIST *pList);
extern int 	    dllCount (DL_LIST *pList);
extern void 	dllAdd (DL_LIST *pList, DL_NODE *pNode);
extern void 	dllInsert (DL_LIST *pList, DL_NODE *pPrev, DL_NODE *pNode);
extern void 	dllRemove (DL_LIST *pList, DL_NODE *pNode);

#ifdef __cplusplus
}
#endif

#endif /* DLL_LIB_92CF3DC0_2071_4337_9C9B_197B18F90F6A_LINUX_H */
