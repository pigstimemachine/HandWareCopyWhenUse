; CH451
; 4-Wire Interface Program for MCS-51
;
; Website:  http://winchiphead.com
; Email:    tech@winchiphead.com
; Author:   W.ch 2003.03
;
;****************************************************************************
;需要主程序定义的参数
;CH451_DCLK		BIT   P1.7             ;串行数据时钟,上升沿激活
;CH451_DIN		BIT   P1.6             ;串行数据输出,接CH451的数据输入
;CH451_LOAD		BIT   P1.5             ;串行命令加载,上升沿激活
;CH451_DOUT		BIT   P3.2             ;INT0,键盘中断和键值数据输入,接CH451的数据输出
;CH451_KEY		DATA  7FH              ;存放键盘中断中读取的键值
;****************************************************************************
;
;对外接口子程序
;
;  1. CH451_INIT     初始化子程序
;
;  2. CH451_WRITE    输出命令子程序
;       入口: B-ACC 待写的12位数据, 低8位在ACC中, 高4位在B的低4位中
;       使用: ACC, B, R7
;
;  3. CH451_READ     输入键值子程序
;       出口: ACC 键值数据
;       使用: ACC, R7
;
;  4. CH451_INTER    键盘中断子程序
;       使用: 堆栈4字节
;
;  如果使用CH451的键盘中断, 强烈建议在子程序CH451_WRITE和CH451_READ中
;  添加屏蔽键盘中断的代码, 以防止在子程序运行过程中被键盘中断
;
;****************************************************************************
;主程序的例子
;				ORG   0000H            ;复位后单片机入口
;				LJMP  START
;				ORG   0003H            ;键盘中断
;				LJMP  CH451_INTER
;
;START:			...
;				CALL  CH451_INIT       ;初始化
;				...
;				SETB  EA               ;允许中断
;				...
;				MOV   B,#04H           ;设置系统参数命令
;				MOV   A,#03H           ;禁止看门狗,使能键盘功能,使能显示驱动
;				CALL  CH451_WRITE      ;发出命令
;				...
;				MOV   B,#09H           ;加载字数据到DIG1对应的数据寄存器
;				MOV   A,#5BH           ;字数据,如果是数码管,则第二个数码管显示"2"
;				CALL  CH451_WRITE      ;发出命令
;				...
;WAIT_KEY:		MOV   A,#0FFH          ;无效按键代码
;				XCH   A,CH451_KEY      ;取键盘中断获取的键值,同时置为无效值
;				MOV   R6,A             ;键值
;				XRL   A,#0FFH          ;有键按下?
;				JZ    WAIT_KEY         ;还没有检测到有效按键,继续等待
;				CJNE  R6,??,...        ;判别按键
;				...
;
;;				CALL  CH451_READ       ;重新读取最近一次有效按键的键值
;				...
;
;
;****************************************************************************
;
;
; 初始化子程序
CH451_INIT:		CLR   CH451_DIN        ;先低后高,输出上升沿通知CH451选择4线串行接口
				SETB  CH451_DCLK       ;置为默认的高电平
				SETB  CH451_DIN
				SETB  CH451_LOAD
				SETB  CH451_DOUT       ;置为输入
; 下述三条指令用于键盘中断
;				CLR   IT0              ;置外部信号为低电平触发
;				SETB  PX0              ;置高优先级或者低优先级
;				CLR   IE0              ;清中断标志
;				SETB  EX0              ;允许键盘中断
				RET
;
;
; 输出命令子程序
; ENTRY: B-ACC 待写的12位数据, 低8位在ACC中, 高4位在B的低4位中
; USE:   ACC, B, R7
CH451_WRITE:	;CLR   EX0              ;禁止键盘中断
				CLR   CH451_LOAD       ;命令开始,此命令可以放在后面
				MOV   R7,#08H          ;将ACC中8位送出
CH451_WRITE_8:	RRC   A                ;低位在前,高位在后
				CLR   CH451_DCLK
				MOV   CH451_DIN,C      ;送出一位数据
				SETB  CH451_DCLK       ;产生时钟上升沿通知CH451输入位数据
				DJNZ  R7,CH451_WRITE_8 ;位数据未完继续
				MOV   A,B
				MOV   R7,#04H          ;将B中4位送出
CH451_WRITE_4:	RRC   A                ;低位在前,高位在后
				CLR   CH451_DCLK
				MOV   CH451_DIN,C      ;送出一位数据
				SETB  CH451_DCLK       ;产生时钟上升沿通知CH451输入位数据
				DJNZ  R7,CH451_WRITE_4 ;位数据未完继续
				SETB  CH451_LOAD       ;产生加载上升沿通知CH451处理命令数据
;				SETB  EX0              ;允许键盘中断
				RET
;
;
; 输入键值子程序
; EXIT:  ACC 键值数据
; USE:   ACC, R7
CH451_READ:		;CLR   EX0              ;禁止键盘中断
				CLR   CH451_LOAD       ;命令开始
				MOV   A,#07H           ;读取键值命令的高4位0111B
				MOV   R7,#04H          ;忽略12位命令的低8位
CH451_READ_4:	RRC   A                ;低位在前,高位在后
				CLR   CH451_DCLK
				MOV   CH451_DIN,C      ;送出一位数据
				SETB  CH451_DCLK       ;产生时钟上升沿锁通知CH451输入位数据
				DJNZ  R7,CH451_READ_4  ;位数据未完继续
				SETB  CH451_LOAD       ;产生加载上升沿通知CH451处理命令数据
				CLR   A                ;先清除键值单元以便移位
				MOV   R7,#07H          ;读入7位键值
CH451_READ_7:	MOV   C,CH451_DOUT     ;读入一位数据
				CLR   CH451_DCLK       ;产生时钟下升沿通知CH451输出下一位
				RLC   A                ;数据移入ACC,高位在前,低位在后
				SETB  CH451_DCLK
				DJNZ  R7,CH451_READ_7  ;位数据未完继续
;				CLR   IE0              ;清中断标志,读操作过程中有低电平脉冲
;				SETB  EX0              ;允许键盘中断
				RET
;
;
; 键盘中断子程序
; USE:   堆栈4字节
CH451_INTER:	PUSH  PSW              ;现场保护
				PUSH  ACC
				CLR   CH451_LOAD       ;命令开始
				MOV   A,#0F7H          ;读键值命令,忽略12位命令的低8位,高4位用作结束标志
CH451_INTER_4:	SETB  C                ;在高位添0以检测位数据结束
				RRC   A                ;低位在前,高位在后
				CLR   CH451_DCLK
				MOV   CH451_DIN,C      ;送出一位数据
				SETB  CH451_DCLK       ;产生时钟上升沿锁通知CH451输入位数据
				CJNE  A,#0FFH,CH451_INTER_4 ;位数据未完继续,共4位,完成后为0FFH
				SETB  CH451_LOAD       ;产生加载上升沿通知CH451处理命令数据
				MOV   A,#0FCH          ;该数据用以检测位数据结束
CH451_INTER_7:	MOV   C,CH451_DOUT     ;读入一位数据
				CLR   CH451_DCLK       ;产生时钟下升沿通知CH451输出下一位
				RLC   A                ;数据移入ACC,高位在前,低位在后
				SETB  CH451_DCLK
				JC    CH451_INTER_7    ;位数据未完继续,共7位,完成后才移出0
				MOV   CH451_KEY,A      ;保存键值
				POP   ACC
				POP   PSW
				CLR   IE0              ;清中断标志,该指令需根据实际情况作修改
				RETI                   ;中断返回
;
;****************************************************************************
;
