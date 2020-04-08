#include <SP_NET.h>

#include <string.h>
#include <stdlib.h>

/* makra polecen mikrokontrolera ESP 8266 */
#define AT_HELLO 					"AT"
#define AT_SOFTWARE_VERSION		"AT+GMR"
#define AT_SET_BAUD(a)				"AT+CIOBAUD="#a
#define AT_GET_BAUD				"AT+CIOBAUD?"
#define AT_RESET					"AT+RST"
#define AT_SET_MODE(a)				"AT+CWMODE="#a
#define AT_DHCP_CONF(a1, a2)		"AT+CWDHCP="#a1","#a2
#define AT_GET_MODE				"AT+CWMODE?"
#define AT_GET_CONN_STATUS			"AT+CIPSTATUS"
#define AT_SET_RECEIVE_MODE(a)		"AT+CIPMODE="#a
#define AT_GET_RECEIVE_MODE		"AT+CIPMODE?"
#define AT_CONN_0(a1,a2,a3,a4)		"AT+CIPSTART="#a1","#a2","#a3","#a4
#define AT_CONN_1(a1,a2,a3)		"AT+CIPSTART="#a1","#a2","#a3
#define AT_SET_MUX(a)				"AT+CIPMUX="#a
#define AT_SEND_00(a)				"AT+CIPSEND="#a
#define AT_SEND_10(a1,a2)			"AT+CIPSEND="#a1","#a2
#define AT_SEND_01					"AT+CIPSEND"
#define AT_SET_SERVER(a1,a2)		"AT+CIPSERVER="#a1","#a2
#define AT_SHOW_GLO_IP				"AT+CIFSR"
#define AT_DISCONN_0(a)			"AT+CIPCLOSE="#a
#define AT_DISCONN_1				"AT+CIPCLOSE"
#define AT_SET_TIMEOUT(a)			"AT+CIPSTO="#a
#define AT_GET_TIMEOUT				"AT+CIPSTO"
#define AT_SET_SLEEP_INT(a)		"AT+GSLP="#a
#define AT_CLI_SHOW_MAC			"AT+CIPSTAMAC?"
#define AT_CLI_SET_MAC(a)			"AT+CIPSTAMAC="#a
#define AT_AP_SHOW_MAC				"AT+CIPAPMAC?"
#define AT_AP_SET_MAC(a)			"AT+CIPAPAMAC="#a
#define AT_CLI_SHOW_IP				"AT+CIPSTA?"
#define AT_CLI_SET_IP(a)			"AT+CIPSTA="#a
#define AT_AP_SHOW_IP				"AT+CIPAP?"
#define AT_AP_SET_IP(a)			"AT+CIPAP="#a
/* tylko dla klienta */
#define AT_CLI_SHOW_NETWORKS		"AT+CWLAP"
#define AT_CLI_CONN_TO(a1,a2)		"AT+CWJAP="#a1","#a2
#define AT_CLI_CONN_STATE			"AT+CWJAP?"
#define AT_CLI_DISCONN				"AT+CWQAP"
/* tylko dla ap */
#define AT_AP_SETUP(a1,a2,a3,a4)	"AT+CWSAP="#a1","#a2","#a3","#a4
#define AT_AP_SHOW_DEV				"AT+CWLIF"

#define RECEIVE_BUFFER_SIZE 500

extern UART_HandleTypeDef huart3;

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

	HAL_UART_Receive(&huart3, (uint8_t*) _receive, RECEIVE_BUFFER_SIZE, rTimeout);

	/* szukaj odpowiedzi 'OK\r\n' */
	for (int i = RECEIVE_BUFFER_SIZE; i >= 0; i--) {
		if (_receive[i] == '\n' && _receive[i-1] == '\r' && _receive[i-2] == 'K' && _receive[i-3] == 'O') {
			return 0;
		}
	}

	return 1;
}

void NET_Init(void) {
	_NET_ResetBuffer();
}

char* NET_RequestNetworkList(void) {
	if (_NET_SendCommand(AT_CLI_SHOW_NETWORKS, 1, 10000) == 0) {
		return (char*)_receive;
	}
	return NULL;
}

char* NET_ShowMAC(void) {
	if (_NET_SendCommand(AT_CLI_SHOW_MAC, 1, 5) == 0) {
		return (char*)_receive;
	}
	return NULL;
}
