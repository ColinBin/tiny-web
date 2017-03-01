#include "socket_pack.h"
int open_clientfd(char *dest, int port) {
	struct sockaddr_in sockaddr;
	struct hostent *host_entry;
	int socket_fd;
	
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
	if(!inet_aton(dest, &(sockaddr.sin_addr))){
		host_entry = gethostbyname(dest);
		if(host_entry == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(h_errno));
			return -2;							// invalid hostname
		}
		memcpy(&sockaddr.sin_addr.s_addr, host_entry->h_addr_list[0], host_entry->h_length);
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);

	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}	
	if(connect(socket_fd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -3;
	}

	return socket_fd;
}

int open_listenfd(int port) {
	struct sockaddr_in sockaddr;
	int socket_fd;
	int optval = 1;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons((unsigned short)port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -2;
	}
	if(bind(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));	
		return -3;
	}	
	if(listen(socket_fd, LISTENQ) < 0){
		fprintf(stderr, "Error: %s\n", strerror(errno));	
		return -4;
	}
	return socket_fd;
}
