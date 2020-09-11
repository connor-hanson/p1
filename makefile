CC = gcc
CFLAGS = -c -Wall

all: 
	$(CC) $(CFLAGS) optproc.c
	$(CC) $(CFLAGS) parser.c
	$(CC) $(CFLAGS) main.c
	$(CC) -Wall -o 537ps main.o parser.o optproc.o

clean: 
	rm -f main.o optproc.o parser.o