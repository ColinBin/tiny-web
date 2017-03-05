#include "../robust_io/rio.h"
#include "../socket_pack/socket_pack.h"
#include <signal.h> 	/* for capturing SIGCHLD*/
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define MAXBUF 4096

extern char **environ;

void serve(int connfd);
void client_error(int connfd, char *cause, char *error_number, char *short_msg, char *long_msg);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int connfd, char *filename, int filesize, int method_head);
void serve_dynamic(int connfd, char *filename, char *cgiargs, int method_head);
void check_filetype(char *filename, char *filetype);
int check_method(char *method, char *standard_method);				// return 1 if method is standard_method, 0 otherwise

void handler(int sig) {
	pid_t pid;
	while((pid = waitpid(-1, NULL, 0)) > 0) {  }
	if(errno != ECHILD) {
		fprintf(stderr, "Waitpid Error\n");
		exit(3);
	}
}
int main(int argc, char **argv) {

	int server_fd;
	int port;
	int connfd;
	socklen_t clientaddr_len;
	struct sockaddr_in clientaddr;

	/* reaps cgi children with a signal handler */
	if(signal(SIGCHLD, handler) == SIG_ERR) {
		fprintf(stderr, "Signal Error\n");
		exit(3);
	}

	if(argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}

	port = atoi(argv[1]);

	if((server_fd = open_listenfd(port)) < 0) {
		exit(2);
	}

	while(1) {
		clientaddr_len = sizeof(clientaddr);
		connfd = accept(server_fd, (struct sockaddr *)&clientaddr, &clientaddr_len);
		serve(connfd);
		close(connfd);
	}

	return 0;
}

void serve(int connfd) {
	rio_t rio;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char cgiargs[MAXLINE], filename[MAXLINE];
	int is_static;
	int method_head;
	struct stat filestat;
	rio_readinitb(&rio, connfd);

	rio_readlineb(&rio, buf, MAXLINE);
	sscanf(buf, "%s %s %s", method, uri, version);

	if(check_method(method, "GET")) {
		method_head = 0;
	} else if(check_method(method, "HEAD")) {
		method_head = 1;
	} else {
		client_error(connfd, method, "501", "Not Implemented", "Tiny web does not support this method(, yet)");
		return ;
	}

	printf("%s", buf);		/* print request line */
	read_requesthdrs(&rio);

	is_static = parse_uri(uri, filename, cgiargs);
	if(stat(filename, &filestat) < 0) {
		client_error(connfd, filename, "404", "Not found", "Tiny cannot find this file");
		return;
	}
	if(is_static) {
		if(!(S_ISREG(filestat.st_mode)) || !(S_IRUSR & filestat.st_mode)){
			client_error(connfd, filename, "403", "Forbidden", "Tiny cannot open this file");
			return ;
		}
		serve_static(connfd, filename, filestat.st_size, method_head);
	} else {
		if(!(S_ISREG(filestat.st_mode)) || !(S_IXUSR & filestat.st_mode)) {
			client_error(connfd, filename, "403", "Forbidden", "Tiny cannot execute this file");
			return;
		}
		serve_dynamic(connfd, filename, cgiargs, method_head);
	}

}

void client_error(int connfd, char *cause, char *error_number, char *short_msg, char *long_msg) {
	char buf[MAXLINE], body[MAXBUF];
	sprintf(body, "<html><title>Tiny Web Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n%s: %s\r\n", body, error_number, short_msg);
	sprintf(body, "%s<h2>%s: %s</h2>\r\n</body></html>", body, long_msg, cause);

	sprintf(buf, "HTTP/1.1 %s %s\r\n", error_number, short_msg);
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Content-length: %lu\r\n\r\n", strlen(body));
	rio_writen(connfd, buf, strlen(buf));
	rio_writen(connfd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp) {
	char buf[MAXLINE];
	rio_readlineb(rp, buf, MAXLINE);
	while(strcmp(buf, "\r\n")) {
		printf("%s", buf);
		rio_readlineb(rp, buf, MAXLINE);
	}
}

int parse_uri(char *uri, char *filename, char *cgiargs) {
	char *ptr;
	if(strstr(uri, "cgi-bin")) {
		ptr = strchr(uri, '?');
		if(ptr) {
			strcpy(cgiargs, ptr + 1);
			*ptr = '\0';
		} else {
			strcpy(cgiargs, "");
		}
		strcpy(filename, ".");
		strcat(filename, uri);
		return 0;
	} else {
		strcpy(filename, ".");
		strcat(filename, uri);
		if(uri[strlen(uri) - 1] == '/') {
			strcat(filename, "home.html");
		}
		strcpy(cgiargs, "");
		return 1;
	}
}

void serve_static(int connfd, char *filename, int filesize, int method_head){
	char buf[MAXLINE];
	char filetype[MAXLINE];
	check_filetype(filename, filetype);

	sprintf(buf, "HTTP/1.1 200 OK\r\n");
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Content-type: %s\r\n", filetype);
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n", filesize);
	rio_writen(connfd, buf, strlen(buf));
	if(method_head) {
		return;
	}
	int filefd = open(filename, O_RDONLY, 0);
	/* use memory mapping to copy file content to memory */
	char *mapped_ptr;
	mapped_ptr = mmap(0, filesize, PROT_READ, MAP_PRIVATE, filefd, 0);
	close(filefd);
	rio_writen(connfd, mapped_ptr, filesize);
	munmap(mapped_ptr, filesize);
}

void check_filetype(char *filename, char *filetype){
	if(strstr(filename, "html")) {
		strcpy(filetype, "text/html");
	} else if(strstr(filename, "jpg")) {
		strcpy(filetype, "image/jpeg");
	} else if(strstr(filename, "gif")) {
		strcpy(filetype, "image/gif");
	} else {
		strcpy(filetype, "text/plain");
	}
}

int check_method(char *method, char *standard_method){
	if(strcasecmp(method, standard_method))
		return 0;
	else
		return 1;
}
void serve_dynamic(int connfd, char *filename, char *cgiargs, int method_head) {
	char buf[MAXLINE];
	char *emptylist[] = { NULL };
	sprintf(buf, "HTTP/1.1 200 OK\r\n");
	rio_writen(connfd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
	rio_writen(connfd, buf, strlen(buf));
	if(fork() == 0) {
		/* should set all environment variables */
		if(method_head){
			setenv("METHOD_HEAD", "TRUE", 1);
		} else{
			setenv("METHOD_HEAD", "FALSE", 1);
		}
		setenv("QUERY_STRING", cgiargs, 1);
		dup2(connfd, STDOUT_FILENO);
		execve(filename, emptylist, environ);
	}
}
