#ifndef SP_THS_h
#define SP_THS_h

#include "main.h"

typedef enum
{ 
	THS_In,
	THS_Out
} THS_Sensor;

uint8_t THS_InitConn(THS_Sensor);
uint8_t THS_ReadByte(THS_Sensor);

void THS_Init(TIM_HandleTypeDef*);
void THS_Delay(uint16_t);

#endif