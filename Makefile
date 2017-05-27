CC=gcc
CFLAGS=-I.

all: barco_cli.c
	$(CC) -o barco_cli barco_cli.c -I.
