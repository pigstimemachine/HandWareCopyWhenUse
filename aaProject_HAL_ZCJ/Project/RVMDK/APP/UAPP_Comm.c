#include "UAPP_Comm.h"
#include 	"function.h"


/////////////COMM_TASK//////////////////////////////////

void _Comm_TASK1000ms(void)  //1HZ
{

        GetRandChar_test();
}	

void _Comm_TASK1ms(void) //1000HZ
{


   
}	

void _Comm_TASK10ms(void) //100HZ
{

}	

void _Comm_TASK100ms(void) //10HZ
{
    

}	

void _Comm_TASK250ms(void) //4HZ
{
//	 SENSER_VAL SenserData;
//	 _READ_SenserData(&SenserData);//��ȡ�������е�����ѹ�¶ȽǶȵ�ֵ
//   _SEND_SensorValue(SenserData.Temp1,SenserData.TempA,SenserData.AngleX,SenserData.AngleY,SenserData.AngleZ,Hostaddr);//�����¶ȽǶȵ����ݰ�
//   _CodeApply_Task();
}	

void _Comm_TASK5ms(void)
{	 	 
 //  TASK_NRF24L01_AutoSend();

}	
