#ifndef SP_THS_h
#define SP_THS_h

#include "main.h"

typedef enum
{ 
	THS_In,
	THS_Out
} THS_Sensor;

void THS_Init();
void THS_Delay(uint16_t);
void THS_ErrorClock(void);

uint8_t THS_ReadData(THS_Sensor, float*);

#endif
