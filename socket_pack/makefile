client: rio.o socket_pack.o client.o
	gcc rio.o socket_pack.o client.o -o client
server: rio.o socket_pack.o server.o
	gcc rio.o socket_pack.o server.o -o server
client.o: rio.h socket_pack.h client.c
	gcc -c client.c -o client.o
server.o: rio.h socket_pack.h server.c
	gcc -c server.c -o server.o
clean:
	rm client.o server.o client server
