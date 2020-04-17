#include "SP_MENU.h"

#include "SP_LCD.h"
#include "SP_THS.h"
#include "SP_SD.h"
#include "SP_NET.h"
#include "SP_RGB.h"

#include <stdlib.h>
#include <stdbool.h>

#define BTN_PRESSED_TIME	10
#define MAX_PASSWD_LEN 		40
#define DT_LEN				19

#define MIN_PWD_CHAR 	' '
#define MAX_PWD_CHAR 	'~'
#define BEGIN_PWD_CHAR	'@'

#define MIN_DT_CHAR '0'
#define MAX_DT_CHAR '9'

/* ----------------- Konfiguracja uzytkownika ------------------- */
#define PORT 	GPIOE

#define UP 		GPIO_PIN_7
#define DOWN	GPIO_PIN_8
#define LEFT 	GPIO_PIN_9
#define RIGHT 	GPIO_PIN_10

extern StateEnum M_State;
extern ModeEnum M_Mode;
extern RGB_Mode R_Mode;
/* ----------------- /Konfiguracja uzytkownika ------------------ */

/* Makra dla przyciskow */
#define IfPressed(arg) if(HAL_GPIO_ReadPin(PORT,arg)){HAL_Delay(BTN_PRESSED_TIME);if(HAL_GPIO_ReadPin(PORT,arg)){while(HAL_GPIO_ReadPin(PORT,arg));
#define Or(arg) }}else IfPressed(arg)
#define IfEnd }}

/* Makra dla daty */
#define ColIs(arg) case arg:{
#define ColEnd break;}

#define SetBetween(min, max) if(_optionsChar>= max){_optionsChar = min;}else if(_optionsChar<min){_optionsChar=min;}else _optionsChar++;
#define Month(a1, a2) (date[2]==a1&&date[3]==a2)

/* kazda opcja musi sie konczyc srednikiem
 * oraz musi maks 20 znakow,
 * dodatkowo update NOF_OPTIONS i O_* */
#define OPTIONS_STRING "1: Force update;2: Connect to WiFi;3: WiFi Disconnect;4: Set time and date;5: Clear SD data;6: Shutdown device;"
#define NOF_OPTIONS	 6

#define O_FORCE_U	1
#define O_CONN_W	2
#define O_DISCONN	3
#define O_SET_DT	4
#define O_CLEAR_SD	5
#define O_SHUTDOWN	6

#define MINUTE 60
#define UPDATE_INTERVAL 10

char WiFiPassword[MAX_PASSWD_LEN];
uint8_t _PWD_index;

uint8_t UserDateTime[DT_LEN];

char _optionsChar;
uint8_t _optionsRow;
uint8_t _optionsCol;

uint16_t _menuTick;

bool _updateClock;
bool _updateWeather;

bool _userReaction;

uint8_t _networksIn;
uint8_t _currentOption;
char *_networksList;

/* zmienne glownego loopa */
char _date[9], _time[9];
float *_dataIn, *_dataOut;

void MENU_Init(void) {
	M_State = ST_Clock;

	_menuTick = 0;
	_updateClock = true;
	_updateWeather = true;
	_userReaction = false;

	_dataIn = (float*) malloc(2 * sizeof(float));
	_dataOut = (float*) malloc(2 * sizeof(float));

	for (int i = 0; i < 2; i++) {
		_dataIn[i] = 0.f;
		_dataOut[i] = 0.f;
	}

	THS_ReadData(THS_In, _dataIn);
	HAL_Delay(1000);

	R_Mode = RGB_Disabled;
}

void _PWD_ResetPasswd(void) {
	for (int i = 0; i < MAX_PASSWD_LEN; i++) {
		WiFiPassword[i] = 0;
	}

	_PWD_index = 0;
	_optionsChar = BEGIN_PWD_CHAR;
}

char _PWD_NextChar(void) {
	if (++_optionsChar > MAX_PWD_CHAR)
		_optionsChar = MIN_PWD_CHAR;
	return _optionsChar;
}

void _PWD_SaveAndWrite(char c) {
	WiFiPassword[_PWD_index] = (c == ' ') ? 0 : c;
	LCD_WriteChar(c);
}

void _CLK_HandleDateTimeInput(void) {
	switch (_optionsCol) {
	ColIs(1)
			SetBetween('0', '3');
			ColEnd
	ColIs(2)
			SetBetween('0', '9');
			ColEnd

	ColIs(4)
			SetBetween('0', '1');
			ColEnd
	ColIs(5)
			SetBetween('0', '9');
			ColEnd

	ColIs(7)
			SetBetween('2', '9');
			ColEnd
	ColIs(8)
			SetBetween('0', '9');
			ColEnd

	ColIs(11)
			SetBetween('0', '2');
			ColEnd
	ColIs(12)
			SetBetween('0', '9');
			ColEnd

	ColIs(14)
			SetBetween('0', '5');
			ColEnd
	ColIs(15)
			SetBetween('0', '9');
			ColEnd

	ColIs(17)
			SetBetween('0', '5');
			ColEnd
	ColIs(18)
			SetBetween('0', '9');
			ColEnd
	}

	LCD_WriteChar(_optionsChar);
	UserDateTime[_optionsCol] = _optionsChar;
}

void _CLK_MoveInputRight(void) {
	switch (_optionsCol) {
	ColIs(18)
			ColEnd

	ColIs(15)
			LCD_SetCursor(17, 1);
			_optionsCol = 17;
			ColEnd

	ColIs(12)
			LCD_SetCursor(14, 1);
			_optionsCol = 14;
			ColEnd

	ColIs(8)
			LCD_SetCursor(11, 1);
			_optionsCol = 11;
			ColEnd

	ColIs(5)
			LCD_SetCursor(7, 1);
			_optionsCol = 7;
			ColEnd

	ColIs(2)
			LCD_SetCursor(4, 1);
			_optionsCol = 4;
			ColEnd

	default:
		_optionsCol = LCD_CursorRight();
	}
}

void _CLK_MoveInputLeft(void) {
	switch (_optionsCol) {
	ColIs(1)
			ColEnd

	ColIs(4)
			LCD_SetCursor(2, 1);
			_optionsCol = 2;
			ColEnd

	ColIs(7)
			LCD_SetCursor(5, 1);
			_optionsCol = 5;
			ColEnd

	ColIs(11)
			LCD_SetCursor(8, 1);
			_optionsCol = 8;
			ColEnd

	ColIs(14)
			LCD_SetCursor(12, 1);
			_optionsCol = 12;
			ColEnd

	ColIs(17)
			LCD_SetCursor(15, 1);
			_optionsCol = 15;
			ColEnd

	default:
		_optionsCol = LCD_CursorLeft();
	}
}

void _CLK_ParseAndSetDateTime(void) {
	uint8_t date[6], time[6];
	bool error = false;

	date[0] = UserDateTime[1] - '0';
	date[1] = UserDateTime[2] - '0';
	date[2] = UserDateTime[4] - '0';
	date[3] = UserDateTime[5] - '0';
	date[4] = UserDateTime[7] - '0';
	date[5] = UserDateTime[8] - '0';

	time[0] = UserDateTime[11] - '0';
	time[1] = UserDateTime[12] - '0';
	time[2] = UserDateTime[14] - '0';
	time[3] = UserDateTime[15] - '0';
	time[4] = UserDateTime[17] - '0';
	time[5] = UserDateTime[18] - '0';

	/* Odrzucenie błędnych danych */
	if (date[2] == 1 && date[3] > 2) {
		/* ponad 12 miesięcy */
		error = true;
	}
	if (Month(0, 0)) {
		/* zerowy miesiac */
		error = true;
	}
	if (time[0] == 2 && time[1] > 3) {
		/* ponad 23 godziny */
		error = true;
	}
	if (Month(0, 1)||Month(0,3)||Month(0,5)||
	Month(0,7)||Month(0,8)||Month(1,0)||
	Month(1,2)) {
		/* miesiac 31 dniowy */
		if (date[0] == 3 && date[1] > 1) {
			error = true;
		}
	}
	if (Month(0,4) || Month(0, 6) || Month(0, 9) || Month(1, 1)) {
		/* miesiac 30 dniowy */
		if (date[0] == 3 && date[1] != 0) {
			error = true;
		}
	}
	if (Month(0, 2)) {
		/* luty */
		if (date[0] > 2) {
			error = true;
		}
		if (date[5] % 4 != 0) {
			/* rok zwykly */
			if (date[0] == 2 && date[1] == 9) {
				error = true;
			}
		}
	}

	LCD_DisableCursor();

	if (error) {
		/* handluj z tym */
		LCD_ClearScreen();

		LCD_SetCursor(0, 1);
		LCD_PrintCentered("Invalid data");
		LCD_SetCursor(0, 2);
		LCD_PrintCentered("Ommiting update!");

		HAL_Delay(1000);
	} else {
		SD_SetDateTime(date, time);
	}
}

void MENU_PasswdInput(void) {
	if (M_State != ST_PassInput) {
		M_State = ST_PassInput;
		LCD_ClearScreen();

		LCD_PrintCentered("Enter WiFi password:");
		LCD_SetCursor(0, 3);
		LCD_PrintCentered("Press DOWN to accept");
		LCD_SetCursor(0, 1);

		_PWD_ResetPasswd();

		LCD_DisableBlink();
		LCD_EnableCursor();
	}
}

void MENU_Options(void) {
	if (M_State != ST_Options) {
		M_State = ST_Options;
		LCD_ClearScreen();

		_currentOption = 1;
		LCD_PrintOptionsScreen(OPTIONS_STRING, _currentOption);
		LCD_SetCursor(0, 1);
		_optionsRow = 1;

		LCD_EnableBlink();
	}
}

void MENU_OptionsSetDateTime(void) {
	if (M_State != ST_SetDateTime) {
		M_State = ST_SetDateTime;
		LCD_ClearScreen();

		LCD_Print("---Date------Time---");
		LCD_SetCursor(0, 1);
		LCD_Print("|00.00.00||00.00.00|");
		LCD_SetCursor(0, 2);
		LCD_Print("--------------------");
		LCD_SetCursor(0, 3);
		LCD_Print("Press DOWN to accept");

		LCD_SetCursor(1, 1);
		_optionsCol = 1;

		LCD_DisableBlink();
		LCD_EnableCursor();

		for (int i = 0; i < DT_LEN; i++) {
			UserDateTime[i] = MIN_DT_CHAR;
		}

		_optionsChar = MIN_DT_CHAR;
	}
}

uint8_t _WiFi_NofNetworks(char *data) {
	int amount = 0;
	for (int i = 0;; i++) {
		if (data[i] == 0) {
			return amount;
		}

		if (data[i] == ';') {
			amount++;
		}
	}
	return amount;
}

void _WiFi_RequestConn(void) {
	int clearRest = 0;
	for (int i = 0; i < MAX_PASSWD_LEN; i++) {
		if (!clearRest && WiFiPassword[i] == 0)
			clearRest = i;
		if (clearRest)
			WiFiPassword[i] = 0;
	}

	LCD_ClearScreen();
	LCD_DisableCursor();

	LCD_SetCursor(0, 1);
	LCD_PrintCentered("Connecting");
	LCD_SetCursor(0, 3);
	LCD_PrintCentered("please wait");

	R_Mode = RGB_Rainbow;
	uint8_t result = NET_ConnectToWiFi((char*) WiFiPassword, _currentOption);
	LCD_ClearScreen();
	R_Mode = RGB_Disabled;

	if (result == 0) {
		LCD_SetCursor(0, 1);
		LCD_PrintCentered("Connected!");
	} else {
		LCD_SetCursor(0, 0);
		LCD_PrintCentered("Connection might");
		LCD_SetCursor(0, 1);
		LCD_PrintCentered("not be established");
		LCD_SetCursor(0, 3);
		LCD_Print("Check status in menu");
	}

	HAL_Delay(1000);
	NET_GetConnInfo();
}

void MENU_OptionsWifiList(void) {
	if (M_State != ST_WiFi) {
		M_State = ST_WiFi;
		LCD_ClearScreen();
		LCD_DisableBlink();

		LCD_PrintCentered("Select a network");
		LCD_SetCursor(0, 2);
		LCD_PrintCentered("searching...");
		LCD_SetCursor(0, 3);
		LCD_PrintCentered("please wait");

		_optionsRow = 0;

		R_Mode = RGB_Rainbow;
		char *data = NET_RequestNetworkList();
		R_Mode = RGB_Disabled;

		if (data != NULL) {
			/* liczba rzedow do poruszania sie */
			_networksIn = _WiFi_NofNetworks(data);
			_currentOption = 1;
			_networksList = data;

			LCD_PrintNetworks(data, _currentOption);

			/* przygotuj sie na wybor */
			LCD_EnableBlink();
			LCD_SetCursor(0, 0);
		} else {
			LCD_ClearScreen();
			LCD_SetCursor(0, 1);

			LCD_PrintCentered("No networks found!");
			HAL_Delay(1000);
			MENU_Options();
		}
	}
}

void MENU_Clock() {
	if (M_State != ST_Clock) {
		M_State = ST_Clock;

		LCD_ClearScreen();
		LCD_DisableBlink();

		_menuTick = 0;
		_updateClock = true;
	}

	if (_updateWeather) {
		/* update jsona do wyslania dla strony, nie przeszkadzac */
		NET_AbortIT();

		if (THS_ReadData(THS_In, _dataIn)) {
			SD_CreateJson(true, _dataIn, _date, _time);
		} else {
			_dataIn = NULL;
		}

		if (THS_ReadData(THS_Out, _dataOut)) {
			SD_CreateJson(false, _dataOut, _date, _time);
			LCD_SetCursor(0, 1);
			LCD_Print("                    ");
			LCD_SetCursor(0, 2);
			LCD_Print("                    ");
		} else {
			_dataOut = NULL;
		}

		_updateWeather = false;
	}

	if (_userReaction) {
		NET_GetConnInfo();
	}

	if (_updateClock || _updateWeather || _userReaction) {
		SD_RefreshDateTime();
		SD_GetDateTime(_date, _time);

		if (_dataIn != NULL) {
			if ((int) _dataIn[0] < 5 || (int) _dataIn[1] < 20) {
				/* jest tragicznie */
				R_Mode = RGB_BlinkRed;
			} else if ((int) _dataIn[0] < 10 || (int) _dataIn[1] < 30) {
				/* jest slabo */
				R_Mode = RGB_Red;
			} else if ((int) _dataIn[0] > 35 || (int) _dataIn[1] > 65) {
				/* jest slabo */
				R_Mode = RGB_Red;
			} else if ((int) _dataIn[0] > 40 || (int) _dataIn[1] > 90) {
				/* jest tragicznie */
				R_Mode = RGB_BlinkRed;
			} else {
				/* ok */
				R_Mode = RGB_Green;
			}
		}

		LCD_PrintDateTime(_date, _time);
		LCD_PrintTempInfo(_dataIn, _dataOut);
		LCD_PrintNetworkStatus(M_Mode, NET_GetCurrentConnStatus());

		_updateClock = false;
	}
}

uint8_t MENU_HandleInput(void) {
	_userReaction = false;

	IfPressed (UP)
			{
				LCD_WakeScreen();

				if (M_State == ST_Clock) {
					/* Przejdz w ekran opcji */
					MENU_Options();
				} else if (M_State == ST_Options) {
					if (_optionsRow > 1) {
						_optionsRow = LCD_CursorUp();
						_currentOption--;
					} else if (_currentOption != 1) {
						LCD_PrintOptionsScreen(OPTIONS_STRING,
								--_currentOption);
						LCD_SetCursor(0, 1);
					}
				} else if (M_State == ST_PassInput) {
					/* Dopasuj kolejny znak ASCII */
					_PWD_SaveAndWrite(_PWD_NextChar());
				} else if (M_State == ST_SetDateTime) {
					/* wstepne ograniczenie inputu */
					_CLK_HandleDateTimeInput();
				} else if (M_State == ST_WiFi) {
					if (_optionsRow > 0) {
						_optionsRow = LCD_CursorUp();
						_currentOption--;
					} else if (_networksIn > 4 && _currentOption != 1) {
						LCD_PrintNetworks(_networksList, --_currentOption);
						LCD_SetCursor(0, 0);
					}
				}

				_userReaction = true;
			}Or (DOWN)
			{
				LCD_WakeScreen();

				if (M_State == ST_Options) {
					if (_optionsRow < 3) {
						_optionsRow = LCD_CursorDown();
						_currentOption++;
					} else if (_currentOption != NOF_OPTIONS) {
						LCD_PrintOptionsScreen(OPTIONS_STRING,
								++_currentOption - 2);
						LCD_SetCursor(0, 3);
					}
				} else if (M_State == ST_PassInput) {
					/* Powrot do trybu zegara */
					_WiFi_RequestConn();
					MENU_Clock();
				} else if (M_State == ST_SetDateTime) {
					_CLK_ParseAndSetDateTime();
					MENU_Clock();
				} else if (M_State == ST_WiFi) {
					//TODO DEBUG n TESTING
					if (_optionsRow < 4 && _optionsRow < _networksIn - 1) {
						_optionsRow = LCD_CursorDown();
						_currentOption++;
					} else if (_networksIn > 4 && _currentOption != 4) {
						LCD_PrintNetworks(_networksList, ++_currentOption - 4);
						LCD_SetCursor(0, 3);
					}
				}

				_userReaction = true;
			}Or (LEFT)
			{
				LCD_WakeScreen();

				if (M_State == ST_PassInput) {
					/* Poprzedni znak w jednej z dwoch kolumn */
					if (_PWD_index > 0) {
						--_PWD_index;
						LCD_CursorLeft();
					}
				} else if (M_State == ST_Options) {
					MENU_Clock();
				} else if (M_State == ST_SetDateTime) {
					_CLK_MoveInputLeft();
				} else if (M_State == ST_WiFi) {
					MENU_Options();
				}

				_userReaction = true;
			}Or (RIGHT)
			{
				LCD_WakeScreen();

				if (M_State == ST_PassInput) {
					/* Kolejny znak w jednej z dwoch kolumn */
					if (WiFiPassword[_PWD_index] != 0
							&& _PWD_index < MAX_PASSWD_LEN - 1) {
						++_PWD_index;
						LCD_CursorRight();
					}
				} else if (M_State == ST_Options) {
					if (_currentOption == O_CONN_W) {
						MENU_OptionsWifiList();
					} else if (_currentOption == O_DISCONN) {
						NET_WiFiDisconnect();
						MENU_Clock();

					} else if (_currentOption == O_FORCE_U) {
						_updateWeather = true;
						MENU_Clock();

					} else if (_currentOption == O_SET_DT) {
						MENU_OptionsSetDateTime();

					} else if (_currentOption == O_CLEAR_SD) {
						LCD_ClearScreen();
						LCD_SetCursor(0, 1);
						LCD_PrintCentered("Please wait");

						R_Mode = RGB_Rainbow;
						SD_RemoveAllJsons();
						R_Mode = RGB_Disabled;

						MENU_Clock();

					} else if (_currentOption == O_SHUTDOWN) {
						/* TODO Ogarnac wylaczanie  */
					}
				} else if (M_State == ST_SetDateTime) {
					_CLK_MoveInputRight();
				} else if (M_State == ST_WiFi) {
					MENU_PasswdInput();
				}

				_userReaction = true;
			}IfEnd;

	return 0;
}

void MENU_IncTick(void) {
	++_menuTick;

	LED_T(Orange);

	if (_menuTick % MINUTE == 0) {
		_updateClock = true;
	} else if (_menuTick == MINUTE / 2) {
		LCD_BackgroundOff();
	}

	if (_menuTick >= MINUTE * UPDATE_INTERVAL) {
		_updateWeather = true;
		_menuTick = 0;
	}
}
