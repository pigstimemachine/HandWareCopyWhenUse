;CH451 PIC系列单片机演示程序
;USE PIC16XX
;Web http//www.winchiphead.com
;Author YHW  2003.8
;*************************************************************
;需要主程序定义的参数
CH451_DCLK    EQU     1		;串行数据时钟线，上升沿激活
CH451_DIN     EQU     2		;串行数据输出，按451数据加载
CH451_LOAD    EQU     3		;串行命令加载，上升沿激活
CH451_DOUT    EQU     0		;INT0，键盘中断和键值数据输入，按451的数据传输
CH451_COM1    EQU    20H	;451数据底8位数据
CH451_COM2    EQU    21H	;451数据高8位
CH451_CON     EQU    22H	;数据加载计数器
CH451_KEY     EQU    24H	;中断451键值存放

CH451_READ1    EQU    23H	;读出的451键值
;STATUS	      EQU    03H
;OPTION	      EQU    81H
;TRISA	      EQU    85H
;TRISB         EQU    86H
PA	      EQU    05H
PB	      EQU    06H
;INTCON        EQU    0BH
;RP0	      EQU    05H
;
;***********************************************************
;主程序
		ORG   00H
		GOTO	STARTE
		ORG	04H
		GOTO	CH451_INTER
		ORG	05H
STARTE:		NOP
		BSF  STATUS,RP0
		MOVLW    01H
 		MOVWF    TRISB
		BCF  STATUS,RP0
		MOVLW    0FFH
                MOVWF    CH451_KEY
;		............
		CALL	CH451_INIT
		MOVLW	3H
		MOVWF	CH451_COM1
		MOVLW	4H
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	80H
		MOVWF	CH451_COM1
		MOVLW	05H
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	1H
		MOVWF	CH451_COM1
		MOVLW	08H
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	2H
		MOVWF	CH451_COM1
		MOVLW	09H
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	3H
		MOVWF	CH451_COM1
		MOVLW	0AH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	4H
		MOVWF	CH451_COM1
		MOVLW	0BH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	5H
		MOVWF	CH451_COM1
		MOVLW	0CH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	6H
		MOVWF	CH451_COM1
		MOVLW	0DH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	7H
		MOVWF	CH451_COM1
		MOVLW	0EH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
                MOVLW	8H
		MOVWF	CH451_COM1
		MOVLW	0FH
		MOVWF	CH451_COM2
		CALL	CH451_WRITE
               BCF     INTCON,1
                BSF     INTCON,7
                MOVLW   00H
                MOVWF   29H

X6:            BSF      PB,5
                MOVF    CH451_KEY,0
                   BSF     PB,6
                BCF     STATUS,0
                SUBLW   0FEH
                BTFSS   STATUS,0
                GOTO    X6
X5:             MOVF   CH451_KEY,0

                ANDLW  3FH
                MOVWF  28H
                MOVLW   0FFH
                MOVWF   CH451_KEY
                INCF    29H
               ; MOVF    30H,0
               MOVF     29H,0
               MOVWF     CH451_COM1
                MOVLW   08H
                MOVWF   CH451_COM2
                CALL    CH451_WRITE
                 ;GOTO   X6
                 
                 MOVF    28H,0
                 ANDLW   0FH
                 ;SUBLW   080H
                 MOVWF   CH451_COM1
                 MOVLW   09H
                 MOVWF   CH451_COM2
                 CALL    CH451_WRITE
                 MOVF   28H,0

                 ANDLW   0F0H
                 MOVWF   2AH
                 SWAPF   2AH,0
                 MOVWF   CH451_COM1
                 MOVLW   0AH
                 MOVWF   CH451_COM2
                 CALL    CH451_WRITE
                ;GOTO    X6

               MOVF    28H,0
               ;SUBLW   40H
              ; MOVWF   25H
               CLRF   27H
               ;MOVF    25H,0

X7:            BCF     STATUS,0
                INCF    27H
                BCF     PB,5
                MOVF    28H,0
                MOVWF   26H
                MOVLW   0AH
                SUBWF   28H,1
                BTFSc   STATUS,0
                GOTO    X7
                MOVF    26H,0
                MOVWF   CH451_COM1
                MOVLW   0DH
                MOVWF   CH451_COM2
                CALL    CH451_WRITE
                DECF    27H,0

                ;BCF     STATUS,0

                MOVWF   CH451_COM1
                MOVLW   0EH
                MOVWF   CH451_COM2
                CALL    CH451_WRITE

               BCF     PB,4
              GOTO    X6
;
;************************************************************
;CH451初始化程序
CH451_INIT:	MOVLW	01H
		BSF	STATUS,5          ;选择体1
		MOVWF	TRISB
		BCF	STATUS,5
		BCF	PB,CH451_DIN      ;先高后低通知451选择四线传输
		BSF	PB,CH451_DCLK     ;置为高电平
		BSF	PB,CH451_DIN
		BSF	PB,CH451_LOAD
		BSF	PB,CH451_DOUT
;下面是开启中断指令
                BSF	STATUS,5	  ;选择体1
		BCF   81H,6		  ;设置下降沿有效
		BCF	STATUS,5
		BSF	INTCON,4	  ;INT0使能
               BCF	INTCON,1	  ;清中断标志
                RETURN
;*************************************************************
;CH451写入子程序
;USE W COM1 COM2 CON
CH451_WRITE:	BCF	INTCON,4	;关INT0中断
		BCF	PB,CH451_LOAD	;命令开始，此句也可以放在后面
		MOVLW	8H
		MOVWF	CH451_CON	;将低8位送入
CH451_WRITE_0:		RRF	CH451_COM1	;低位在前，高位在后
               BCF	PB,CH451_DCLK
		BTFSC	STATUS,0		;判断送入位高低电平
		GOTO	CH451_WRITE_1		;为高跳转
		GOTO	CH451_WRITE_2		;为低跳转
CH451_WRITE_1:
		BSF	PB,CH451_DIN		;送入高电平
		GOTO	CH451_WRITE_3
CH451_WRITE_2:
		BCF	PB,CH451_DIN		;送入低电平
CH451_WRITE_3:		BSF	PB,CH451_DCLK	;产生上升沿通知451输入位数据
                DECFSZ	CH451_CON		 ;未送完继续
		GOTO	CH451_WRITE_0
		MOVLW	04H			;送入低四位
		MOVWF	CH451_CON
CH451_WRITE_4:		RRF	CH451_COM2	;低位在前，高位在后
		BCF	PB,CH451_DCLK
		BTFSC	STATUS,0		;判断送入位高低电平
		GOTO	CH451_WRITE_5		;为高转移
		GOTO	CH451_WRITE_6		 ;为低转移
CH451_WRITE_5:
		BSF	PB,CH451_DIN		;送入高电平
		GOTO	CH451_WRITE_7
CH451_WRITE_6:	BCF	PB,CH451_DIN		;送入低电平

CH451_WRITE_7:	BSF	PB,CH451_DCLK
		DECFSZ	CH451_CON
		GOTO	CH451_WRITE_4
		BSF	PB,CH451_LOAD		;加载数据
		BSF	INTCON,4		 ;开中断
		RETURN
;**************************************************************
;CH451读入子程序
;
CH451_READ:
           	BCF	INTCON,4
		BCF	PB,CH451_LOAD		;命令开始
		MOVLW	03H			;写入控制字
		MOVWF	CH451_CON
CH451_READ_1:	BCF	PB,CH451_DCLK
		BSF	PB,CH451_DIN		;输入1
		BSF	PB,CH451_DCLK
		DECFSZ	CH451_CON
		GOTO	CH451_READ_1
		BCF	PB,CH451_DCLK
		BCF	PB,CH451_DIN		;输入0
		BSF	PB,CH451_DCLK
		BSF	PB,CH451_LOAD		;加载命令字
		MOVLW	07H			;读出7位
		MOVWF	CH451_CON
CH451_READ_4:		BTFSS	PB,CH451_DOUT	;判断CH451_DOUT高低电平
		GOTO	CH451_READ_2		;低电平跳转
		GOTO	CH451_READ_3		;高电平跳转
CH451_READ_2:		BCF	STATUS,0	;置C为0
			GOTO	CH451_READ_5
CH451_READ_3:	BSF	STATUS,0
CH451_READ_5:	BCF	PB,CH451_DCLK		;下降沿通知451送出下一组数据
		RLF	CH451_READ		;读进寄存器
		BSF	PB,CH451_DCLK		;置为
		DECFSZ	CH451_CON		;判断七组数据是否接受完
		GOTO	CH451_READ_4
		BCF     INTCON,1		;清中断标志
		BSF	INTCON,4		;开中断
		RETURN				;不带参数反回
;***********************************************************
;键盘中断程序
CH451_INTER:
		BTFSS	INTCON,1		;判断是否键盘中断
		GOTO	CH451_INTER_NO		 ;非键盘中断眺出
		BCF	PB,CH451_LOAD		;命令开始
	;	送入读控制字
		MOVLW	03H			;送入3位1
		MOVWF	CH451_CON
CH451_INTER_1:	BCF	PB,CH451_DCLK
		BSF	PB,CH451_DIN		;送入1
		BSF	PB,CH451_DCLK		;上升沿通知接收
		DECFSZ	CH451_CON
		GOTO	CH451_INTER_1
		BCF	PB,CH451_DCLK		 ;送入一位0
		BCF	PB,CH451_DIN
		BSF	PB,CH451_DCLK		;
		BSF	PB,CH451_LOAD		;加载数据
		MOVLW	07H			;读进七位
		MOVWF	CH451_CON
CH451_INTER_4:	BTFSS	PB,CH451_DOUT		;判断读出是低是高
		GOTO	CH451_INTER_2		;低转移
		GOTO	CH451_INTER_3		;高转移
CH451_INTER_2:		BCF	STATUS,0	;标志位置0
			GOTO	CH451_INTER_5
CH451_INTER_3:	BSF	STATUS,0		;标志位置1
CH451_INTER_5:	BCF	PB,CH451_DCLK		;产生下降沿通知451输出下一位
		RLF	CH451_KEY		;接收键值
		BSF	PB,CH451_DCLK
		DECFSZ	CH451_CON		;数据接收完眺一步
		GOTO	CH451_INTER_4
       		 BCF     INTCON,1		;清中断标志
CH451_INTER_NO:    NOP
		RETFIE

 END

