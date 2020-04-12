#include "SP_HTTP.h"
#include "SP_SD.h"
#include "SP_NET.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define GET_FILE_PATTERN 	"GET /"

#define MAX_FILENAME_LEN	13
#define MAX_RESPONSE_LEN	400
#define MAX_LINE_LEN		100

#define RSP_NOT_FOUND 	"404 Not Found"
#define RSP_OK			"200 OK"

#define CT_JS 		"text/javascript"
#define CT_CSS 		"text/css"
#define CT_HTML 	"text/html"

#define CN_CLOSE 	"close"
#define CN_KEEP		"keep-alive"

char _response[MAX_RESPONSE_LEN];
char _line[MAX_LINE_LEN];

#define _resetResponse() for(int i=0;i<MAX_RESPONSE_LEN;i++)_response[i]=0
#define _resetLine() for(int i=0;i<MAX_LINE_LEN;i++)_line[i]=0

void HTTP_Init(void) {

}

char* _HTTP_ParseHeader(char *response, char *contentType, uint32_t length,
		char *connection) {
	_resetResponse();
	_resetLine();

	sprintf(_line, "HTTP/1.1 %s\r\n", response);
	strcpy(_response, _line);
	_resetLine();

	sprintf(_line, "Content-Type: %s\r\n", contentType);
	strcat(_response, _line);
	_resetLine();

	sprintf(_line, "Content-Lenght: %ld\r\n", length);
	strcat(_response, _line);
	_resetLine();

	sprintf(_line, "Connection: %s\r\n\r\n", connection);
	strcat(_response, _line);

	return (char*) _response;
}

void HTTP_HandleRequest(char *request, char connID) {
	int cursor = NET_GetIndexForPattern(GET_FILE_PATTERN);
	int index = 0;

	if (cursor != -1) {
		while (request[cursor++] != ' ')
			;
	}

	if (index == 0) {
		uint32_t size = 0;
		bool close = false;

		char *data = SD_ReadFile("index.htm", &size);
		char *header = _HTTP_ParseHeader(RSP_OK, CT_HTML, size, CN_KEEP);

		if (NET_GetIndexForPattern(CN_KEEP) == -1) {
			close = true;
		}

		NET_SendData(connID, header);
		NET_SendData(connID, data);

		NET_CloseConn(connID);

	} else {
		NET_SendData(connID,
				_HTTP_ParseHeader(RSP_NOT_FOUND, CT_HTML, 0, CN_CLOSE));
		NET_CloseConn(connID);
	}
}
