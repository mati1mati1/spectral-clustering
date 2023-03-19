CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -lm -g

spkmeans: spkmeans.o jacobi.o file_utils.o matrix.o
	$(CC) -o spkmeans spkmeans.o jacobi.o file_utils.o matrix.o $(CFLAGS)
	make clean

spkmeans.o: spkmeans.c
	$(CC) -c spkmeans.c jacobi.h file_utils.h matrix.h $(CFLAGS)

matrix.o: matrix.c
	$(CC) -c matrix.c matrix.h $(CFLAGS)

jacobi.o: jacobi.c
	$(CC) -c jacobi.c jacobi.h $(CFLAGS)

file_utils.o: file_utils.c
	$(CC) -c file_utils.c file_utils.h $(CFLAGS)

clean:
	rm -f *.o
	rm -f *.gch