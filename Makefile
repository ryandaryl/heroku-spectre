CC=gcc

all:
	$(CC) -Wall -Werror -o main main.c
	strip main