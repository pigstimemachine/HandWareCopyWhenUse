#include "DataStructuresAndAlgorithmAnalysisInC.h"
/*Euler's constant  r��0.57721566*/










/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	һ���ݹ麯������
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
�ݹ麯����������

1. ��׼���Σ�base case��.������ĳЩ��׼�����Σ����ǲ��õݹ������⡣
2. �����ƽ���making progress����������Щ��Ҫ�ݹ��������Σ��ݹ���ñ����ܹ����Ų�����׼���εķ����ƽ���
3. ��Ʒ���design rule �����������еĵݹ���ö������С�

4.�ϳ�Ч�淨��compound interest rule���������һ�������ͬһʵ��ʱ�������ڲ�ͬ�ĵݹ���������ظ��ԵĹ�����
  */
  /*�����ݹ����*/
int F(int x)
{
	if(x == 0)
		return 0 ;
	else 
		return  2 * F(x-1) + x * x ;
}

/*1-3*����ֹ�ݹ����**/
int Bad( unsigned int N )
{
	if( N == 0 )
		return 0 ;
	else
		return Bad( N / 3 + 1 ) + N - 1 ;
}
/*1-4��ӡ�����ĵݹ�����*/
void PrintOut( unsigned in N )/*print nonnegative N*/
{
	if( N >= 10 )
		PrintOut( N / 10 ) ;
	PrintDigit( N % 10 ) ;
}

/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	������������к�
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note ������һ�����������ļ�������ӣ�Ҳ������ĳ���������ҳ���Ӻ����Ľ���� 
  ����Щ�Ӽ�������λ�ñ����������ģ����Խ�������
	ͼ2-5
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
/*ͼ2-6 �㷨2 ������������к�*/
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
/*ͼ2-7 �㷨3 ������������к�*/
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
/*α���̣�pseudoroutine��Max3 ���� ���������ܵ������е������*/
	return Max3(MaxLeftSum , MaxRightSum , 
				MaxLeftBorderSum + MaxRightBorderSum);
}
int MaxSubSequenceSum(const int A [ ], int N)
{
	return MaxSubSum(A,0,N-1);
}

/*ͼ2-8 �㷨4 ������������к� ����*/
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
  * @Name	 	�Էֲ��� ��ʹ��A=X ���±꣬�������򷵻�-1
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note ĳ�����Ѿ����к�˳��
	ͼ2-9
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
  * @Name	 	ŷ������㷨 �������������������Gcd�� ͬʱ�������ߵ��������
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 
	ͼ2-10
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
  * @Name	 	��Ч�ʵ�ȡ������
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/22/  
  *		     	1.1 2020/07/22/ change sth
  * @Note 
	ͼ2-11
  */

long int Pow(long int X , unsigned int N)
{
	if(N==0)
		return 1;
	if(N==1)
		return X;
	if(IsEven(N))//�Ƿ�Ϊż����
		return Pow(X*X,N/2);
	else
		return Pow(X*X,N/2)*X;//Pow(X,N-1)*X
}


/**
@@@@@@ @ָ����һ�����Դ�������������ַ�ı���@@@@@@@@@@@@@
  * @brief   	how to use it ,and where to use it
  * @Name	 	�����������ͣ�Abstract Data Type , ADT����һЩ�����ļ��ϡ�
  * @param   	None   ����
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/23/  
  *		     	1.1 2020/07/23/ change sth
  * @Note ����union�� ����intersection�� ���С��size�� ȡ�ࣨcomplement���Ȳ���
	//
  */
/*ͼ3-8 Return true if L is empty ����һ�������Ƿ��ǿձ�ĺ��� */
int IsEmpty(List L)
{
	return L->next==NULL;
}
/*ͼ3-9 ���Ե�ǰλ���Ƿ��������ĩβ�ĺ���
Return true if P is the last position in last L 
Parameter L is unused in this implementation*/
int IsLast(Position P, List L)
{
	return P->next==NULL;
}
/*ͼ3-10 Find����
Return Position of X in L ;NULL if not found */
Position Find(ElementType X, List L)
{
	Position P;

	P=L->next;
	while(P!=NULL && P->Element != X)
		P = P->next ;

	return P;
}
/*ͼ3-11 �����ɾ������
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
/*ͼ3-12 FindPrevious----��Deleteһ��ʹ�õ�Find����
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

/*ͼ3-13 ����Ĳ�������
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
/*ͼ3-14 ɾ��һ����Ĳ���ȷ�ķ���
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
/*ͼ3-15 ɾ�������ȷ����
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
  * @Name	 	�����������ͣ�Abstract Data Type , ADT����һЩ�����ļ��ϡ�
  * @param   	None   
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/07/ Create
  * @Version 	1.0 2020/07/25/  
  *		     	1.1 2020/07/25/ change sth
  * @Note 
	//
��� 22w �ȶϢ   957.45*30 �����ܶ� 34.47 ֧����Ϣ   12.47   
					1134.40*30		   40.84			18.84
						
			      1072.10*25          32.16          10.16
			      1241.64*25		  37.25			 15.25
			      
			      1247.83*20		 29.95 			7.95
			      1409.7*20 		 33.83			11.83
		     
  */
/*ͼ3-18 ����ʽADT������ʵ�ֵĽṹ����*/
typedef  struct
{
	int CoeffArray[MaxDegree + 1 ];
	int HighPower;
}*Polynomial;

/*ͼ3-19 ������ʽ��ʼ��Ϊ��Ĺ���*/
void ZeroPolynomial( Polynomial Poly)
{
	int i;

	for(i=0;i<=MaxDegree;i++)
		Poly->CoeffArray[i]=0;
	Poly->HighPower =0;
}
/*Dynamic Link Library���Զ�̬���ӿ�*/
/*ͼ3-20 ��������ʽ��ӵĹ���*/
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

/*ͼ3-21 ��������ʽ��˵Ĺ���*/
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


/*ͼ3-23 ����ʽADT����ʵ�ֵ���������*/
typedef struct Node *PtrToNode;

struct Node
{
	int Coefficient;
	int Exponent;
	PtrToNode Next;
};
typedef PtrToNode Polynomial;/*Nodes sorted by exponent*/


/*ͼ3-28 �����α��ʵ������*/
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

/*ͼ3-30 ���̣�CursorAlloc��CursorFree*/
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



/*ͼ3-32 ����һ�������Ƿ�Ϊ�յĺ���--�α�ʵ��*/
/*Return true if L is empty*/
int IsEmpty(List L)
{
	return CursorSpace[L].Next == 0 ;
}


/*ͼ3-33 ����P�Ƿ��������ĩβ�ĺ���---�α�ʵ��
Return true if P is the last position in list L 
Parameter L is unused in this implementation*/
int IsLast(Position P, List L)
{
	return CursorSpace[p].Next == 0;
}


/*ͼ3-34 ����Find --�α�ʵ��
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


/*ͼ3-35 ���������ɾ������������Delete--�α�ʵ��
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


/*ͼ3-36 ��������в������������Insert--�α�ʵ��
Insert (after legal position P)
Header implementation assmed 
Parameter L is unused in this implementation*/
void Insert(ElementType X, List L, Position P)
{
	Position TmpCell;
	TmpCell = CursorAlloc();
	if( TmpCell == 0 )
		FatalError("Out of space������");

	CursorSpace[TmpCell].Element = X;
	CursorSpace[TmpCell].Next = CursorSpace[P].Next;
	CursorSpace[P].Next = TmpCell;
}


/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	ջADT
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*ͼ3-39 ջADT����ʵ�ֵ���������*/
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

/*ͼ3-40 ����ջ�Ƿ��ǿ�ջ������--����ʵ��*/
int IsEmpty(Stack S)
{
	return S->Next == NULL;
}


/*ͼ3-41 ����һ����ջ������--����ʵ��*/
Stack CreatStack(void)
{
	Stack S;

	S =malloc(sizeof(struct Node));
	if(S == NULL )
		FatalError("Out of space!!!");//��������
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


/*ͼ3-42 Ԫ�ؽ�ջ������--����ʵ��*/
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


/*ͼ3-43 ����ջ��Ԫ�ص�����--����ʵ��*/
ElementType Top(Stack S)
{
	if(!IsEmpty(S))
		return S->Next->Element;
	Error("Empty stack");
	return 0;/*Return value used to avoid warning*/
}


/*ͼ3-44 ��ջ����Ԫ�ص�����--����ʵ��*/
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


/*ͼ3-45 ջ������--����ʵ��*/
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


/*ͼ3-46 ջ�Ĵ���--����ʵ��*/
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


/*ͼ3-47 �ͷ�ջ�Ĺ���--����ʵ��*/
void DisposeStack(Stack S)
{
	if(S != NULL )
	{
		free(S->Array);
		free(S);
	}
}


/*ͼ3-48 ���ջ�Ƿ��ǿ�ջ������--����ʵ��*/
int IsEmpty(Stack S)
{
	return S->TopOfStack == EmptyTOS;
}


/*ͼ3-49 ����һ����ջ������--����ʵ��*/
void MakeEmpty(Stack S)
{
	S->TopOfStack = EmptyTOS;
}

/*ͼ3-50 Ԫ�ؽ�ջ������--����ʵ��*/
void Push(ElementType X, Stack S)
{
	if(IsFull(S))
		Error("Full stack");
	else
		S->Array[ ++S->TopOfStack ] = X; 
}

/*ͼ3-51 ����ջ��Ԫ�ص�����--����ʵ��*/
ElementType Top(Stack S)
{
	if(!IsEmpty(S))
		return S->Array[ S->TopOfStack ] ;
	Error("Empty stack");
	return 0 ; /*Return value used to avoid warning*/
}


/*ͼ3-52 ��ջ����Ԫ�ص�����--����ʵ��*/
void Pop(Stack S)
{
	if(IsEmpty(S))
		Error("Empty stack");
	else
		S->TopOfStack--;
}

/*ͼ3-53 ����ջ��Ԫ�ز���ջ����������--����ʵ��*/
ElementType TopAndPop(Stack S)
{
	if(!IsEmpty(S))
		return S->Array[ S->TopOfStack-- ];
	Error("Empty stack");
	return 0;/*Return value used to avoid warning */
}


/*ͼ3-54 �ݹ�Ĳ���ʹ�ã���ӡһ������
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


/*ͼ3-55 ���õݹ��ӡһ�������������������������㲻Ҫȥ����
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
  * @Name	 	����ADT
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/28 
  *		     	1.1 2020/07/28 change sth
  * @Note  
  
  **/
  /*ͼ3-57 ���е���������*/
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

/*ͼ3-58 ���Զ����Ƿ�Ϊ�յ�����--����ʵ��*/
int IsEmpty(Queue Q)
{
	return Q->Size == 0 ;
}

/*ͼ3-59 ����ն��е�����--����ʵ��*/
void MakeEmpty(Queue Q)
{
	Q->Size = 0;
	Q->Front = 1;
	Q->Rear = 0 ;
}

/*ͼ3-60 ��ӵ�����--����ʵ��*/
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
		Q->Rear = Succ(Q->Rear��  Q);
		Q->Array[Q->Rear] = X;
	}
}




/**

  * @brief   	how to use it ,and where to use it
  * @Name	 	��tree ����treelike
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/29  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*ͼ4-3 ���Ľڵ�����*/
typedef struct TreeNode *PtrToNode;

struct TreeNode
{
	ElementType Element;
	PtrToNode FirstChild;
	PtrToNode NextSibling
}



/*ͼ4-6 �г��ּ��ļ�ϵͳ��Ŀ¼������*/
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


/*ͼ4-9 ����һ��Ŀ¼��С������*/
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

/*ͼ4-13 �������ڵ�����*/
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
  * @Name	 	������ADT ���������
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/06/ Create
  * @Version 	1.0 2020/07/29  
  *		     	1.1 2020/07/ change sth
  * @Note  
  
  **/
/*ͼ4-16 �������������*/
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


/*ͼ4-17 ���� һ�ÿ���������*/
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

/*ͼ4-18 �����������Find����*/
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


/*ͼ4-19 �Զ����������FindMin�ĵݹ�ʵ��*/
Position FindMin(SearchTree T)
{
	if(T == NULL)
		return NULL;
	else if(T->Left == NULL)
		return T;
	else
		return FindMin(T->Left)
}

/*ͼ4-20 �Զ����������FindMax�ķǵݹ�ʵ��*/
Position FindMax(SearchTree T)
{
	if( T!= NULL)
		while(T->Right != NULL)
			T=T->Right;

	return T;
}

/*ͼ4-22 ����Ԫ�ص����������������*/
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


/*ͼ4-25 �����������ɾ������*/
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