CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

spkmeans: eigen_params.o
	$(CC) -o spkmeans eigen_params.o $(CFLAGS)

eigen_params.o: eigen_params.c
	$(CC) -o eigen_params.c eigen_params.h $(CFLAGS)

clean:
	rm -f *.o