#ifndef SP_MENU_H_
#define SP_MENU_H_

#include "main.h"

typedef enum {
	ST_Clock,
	ST_Options,
	ST_WiFi,
	ST_PassInput
} StateEnum;

void MENU_Init(void);
void MENU_PasswdInput(void);
uint8_t MENU_HandleKeys(void);

#endif
