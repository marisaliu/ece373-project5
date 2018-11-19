#include "csapp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "deck.h"
#include "player.h"
#include <ctype.h>

/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
		int play = 1;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

		//while play is true prompt the user for a rank and then display his hand and the books
    while (play) {
			buf = user_play(&user);
			Rio_writen(clientfd, buf, strlen(buf)); //writes/sends it to server
	//		Rio_readlineb(&rio, buf, MAXLINE);  //reads in from server
			display_hand(&user);
			display_book(&user,1);
			display_book(&computer,2);
			//	Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
