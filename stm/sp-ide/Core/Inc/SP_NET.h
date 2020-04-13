#ifndef SP_NET_H_
#define SP_NET_H_

#include "main.h"

void NET_Init(void);

char* NET_GetConnInfo(void);
char* NET_RequestNetworkList(void);

uint8_t NET_ConnectToWiFi(char*, int);
uint8_t NET_WiFiDisconnect(void);
uint8_t NET_HTTPSetup(void);

int NET_GetIndexForPattern(char[]);

void NET_HandleUART_IT(void);
void NET_SendTCPData(char, char*);
void NET_CloseConnSignal(char);

#endif
