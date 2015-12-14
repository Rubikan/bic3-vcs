CC 		= gcc52
CFLAGS  = -Wall -Werror -Wextra -Wstrict-prototypes -pedantic -fno-common -g -O3 -std=gnu11
DOXYGEN = doxygen
SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin
CP		= cp
CD		= cd
MV		= mv
GREP	= grep

default: all

client: simple_message_client.c
	$(CC) $(CFLAGS) -c simple_message_client.c -o simple_message_client.o
	$(CC) simple_message_client.o -o client -lsimple_message_client_commandline_handling

server: simple_message_server.c
	$(CC) $(CFLAGS) -c simple_message_server.c -o simple_message_server.o
	$(CC) simple_message_server.o -o server

all: client server

clean:
	rm -f *.o

tar:
	tar -cvzf VCS_TCPIP_AnnaGlock_AndreasRubik.tgz simple_message_client.c simple_message_server.c Makefile README.md

zip:
	zip VCS_TCPIP_AnnaGlock_AndreasRubik.zip simple_message_client.c simple_message_server.c Makefile README.md