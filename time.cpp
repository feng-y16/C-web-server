#include "server.h"
#include <time.h>

const char* begin =
"HTTP/1.0 200 OK\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <head>\n"
" <meta http-equiv=\"refresh\" content=\"5\">\n"
" </head>\n"
" <body>\n"
" <p>The current time is ";

const char* end =
".</p>\n"
" </body>\n"
"</html>\n";


extern "C" void module_generate(int client_socket)
{
	time_t TIME;
	struct tm *TM;
	time(&TIME);
	TM = localtime(&TIME);
	send(client_socket, begin, strlen(begin), 0);
	send(client_socket, asctime(TM), strlen(asctime(TM)), 0);
	send(client_socket, end, strlen(end), 0);
}
