#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF 4096
int main(int argc, char **argv){
	int a, b, sum;
	char *cgiargs, *ptr;
	char buf[MAXBUF];
	cgiargs = getenv("QUERY_STRING");
	if(cgiargs && (ptr = strchr(cgiargs, '&'))) {
	 	*ptr = '\0';
		a = atoi(cgiargs);
		b = atoi(ptr + 1);
		sum = a + b;
	} 
	else 
		sum = 0;
	
	sprintf(buf, "<html><title>Tiny Web Server</title>\r\n");
	sprintf(buf, "%s<body><h1>The result is: %d</h1>\r\n", buf, sum);
	sprintf(buf, "%s<p>Thanks for using adder!\r\nHave a nice day</p></body></html>", buf);

	printf("Content-type: text/html\r\n");
	printf("Content-length: %lu\r\n", strlen(buf));
	printf("Server: Tiny Web Server\r\n\r\n");

	printf("%s", buf);

	return 0;
}
