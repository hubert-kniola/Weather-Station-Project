#include "SP_SD.h"
#include "ff.h"
#include <stdio.h>

typedef struct {
	uint8_t date[6];
	uint8_t time[6];
} SD_Time;

/* ----------------- Konfiguracja uzytkownika ------------------- */
extern RTC_HandleTypeDef hrtc;
/* ----------------- /Konfiguracja uzytkownika ------------------ */

RTC_TimeTypeDef _Time;
RTC_DateTypeDef _Date;

static FATFS _ffHandle; //uchwyt do urządzenia FatFs (dysku, karty SD...)
FRESULT _fresult; //do przechowywania wyniku operacji na bibliotece
FIL _fileHandle; //uchwyt do otwartego plik
SD_Time _dTime;

uint8_t _buffer[256]; //bufor odczytu i zapisu
uint16_t _bytesWritten; //liczba zapisanych byte
uint16_t _bytesRead;

void SD_Init(void) {
	f_mount(&_ffHandle, "", 0);

	SD_RefreshDateTime();
}

void SD_RefreshDateTime(void) {
	HAL_RTC_GetTime(&hrtc, &_Time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &_Date, RTC_FORMAT_BCD);

	_dTime.time[0] = (_Time.Hours / 16) + 48;
	_dTime.time[1] = (_Time.Hours % 16) + 48;
	_dTime.time[2] = (_Time.Minutes / 16) + 48;
	_dTime.time[3] = (_Time.Minutes % 16) + 48;
	_dTime.time[4] = (_Time.Seconds / 16) + 48;
	_dTime.time[5] = (_Time.Seconds % 16) + 48;

	_dTime.date[0] = (_Date.Date / 16) + 48;
	_dTime.date[1] = (_Date.Date % 16) + 48;
	_dTime.date[2] = (_Date.Month / 16) + 48;
	_dTime.date[3] = (_Date.Month % 16) + 48;
	_dTime.date[4] = (_Date.Year / 16) + 48;
	_dTime.date[5] = (_Date.Year % 16) + 48;
}

void SD_GetDateTime(char date[], char time[]) {
	sprintf(date, "%c%c.%c%c.%c%c", _dTime.date[0], _dTime.date[1],
			_dTime.date[2], _dTime.date[3], _dTime.date[4], _dTime.date[5]);
	sprintf(time, "%c%c:%c%c:%c%c", _dTime.time[0], _dTime.time[1],
			_dTime.time[2], _dTime.time[3], _dTime.time[4], _dTime.time[5]);
}

void SD_SetDateTime(uint8_t date[], uint8_t time[]) {
	_Date.Date = date[0] * 16 + date[1];
	_Date.Month = date[2] * 16 + date[3];
	_Date.Year = date[4] * 16 + date[5];

	_Time.Hours = time[0] * 16 + time[1];
	_Time.Minutes = time[2] * 16 + time[3];
	_Time.SecondFraction = time[4] * 16 + time[5];

	HAL_RTC_SetTime(&hrtc, &_Time, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &_Date, RTC_FORMAT_BCD);
}

/* Template structure
 *
 * {
 * 		"innTHS" : true,
 * 		"temper" : 24,
 * 		"humidi" : 35,
 * 		"locati" : "PL",
 * 		"time"	 : "12:03:30",
 * 		"date"   : "03.30.20"
 * }
 */
uint8_t SD_WriteJSON(float *data) {
	//TODO zapis i odczyt plikow
	return 1;
}
