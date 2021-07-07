#ifndef _ARRAY_H_
#define _ARRAY_H_


/**

  * @brief   	resizable array 
  * @Name	 	int main Name ( void sth)
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/02/22 Create
  * @Version 	1.0 2020/04/22  
  *		     	1.1 2020/04/22 change sth
  * @Note  
  
  **/
const BLOCK_SIZE = 20 ;
typedef struct 
{
	int *array;
	int size;
}Array;//可变数组


typedef struct _node
{
	int value ;
	struct _node *next ;
}Node;//链表

typedef struct _list
{
	Node* head ;
	Node* tail ;
}List;


Array array_create(int init_size);
void array_free(Array *a);
int array_size(const Array *a);
int* array_at(Array *a ,int index);
void array_inflate(Array *a , int more_size);
void array_set(Array *a , int index ,int value );
#endif
