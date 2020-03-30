#include "SP_SD.h"
#include "ff.h"

static FATFS _ffHandle; //uchwyt do urządzenia FatFs (dysku, karty SD...)
FRESULT _fresult; //do przechowywania wyniku operacji na bibliotece
FIL _fileHandle; //uchwyt do otwartego plik

uint8_t _buffer[256]; //bufor odczytu i zapisu
uint16_t _bytesWritten; //liczba zapisanych byte
uint16_t _bytesRead;

void SD_Init(void) {
	f_mount(&_ffHandle, "", 0);
}

/* Template structure
 *
 * {
 * 		"innTHS" : true,
 * 		"temper" : 24,
 * 		"humidi" : 35,
 * 		"locati" : "PL",
 * 		"time"	 : "12:03:30"
 * }
 */

uint8_t SD_WriteJSON(float *data) {
	return 1;
}
