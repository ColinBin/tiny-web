echo_client: ../robust_io/rio.o socket_pack.o echo_client.o
	gcc ../robust_io/rio.o socket_pack.o echo_client.o -o echo_client
echo_server: ../robust_io/rio.o socket_pack.o echo_server.o
	gcc ../robust_io/rio.o socket_pack.o echo_server.o -o echo_server
echo_client.o: ../robust_io/rio.h socket_pack.h echo_client.c
	gcc -c echo_client.c -o echo_client.o
echo_server.o: ../robust_io/rio.h socket_pack.h echo_server.c
	gcc -c echo_server.c -o echo_server.o
clean:
	rm echo_client.o echo_server.o echo_client echo_server
