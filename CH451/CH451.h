#ifndef _CH451_H
#define _CH451_H


#define DCLK_H    GPIOB->ODR|=1<<3     //DCLK置高  PB3  89 PE3 右  CH451时钟信号
#define DCLK_L    GPIOB->ODR&=~(1<<3)  //DCLK置低
#define DAT_H     GPIOB->ODR|=1<<4     //DAT置高   PB4  90 PE2 中CH451数据信号
#define DAT_L     GPIOB->ODR&=~(1<<4)  //DAT置低
#define LOAD_H    GPIOB->ODR|=1<<5     //LOAD置高  PB5  91  左  CH451（数码管，LED）片选信号
#define LOAD_L    GPIOB->ODR&=~(1<<5)  //LOAD置低




void ch451_write(unsigned int command);
void ch451_init(void);
void DISP(unsigned int dis_num,unsigned char point);

#endif