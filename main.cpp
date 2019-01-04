#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "server.h"

const char* program_name;
const char* const shortopt = "a:hm:p:v";
const struct option longopt[] = {
	{ "address", 1, NULL, 'a' },
{ "help", 0, NULL, 'h' },
{ "modir", 1, NULL, 'm' },
{ "port", 1, NULL, 'p' },
{ "verbose", 0, NULL, 'v' },
{ NULL, 0, NULL, 0 }
};

const char* modir = "lib/";

void Printinfo(FILE* stream, int exit_code)
{
	fprintf(stream, "Usage: %s options \n", program_name);
	fprintf(stream,
		" -a --address set address.\n"
		" -h --help    show help.\n"
		" -m --modir   set module directory.\n"
		" -p --port    set port.\n"
		" -v --verbose show verbose messages.\n");
	exit(exit_code);
}

int verbose = 0;

int main(int argc, char* argv[])
{
	program_name = argv[0];
	char nextopt;
	int addressChange = 0;
	const char* address = "localhost";
	int port = 8080;
	nextopt = getopt_long(argc, argv, shortopt, longopt, NULL);
	while (~nextopt) {
		switch (nextopt) {
		case 'a': 
			address = optarg;
			addressChange = 1;
			break;
		case 'h': 
			Printinfo(stdout, 0);
		case 'm': 
			modir = optarg;
			break;
		case 'p': 
			port = atoi(optarg);
			break;
		case 'v': 
			verbose = 1;
			break;
		case '?': 
			Printinfo(stderr, 1);
		case -1: 
			break;
		default: 
			abort();
		}
		nextopt = getopt_long(argc, argv, shortopt, longopt, NULL);
	}
	printf("ServerRunning\n");
	printf("address: %s\n", address);
	printf("port:    %d\n", port);
	printf("modir:   %s\n", modir);
	if (verbose)
		printf("verbose: true\n");
	struct in_addr addr;
	addr.s_addr = inet_addr(address);
	server_run(addr, htons(port));
	printf("ServerEndding\n");
	return 0;
}