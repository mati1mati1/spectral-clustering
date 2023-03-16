CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: eigen_params.o file_utils.o
	$(CC) -o spkmeans eigen_params.o file_utils.o $(CFLAGS)

eigen_params.o: eigen_params.c
	$(CC) -c eigen_params.c eigen_params.h $(CFLAGS)

file_utils.o: file_utils.c
	$(CC) -c file_utils.c file_utils.h $(CFLAGS)

clean:
	rm -f *.o