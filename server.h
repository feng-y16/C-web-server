#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

struct server_module 
{
	void* handle;
	const char* name; 
	void(*generate_function) (int);
	server_module(const char* module_name);
};

extern struct server_module* module_open(const char* module_name);

extern void module_close(server_module* module);

extern void server_run(struct in_addr local_address, uint16_t port);