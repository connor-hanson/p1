CC = gcc
CFLAGS = -c -Wall -pedantic

all: 
	$(CC) $(CFLAGS) optproc.c
	$(CC) $(CFLAGS) parser.c
	$(CC) $(CFLAGS) main.c
	$(CC) -Wall -pedantic -o 537ps main.o parser.o optproc.o

debug:
	$(CC) $(CFLAGS) -g optproc.c
	$(CC) $(CFLAGS) -g parser.c
	$(CC) $(CFLAGS) -g main.c
	$(CC) -Wall -pedantic -o 537ps main.o parser.o optproc.o

clean: 
	rm -f main.o optproc.o parser.o
