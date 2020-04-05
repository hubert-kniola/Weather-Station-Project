#include "SP_MENU.h"

#include "SP_LCD.h"
#include "SP_THS.h"

#define BTN_PRESSED_TIME	5
#define MAX_PASSWD_LEN 		40
#define MIN_CHAR ' '
#define MAX_CHAR '~'
#define BEGIN_CHAR '@'

/* ----------------- Konfiguracja uzytkownika ------------------- */
#define PORT 	GPIOE

#define UP 		GPIO_PIN_7
#define DOWN	GPIO_PIN_8
#define LEFT 	GPIO_PIN_9
#define RIGHT 	GPIO_PIN_10

extern StateEnum State;
/* ----------------- /Konfiguracja uzytkownika ------------------ */

#define IfPressed(arg) if(HAL_GPIO_ReadPin(PORT, arg)){HAL_Delay(BTN_PRESSED_TIME);	if(HAL_GPIO_ReadPin(PORT, arg)){while(HAL_GPIO_ReadPin(PORT, arg));
#define Or(arg) }}else IfPressed(arg)
#define End }}

char WiFiPassword[MAX_PASSWD_LEN];

char _PWD_char;
uint8_t _PWD_index;

void MENU_Init(void) {
	State = ST_Clock;
}

void _PWD_ResetPasswd(void) {
	for (int i = 0; i < MAX_PASSWD_LEN; i++) {
		WiFiPassword[i] = 0;
	}

	_PWD_index = 0;
	_PWD_char = BEGIN_CHAR;
}

char _PWD_NextChar(void) {
	if (++_PWD_char > MAX_CHAR) _PWD_char = MIN_CHAR;
	return _PWD_char;
}

void _PWD_SaveAndWrite(char c) {
	WiFiPassword[_PWD_index] = (c == ' ') ? 0 : c;
	LCD_WriteChar(c);
}

void _PWD_ParsePasswd(void) {
	bool clearRest = false;
	for (int i = 0; i < MAX_PASSWD_LEN; i++) {
			if (!clearRest && WiFiPassword[i] == 0) clearRest = true;
			if (clearRest) WiFiPassword[i] = 0;
	}
}

void MENU_PasswdInput(void) {
	if (State != ST_PassInput) {
		State = ST_PassInput;
		LCD_ClearScreen();

		LCD_PrintCentered("Enter WiFi password:");
		LCD_SetCursor(0, 3);
		LCD_PrintCentered("Press DOWN to accept");
		LCD_SetCursor(0, 1);

		_PWD_ResetPasswd();

		LCD_EnableCursor();
	}
}

uint8_t MENU_HandleKeys(void) {
	IfPressed (UP) {
		if (State == ST_Options || State == ST_WiFi) {
			LCD_CursorUp();
		} else if (State == ST_PassInput) {
			/* Dopasuj olejny znak ASCII */
			_PWD_SaveAndWrite(_PWD_NextChar());
		}

		LED_T(Red);
		return 1;

	} Or (DOWN) {
		if (State == ST_Options || State == ST_WiFi) {
			LCD_CursorDown();
		} else if (State == ST_PassInput) {
			/* Powrot do trybu zegara */
			_PWD_ParsePasswd();

			State = ST_Clock;

			LCD_ClearScreen();
			LCD_DisableCursor();
		}

		LED_T(Green);
		return 1;

	} Or (LEFT) {
		if (State == ST_PassInput) {
			/* Poprzedni znak w jednej z dwoch kolumn */
			if (_PWD_index > 0) {
				--_PWD_index;
				LCD_CursorLeft();
			}
		}
		LED_T(Blue);
		return 1;

	} Or (RIGHT) {
		if (State == ST_PassInput) {
			/* Kolejny znak w jednej z dwoch kolumn */
			if (WiFiPassword[_PWD_index] != 0 && _PWD_index < MAX_PASSWD_LEN - 1) {
				++_PWD_index;
				LCD_CursorRight();
			}
		}

		LED_T(Orange);
		return 1;

	} End;
	return 0;
}
