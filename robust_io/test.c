#include "rio.h"
#include <stdio.h>
#include <string.h>

void check_readlineb();
void check_readnb();
void check_writen();

int main(){
	check_readlineb();
	check_readnb();
	check_writen();
	return 0;
}

void check_readlineb( ){
	int fd = open("buffer_input", O_RDONLY, 0);
	char user_buf[1024];
	int nread;
	rio_t rio;
	rio_readinitb(&rio, fd);
	while((nread = rio_readlineb(&rio, user_buf, 20)) > 0) {
		printf("\nbytes read: %d content: ", nread);
		fflush(stdout);
		rio_writen(STDOUT_FILENO, user_buf, nread);
	}
	close(fd);	
}

void check_readnb( ){
	int fd = open("buffer_input", O_RDONLY, 0);
	char user_buf[1024];
	int nread;
	rio_t rio;
	rio_readinitb(&rio, fd);
	while((nread = rio_readnb(&rio, user_buf, 20)) > 0){
		printf("\nbytes read: %d content: ", nread);
		fflush(stdout);
		rio_writen(STDOUT_FILENO, user_buf, nread);
	}
	close(fd);

}

void check_writen(){
	int fd = open("write_output", O_WRONLY, 0);
	char to_write[] = "I love Insanely Simple.\n It is one of the best book I have ever read.\n Jobs is outstanding.";
	int nwrite = rio_writen(fd, to_write, sizeof(to_write));
	printf("bytes written: %d\n", nwrite);
	close(fd);
}
