#include "SP_SD.h"
#include "ff.h"
#include <stdio.h>

typedef struct {
	uint8_t date[6];
	uint8_t time[6];
} SD_Time;

static FATFS _ffHandle; //uchwyt do urządzenia FatFs (dysku, karty SD...)
FRESULT _fresult; //do przechowywania wyniku operacji na bibliotece
FIL _fileHandle; //uchwyt do otwartego plik

uint8_t _buffer[256]; //bufor odczytu i zapisu
uint16_t _bytesWritten; //liczba zapisanych byte
uint16_t _bytesRead;

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef hTime;
extern RTC_DateTypeDef hDate;

SD_Time _tHandle;

void SD_Init(void) {
	f_mount(&_ffHandle, "", 0);

	SD_RefreshDateTime();
}

void SD_RefreshDateTime(void) {
	HAL_RTC_GetTime(&hrtc, &hTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &hDate, RTC_FORMAT_BCD);

	_tHandle.time[0] = (hTime.Hours / 16) + 48;
	_tHandle.time[1] = (hTime.Hours % 16) + 48;
	_tHandle.time[2] = (hTime.Minutes / 16) + 48;
	_tHandle.time[3] = (hTime.Minutes % 16) + 48;
	_tHandle.time[4] = (hTime.Seconds / 16) + 48;
	_tHandle.time[5] = (hTime.Seconds % 16) + 48;

	_tHandle.date[0] = (hDate.Date / 16) + 48;
	_tHandle.date[1] = (hDate.Date % 16) + 48;
	_tHandle.date[2] = (hDate.Month / 16) + 48;
	_tHandle.date[3] = (hDate.Month % 16) + 48;
	_tHandle.date[4] = (hDate.Year / 16) + 48;
	_tHandle.date[5] = (hDate.Year % 16) + 48;
}

void SD_GetDateTime(char date[], char time[]) {
	sprintf(date, "%c%c.%c%c.%c%c", _tHandle.date[0], _tHandle.date[1],
			_tHandle.date[2], _tHandle.date[3], _tHandle.date[4], _tHandle.date[5]);
	sprintf(time, "%c%c:%c%c:%c%c", _tHandle.time[0], _tHandle.time[1],
			_tHandle.time[2], _tHandle.time[3], _tHandle.time[4], _tHandle.time[5]);
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
	return 1;
}
