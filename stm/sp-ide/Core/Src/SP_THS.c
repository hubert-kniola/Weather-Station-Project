#include "SP_THS.h"
#include <stdbool.h>
#include <stdlib.h>

#define THS_START_LOW 18000
#define THS_START_HI 20
#define THS_RESPONSE 40
#define THS_SIGNAL_WAIT 60
#define THS_ERROR_RANGE 10
#define THS_SECOND 1000

#define THS_PORT GPIOC

bool _ready;

uint8_t _data[5];
uint16_t _pin1;
uint16_t _pin2;

uint32_t _clockCounter;

extern TIM_HandleTypeDef htim4;

void THS_Init() {
	_pin1 = GPIO_PIN_0;
	_pin2 = GPIO_PIN_1;

	_clockCounter = 0;

	HAL_Delay(500);
	_ready = true;
}

void THS_Delay(uint16_t micros) {
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	while (__HAL_TIM_GET_COUNTER(&htim4) < micros)
		;
}

void _THS_SetPinOutput(uint16_t pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(THS_PORT, &GPIO_InitStruct);
}

void _THS_SetPinInput(uint16_t pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(THS_PORT, &GPIO_InitStruct);
}

#define Write(val) HAL_GPIO_WritePin(THS_PORT, pin, val)
#define Read() HAL_GPIO_ReadPin(THS_PORT, pin)
#define WaitOn(arg) while (HAL_GPIO_ReadPin(THS_PORT, pin) == arg)

uint8_t _THS_InitConn(uint16_t pin) {
	if (!_ready)
		return 0;
	_THS_SetPinOutput(pin);

	/* Start */
	Write(0);
	HAL_Delay(18);
	Write(1);
	THS_Delay(THS_START_HI);

	/* Synchro */
	_THS_SetPinInput(pin);
	THS_Delay(THS_RESPONSE);

	if (!Read()) {
		THS_Delay(2 * THS_RESPONSE);
		if (!Read()) {
			/* Brak synchro */
			return 0;
		}
	}
	WaitOn(1);
	/* Gotowy */
	return 1;
}

uint8_t _THS_ReadByte(uint16_t pin) {
	uint8_t byte = 0;

	for (uint8_t bit = 0; bit < 8; bit++) {
		_clockCounter = 0;
		WaitOn(0) {
			/* Podany pin nie jest podpiety, eternal loop */
			if (_clockCounter > 200)
				return 255;
		}
		THS_Delay(THS_SIGNAL_WAIT);

		byte <<= 1;
		if (Read()) {
			byte |= 1; /* mamy 1 */
			WaitOn(1);
		}
		/* else mamy 0 */
	}

	return byte;
}

uint8_t _THS_CCR_Acceptable(uint8_t ccrIn, uint8_t ccrAcc) {
	if (ccrAcc - THS_ERROR_RANGE <= ccrIn && ccrIn <= ccrAcc + THS_ERROR_RANGE)
		return 1;
	return 0;
}

/* Funkcja wymaga jako argumentu tablicy conajmniej 2 elementowej else panic */
uint8_t THS_ReadData(THS_Sensor sensor, float data[]) {
	uint16_t pin = (sensor == THS_In) ? _pin1 : _pin2;

	if (!_THS_InitConn(pin))
		return 0;

	uint16_t rh1 = _THS_ReadByte(pin);
	uint8_t rh2 = _THS_ReadByte(pin);
	uint16_t temp1 = _THS_ReadByte(pin);
	uint8_t temp2 = _THS_ReadByte(pin);
	uint8_t ccr = _THS_ReadByte(pin);
	/* co namniej sekunda przerwy miedzy updateami */
	_ready = false;

	if (_THS_CCR_Acceptable(ccr, rh1 | rh2 | temp1 | temp2)) {
		/* 0-Temperatura, 1-Wilgotnosc */
		float t = (float) ((temp1 << 8) | temp2) / (float) (1 << 8);
		float r = (float) ((rh1 << 8) | rh2) / (float) (1 << 8);

		/* Dane spoza zakresu czujnika, musial wystapic blad */
		if ((0.f > t || t > 50.f) || (0.f > r || r > 100.f)) {
			data[0] = 0.f;
			data[1] = 0.f;
			return 0;
		}
		data[0] = t;
		data[1] = r;
		return 1;
	}
	/* Dane zbyt odbiegają od prawdziwych */
	data[0] = 0.f;
	data[1] = 0.f;
	return 0;
}

/* Umiescic w obsludze przerwania zegara  tak by if dzialal co sekunde*/
void THS_ErrorClock(void) {
	if (_clockCounter > THS_SECOND) {
		_ready = true;
		_clockCounter = 0;
	}
	++_clockCounter;
}

/* Gdy juz niepotrzebny uzyc free */
float* THS_NewContainer(void) {
	float *cont = (float*) malloc(2 * sizeof(float));

	cont[0] = 0.f;
	cont[1] = 0.f;

	return cont;
}
