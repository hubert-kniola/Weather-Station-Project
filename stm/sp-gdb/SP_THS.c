#include "SP_THS.h"

#define THS_START_LOW 18000
#define THS_START_HI 20
#define THS_RESPONSE 40

#define THS_PORT GPIOC

uint8_t _data[5];
uint16_t _pin1;
uint16_t _pin2;

TIM_HandleTypeDef* _clock;

void THS_Init(TIM_HandleTypeDef* htim4)
{
	_clock = htim4;
	
	_pin1 = GPIO_PIN_0;
	_pin2 = GPIO_PIN_1;
	
	HAL_Delay(500);
}

void THS_Delay(uint16_t micros)
{
	__HAL_TIM_SET_COUNTER(_clock, 0);
	while (__HAL_TIM_GET_COUNTER(_clock) < micros) ;
}

void _THS_SetPinOutput(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(THS_PORT, &GPIO_InitStruct);
}

void _THS_SetPinInput(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(THS_PORT, &GPIO_InitStruct);
}

#define Write(val) HAL_GPIO_WritePin(THS_PORT, pin, val)
#define Read() HAL_GPIO_ReadPin(THS_PORT, pin)
#define WaitOn(arg) while (HAL_GPIO_ReadPin(THS_PORT, pin) == arg)

uint8_t THS_InitConn(THS_Sensor sensor)
{
	uint16_t pin = (sensor == THS_In) ? _pin1 : _pin2;
	
	_THS_SetPinOutput(pin);
	
	/* Start */
	Write(0);
	HAL_Delay(18);
	Write(1);
	THS_Delay(THS_START_HI);
	
	/* Synchro */
	_THS_SetPinInput(pin);
	THS_Delay(THS_RESPONSE);
	
	if (!Read())
	{
		THS_Delay(2*THS_RESPONSE);
		if (!Read()) return 1;
	}
	WaitOn(1);
	
	return 0;
}

uint8_t THS_ReadByte(THS_Sensor sensor)
{
	uint16_t pin = (sensor == THS_In) ? _pin1 : _pin2;
	uint8_t byte = 0;
	
	for (uint8_t bit = 0; bit < 8; bit++)
	{		
		WaitOn(0);
		THS_Delay(60);
		
		byte = (byte << 1);
		if (Read()) byte |= 1; /* mamy 1 */ 
		/* else mamy 0 */
	}
	
	return byte;
}