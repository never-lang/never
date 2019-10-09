
CC = gcc -g
CFLAGS = -Wall

dyntest.o: dyntest.c dyntest.h
	$(CC) $(CFLAGS) -fPIC -c dyntest.c -o dyntest.o
	$(CC) -shared dyntest.o -o dyntest.so
