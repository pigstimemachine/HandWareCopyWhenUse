#include "DataStructuresAndAlgorithmAnalysisInC.h"
/*Euler's constant  r≈0.57721566*/










/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	一个递归函数例子
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/06/30/  
  *		     	1.1 2020/06/30/ change sth
  * @Note  
  1-2

  **/
  /*
递归函数基本法则：

1. 基准情形（base case）.必须有某些基准的情形，它们不用递归就能求解。
2. 不断推进（making progress）。对于那些需要递归求解的情形，递归调用必须能够朝着产生基准情形的方向推进。
3. 设计法则（design rule ）。假设所有的递归调用都能运行。

4.合成效益法则（compound interest rule）。在求解一个问题的同一实例时，切勿在不同的递归调用种做重复性的工作。
  */
  /*正常递归调用*/
int F(int x)
{
	if(x == 0)
		return 0 ;
	else 
		return  2 * F(x-1) + x * x ;
}

/*1-3*无终止递归程序**/
int Bad( unsigned int N )
{
	if( N == 0 )
		return 0 ;
	else
		return Bad( N / 3 + 1 ) + N - 1 ;
}
/*1-4打印整数的递归例程*/
void PrintOut( unsigned in N )/*print nonnegative N*/
{
	if( N >= 10 )
		PrintOut( N / 10 ) ;
	PrintDigit( N % 10 ) ;
}

/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	计算最大子序列和
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 就是在一个集合里，任意的几个数相加，也可以是某个数本身，找出相加后最大的结果， 
  （这些子集所处的位置必须是连续的，所以叫子序列
	图2-5
  */
int MaxSubsequenceSum( const int A[] ,int N )
{
	int ThisSum , MaxSum , i ,j ,k;
	MaxSum = 0 ;
	for( i =0; i<N ;i++)
		for(j=i;j<N;j++)
		{
			ThisSum=0;
			for(k=i;k<=j;k++)
				ThisSum+=A[k];

			if(ThisSum>MaxSum)
				MaxSum=ThisSum;
		}
	return MaxSum;
}
/*图2-6 算法2 计算最大子序列和*/
int MaxSubSequenceSum(const int A[], int N)
{
	int ThisSum ,MaxSum,i,j;

	MaxSum=0;
	for(i=0;i<N;i++)
	{
		ThisSum=0;
		for(j=i;j<N;j++)
		{
			ThisSum+=A[j];

			if(ThisSum >MaxSum)
				MaxSum=ThisSum;
		}
	}
	return MaxSum;
}
/*图2-7 算法3 计算最大子序列和*/
static int MaxSubSum(const int A[] ,int Left , int Right )
{
	int MaxLeftSum , MaxRightSum;
	int MaxLeftBorderSum, MaxRightBorderSum;
	int LeftBorderSum, RightBorderSum;
	int Center , i;

	if(Left == Right )/*Base Case*/
		if(A[Left] > 0 )
			return A[Left];
		else
			return 0;
	Center = (Left + Right ) / 2 ;
	MaxLeftSum = MaxSubSum(A , Left , Center);
	MaxRightSum = MaxSubSum(A , Center +1 ; Right );

	MaxLeftBorderSum = 0 ;LeftBorderSum = 0 ;
	for( i = Center; i>= Left ; i-- )
	{
		LeftBorderSum += A[i];
		if(LeftBorderSum > MaxLeftBorderSum )
			MaxLeftBorderSum = LeftBorderSum;
	}

	MaxRightBorderSum = 0 ; RightBorderSum = 0;	
	for( i = Center + 1; i<= Right ; i++ )
	{
		RightBorderSum += A[i];
		if(RightBorderSum > MaxRightBorderSum )
			MaxRightBorderSum = RightBorderSum;
	}
/*伪例程（pseudoroutine）Max3 返回 这三个可能的最大和中的最大者*/
	return Max3(MaxLeftSum , MaxRightSum , 
				MaxLeftBorderSum + MaxRightBorderSum);
}
int MaxSubSequenceSum(const int A [ ], int N)
{
	return MaxSubSum(A,0,N-1);
}

/*图2-8 算法4 计算最大子序列和 最优*/
int MaxSubSequenceSum(const int A [ ], int N)
{
	int ThisSum , MaxSum , j;

	ThisSum = MaxSum=0;
	for(j=0;j<N;j++)
	{
		ThisSum+= A[j];

		if(ThisSum >MaxSum)
			MaxSum =  ThisSum ;
		else if(ThisSum <0)
			ThisSum = 0 ;
	}
	return MaxSum;
}

/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	对分查找 求使得A=X 的下标，不存在则返回-1
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 某数组已经排列好顺序
	图2-9
  */
int BinarySearch(const ElementType A[],ElementType X , int N)
{
	int Low, Mid ,High;

	Low=0;High = N-1 ;
	while(Low<=High)
	{
		Mid = (Low + High) / 2 ; 
		if(A[Mid] < X)
			Low = Mid +1 ;
		else if(A[Mid] > X)
			High = Mid - 1;
		else 
			return Mid;/*Found*/
	}
	return  NotFound; /*NotFoud is defined as -1*/
}
/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	欧几里得算法 两个整数的最大公因数（Gcd） 同时整除二者的最大整数
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 
	图2-10
  */
unsigned int Gcd(unsigned int M, unsigned int N)
{
	unsigned int Rem;

	while(N>0)
	{
		Rem = M % N;
		M=N;
		N=Rem;
	}
	return M;
}
/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	高效率的取幂运算
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 
	图2-11
  */

long int Pow(long int X , unsigned int N)
{
	if(N==0)
		return 1;
	if(N==1)
		return X;
	if(IsEven(N))//是否为偶数？
		return Pow(X*X,N/2);
	else
		return Pow(X*X,N/2)*X;//Pow(X,N-1)*X
}


/**
@@@@@@ @指针是一个用以储存其他变量地址的变量@@@@@@@@@@@@@
  * @brief   	how to use it ,and where to use it
  * @Name	 	抽象数据类型（Abstract Data Type , ADT）是一些操作的集合。
  * @param   	None   链表
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/23/  
  *		     	1.1 2020/07/23/ change sth
  * @Note 并（union） 交（intersection） 求大小（size） 取余（complement）等操作
	//
  */
/*图3-8 Return true if L is empty 测试一个链表是否是空表的函数 */
int IsEmpty(List L)
{
	return L->next==NULL;
}
/*图3-9 测试当前位置是否是链表的末尾的函数
Return true if P is the last position in last L 
Parameter L is unused in this implementation*/
int IsLast(Position P, List L)
{
	return P->next==NULL;
}
/*图3-10 Find例程
Return Position of X in L ;NULL if not found */
Position Find(ElementType X, List L)
{
	Position P;

	P=L->next;
	while(P!=NULL && P->Element != X)
		P = P->next ;

	return P;
}
/*图3-11 链表的删除例程
Delete first occurrence of X from a list 
Assume use of a header node */
void Delete(ElementType X, List L)
{
	Position P ,TmpCell;

	P = FindPrevious( X,  L);

	if(!IsLast( P,  L)) /*Assumption of header use*/
	{					/*X is found ; delete it */
		TmpCell = P->next;
		P->next =TmpCell->next;/*Bypass deleted cell */
		free(TmpCell);
	}
}
/*图3-12 FindPrevious----与Delete一起使用的Find例程
If X is not found ,then Next field of returned 
Position is NULL
Assumes a header */
Position Find(ElementType X, List L)
{
	Position P;

	P=L;
	while(P->next != NULL && P->next->Element != X )
		P=P->next;

	return P;
}

/*图3-13 链表的插入例程
Insert (after legal position P)
Header implementation assumed 
Parameter L is unused in this implementation */
void Insert(ElementType X, List L, Position P)
{
	Position TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if(TmpCell == NULL)
		FatalError("Out of space!!!");
	TmpCell->Element =X;
	TmpCell->next=P->next;
	P->next=TmpCell;
}
/*图3-14 删除一个表的不正确的方法
Incorrect DeleteList algorithm */
void DeleteList(List L)
{
	Position P;

	P = L->next;/*Header assumed*/
	L->next = NULL;
	while( P != NULL )
	{
		free(P);
		P=P->next;
	}
}
/*图3-15 删除表的正确方法
Correct DeleteList algorithm*/
void DeleteList(List L)
{
	Position P,Tmp;

	P = L->next;/*Header assumed*/
	L->next=NULL;
	while(P != NULL )
	{
		Tmp	= P->next;
		free(P);
		P=Tmp;
	}
}


/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	抽象数据类型（Abstract Data Type , ADT）是一些操作的集合。
  * @param   	None   
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/25/  
  *		     	1.1 2020/07/25/ change sth
  * @Note 
	//
金额 22w 等额本息   957.45*30 还款总额 34.47 支付利息   12.47   
					1134.40*30		   40.84			18.84
						
			      1072.10*25          32.16          10.16
			      1241.64*25		  37.25			 15.25
			      
			      1247.83*20		 29.95 			7.95
			      1409.7*20 		 33.83			11.83
		     
  */
/*图3-18 多项式ADT的数组实现的结构类型*/
typedef  struct
{
	int CoeffArray[MaxDegree + 1 ];
	int HighPower;
}*Polynomial;

/*图3-19 将多项式初始化为零的过程*/
void ZeroPolynomial( Polynomial Poly)
{
	int i;

	for(i=0;i<=MaxDegree;i++)
		Poly->CoeffArray[i]=0;
	Poly->HighPower =0;
}
/*Dynamic Link Library调试动态链接库*/
/*图3-20 两个多项式相加的过程*/
void AddPolynomial(const Polynomial Poly1,
					const Polynomial Poly2, Polynomial PolySum )
{
	int i;

	ZeroPolynomial(PolySum);
	PolySum->HighPower = Max(Poly1->HighPower , Poly2->HighPower);

	for(i=PolySum->HighPower;i>=0;i--)
	{
		PolySum->CoeffArray[i]=Poly1->CoeffArray[i] +Poly2->CoeffArray[i];	
	}
}

/*图3-21 两个多项式相乘的过程*/
void MultPolynomial(const Polynomial Poly1,const Polynomial Poly2,Polynomial PolyProd)
{
	int i,j;

	ZeroPolynomial(PolyProd);
	PolyProd->HighPower = Poly1->HighPower + Poly2->HighPower;

	if(PolyProd->HighPower > MaxDegree)
		Error("Exceeded array size");
	else
		for(i=0;i<=Poly1->HighPower;i++)
			for(j=0;j<=Poly2->HighPower;j++)
				PolyProd->CoeffArray[i+j] += Poly1->CoeffArray[i] * Poly2->CoeffArray[j] ;
}


/*图3-23 多项式ADT链表实现的类型声明*/
typedef struct Node *PtrToNode;

struct Node
{
	int Coefficient;
	int Exponent;
	PtrToNode Next;
};
typedef PtrToNode Polynomial;/*Nodes sorted by exponent*/


/*图3-28 链表游标的实现声明*/
#ifdef _Cursor_H

typedef int PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

void InitializeCursorSpace(void);

List MakeEmpty(List L);
int IsEmpty(const List L);
int IsLast(const Position P, const List L);
Position Find(ElementType X,const List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X,const List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
Position Header(const List L);
Position First(const List L);
Position Advance(const Position P);
ElementType Retrieve(const Position P);

#endif /*_Cursor_H*/

/*Place in the implementation file*/
struct Node
{
	ElementType Element;
	Position Next ;
};
struct Node CursorSpace[SpaceSize];

/*图3-30 例程：CursorAlloc和CursorFree*/
static Position CursorAlloc(void)
{
	Position P;

	P = CursorSpace[0].Next;
	CursorSpace[0].Next = CursorSpace[P].Next;

	return P;
}
static void 
CursorFree(Position P)
{
	CursorSpace[P].Next = CursorSpace[0].Next ;
	CursorSpace[0].Next = P;
}



/*图3-32 测试一个链表是否为空的函数--游标实现*/
/*Return true if L is empty*/
int IsEmpty(List L)
{
	return CursorSpace[L].Next == 0 ;
}


/*图3-33 测试P是否是链表的末尾的函数---游标实现
Return true if P is the last position in list L 
Parameter L is unused in this implementation*/
int IsLast(Position P, List L)
{
	return CursorSpace[p].Next == 0;
}


/*图3-34 例程Find --游标实现
Return Position of X in L ;0 if not found
Uses a header node*/
Position Find(ElementType X,  List L)
{
	Position P;

	P = CursorSpace[L].Next;
	while(P && CursorSpace[P].Element !=X )
		P = CursorSpace[P].Next;

	return P;
}


/*图3-35 对链表进行删除操作的例程Delete--游标实现
Delete first occurrence of X from a list
Assume use of a header node*/
void Delete(ElementType X, List L)
{
	Position P,TmpCell;

	P = FindPrevious( X,  L);

	if(! IsLast( P,  L)) /*Assumption of header use*/
	{
		TmpCell = CursorSpace[P].Next;
		CursorSpace[P].Next = CursorSpace[TmpCell].Next;
		CursorFree(TmpCell);
	}	
}


/*图3-36 对链表进行插入操作的例程Insert--游标实现
Insert (after legal position P)
Header implementation assmed 
Parameter L is unused in this implementation*/
void Insert(ElementType X, List L, Position P)
{
	Position TmpCell;
	TmpCell = CursorAlloc();
	if( TmpCell == 0 )
		FatalError("Out of space！！！");

	CursorSpace[TmpCell].Element = X;
	CursorSpace[TmpCell].Next = CursorSpace[P].Next;
	CursorSpace[P].Next = TmpCell;
}


/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	栈ADT
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*图3-39 栈ADT链表实现的类型声明*/
#ifdef _Stack_h

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

int IsEmpty(Stack S );
Stack CreatStack(void);
void DisposeStack(Stack S );
void MakeEmpty(Stack S );
void Push(ElementType X ,Stack S);
ElementType Top(Stack S);
void Pop(Stack S);

#endif /*_Stack_h*/


/*Place in implementation file 
Stack implementation is a linked list with a header*/
struct Node
{
	ElementType Element;
	PtrToNode Next;
};

/*图3-40 测试栈是否是空栈的例程--链表实现*/
int IsEmpty(Stack S)
{
	return S->Next == NULL;
}


/*图3-41 创建一个空栈的例程--链表实现*/
Stack CreatStack(void)
{
	Stack S;

	S =malloc(sizeof(struct Node));
	if(S == NULL )
		FatalError("Out of space!!!");//致命错误
	S->Next == NULL;
	MakeEmpty(S);
	return S;
}
void MakeEmpty(Stack S)
{
	if(S == NULL)
		Error("Must use CreateStack first");
	else
		while(!IsEmpty(S))
			Pop(S);
}


/*图3-42 元素进栈的例程--链表实现*/
void Push(ElementType X, Stack S)
{
	PtrToNode TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if(TmpCell ==  Null)
		FatalError("Out of space!!!");
	else
	{
		TmpCell->Element =X;
		TmpCell->Next = S->Next;
		S->Next =TmpCell;
	}
}


/*图3-43 返回栈顶元素的例程--链表实现*/
ElementType Top(Stack S)
{
	if(!IsEmpty(S))
		return S->Next->Element;
	Error("Empty stack");
	return 0;/*Return value used to avoid warning*/
}


/*图3-44 从栈弹出元素的例程--链表实现*/
void Pop(Stack S)
{
	PtrToNode FirstCell;

	if(IsEmpty(S))
		Error("Error stack");
	else
	{
		FirstCell = S->Next;
		S->Next = S->Next->Next;
		free(FirstCell);
	}
}


/*图3-45 栈的声明--数组实现*/
#ifndef _Stack_h

struct StackRecord;
typedef struct StackRecord *Stack;

int IsEmpty(Stack S);
int IsFull(Stack S);
Stack CreatStack(int MaxElements);
void DisposeStack(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
ElementType Top(Stack S);
void Pop(Stack S);
ElementType TopAndPop(Stack S);

#endif /*_Stack_h*/


/*Place in implementation file 
Stack implementation is a dynamically allocated array*/
#define EmptyTOS(-1)
#define MinStackSize (5)

struct StackRecord
{
	int Capacity;
	int TopOfStack;
	ElementType *Array;
};


/*图3-46 栈的创建--数组实现*/
Stack CreatStack(int MaxElements)
{
	Stack S;

	if(MaxElements < MinStackSize)
		Error("Stack size is too small");
	S = malloc(sizeof(struct StackRecord));
	if(S == NULL)
		FatalError("Out of space !!!");
	S->Array = malloc(sizeof(ElementType)* MaxElements);
	MakeEmpty(S);

	return S;
}


/*图3-47 释放栈的过程--数组实现*/
void DisposeStack(Stack S)
{
	if(S != NULL )
	{
		free(S->Array);
		free(S);
	}
}


/*图3-48 检测栈是否是空栈的例程--数组实现*/
int IsEmpty(Stack S)
{
	return S->TopOfStack == EmptyTOS;
}


/*图3-49 创建一个空栈的例程--数组实现*/
void MakeEmpty(Stack S)
{
	S->TopOfStack = EmptyTOS;
}

/*图3-50 元素进栈的例程--数组实现*/
void Push(ElementType X, Stack S)
{
	if(IsFull(S))
		Error("Full stack");
	else
		S->Array[ ++S->TopOfStack ] = X; 
}

/*图3-51 返回栈顶元素的例程--数组实现*/
ElementType Top(Stack S)
{
	if(!IsEmpty(S))
		return S->Array[ S->TopOfStack ] ;
	Error("Empty stack");
	return 0 ; /*Return value used to avoid warning*/
}


/*图3-52 从栈弹出元素的例程--数组实现*/
void Pop(Stack S)
{
	if(IsEmpty(S))
		Error("Empty stack");
	else
		S->TopOfStack--;
}

/*图3-53 给出栈顶元素并从栈弹出的例程--数组实现*/
ElementType TopAndPop(Stack S)
{
	if(!IsEmpty(S))
		return S->Array[ S->TopOfStack-- ];
	Error("Empty stack");
	return 0;/*Return value used to avoid warning */
}


/*图3-54 递归的不当使用：打印一个链表
Bad use of recursion:Printing a linked list
No header */
void PrintList(List L)
{
	if( L != NULL )
	{
		PrintElement(L->Element);
		PrintList(L->Next);
	}
}


/*图3-55 不用递归打印一个表，编译器可以完成这项工作（你不要去做）
Printing a linked list non-recursively
Uses a mechanical translation
No header*/
void PrintList(List L)
{
	top:
		if(L != NULL )
		{
			PrintElement(L->Element);
			L = L->Next;
			goto top;
		}
}


/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	队列ADT
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/28 
  *		     	1.1 2020/07/28 change sth
  * @Note  
  
  **/
  /*图3-57 队列的类型声明*/
#ifdef _Queue_h

struct QueueRecord;
typedef struct QueueRecord *Queue;

int IsEmpty(Queue Q);
int IsFull(Queue Q);
Queue CreateQueue(int MaxElements);
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
void Enqueue(ElementType X,Queue Q);
ElementType Front(Queue Q);
ElementType FrontAndDequeue(Queue Q);

#endif /*_Queue_h*/


/*Place in implementation file
Queue implementation is a dynamically allocated array*/
#define MinQueueSize (5)

struct QueueRecord
{
	int Capacity;
	int Front;
	int Rear;
	int Size;
	ElementType *Array;
};

/*图3-58 测试队列是否为空的例程--数组实现*/
int IsEmpty(Queue Q)
{
	return Q->Size == 0 ;
}

/*图3-59 构造空队列的例程--数组实现*/
void MakeEmpty(Queue Q)
{
	Q->Size = 0;
	Q->Front = 1;
	Q->Rear = 0 ;
}

/*图3-60 入队的例程--数组实现*/
static int Succ(int Value , Queue Q)
{
	if( ++Value == Q->Capacity )
		Value = 0;
	return Value;
}
void Enqueue(ElementType X, Queue Q)
{
	if(IsFull(Q))
		Error("Full  queue");
	else
	{
		Q->Size ++;
		Q->Rear = Succ(Q->Rear，  Q);
		Q->Array[Q->Rear] = X;
	}
}




/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	树tree 类树treelike
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/29  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*图4-3 树的节点声明*/
typedef struct TreeNode *PtrToNode;

struct TreeNode
{
	ElementType Element;
	PtrToNode FirstChild;
	PtrToNode NextSibling
}



/*图4-6 列出分级文件系统中目录的例程*/
static void ListDir(DirectoryOrFile D,int Depth)
{
	if( D is a legitimate entry)
	{
		PrintName(D,Depth);	
		if(D is a directory)
			for each child ,C,of D
				ListDir(C,Depth + 1);
	}
}
void ListDirectory(DirectoryOrFile D)
{
	ListDir(D,0);
}


/*图4-9 计算一个目录大小的例程*/
static void SizeDirectory(DirectoryOrFile D)
{
	int TotalSize;

	TotalSize =0;
	if(D is a legitimate entry)
	{
		TotalSize = FileSize(D);
		if(D is a directory)
			for each chile,C,of D
				TotalSize += SizeDirectory(C);
	}
	return TotalSize;
}

/*图4-13 二叉树节点声明*/
typedef struct TreeNode *PtrToNode;
typedef struct PtrToNode Tree;

struct TreeNode
{
	ElementType Element;
	Tree Left;
	Tree Right;
};


/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	查找树ADT 二叉查找树
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/29  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*图4-16 二叉查找树声明*/
#ifndef _Tree_H

struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree;

SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType X,SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType X,SearchTree T);
SearchTree Delete(ElementType X,SearchTree T);
ElementType Retrieve(Position P)

#endif /*_Tree_H*/

/*Place in the implementation file*/
struct TreeNode
{
	ElementType Element;
	SearchTree Left;
	SearchTree Right;
};


/*图4-17 建立 一棵空树的例程*/
SearchTree MakeEmpty(SearchTree T)
{
	if( T!= NULL )
	{
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}

/*图4-18 二叉查找树的Find操作*/
Position Find(ElementType X,SearchTree T)
{
	if(T == NULL)
		return NULL;
	if(X < T->Element)
		return Find(X,T->Left);
	else if(X > T->Element)
		return Find(X,T->Right);
	else
		return T;
}


/*图4-19 对二叉查找树的FindMin的递归实现*/
Position FindMin(SearchTree T)
{
	if(T == NULL)
		return NULL;
	else if(T->Left == NULL)
		return T;
	else
		return FindMin(T->Left)
}

/*图4-20 对二叉查找树的FindMax的非递归实现*/
Position FindMax(SearchTree T)
{
	if( T!= NULL)
		while(T->Right != NULL)
			T=T->Right;

	return T;
}

/*图4-22 插入元素到二叉查找树的例程*/
SearchTree Insert(ElementType X, SearchTree T)
{
	if(T == NULL)
	{
		/*Create and return a one-node tree*/
		T = malloc(sizeof(struct TreeNode));
		if(T == NULL)
			FatalError("Out of space!!!");
		else
		{
			T->Element =X;
			T->Left = T->Right = NULL;
		}
	}
	else if(X < T->Element)
		T->Left = Insert( X,T->Left);
	else if(X > T->Element)
		T->Right =Insert(X, T->Right);
	/*Else X is in the tree already,we will do nothing*/

	return T;/*Do not forget this line!!*/
}


/*图4-25 二叉查找树的删除例程*/
SearchTree Delete(ElementType X, SearchTree T)
{
	Position TmpCell;

	if( T==NULL)
		Error("Element not found");
	else if(X < T->Element)/*Go left*/
		T->Left = Delete(X,T->Left);
	else if(X > T->Element)/*Go right*/
		T->Right = Delete(X,T->Left);
	else if(T->Left && T->Right)/*Found element to be deleted */
				/*Two children*/
	{
		/*Replace with smallest in right subtree*/
		TmpCell = FindMin(T->Right);
		T->Element =TmpCell->Element;
		T->Right = Delete(T->Element , T->Right);
	}
	else /*One or zero children*/
	{
		TmpCell = T;
		if(T->Left == NULL)/*Also handles 0 children*/
			T=T->Right;
		else if(T->Right == NULL)
			T=T->Left;
		free(TmpCell);
	}
	return T;
}


/**/