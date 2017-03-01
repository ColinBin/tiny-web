#ifndef RIO_H
#define RIO_H

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define RIOBUFSIZE 8192

ssize_t rio_readn(int fd, void *user_buf, size_t n);
ssize_t rio_writen(int fd, void *user_buf, size_t n);

typedef struct{
	int fd;						// file descriptor that is bound 
	int unread_cnt;				// unread bytes left in the buffer
	char *rio_bufptr;			// points to the first byte unread
	char rio_buf[RIOBUFSIZE];  	// internal buffer
} rio_t;

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *user_buf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *user_buf, size_t maxlen);


#endif
