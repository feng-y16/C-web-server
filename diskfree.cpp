#include "server.h"

const char* start =
"HTTP/1.0 200 OK\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <head>\n"
" <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"
" </head>"
" <body>\n"
" <pre>\n";

const char* end =
" </pre>\n"
" </body>\n"
"</html>\n";


extern "C" void module_generate(int client_socket)
{
	int len = 128;
	char* response = (char*)malloc(len);
	FILE* stream = popen("df", "r");
	send(client_socket, start, strlen(start), 0);
	while (!feof(stream))
	{
		memset(response, 0, len * sizeof(char));
		fgets(response, len, stream);
		send(client_socket, response, strlen(response), 0);
	}
	send(client_socket, end, strlen(end), 0);
	pclose(stream);
	free(response);
}