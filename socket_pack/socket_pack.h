#ifndef SOCKET_PACK_H
#define SOCKET_PACK_H

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#define LISTENQ 1024

int open_clientfd(char *dest, int port);
int open_listenfd(int port);

#endif
