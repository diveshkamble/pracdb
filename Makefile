CC=gcc
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -Wextra -g $(INCDIRS) $(OPT)

CFILES=pracdb.C
OBJECTS=pracdb.o

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
		$(CC) -o $@ $^

%.o:%.c
		$(CC) $(CFLAGS) -c -o $@ $^

clean: 
		rm -rf $(BINARY) *.o