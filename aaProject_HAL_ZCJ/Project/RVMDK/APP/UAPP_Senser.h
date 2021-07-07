#ifndef UAPP_SENSER_H
#define UAPP_SENSER_H
#include "sys.h"

typedef struct
{
	short  Current;
	short  Voltage;
  short  Temp1;
	short  Temp2;
	short  TempA;
	short  AngleX;
	short  AngleY;
	short  AngleZ;
}SENSER_VAL;

#define Filternum            20


void _SenserFilter_Task(void);
void _READ_SenserData(SENSER_VAL* Senval);

#endif



