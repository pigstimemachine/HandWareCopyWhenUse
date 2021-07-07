#include 	"function.h"
#include	"stdlib.h"
#include	"stdio.h"




 /**
暂未使用过该函数 该函数至少应该增加一个while循环来清楚所有相同的字符串
  * @brief   	删除删除字符串中的某个字符串或者字符， 该函数只能删除一个字符串 一般不使用。 
  * @Name	 	 int del_substr(char *str,char const *substr)
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/02/22 Create
  * @Version 	1.0 2020/04/27  
  *		     	1.1 2020/04/27 change sth
  * @Note   qrcode:#04HDownANGDownONGDownDDDownADDFAS#
  
  **/
 int del_substr(char *str,char const *substr)
 {
	 int len = strlen(str);
	 int sublen = strlen(substr);
	 char *string = str;
	 int i;
	 int j;
	 int tmp=0;
	 int flag = 0;
	 for(i = 0;i<len;i++)
	 {
		 tmp = 0;
		 if(*str == *substr)			//如果有发现相同的字符则进行字符串比对
		 {//该处比对只针对单个字符
			  for(j = 0;j<sublen;j++)
			  {
					if(*str == *substr)
				  {
					  str++;
					  substr++;
					  tmp++;
				  }
				  else
				  {
					  substr-=tmp;
					  str -=tmp;
					  break;
				  }
			  }
		 }
		  if(tmp == sublen)
		  {  
			 str -= tmp; 
			 char *p = str;
			 p += tmp;
					 while(*str != NULL)  
			 {
				 *str++ = *p++;
				 flag++;
			 }
			 break;
		  }
		 else
		 {
			 str++;
		 }
	 }	 
	 if(i==len)
	 {
		 return 0;
	 }
	 if(j==sublen)
	 {
		 str = string;	
		 return 1;
	 }
 
 
 }

 /**

  * @brief   	删除字符串中的某个字符串或者字符，并返回该指针。
  * @Name	 	  int del_str(char* qrcode ,char* need_del_char)
  * @param   	qrcode 输入     need_del_char准备删除的字符串
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/02/22 Create
  * @Version 	1.0 2020/04/27  
  *		     	1.1 2020/04/27 change sth
  * @Note   测试多种情况，目前暂未发现该函数的bug
  
  **/
 int del_str(char* qrcode ,char* need_del_char)
 {
	char *p;	
	char temp[1024];
	PX("the input qrcode=%s\n" , qrcode);
	while((p=strstr(qrcode,need_del_char)) != NULL) 
	{ //在s1和s2字符中找不到相同元素时停止，strstr字符串中招字符串 
		*p = '\0';//把s1自与s2相等的头处改为\0,截断 
		strcpy(temp , p+strlen(need_del_char));//把p向后延到s1与s2相等的那段末尾处拷贝到	strcpy拷贝字符串 
		strcat(qrcode, temp );//把temp 接在s1后面	  
	} 
	//PX("the final qrcode=%s\n" , qrcode);
	return 0; 
 }


 
 /***********************************************************************
  *函数名称:del_useless()
  *输入参数:						 
  *返回参数:
  *函数功能：删除单个字符
  *函数说明：
  *备	  注:20191127 ZCJ
  ************************************************************************/
 int del_useless(char *src)
 {	  
	 char *pTmp = src;	  
	 unsigned int iSpace = 0;	  
	 while(*src != '\0') 
	 {		  
		 if(*src != 0x20 && *src != 0x0D && *src != 0x0A ) //0x20空格 0x0D回车	0X0A换行
		 {			 
			 *pTmp++ = *src; //  *pTmp = src；*pTmp++；
		 } 
		 else
		 {			 
			 iSpace++;		  
		 }		   
		 src++;    
	 }	   
	 *pTmp = '\0';	   
	 return iSpace;
 }


 /**

  * @brief   	删除回车符Down 删除字符串中的某个字符串或者字符，并返回删除后正确的字符串。
  * @Name	 	 int del_Enter( char* qrcode	)
  * @param   	None
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/02/22 Create
  * @Version 	1.0 2020/02/22  
  *		     	1.1 2020/02/22 change sth
  * @Note   qrcode:#04HDownANGDownONGDownDDDownADDFAS#
  
  **/
  
 char* del_Enter( char* qrcode )
 {		 
	char *subString;	
	char *subStringNext; 
	char *subStringNext1;
	char newcode[1024] = { 0} ;
	char *finalcode;
	char i = 0; 
	char SUBSTRING[20];
	subString = qrcode;	
//	newcode = malloc(sizeof(char)*1024);
	finalcode = malloc(sizeof(char)*1024);
	subStringNext =  strstr(subString, "Down");
	 while( subStringNext != NULL)
	{
		
		subStringNext = strstr(subString, "Down");
		if( subStringNext  != NULL )
		{
		//	printx("subStringNext :%s\n" , subStringNext);	
			subStringNext = subStringNext +  4 ;	
		}
		else
		{
			qrcode = finalcode ;
			PX("qrcode=%s\n" , qrcode);	
			free(finalcode);
			return qrcode;		
		}
		memset(newcode , 0 , sizeof(newcode));
	//	memset(finalcode , 0 , sizeof(finalcode));
		printx("subString :%s\n" , subString);
		printx("subStringNext :%s\n" , subStringNext);			
		PX("PsubString=%p PsubStringNext=%p\n" , subString ,subStringNext);
		memcpy(newcode , subString ,  abs((subStringNext)  - (subString))  -4);
		sprintf(finalcode , "%s%s" , newcode ,subStringNext );
		PX("newcode=%s\n" , newcode );
		PX("finalcode=%s\n\n" , finalcode );	
		subString =  finalcode ;
	}
	 PX("NO ENTER,ALL IS WELL!\n");
	 return qrcode ;
//	return 0;  
 }

/**

  * @brief   	获取随机字符串 包括大小写字母和数字
  * @Name	 	void GetRandChar(char * buff, int n)
  * @param   	char * buff 存储到该数组或者指针中    				int n 字符数量为n-1
  * @retval  	None
  * @Author  	ZCJ
  * @Date 	 	2020/04/23 Create
  * @Version 	1.0 2020/04/23  
  * @Note  
  

  **/

void GetRandChar(char * buff, int n)
{
	int i = 0 ;
    char metachar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
//    srand(time(NULL));
/*/c语言中语句srand( (time(NULL) ) ;
表示设置一个随机种子，每次运行都能保证随机种子不同。
部分软件不支持，支持的可以加上，确保更好的随机性
*/
    for( i = 0; i < n - 1; i++)
    {
        buff[i] = metachar[rand() % 62]; 
    }
    buff[n - 1] = '\0';
}
 int GetRandChar_test()
{
    char r[10];
    GetRandChar(r, 10);
    printf("GetRandChar=%s\n", r);
}

 /**
 
   * @brief 	 获取当前时间 
   * @Name		 int GetCurTime(void)
   * @param 	 
   * @retval	 None
   * @Author	 ZCJ
   * @Date		 2020/04/24 Create
   * @Version	 1.0 2020/04/24  
   * @Note	
 /*  
 #ifndef _TM_DEFINED
struct tm {
int tm_sec; //* 秒 – 取值区间为[0,59] 
int tm_min; //* 分 - 取值区间为[0,59] 
int tm_hour; //* 时 - 取值区间为[0,23] *
int tm_mday; //* 一个月中的日期 - 取值区间为[1,31] *
int tm_mon; //* 月份（从一月开始，0代表一月） - 取值区间为[0,11] *
int tm_year; //* 年份，其值等于实际年份减去1900 *
int tm_wday; //* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 *
int tm_yday; //* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 *
int tm_isdst; //* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的时候，tm_isdst为0；不了解情况时，tm_isdst()为负。*
long int tm_gmtoff; //*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*
const char *tm_zone; //*当前时区的名字(与环境变量TZ有关)*
};

#define _TM_DEFINED
#endif
   */
 int GetCurTime(void)
 
 {
	 struct tm *ptr;
	 time_t lt;
	 lt =time(NULL);
	 ptr=localtime(&lt);
	 /*
	 printf("second:%d\n",ptr->tm_sec);
	 printf("minute:%d\n",ptr->tm_min);
	 printf("hour:%d\n",ptr->tm_hour);
	 printf("mday:%d\n",ptr->tm_mday);
	 printf("month:%d\n",ptr->tm_mon+1);
	 */
	 printf("TIME IS:%d-%d-%d-%d-%d-%d\n", ptr->tm_year+1900 ,ptr->tm_mon+1 , ptr->tm_mday, ptr->tm_hour, ptr->tm_min , ptr->tm_sec);
	 return 0;
 }


