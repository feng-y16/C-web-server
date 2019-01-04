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
	char response;
	FILE* stream = popen("ps aux", "r");
	send(client_socket, start, strlen(start), 0);
	response = fgetc(stream);
	while (!feof(stream))
	{
		if (response != '<')
			send(client_socket, &response, sizeof(char), 0);
		else
			send(client_socket, "\n", sizeof(char), 0);
		response = fgetc(stream);
	}
	send(client_socket, end, strlen(end), 0);
	pclose(stream);
}