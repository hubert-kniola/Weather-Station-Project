#ifndef SP_SD_H_
#define SP_SD_H_

#include "main.h"

void SD_Init(void);

void SD_RefreshDateTime(void);
void SD_GetDateTime(char[], char[]);
void SD_SetDateTime(uint8_t[], uint8_t[]);

uint8_t SD_WriteFile(char*, char*);
char* SD_ReadFile(char*, uint32_t*);

#endif
