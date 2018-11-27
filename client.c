#include "csapp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "card.h"
#include <ctype.h>

/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"

//////////////////////////////////////////////////////////////////
///////////////////Prompts for User Input/////////////////////////
/////////////////////////////////////////////////////////////////
char user_play(char* str)
{ 
	char input;
	int boolInHand = 0;
	char rank[(strlen(str)/2)+1];
	int x = 0;
	for(int i =0; i < ((strlen(str)/2)+1); i++){
		if(!isspace(str[i])){
			rank[x] = str[i];
			x++;
		}
	}
	while(boolInHand == 0)
	{
		printf("\nPlayer 1's turn, enter a Rank: ");
		scanf("%c", &input);
		while((getchar()) != '\n');
		for(int i = 0; i < strlen(rank); i++){
			if(rank[i]== toupper(input))
			{ 
				boolInHand = 1;
				return toupper(input);
			}
		}
		printf("Error - must have at least one card from rank to play");
	}
	return -1;
}


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
		char inputRank;
		Rio_readlineb(&rio,buf,MAXLINE);
		Fputs(buf,stdout);
		//while play is true prompt the user for a rank and then display his hand and the books
    while (play) {
			inputRank = user_play(&buf);
			Rio_writen(clientfd, inputRank, strlen(buf)); //writes/sends it to server
			Rio_readlineb(&rio, buf, MAXLINE);  //reads in from server
			Fputs(buf, stdout);
			Rio_readlineb(&rio, buf, MAXLINE);
			Fputs(buf, stdout);
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}

