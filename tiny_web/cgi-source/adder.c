#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF 4096
#define MAXLINE 1024
int main(int argc, char **argv){
	int a, b, sum;
	char *cgiargs, *ptr;
	char buf[MAXBUF];
	char *method_head;
	cgiargs = getenv("QUERY_STRING");
	
	if(cgiargs && (ptr = strchr(cgiargs, '&')) && strstr(cgiargs, "first=") && strstr(cgiargs, "second=")) {
		// display the result
		*ptr = '\0';
		a = atoi(cgiargs + strlen("first="));
		b = atoi(ptr + 1 + strlen("second="));
		sum = a + b;

		sprintf(buf, "<html><title>Tiny Web Server</title>\r\n");
		sprintf(buf, "%s<body><h1>The result is: %d</h1>\r\n", buf, sum);
		sprintf(buf, "%s<p>Thanks for using adder!\r\nHave a nice day</p></body></html>", buf);
	}
	else {
		// display the form
		sprintf(buf, "<html><form action=\"adder\" method=\"get\">\r\n");
		sprintf(buf, "%s<p>#1 number: <input type=\"text\" name=\"first\"/></p>\r\n", buf);
		sprintf(buf, "%s<p>#2 number: <input type=\"text\" name=\"second\"/></p>\r\n", buf);
		sprintf(buf, "%s<input type=\"submit\" value=\"submit\" /></form></html>\r\n", buf);
	}

	printf("Content-type: text/html\r\n");
	printf("Content-length: %lu\r\n", strlen(buf));
	printf("Server: Tiny Web Server\r\n\r\n");

	method_head = getenv("METHOD_HEAD");
	if(!strcmp(method_head, "TRUE")) {
		return 0;
	}
	printf("%s", buf);

	return 0;
}
