#include "SP_HTTP.h"
#include "SP_SD.h"
#include "SP_NET.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define GET_FILE_PATTERN 	"GET "

#define MAX_REQUEST_LEN		20
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
char _request[MAX_REQUEST_LEN];

#define __resetResponse() for(int i=0;i<MAX_RESPONSE_LEN;i++)_response[i]=0
#define __resetLine() for(int i=0;i<MAX_LINE_LEN;i++)_line[i]=0
#define __resetRequest() for(int i=0;i<MAX_REQUEST_LEN;i++)_request[i]=0

#define CHECK_CONNECTION(a1,a2) arg=(NET_GetIndexForPattern(a2)!=-1)?false:true
#define IF_GET(arg) if(strcmp(req,arg)==0)
#define OR_GET(arg) else if(strcmp(req,arg)==0)

char* _HTTP_ParseHeader(char *response, char *contentType, uint32_t length,
		char *connection) {
	__resetResponse();
	__resetLine();

	sprintf(_line, "HTTP/1.1 %s\r\n", response);
	strcpy(_response, _line);
	__resetLine();

	sprintf(_line, "Content-Type: %s\r\n", contentType);
	strcat(_response, _line);
	__resetLine();

	sprintf(_line, "Content-Lenght: %ld\r\n", length);
	strcat(_response, _line);
	__resetLine();

	sprintf(_line, "Connection: %s\r\n\r\n", connection);
	strcat(_response, _line);

	return (char*) _response;
}

char* _NET_GetRequest(char *request) {
	int cursor = NET_GetIndexForPattern(GET_FILE_PATTERN);
	int index = 0;

	__resetRequest();
	if (cursor != -1) {
		while (request[cursor] != ' ') {
			_request[index++] = request[cursor++];
		}
		return (char*) _request;
	}
	return NULL;
}

void HTTP_HandleRequest(char *request, char connID) {
	char *req = _NET_GetRequest(request);
	char *header;
	char *file;
	uint32_t size;

	IF_GET("/") {
		file = SD_ReadFile("index.htm", &size);
		header = _HTTP_ParseHeader(RSP_OK, CT_HTML, size, CN_CLOSE);

		NET_SendTCPData(connID, header);
		NET_SendTCPData(connID, file);

		NET_CloseConnSignal(connID);
	}
	OR_GET("/#about") {

	} else {
		/* nieobslugiwane zadanie */
		header = _HTTP_ParseHeader(RSP_NOT_FOUND, CT_HTML, 0, CN_CLOSE);
		NET_SendTCPData(connID, header);
		NET_CloseConnSignal(connID);
	}
}
