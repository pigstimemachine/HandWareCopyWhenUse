#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#define MAX(a,b)	(a>b ? a:b )
#define MIN(a,b)	(a<b ? a:b )

#define CONNECT(a,b)	a##b //表示连接a b 如 n=connect(123,456) n=123456 cha* str=connect("abcd","efg") str="abcdefg"
//#define	TOCHAR(a)		#@a //单引号包括参数a，返回一个字符 char* ch1 =TOCHAR(1) ch1 = '1' 只有单字符有用
#define	TOSTRING(a)		#a //双引号包括参数a，返回一个字符串 char* str =TOSTRING(123) str1="123"

#define	MEM_B(x)		( *( (byte *) (x) ) ) //得到指定地址上的一个字节值  typedef unsigned char byte;
#define	MEM_W(x)		( *( (word *) (x) ) ) //得到指定地址上的一个字值 word一般叫作“字”：系统硬件有关，数据总线为16位，则1word为2byte；32位时，1word为4byte；

#define OFFSETOF(type ,field) ((size_t) & ((type *) 0) ->field ) //得到一个field在结构体中的偏移量
#define	FSIZ(type , field )	sizeof( ( (type *) 0) ->field ) //得到一个结构体中field所占用的字节数
#define B_PTR(var)	( (byte *) (void *) &(var)) //得到一个变量的地址
#define W_PTR(var)	( (word *) (void *) &(var)) //得到一个变量的宽度

#define	UPCASE(c)	( ( (c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) ) //将一个字母转换为大写  思考 如何将大写转换为小写

#define INC_SAT(val)	(val = ((val) + 1 > (val)) ? (val) +1 : (val)) //防止溢出的一个办法

#define	ARR_SIZE(a)		(sizeof((a)) / sizeof((a[0]))) //返回数组元素的个数

#define DECCHK( c ) ((c) >= ''0'' && (c) <= ''9'')	//判断字符是不是10进值的数字
#define HEXCHK( c ) ( ((c) >= ''0'' && (c) <= ''9'') ||((c) >= ''A'' && (c) <= ''F'') ||((c) >= ''a'' && (c) <= ''f'') )  //8 判断字符是不是16进值的数字




int GetRandChar_test();
void GetRandChar(char * buff, int n);
int GetCurTime(void);
char* del_Enter( char* qrcode );
int del_useless(char *src);

int del_str(char* qrcode ,char* need_del_char);




#endif










