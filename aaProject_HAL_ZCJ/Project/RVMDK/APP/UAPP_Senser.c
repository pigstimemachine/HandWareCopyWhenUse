#include "lib.h"

SENSER_VAL   SenserVal,SenserOld;

short filter_1(short NEW_DATA,short OLD_DATA,short k)   
{
    long result;
    if(NEW_DATA<OLD_DATA)
    {
        result=OLD_DATA-NEW_DATA;
        result=result*k;
        result=result+128;
        result=result>>8;
        result=OLD_DATA-result;
    }
    else if(NEW_DATA>OLD_DATA)
    {
        result=NEW_DATA-OLD_DATA;
        result=result*k;
        result=result+128;
        result=result>>8;
        result=OLD_DATA+result;
    }
    else result=OLD_DATA;
    return((short)result);
}

void _SenserFilter_Task(void)    //10HZ
{
	  short Tempdata1,TempdataA,AngleXdata,AngleYdata,AngleZdata,Currdata,Voltdata;
	  Tempdata1=_READ_Tempdata1();
    TempdataA=_READ_TempdataA();	
	  AngleXdata=Get_Angle_Average(1,Angx);
	  AngleYdata=Get_Angle_Average(1,Angy);
	  AngleZdata=Get_Angle_Average(1,Angz);
    SenserVal.AngleX= filter_1(AngleXdata,SenserOld.AngleX,Filternum);
	  SenserOld.AngleX=AngleXdata;
	  SenserVal.AngleY= filter_1(AngleYdata,SenserOld.AngleY,Filternum);
	  SenserOld.AngleY=AngleYdata;
	  SenserVal.AngleZ= filter_1(AngleZdata,SenserOld.AngleZ,Filternum);
	  SenserOld.AngleZ=AngleZdata;
	  SenserVal.Temp1= filter_1(Tempdata1,SenserOld.Temp1,Filternum);
	  SenserOld.Temp1=Tempdata1;
	  SenserVal.TempA= filter_1(TempdataA,SenserOld.TempA,Filternum);
	  SenserOld.TempA=TempdataA;
}	

void _READ_SenserData(SENSER_VAL* Senval)
{
    memcpy(Senval,&SenserVal,sizeof(SenserVal));
}	

