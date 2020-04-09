#include <SP_NET.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* makra polecen mikrokontrolera ESP 8266 */
#define AT_HELLO 					"AT"
#define AT_SOFTWARE_VERSION			"AT+GMR"
#define AT_SET_BAUD(a)				"AT+CIOBAUD="a
#define AT_GET_BAUD					"AT+CIOBAUD?"
#define AT_RESET					"AT+RST"
#define AT_SET_MODE(a)				"AT+CWMODE="a
#define AT_DHCP_CONF(a1, a2)		"AT+CWDHCP="a1","a2
#define AT_GET_MODE					"AT+CWMODE?"
#define AT_GET_CONN_STATUS			"AT+CIPSTATUS"
#define AT_SET_RECEIVE_MODE(a)		"AT+CIPMODE="a
#define AT_GET_RECEIVE_MODE			"AT+CIPMODE?"
#define AT_CONN_0(a1,a2,a3,a4)		"AT+CIPSTART="a1","a2","a3","a4
#define AT_CONN_1(a1,a2,a3)			"AT+CIPSTART="a1","a2","a3
#define AT_SET_MUX(a)				"AT+CIPMUX="a
#define AT_SEND_00(a)				"AT+CIPSEND="a
#define AT_SEND_10(a1,a2)			"AT+CIPSEND="a1","a2
#define AT_SEND_01					"AT+CIPSEND"
#define AT_SET_SERVER(a1,a2)		"AT+CIPSERVER="a1","a2
#define AT_SHOW_GLO_IP				"AT+CIFSR"
#define AT_DISCONN_0(a)				"AT+CIPCLOSE="a
#define AT_DISCONN_1				"AT+CIPCLOSE"
#define AT_SET_TIMEOUT(a)			"AT+CIPSTO="a
#define AT_GET_TIMEOUT				"AT+CIPSTO"
#define AT_SET_SLEEP_INT(a)			"AT+GSLP="a
#define AT_CLI_SHOW_MAC				"AT+CIPSTAMAC?"
#define AT_CLI_SET_MAC(a)			"AT+CIPSTAMAC="a
#define AT_AP_SHOW_MAC				"AT+CIPAPMAC?"
#define AT_AP_SET_MAC(a)			"AT+CIPAPAMAC="a
#define AT_CLI_SHOW_IP				"AT+CIPSTA?"
#define AT_CLI_SET_IP(a)			"AT+CIPSTA="a
#define AT_AP_SHOW_IP				"AT+CIPAP?"
#define AT_AP_SET_IP(a)				"AT+CIPAP="a
/* tylko dla klienta */
#define AT_CLI_SHOW_NETWORKS		"AT+CWLAP"
#define AT_CLI_CONN_TO(a1,a2)		"AT+CWJAP="a1","a2
#define R_AT_CLI_CONN_TO			"AT+CWJAP=\"" /* 2 arg */
#define AT_CLI_CONN_STATE			"AT+CWJAP?"
#define AT_CLI_DISCONN				"AT+CWQAP"
/* tylko dla ap */
#define AT_AP_SETUP(a1,a2,a3,a4)	"AT+CWSAP="a1","a2","a3","a4
#define AT_AP_SHOW_DEV				"AT+CWLIF"

#define MODE_CLI	"1"
#define MODE_AP		"2"
#define MODE_MIX	"3"

#define RECEIVE_BUFFER_SIZE 	500
#define NETWORK_LIST_TIMEOUT 	6000
#define MAX_SSID_LEN			100

extern UART_HandleTypeDef huart3;
extern ModeEnum Mode;

char _receive[RECEIVE_BUFFER_SIZE];

void _NET_ResetBuffer(void) {
	for (int i = 0; i < RECEIVE_BUFFER_SIZE; i++) {
		_receive[i] = 0;
	}
}

uint8_t _NET_SendCommand(char command[], uint32_t tTimeout, uint32_t rTimeout) {
	_NET_ResetBuffer();
	size_t len = strlen(command);

	HAL_UART_Transmit(&huart3, (uint8_t*) command, len, tTimeout);
	HAL_UART_Transmit(&huart3, (uint8_t*) "\r\n", 2, 1);

	HAL_UART_Receive(&huart3, (uint8_t*) _receive, RECEIVE_BUFFER_SIZE,
			rTimeout);

	/* szukaj odpowiedzi 'OK\r\n' */
	for (int i = RECEIVE_BUFFER_SIZE; i >= 0; i--) {
		if (_receive[i] == '\n' && _receive[i - 1] == '\r'
				&& _receive[i - 2] == 'K' && _receive[i - 3] == 'O') {
			return 0;
		}
	}
	/* komunikat niekompletny lub niepoprawny */
	return 1;
}

void _NET_SetClientDConnMode(void) {
	while (_NET_SendCommand(AT_RESET, 1, 50) != 0)
		HAL_Delay(1);
	while (_NET_SendCommand(AT_SET_MODE(MODE_CLI), 5, 100) != 0)
		HAL_Delay(1);
	NET_WiFiDisconnect();
}

void NET_Init(void) {
	_NET_SetClientDConnMode();
}

char* NET_RequestNetworkList(void) {
	if (_NET_SendCommand(AT_CLI_SHOW_NETWORKS, 1, NETWORK_LIST_TIMEOUT) == 0) {
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
	char cmd[RECEIVE_BUFFER_SIZE] = { 0 };

	/* ustaw indeks na wlasciwym ssid */
	for (int i = 0; i < network - 1; i++) {
		while (_receive[ssidIndex++] != ';')
			;
	}

	strcpy(cmd, R_AT_CLI_CONN_TO);
	cmdIndex += strlen(R_AT_CLI_CONN_TO);

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
		return 0;
	}
	Mode = MD_LostHost;
	return 1;
}

char* NET_GetConnInfo(void) {
	if ((Mode == MD_ClientConn || Mode == MD_LostHost) && _NET_SendCommand(AT_CLI_SHOW_IP, 5, 50) == 0) {
		int cursor = 0, index = 0;
		while (_receive[cursor++] != '"')
			;
		/* bierzemy tylko ip */
		while (_receive[cursor] != '"') {
			_receive[index++] = _receive[cursor++];
		}
		_receive[index] = 0;
		if (index - 1 > 15) return NULL; /* to nie jest adres ip */
		Mode = MD_ClientConn;

		if (strcmp("0.0.0.0", _receive) == 0) {
			Mode = MD_LostHost;
			return NULL;
		}
	}

	return (char*) _receive;
}

uint8_t NET_WiFiDisconnect(void) {
	while (_NET_SendCommand(AT_CLI_DISCONN, 5, 100) != 0)
		HAL_Delay(1);
	Mode = MD_ClientDConn;
	return 0;
}
