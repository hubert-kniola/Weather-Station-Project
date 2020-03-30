#ifndef SP_SD_H_
#define SP_SD_H_

#include "main.h"

void SD_Init(void);
uint8_t SD_WriteJSON(float[]);
void SD_RefreshDateTime(void);
void SD_GetDateTime(char[], char[]);

#endif
