output: rio.h test.o rio.o
	gcc test.o rio.o -o output
rio.o: rio.h rio.c
	gcc -c rio.c -o rio.o
test.o: test.c rio.h
	gcc -c test.c -o test.o
clean:
	rm test.o rio.o output
	echo "" > write_output
