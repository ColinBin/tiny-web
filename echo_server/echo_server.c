#include <stdio.h>
#include "../socket_pack/socket_pack.h"
#include "../robust_io/rio.h"

#define MAXLINE 1024

int main(int argc, char **argv) {
	char buf[MAXLINE];
	char *haddrp;
	int server_fd;
	int connfd;
	int bytes_read;
	socklen_t client_len;
	rio_t rio;
	struct sockaddr_in client_addr;
	struct hostent *hp;
	if(argc != 2){
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}
	if((server_fd = open_listenfd(atoi(argv[1]))) < 0){
		exit(1);
	}
	while(1) {
		client_len = sizeof(client_addr);
		connfd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		hp =gethostbyaddr((const char *)&client_addr.sin_addr.s_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(client_addr.sin_addr);
		printf("server connected to %s (%s)\n", hp->h_name, haddrp);
		rio_readinitb(&rio, connfd);
		while((bytes_read = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
			printf("server received %d bytes\n", bytes_read);
			rio_writen(connfd, buf, bytes_read);
		}
		close(connfd);
	}
	return 0;
}
