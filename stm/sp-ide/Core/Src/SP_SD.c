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

FATFS _ff;
uint32_t DISK_LEFT;

FRESULT _res;
FIL _fileH;
SD_Time _dTime;

BYTE _work[FF_MAX_SS];
uint32_t _writtenB;
uint32_t _readB;

uint32_t _SD_GetDiskSpace(void) {
	FATFS *ptr;
	uint32_t fre_clust = 0;

	if (f_getfree("", &fre_clust, &ptr) != FR_OK) {
		return 0;
	}

	DISK_LEFT = (fre_clust * ptr->csize) / 2;
	return ((ptr->n_fatent - 2) * ptr->csize) / 2; /* kilobajty */
}

void _SD_InitDisk(void) {

}

void _SD_FormatDisk(void) {
	/* fat32 */
	while (f_mkfs("", FM_FAT32, (DWORD)0, _work, sizeof(_work)) != FR_OK) HAL_Delay(1);
	_SD_GetDiskSpace();
	/* init wymaganych plikow */
	_SD_InitDisk();
}

void SD_Init(void) {
	if (f_mount(&_ff, "", 1) == FR_NO_FILESYSTEM) {
		_SD_FormatDisk();
	}

	SD_RefreshDateTime();
}

void _SD_ClearDateTimeRegisters(void) {
	_Time.DayLightSaving = 0;
	_Time.Hours = 0;
	_Time.Minutes = 0;
	_Time.SecondFraction = 0;
	_Time.Seconds = 0;
	_Time.StoreOperation = 0;
	_Time.SubSeconds = 0;
	_Time.TimeFormat = 0;

	_Date.Date = 0;
	_Date.Month = 0;
	_Date.WeekDay = 0;
	_Date.Year = 0;
}

void SD_RefreshDateTime(void) {
	_SD_ClearDateTimeRegisters();

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
	_SD_ClearDateTimeRegisters();

	_Date.Date = date[0] * 16 + date[1];
	_Date.Month = date[2] * 16 + date[3];
	_Date.Year = date[4] * 16 + date[5];

	_Time.Hours = time[0] * 16 + time[1];
	_Time.Minutes = time[2] * 16 + time[3];
	_Time.Seconds = time[4] * 16 + time[5];

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
