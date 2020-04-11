#include <SP_NET.h>

#include "SP_HTTP.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* makra polecen mikrokontrolera ESP 8266 */
#define RESET						"AT+RST"
#define SET_MODE(a)					"AT+CWMODE="a
#define SET_MUX(a)					"AT+CIPMUX="a
#define SETUP_SERVER(a1,a2)			"AT+CIPSERVER="a1","a2
#define SHOW_IP						"AT+CIFSR"
#define SEND_DATA_TO_CONN(a1, a2) 	"AT+CIPSEND="a1","a2
#define CLOSE_CONN(a) 				"AT+CIPCLOSE="a
#define SHOW_NETWORKS				"AT+CWLAP"
#define CONN_TO_NETWORK				"AT+CWJAP=\"" /* 2 arg */
#define DISCONNECT					"AT+CWQAP"

#define CLIENT			"1"
#define ACCESS_POINT	"2"
#define MIXED			"3"

#define RECEIVE_BUFFER_SIZE 	500
#define READY_DATA_SIZE			100
#define NETWORK_LIST_TIMEOUT 	4000
#define REQUEST_RECEIVE_TIMEOUT	500
#define BYTE_RECEIVE_TIMEOUT	30
#define MAX_SSID_LEN			100
#define IP_SIZE					15

#define CLIENT_IP_PATTERN 			"STAIP,\""
#define OK_PATTERN 					"OK\r\n"
#define REQUEST_RECIEVED_PATTERN 	"+IPD,"

extern UART_HandleTypeDef huart3;
extern ModeEnum Mode;

char _receive[RECEIVE_BUFFER_SIZE];
char _currentIP[IP_SIZE];

char _connID;

uint8_t _uartByte;
uint8_t _requestIndex = 0;

void _NET_ResetBuffer(void) {
	for (int i = 0; i < RECEIVE_BUFFER_SIZE; i++) {
		_receive[i] = 0;
	}
}

void _NET_ResetIP(void) {
	for (int i = 0; i < IP_SIZE; i++) {
		_currentIP[i] = 0;
	}
}

int _NET_GetIndexForPattern(char pattern[]) {
	int find = 0;
	int patternLen = strlen(pattern);

	for (int cursor = 0; cursor < RECEIVE_BUFFER_SIZE; cursor++) {
		if (find == patternLen) {
			return cursor;
		}

		if (_receive[cursor] == pattern[find]) {
			find++;
		} else {
			find = 0;
		}
	}
	return -1;
}

uint8_t _NET_SendCommand(char command[], uint32_t tTimeout, uint32_t rTimeout) {
	_NET_ResetBuffer();
	size_t len = strlen(command);

	HAL_UART_AbortReceive_IT(&huart3);

	HAL_UART_Transmit(&huart3, (uint8_t*) command, len, tTimeout);
	HAL_UART_Transmit(&huart3, (uint8_t*) "\r\n", 2, 1);

	HAL_UART_Receive(&huart3, (uint8_t*) _receive, RECEIVE_BUFFER_SIZE,
			rTimeout);

	HAL_UART_Receive_IT(&huart3, &_uartByte, 1);

	/* szukaj odpowiedzi 'OK\r\n' */
	if (_NET_GetIndexForPattern(OK_PATTERN) != -1) {
		return 0;
	}
	/* komunikat niekompletny lub niepoprawny */
	return 1;
}

void _NET_SetClientDConnMode(void) {
	while (_NET_SendCommand(RESET, 1, 50) != 0)
		HAL_Delay(1);
	while (_NET_SendCommand(SET_MODE(MIXED), 5, 100) != 0)
		HAL_Delay(1);
	NET_WiFiDisconnect();
}

void NET_Init(void) {
	_NET_SetClientDConnMode();

	_connID = 0;
	_uartByte = 0;
	_requestIndex = 0;
}

char* NET_RequestNetworkList(void) {
	if (_NET_SendCommand(SHOW_NETWORKS, 1, NETWORK_LIST_TIMEOUT) == 0) {
		int index = 0;
		for (int cursor = 0; cursor < RECEIVE_BUFFER_SIZE; cursor++) {
			if (_receive[cursor] == '(') {

				_receive[cursor++] = 1;
				int security = _receive[cursor] - '0';

				/* esp8266 nie obluguje wpa2 enterprise*/
				if (security == 5)
					break;

				/* potrzebujemy tylko ssid */
				while (_receive[cursor] != '"') {
					_receive[cursor++] = 1;
				}

				_receive[cursor++] = 1;

				/* przepisz ssid na poczatek buforu */
				while (_receive[cursor] != '"') {
					_receive[index++] = _receive[cursor];
					_receive[cursor++] = 1;
				}

				/* ignoruj reszte */
				while (_receive[cursor] != ')') {
					_receive[cursor++] = 1;
				}

				_receive[index++] = '-';
				_receive[cursor++] = 1;

				/* informacja o zabezpieczeniach */
				if (security == 0) {
					_receive[index++] = 'O';
					_receive[index++] = 'P';
					_receive[index++] = 'E';
					_receive[index++] = 'N';
				} else if (security == 1) {
					_receive[index++] = 'W';
					_receive[index++] = 'E';
					_receive[index++] = 'P';
					_receive[index++] = ' ';
				} else if (security == 2) {
					_receive[index++] = 'W';
					_receive[index++] = 'P';
					_receive[index++] = 'A';
					_receive[index++] = ' ';
				} else if (security == 3) {
					_receive[index++] = 'W';
					_receive[index++] = 'P';
					_receive[index++] = 'A';
					_receive[index++] = '2';
				} else if (security == 4) {
					_receive[index++] = 'M';
					_receive[index++] = 'I';
					_receive[index++] = 'X';
					_receive[index++] = 'D';
				}
				_receive[index++] = ';';
				continue;
			} else if (_receive[cursor] == 0) {
				break;
			}
			/* czysc bezuzyteczne znaki */
			_receive[cursor] = 1;
		}

		/* wyczysc reszte buforu */
		for (int i = RECEIVE_BUFFER_SIZE - 1; i >= index; i--) {
			_receive[i] = 0;
		}

		/* przekaz wyniki wyzej */
		if (index == 0) {
			return NULL;
		} else {
			return (char*) _receive;
		}
	}
	return NULL;
}

uint8_t NET_ConnectToWiFi(char *password, int network) {
	int ssidIndex = 0;
	int cmdIndex = 0;
	char cmd[100] = { 0 };

	/* ustaw indeks na wlasciwym ssid */
	for (int i = 0; i < network - 1; i++) {
		while (_receive[ssidIndex++] != ';')
			;
	}

	strcpy(cmd, CONN_TO_NETWORK);
	cmdIndex += strlen(CONN_TO_NETWORK);

	for (int i = 0;; i++) {
		if (_receive[ssidIndex] == '-') {
			break;
		} else {
			cmd[cmdIndex++] = _receive[ssidIndex++];
		}
	}

	strcat(cmd, "\",\"");
	strcat(cmd, password);
	strcat(cmd, "\"");

	if (_NET_SendCommand(cmd, 10, NETWORK_LIST_TIMEOUT) == 0) {
		Mode = MD_ClientConn;
		NET_HTTPSetup();

		return 0;
	}
	Mode = MD_LostHost;
	return 1;
}

char* NET_GetConnInfo(void) {
	if ((Mode == MD_ClientConn || Mode == MD_LostHost)
			&& _NET_SendCommand(SHOW_IP, 5, 100) == 0) {
		int cursor = _NET_GetIndexForPattern(CLIENT_IP_PATTERN);
		int index = 0;

		_NET_ResetIP();
		/* bierzemy tylko ip */
		while (_receive[cursor] != '"') {
			_currentIP[index++] = _receive[cursor++];

			if (index - 1 > 15) {
				Mode = MD_LostHost;
				return NULL; /* to nie jest adres ip */
			}
		}

		if (strcmp("0.0.0.0", _receive) == 0) {
			Mode = MD_LostHost;
			return NULL;
		}
		/* jesli uda sie nawiazac polaczenie pozniej, ustaw serwer */
		if (Mode == MD_LostHost) {
			NET_HTTPSetup();
			Mode = MD_ClientConn;
		}
	}

	return (char*) _currentIP;
}

uint8_t NET_WiFiDisconnect(void) {
	while (_NET_SendCommand(DISCONNECT, 5, 100) != 0)
		HAL_Delay(1);
	Mode = MD_ClientDConn;
	return 0;
}

uint8_t NET_HTTPSetup(void) {
	while (_NET_SendCommand(SET_MUX("1"), 5, 100) != 0)
		HAL_Delay(1);
	while (_NET_SendCommand(SETUP_SERVER("1", "80"), 5, 100) != 0)
		HAL_Delay(1);

	HAL_UART_Receive_IT(&huart3, &_uartByte, 1);
	/* gniazdo tcp juz nasluchuje na porcie 80 */
	return 0;
}

void NET_SendDataAndCloseConn(char connID, char *data) {
	char cmd[100] = { 0 };

	sprintf(cmd, SEND_DATA_TO_CONN("%c", "%d"), connID, strlen(data));
	_NET_SendCommand(cmd, 5, 100);

	HAL_Delay(10);

	HAL_UART_Transmit(&huart3, (uint8_t*) data, strlen(data), 5000);

	for (int i = 0; i < 100; i++)
		cmd[i] = 0;

	HAL_Delay(50);

	sprintf(cmd, CLOSE_CONN("%c"), _connID);
	_NET_SendCommand(cmd, 5, 100);
}

void NET_HandleUART_IT(void) {
	if (_uartByte == REQUEST_RECIEVED_PATTERN[_requestIndex++]) {
		if (_requestIndex == strlen(REQUEST_RECIEVED_PATTERN)) {
			/* odczyt id polaczenia */
			HAL_UART_Receive(&huart3, (uint8_t*) &_connID, 1,
			BYTE_RECEIVE_TIMEOUT);
			/* odczyt ilosci danych do odebrania */
			char lenString[5] = { 0 }, tempChar = 0;
			int lenIndex = 0;

			while (tempChar != ':') {
				HAL_UART_Receive(&huart3, (uint8_t*) &tempChar, 1,
				BYTE_RECEIVE_TIMEOUT);
				/* jebaniutki nie chce w nulla */
				if (tempChar == ',')
					continue;

				lenString[lenIndex++] = tempChar;
			}
			/* bez dwukropka */
			lenString[--lenIndex] = 0;

			uint16_t requestLen = atoi(lenString);
			requestLen = (requestLen > RECEIVE_BUFFER_SIZE) ?
			RECEIVE_BUFFER_SIZE : requestLen;
			/* odbierz zadanie */
			HAL_UART_Receive(&huart3, (uint8_t*) _receive, requestLen,
			REQUEST_RECEIVE_TIMEOUT);

			char* response = HTTP_HandleRequest((char*) _receive);
			if (response != NULL) {
				NET_SendDataAndCloseConn(_connID, response);
			}
		}
	} else {
		/* nieistotne dane */
		_requestIndex = 0;
	}

	HAL_UART_Receive_IT(&huart3, &_uartByte, 1);
}
