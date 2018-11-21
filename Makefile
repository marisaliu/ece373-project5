CC = gcc
CFLAGS=-O2 -Wall
LDLIBS = -lpthread
OBJS=player.o deck.o

all: client server

player.o: player.c player.h card.h
deck.o: deck.c deck.h card.h player.o
client: client.c csapp.c csapp.h $(OBJS)
server: server.c csapp.c csapp.h $(OBJS)

clean:
	rm -rf *~ client server *.o
