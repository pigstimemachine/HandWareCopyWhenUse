#ifndef __DATASTRUCTURESANDALGORITHMANALYSISINC_H
#define __DATASTRUCTURESANDALGORITHMANALYSISINC_H
#include "sys.h"

#ifndef _List_H

struct Node;
typedef struct _node
{
	int value ;
	struct _node *next ;
}Node;//Á´±í

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P , List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X,List L);
void Insert(ElementType X,List L, Position P);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);

#endif /*_List_H*/









#endif 















