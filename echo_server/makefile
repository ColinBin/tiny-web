riop = ../robust_io/
socketp = ../socket_pack/

echo: echo_client echo_server
echo_client: $(riop)rio.o $(socketp)socket_pack.o echo_client.o
	gcc $(riop)rio.o $(socketp)socket_pack.o echo_client.o -o echo_client
echo_server: $(riop)rio.o $(socketp)socket_pack.o echo_server.o
	gcc $(riop)rio.o $(socketp)socket_pack.o echo_server.o -o echo_server
echo_client.o: $(riop)rio.h $(socketp)socket_pack.h echo_client.c
	gcc -c echo_client.c -o echo_client.o
echo_server.o: $(riop)rio.h $(socketp)socket_pack.h echo_server.c
	gcc -c echo_server.c -o echo_server.o
clean:
	rm echo_client.o echo_server.o echo_client echo_server
	rm $(riop)rio.o $(socketp)socket_pack.o
