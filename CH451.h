#ifndef _CH451_H
#define _CH451_H


#define DCLK_H    GPIOB->ODR|=1<<3     //DCLK�ø�  PB3  89 PE3 ��  CH451ʱ���ź�
#define DCLK_L    GPIOB->ODR&=~(1<<3)  //DCLK�õ�
#define DAT_H     GPIOB->ODR|=1<<4     //DAT�ø�   PB4  90 PE2 ��CH451�����ź�
#define DAT_L     GPIOB->ODR&=~(1<<4)  //DAT�õ�
#define LOAD_H    GPIOB->ODR|=1<<5     //LOAD�ø�  PB5  91  ��  CH451������ܣ�LED��Ƭѡ�ź�
#define LOAD_L    GPIOB->ODR&=~(1<<5)  //LOAD�õ�




void ch451_write(unsigned int command);
void ch451_init(void);
void DISP(unsigned int dis_num,unsigned char point);

#endif