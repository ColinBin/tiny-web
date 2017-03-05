riop = ../robust_io/
socketp = ../socket_pack/

tiny_web: tiny_web.o $(riop)rio.o $(socketp)socket_pack.o
	gcc tiny_web.o $(riop)rio.o $(socketp)socket_pack.o -o tiny_web
tiny_web.o: tiny_web.c $(riop)rio.h $(socketp)socket_pack.h
	gcc -c tiny_web.c -o tiny_web.o
clean: 
	rm tiny_web tiny_web.o
	rm $(riop)rio.o $(socketp)socket_pack.o
