#include "SP_HTTP.h"

#include <string.h>

#define MAX_PAGE_SIZE	500

char WEBPAGE[MAX_PAGE_SIZE] = { 0 };

void HTTP_Init(void) {
	strcpy(WEBPAGE, "HTTP/1.1 200 OK\r\n");
	strcat(WEBPAGE, "Content-Type: text/html\r\n");
	strcat(WEBPAGE, "Content-Lenght: 398\r\n");
	strcat(WEBPAGE, "Connection: close\r\n\r\n");
	strcat(WEBPAGE, "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n");
	strcat(WEBPAGE, "<title>Dioda RGB</title>\r\n</head>\r\n");
	strcat(WEBPAGE, "<body>\r\n<form method=\"get\">\r\n");
	strcat(WEBPAGE, "<p><b>Czerwony (0-255):</b> <input type=\"text\"");
	strcat(WEBPAGE, " name=\"red\" value=\"0\" /></p>\r\n");
	strcat(WEBPAGE, "<p><b>Zielony (0-255):</b> <input type=\"text\"");
	strcat(WEBPAGE, " name=\"green\" value=\"0\" /></p>\r\n");
	strcat(WEBPAGE, "<p><b>Niebieski (0-255):</b> <input type=\"text\"");
	strcat(WEBPAGE, " name=\"blue\" value=\"0\" /></p>\r\n");
	strcat(WEBPAGE, "<input type=\"submit\" value=\"Ustaw kolor\" />\r\n");
	strcat(WEBPAGE, "</form>\r\n</body>\r\n</html>");
}

char* HTTP_HandleRequest(char* request) {
	return (char*) WEBPAGE;
}
