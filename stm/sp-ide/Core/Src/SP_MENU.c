#include "SP_MENU.h"

#include "SP_LCD.h"
#include "SP_THS.h"

#define BTN_PRESSED_TIME	1

/* ----------------- Konfiguracja uzytkownika ------------------- */
#define PORT 	GPIOE

#define UP 		GPIO_PIN_7
#define DOWN	GPIO_PIN_8
#define LEFT 	GPIO_PIN_9
#define RIGHT 	GPIO_PIN_10

extern uint8_t State;
/* ----------------- /Konfiguracja uzytkownika ------------------ */

#define IfPressed(arg) if(HAL_GPIO_ReadPin(PORT, arg)){HAL_Delay(BTN_PRESSED_TIME);	if(HAL_GPIO_ReadPin(PORT, arg)){while(HAL_GPIO_ReadPin(PORT, arg));
#define Or(arg) }}else IfPressed(arg)
#define End }}



void MENU_Init(void) {

}

void _MENU_UpPressed(void) {

}

void _MENU_DownPressed(void) {

}

void _MENU_LeftPressed(void) {

}

void _MENU_RightPressed(void) {

}

void MENU_HandleKeys(void) {
	IfPressed (UP) {

		_MENU_UpPressed();
		LED_T(Red);

	} Or (DOWN) {

		_MENU_DownPressed();
		LED_T(Green);

	} Or (LEFT) {

		_MENU_LeftPressed();
		LED_T(Blue);

	} Or (RIGHT) {

		_MENU_RightPressed();
		LED_T(Orange);

	} End;
}
