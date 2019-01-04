#include "server.h"
#include "pool.h"
#include <signal.h>

#define BACKLOG 5 
#define MAX 200
int quit = 0;

struct sock_in
{
	int fd;
	sockaddr_in add;
};

void serve_kernel(int client_socket)
{
	int len = 1024;
	char* request = new char[len];
	memset(request, 0, len * sizeof(char));
	read(client_socket, request, len);
	char* method = (char*)malloc(8 * sizeof(char));
	char* URL = (char*)malloc(128 * sizeof(char));
	sscanf(request, "%s %s ", method, URL);
	char* module_name = URL + 1;
	if (!strcmp(module_name, "favicon.ico"))
	{
		return;
	}
	server_module* m = new server_module(module_name);
	(*m->generate_function)(client_socket);
	delete m;
	delete[] request;
	free(method);
	free(URL);
	return;
}

void *serve(void *arg)
{
	int client_socket = *(int *)arg;
	serve_kernel(client_socket);
	close(client_socket);
	return NULL;
}

void *getquit(void *arg)
{
	sock_in &server = *(sock_in *)arg;
	do 
	{
		sleep(2);
	}while (!getchar() == 27);
	quit = 1;
	return NULL;
}

void server_run(struct in_addr local_address, uint16_t port)
{
	sock_in server, client;
	socklen_t client_len = sizeof(client.add);
	int err;
	server.fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server.fd<0) { printf("socket error"); return; }
	bzero(&server.add, sizeof(server.add));
	server.add.sin_family = AF_INET;
	server.add.sin_addr = local_address;
	server.add.sin_port = port;
	err = bind(server.fd, (struct sockaddr*)&(server.add), sizeof(server.add));
	if (err<0) 
	{ 
		perror("bind error"); 
		return; 
	}
	err = listen(server.fd, BACKLOG);
	if (err<0) 
	{ 
		printf("listen error"); 
		return; 
	}
	Pool *pool = new Pool(MAX);
	pool->add(getquit, (void*)&server);
	int i = 1;
	while (1)
	{
		client.fd = accept(server.fd, (struct sockaddr*)&(client.add), &client_len);
		if (quit)
			break;
		if (client.fd<0)
			continue;
		pool->add(serve, (void*)&(client.fd));
	}
	delete pool;
	close(server.fd);
}