#include "SP_SD.h"

#include "ff.h"

#include <string.h>
#include <stdio.h>

typedef struct {
	uint8_t date[6];
	uint8_t time[6];
} SD_Time;

#define MAX_FILESIZE 10000

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
UINT _writtenB;
UINT _readB;

BYTE _buffer[MAX_FILESIZE];

void _SD_ResetBuffer(void) {
	for (int i = 0; i < MAX_FILESIZE; i++) {
		_buffer[i] = 0;
	}
}

uint32_t _SD_GetDiskSpace(void) {
	FATFS *ptr;
	uint32_t fre_clust = 0;

	if (f_getfree("", &fre_clust, &ptr) != FR_OK) {
		return 0;
	}

	DISK_LEFT = (fre_clust * ptr->csize) / 2;
	return ((ptr->n_fatent - 2) * ptr->csize) / 2; /* kilobajty */
}

void _SD_FormatDisk(void) {
	/* fat32 */
	while (f_mkfs("", FM_FAT32, (DWORD) 0, _work, sizeof(_work)) != FR_OK)
		HAL_Delay(1);
	_SD_GetDiskSpace();
}

void SD_Init(void) {
	if (f_mount(&_ff, "", 1) == FR_NO_FILESYSTEM) {
		//TODO Fatal
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

char* SD_ReadFile(char *filename, uint32_t *size) {
	if (f_open(&_fileH, filename, FA_READ) != FR_OK) {
		*size = 0;
		return NULL;
	}

	_SD_ResetBuffer();
	uint32_t index = 0;

	while (!f_eof(&_fileH)) {
		_res = f_read(&_fileH, &_buffer[index++], 1, &_readB);
	}

	f_close(&_fileH);
	*size = index;
	return (char*) _buffer;
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
uint8_t SD_WriteFile(char *filename, char *data) {
	uint32_t len = strlen(data);
	_SD_GetDiskSpace();

	if (len > DISK_LEFT) {
		return 1;
	}

	if (f_open(&_fileH, filename, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
		return 2;
	}

	f_write(&_fileH, data, len, &_writtenB);

	f_close(&_fileH);
	return 0;
}
