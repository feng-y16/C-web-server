#include "server.h"
#include <dlfcn.h>
typedef void (*fptr_FUNC)(int a);

const char* notfoundres =
"HTTP/1.0 404 Not Found\n"
"Content-type: text/html\n"
"\n"
"<html>\n"
" <body>\n"
" <h1>Not Found</h1>\n"
" <p>The requested URL was not found on this server.</p>\n"
" </body>\n"
"</html>\n";

void notfound(int client_socket)
{
	send(client_socket, notfoundres,strlen(notfoundres), 0);
}

server_module::server_module(const char* module_name)
{
	this->name = module_name;
	char* dir = (char*)malloc(128);
	memset(dir, 0, 128 * sizeof(char));
	sprintf(dir, "lib/%s.so", module_name);
	this->handle = dlopen(dir, RTLD_LAZY);
	if (this->handle == NULL)
	{
		printf("Error: the module was not found.\n");
		this->generate_function = notfound;
	}
	else
		this->generate_function = (fptr_FUNC)dlsym(this->handle, "module_generate");
	free(dir);
}

void module_close(server_module* module)
{
	if (module->handle != NULL)
	{
		dlclose(module->handle);
	}
}