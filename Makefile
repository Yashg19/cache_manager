CC = gcc
CSCOPE = cscope
CFLAGS += -Wall -Werror
LDFLAGS += -pthread

default: fun

fun: fun.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o fun fun.c

clean:
	-rm -f fun.o
	-rm -f fun
	-rm -f Makefile~
