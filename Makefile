# variables
CC=gcc
CFLAGS=-std=c11 -Wall -Werror
LDLIBS=-lm -lcurses
OUTPUT=$@

# targets
%: %.c
	$(CC) $(CFLAGS) $@.c $(LDLIBS) -o $(OUTPUT)


