#include "rio.h"

ssize_t rio_readn(int fd, void *user_buf, size_t n) {
	size_t nleft = n;
	char *bufp = user_buf;
	int nread;
	while (nleft > 0) {
		nread = read(fd, bufp, nleft);
		if (nread < 0) {
			if(errno == EINTR) { 		// read is interruptted
				continue;
			} 
			else {						// error during read
				return -1;
			}
		} 
		else if (nread == 0) { 			// encounter EOF
			break;
		} 
		else {							// normal
			nleft -= nread;
			bufp += nread;
		}
	}
	return n - nleft;
}

ssize_t rio_writen(int fd, void *user_buf, size_t n) {
	size_t nleft = n;
	char *bufp;
	ssize_t nwrite;
	while(nleft > 0) {
		nwrite = write(fd, user_buf, nleft);
		if (nwrite <= 0) {
			if(errno == EINTR) { 		// write is interrupted 
				continue;
			} else {					// error during write
				return -1;
			}
		} 
		nleft -= nwrite;
		bufp += nwrite;
	}
	return n;
}

void rio_readinitb(rio_t *rp, int fd) {
	rp->fd = fd;
	rp->unread_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, void *user_buf, size_t n) {
	int cnt;
	int nread;
	while(rp->unread_cnt <= 0) {		// empty buffer
		nread = read(rp->fd, rp->rio_buf, sizeof(rp->rio_buf));
		if(nread < 0){
			if(errno == EINTR) {		// interrupted
				continue;	
			} else {
				return -1;
			}
		} else if(nread == 0) {			// encounter EOF
			return 0;
		} else {						// read nread bytes
			rp->unread_cnt += nread;
			rp->rio_bufptr = rp->rio_buf;
		}
	}
	cnt = n;
	if (rp->unread_cnt < n){
		cnt = rp->unread_cnt;			// choose the minimum
	}
	memcpy(user_buf, rp->rio_bufptr, cnt);
	rp->unread_cnt -= cnt;
	rp->rio_bufptr += cnt;
	return cnt;
}

ssize_t rio_readnb(rio_t *rp, void *user_buf, size_t n) {
	size_t nleft = n;
	int nread;
	char *bufp = user_buf;
	while (nleft > 0) {
		nread = rio_read(rp, bufp, nleft);
		if(nread < 0) {
			return -1;
		} else if(nread == 0) {
			break;
		} else {
			nleft -= nread;
			bufp += nread;
		}
	}
	
	return n - nleft;
}

ssize_t rio_readlineb(rio_t *rp, void *user_buf, size_t maxlen) {
	int read_count = 0;
	int nread;
	char *bufp = user_buf;
	char c;
	while(read_count < maxlen - 1) {
		nread = rio_read(rp, &c, 1);
		if(nread < 0) {
			return -1;
		} else if(nread == 0) {
			if (read_count == 0) {
				return 0;
			} else {
				break;	
			}
		} else {
			read_count++;
			*bufp++ = c;
			if(c == '\n') {
				break;
			}
		}
	}	
	*bufp = '\0';
	return read_count;
}
