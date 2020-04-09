#ifndef SP_HTTP_H_
#define SP_HTTP_H_

#include "main.h"

void NET_Init(void);

char* NET_GetConnInfo(void);
char* NET_RequestNetworkList(void);
uint8_t NET_ConnectToWiFi(char*, int);
uint8_t NET_WiFiDisconnect(void);

#endif