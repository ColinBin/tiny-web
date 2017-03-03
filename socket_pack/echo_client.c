#include "socket_pack.h"
#include "../robust_io/rio.h"
#include <stdio.h>
#define MAXLINE 1024
int main(int argc, char **argv){
	rio_t rio;
	char buf[MAXLINE];
	int client_fd;
	if(argc != 3){
		fprintf(stderr, "usage: %s <ip or hostname> <port>\n", argv[0]);
		exit(1);
	}
	if((client_fd = open_clientfd(argv[1], atoi(argv[2]))) < 0) {
		exit(1);
	}
	rio_readinitb(&rio, client_fd);

	while(fgets(buf, MAXLINE, stdin) != NULL){
		rio_writen(client_fd, buf, strlen(buf));
		rio_readlineb(&rio, buf, MAXLINE);
		fputs(buf, stdout);
	}
	close(client_fd);

	return 0;
}
